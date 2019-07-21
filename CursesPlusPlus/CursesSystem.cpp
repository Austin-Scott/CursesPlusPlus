#include "CursesSystem.h"
#include <curses.h>

CursesSystem CursesSystem::system = CursesSystem();
bool CursesSystem::colorEnabled = false;

CursesSystem::CursesSystem()
{
	initscr();
	rootWindow = new Window(stdscr);
}

CursesSystem::~CursesSystem()
{
	endwin();
	delete rootWindow;
}

void CursesSystem::raw(bool value)
{
	if(value) {
		::raw();
	}
	else {
		::noraw();
	}
}

void CursesSystem::echo(bool value)
{
	if (value) {
		::echo();
	}
	else {
		::noecho();
	}
}

void CursesSystem::keypad(bool value)
{
	::keypad(stdscr, value);
}

void CursesSystem::halfDelay(unsigned char tenthsOfSeconds)
{
	::halfdelay(tenthsOfSeconds);
}

void CursesSystem::noDelay(bool value)
{
	::nodelay(stdscr, value);
}

void CursesSystem::inputNewLine(bool value)
{
	if (value) {
		::nl();
	}
	else {
		::nonl();
	}
}

void CursesSystem::color()
{
	if (has_colors()) {
		start_color();
		colorEnabled = true;
	}
}

bool CursesSystem::isColorEnabled()
{
	return colorEnabled;
}

bool CursesSystem::refreshRootDimensions()
{
	if (is_termresized()) {
		resize_term(0, 0);
		return true;
	}
	return false;
}

Window& CursesSystem::getRootWindow()
{
	return *(system.rootWindow);
}
