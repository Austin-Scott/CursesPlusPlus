#include "Window.h"
#include <curses.h>

class WindowReference {
private:
	WINDOW* window;
	bool isRoot;
public:
	WINDOW* getWINDOW() {
		if (isRoot) {
			return stdscr;
		}
		else {
			return window;
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

void Window::refresh()
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
	box(window->getWINDOW(), 0, 0);
	wrefresh(window->getWINDOW());
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

void Window::print(std::string str)
{
	waddstr(window->getWINDOW(), str.c_str());
}

std::optional<int> Window::getChar()
{
	int result = wgetch(window->getWINDOW());
	if (result == ERR) return {};
	return result;
}


