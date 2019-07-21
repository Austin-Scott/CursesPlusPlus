#include "CursesPlusPlus.h"

int main(int argc, char* argv[])
{
	CursesSystem::echo(false);
	CursesSystem::halfDelay(1);
	CursesSystem::color();
	CursesSystem::keypad();

	Window root = CursesSystem::getRootWindow();
	Window win = Window(root, 0.8, 0.8, 0.1, 0.1);
	Window test1 = Window(win, 0.5, 0.8, 0.1, 0.1);
	Window input = Window(1, 1, 0, 0);
	win.setScroll();

	bool firstLoop = true;

	while (true) {
		if (firstLoop || CursesSystem::refreshRootDimensions()) {
			win.updateSizeAndPosition();
			test1.updateSizeAndPosition();

			root << Cursor::moveTo(1, 1) << Cursor::setColor(Color::White, Color::Red) << "I am root." << Cursor::clearToEndOfWindow;
			win << Cursor::moveTo(1, 1) << Cursor::setColor(Color::White, Color::Green) << "I am win." << Cursor::clearToEndOfWindow;
			test1 << Cursor::moveTo(1, 1) << Cursor::setColor(Color::White, Color::Blue) << "I am test1." << Cursor::clearToEndOfWindow;

			root.drawBorder();
			win.drawBorder();
			test1.drawBorder();

			root.refresh();
			win.refresh();
			test1.refresh();

			firstLoop = false;
		}
		else {
			input.getChar();
		}
	}

	return 0;
}