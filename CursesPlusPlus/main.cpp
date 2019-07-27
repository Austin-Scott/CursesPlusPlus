#include "CursesPlusPlus.h"

using namespace cursor;

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

			root << moveTo(1, 1) << setColor(Color::White, Color::Red) << "I am root." << clearToEndOfWindow;
			win << moveTo(1, 1) << setColor(Color::White, Color::Green) << "I am win." << clearToEndOfWindow;
			test1 << moveTo(1, 1) << setColor(Color::White, Color::Blue) << "I am test1." << clearToEndOfWindow;

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