#pragma once
#include <memory>
#include <optional>
#include <string>

class WindowReference;
class Window {
private:
	
	std::shared_ptr<WindowReference> window;
	std::weak_ptr<WindowReference> parent;

	bool snapped = false;
	float height;
	float width;
	float x;
	float y;

	Window(void* win);

	std::weak_ptr<WindowReference> getWindowReference();

public:
	Window(int height, int width, int starty, int startx);
	Window(Window parent, float height, float width, float y, float x);

	~Window();


	void refresh();

	void setScroll(bool value = true);

	int getX();
	int getY();
	int getWidth();
	int getHeight();

	void setPosition(int y, int x);
	void setSize(int height, int width);

	void snapToParent(Window parent, float height, float width, float y, float x);

	void print(std::string str);

	std::optional<int> getChar();

	friend class CursesSystem;
};