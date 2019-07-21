#include "Cursor.h"
#include "Window.h"
#include "CursesSystem.h"
#include <map>
#include <vector>
#include <curses.h>

const std::map<Color, int> colorTable {
	{Color::Black, COLOR_BLACK},
	{Color::Red, COLOR_RED},
	{Color::Green, COLOR_GREEN},
	{Color::Yellow, COLOR_YELLOW},
	{Color::Blue, COLOR_BLUE},
	{Color::Magenta, COLOR_MAGENTA},
	{Color::Cyan, COLOR_CYAN},
	{Color::White, COLOR_WHITE}
};

struct ColorPair {
	unsigned int id;
	bool isSet;
	Color foreground;
	Color background;
};

class WindowReference {
private:
	WINDOW* window;
	bool isRoot;
	Color foreground = Color::White;
	Color background = Color::Black;
public:
	WINDOW* getWINDOW() {
		if (isRoot) {
			return stdscr;
		}
		else {
			return window;
		}
	}
	Color getForeground() {
		return foreground;
	}
	Color getBackground() {
		return background;
	}
	void setForeground(Color value) {
		foreground = value;
	}
	void setBackground(Color value) {
		background = value;
	}
	void useCurrentColor() {
		if (CursesSystem::isColorEnabled()) {
			static std::vector<ColorPair> colorPairs = []() {
				std::vector<ColorPair> result;
				for (int i = 1; i < COLOR_PAIRS; i++) {
					ColorPair pair;
					pair.id = i;
					pair.isSet = false;
					result.push_back(pair);
				}
				return result;
			}();
			int pairIndex = 0;
			for (ColorPair &pair : colorPairs) {
				if (!pair.isSet) {
					pair.isSet = true;
					pair.foreground = foreground;
					pair.background = background;
					init_pair(pair.id, colorTable.at(foreground), colorTable.at(background));
					break;
				}
				if (foreground == pair.foreground && background == pair.background) {
					break;
				}
				pairIndex++;
			}
			if (pairIndex == colorPairs.size()) {
				colorPairs.back().foreground = foreground;
				colorPairs.back().background = background;
				init_pair(colorPairs.back().id, colorTable.at(foreground), colorTable.at(background));
				pairIndex--;
			}
			ColorPair chosenPair = colorPairs[pairIndex];
			for (int i = pairIndex; i > 0; i--) {
				colorPairs[i] = colorPairs[i - 1];
			}
			colorPairs[0] = chosenPair;
			wattron(getWINDOW(), COLOR_PAIR(chosenPair.id));
			wbkgd(getWINDOW(), COLOR_PAIR(chosenPair.id));
		}
	}
	int getX() {
		int x;
		int y;
		getbegyx(getWINDOW(), y, x);
		return x;
	}
	int getY() {
		int x;
		int y;
		getbegyx(getWINDOW(), y, x);
		return y;
	}
	int getWidth() {
		if (isRoot) {
			return COLS;
		}

		int mx;
		int my;
		getmaxyx(getWINDOW(), my, mx);
		return mx;
	}
	int getHeight() {
		if (isRoot) {
			return LINES;
		}

		int mx;
		int my;
		getmaxyx(getWINDOW(), my, mx);
		return my;
	}
	void setPosition(int y, int x) {
		if (isRoot || (x == getX() && y == getY())) return;

		mvwin(window, y, x);
	}
	void setSize(int height, int width) {
		if (isRoot || (width == getWidth() && height == getHeight())) return;

		wresize(window, height, width);
	}
	WindowReference(WINDOW* window, bool isRoot = false) {
		this->isRoot = isRoot;
		if (isRoot) {
			this->window = nullptr;
		}
		else {
			this->window = window;
		}
	}
	~WindowReference() {
		if (!isRoot) {
			delwin(window);
		}
	}
};

Window::Window(void* win)
{
	window = std::make_shared<WindowReference>((WINDOW*)win, true);
}

Window::Window(int height, int width, int starty, int startx)
{
	window = std::make_shared<WindowReference>(newwin(height, width, starty, startx));
}

Window::Window(Window parent, float height, float width, float y, float x) : Window(1, 1, 0, 0)
{
	snapToParent(parent, height, width, y, x);
	refresh();
}

Window::~Window()
{
	
}

std::weak_ptr<WindowReference> Window::getWindowReference() {
	return window;
}

void Window::updateSizeAndPosition()
{
	if (snapped) {
		if (auto prnt = parent.lock()) {
			int xOffset = (x * (float)prnt->getWidth()) + prnt->getX();
			int yOffset = (y * (float)prnt->getHeight()) + prnt->getY();
			int newWidth = width * (float)prnt->getWidth();
			int newHeight = height * (float)prnt->getHeight();
			window->setSize(newHeight, newWidth);
			window->setPosition(yOffset, xOffset);
		}
		else {
			snapped = false;
		}
	}
}

void Window::refresh()
{
	wrefresh(window->getWINDOW());
}

void Window::clear()
{
	wclear(window->getWINDOW());
}

void Window::drawBorder(char ls, char rs, char ts, char bs, char tl, char tr, char bl, char br)
{
	wborder(window->getWINDOW(), ls, rs, ts, bs, tl, tr, bl, br);
}

const Window& Window::operator<<(Cursor rhs) const
{
	window->useCurrentColor();
	switch (rhs.type) {
	case Cursor::Type::Text:
		wprintw(window->getWINDOW(), rhs.text.c_str());
		break;
	case Cursor::Type::AttributesOn:
		wattron(window->getWINDOW(), rhs.attrMask);
		break;
	case Cursor::Type::AttributesOff:
		wattroff(window->getWINDOW(), rhs.attrMask);
		break;
	case Cursor::Type::SetColor:
		window->setBackground(rhs.background);
		window->setForeground(rhs.foreground);
		break;
	case Cursor::Type::Move:
		wmove(window->getWINDOW(), rhs.row, rhs.col);
		break;
	case Cursor::Type::EndLine:
		wprintw(window->getWINDOW(), "\n");
		break;
	case Cursor::Type::ClearToEndOfLine:
		wclrtoeol(window->getWINDOW());
		break;
	case Cursor::Type::ClearToEndOfWindow:
		wclrtobot(window->getWINDOW());
		break;
	default:
		break;
	}
	return *this;
}

void Window::setScroll(bool value)
{
	scrollok(window->getWINDOW(), value);
}

int Window::getX()
{
	return window->getX();
}

int Window::getY()
{
	return window->getY();
}

int Window::getWidth()
{
	return window->getWidth();
}

int Window::getHeight()
{
	return window->getHeight();
}

void Window::setPosition(int y, int x)
{
	snapped = false;
	window->setPosition(y, x);
}

void Window::setSize(int height, int width)
{
	snapped = false;
	window->setSize(height, width);
}

void Window::snapToParent(Window parent, float height, float width, float y, float x)
{
	snapped = true;
	this->parent = parent.getWindowReference();
	this->height = height;
	this->width = width;
	this->x = x;
	this->y = y;
}

std::optional<int> Window::getChar()
{
	int result = wgetch(window->getWINDOW());
	if (result == ERR) return {};
	return result;
}


