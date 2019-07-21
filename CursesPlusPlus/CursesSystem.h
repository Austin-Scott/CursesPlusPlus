#pragma once
#include "Window.h"

class CursesSystem {
private:
	static CursesSystem system;

	Window* rootWindow;

	CursesSystem();
	CursesSystem(const CursesSystem&) {};
	CursesSystem& operator=(const CursesSystem&) {};
	~CursesSystem();
public:
	static void raw(bool value = true);
	static void echo(bool value = true);
	static void keypad(bool value = true);
	static void halfDelay(unsigned char tenthsOfSeconds);
	static void noDelay(bool value = true);
	static void inputNewLine(bool value = true);

	static void refreshRootDimensions();

	static Window& getRootWindow();
};