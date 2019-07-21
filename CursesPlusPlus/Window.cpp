#include "Window.h"
#include <curses.h>

class WindowReference {
private:
	WINDOW* window;
	bool isRoot;
public:
	WINDOW* getWINDOW() {
		if (isRoot) {
			return curscr;
		}
		else {
			return window;
		}
	}
	int getX() {
		int x;
		int y;
		getyx(getWINDOW(), y, x);
		return x;
	}
	int getY() {
		int x;
		int y;
		getyx(getWINDOW(), y, x);
		return y;
	}
	int getWidth() {
		int mx;
		int my;
		getmaxyx(getWINDOW(), my, mx);
		return mx;
	}
	int getHeight() {
		int mx;
		int my;
		getmaxyx(getWINDOW(), my, mx);
		return my;
	}
	void setX(int value) {
		if (isRoot) return;

		mvwin(window, getY(), value);
	}
	void setY(int value) {
		if (isRoot) return;

		mvwin(window, value, getX());
	}
	void setWidth(int value) {
		if (isRoot) return;

		wresize(window, getHeight(), value);
	}
	void setHeight(int value) {
		if (isRoot) return;

		wresize(window, value, getWidth());
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
	box(window->getWINDOW(), 0, 0);
	wrefresh(window->getWINDOW());
	if (snapped) {
		if (auto prnt = parent.lock()) {
			window->setX(prnt->getX() + (x * prnt->getWidth()));
			window->setY(prnt->getY() + (y * prnt->getHeight()));
			window->setWidth(width * prnt->getWidth());
			window->setHeight(height * prnt->getHeight());
		}
		else {
			snapped = false;
		}
	}
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

void Window::setX(int value)
{
	snapped = false;
	window->setX(value);
}

void Window::setY(int value)
{
	snapped = false;
	window->setY(value);
}

void Window::setWidth(int value)
{
	snapped = false;
	window->setWidth(value);
}

void Window::setHeight(int value)
{
	window->setHeight(value);
}

void Window::setPosition(int y, int x)
{
	setX(x);
	setY(y);
}

void Window::setSize(int height, int width)
{
	setWidth(width);
	setHeight(height);
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


