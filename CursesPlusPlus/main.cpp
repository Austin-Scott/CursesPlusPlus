#include "CursesPlusPlus.h"
#include <SDL.h>

int main(int argc, char* argv[])
{
	CursesSystem::halfDelay(1);
	Window root = CursesSystem::getRootWindow();
	Window win = Window(root, 0.8, 0.8, 0.1, 0.1);
	Window test1 = Window(win, 0.5, 0.8, 0.1, 0.1);
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
		test1.refresh();
	}

	return 0;
}