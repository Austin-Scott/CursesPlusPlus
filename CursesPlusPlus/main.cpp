#include "CursesPlusPlus.h"

int main()
{
	CursesSystem::halfDelay(10);
	Window root = CursesSystem::getRootWindow();
	Window win = Window(root, 1, 1, 0, 0);
	win.setScroll();

	while (true) {
		if (auto chr = win.getChar()) {
			win.print("A character was pressed!\n");
		}
		else {
			win.print("No characters were pressed.\n");
		}
		CursesSystem::refreshRootDimensions();
		root.refresh();
		win.refresh();
	}

	return 0;
}