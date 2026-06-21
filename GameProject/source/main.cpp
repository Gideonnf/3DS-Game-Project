#include <3ds.h>
#include <stdio.h>
#include "Core/Timer.hpp"
#include "Core/InputManager.hpp"

int main(int argc, char **argv)
{
	gfxInitDefault();

	//Initialize console on top screen. Using NULL as the second argument tells the console library to use the internal console structure as current one
	consoleInit(GFX_TOP, NULL);

	//Move the cursor to row 15 and column 19 and then prints "This was built and ran through VS Code!"
	//To move the cursor you have to print "\x1b[r;cH", where r and c are respectively the row and column where you want your cursor to move
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns
	printf("\x1b[11;7HThis was built and ran through VS Code!");

	printf("\x1b[25;16HPress Start to exit.");

	Timer timeManager;

	// Main loop
	while (aptMainLoop())
	{
		timeManager.Tick();
		
		InputManager::GetInstance()->Update(timeManager.GetDeltaTime());

		// break in order to return to hbmenu
		if (InputManager::GetInstance()->GetKeyDown() & KEY_START) break; 

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
