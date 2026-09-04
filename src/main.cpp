#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* system = nullptr;
	bool result;
	
	// Create the system object.
	system = new SystemClass;

	// Initialize and run the system object.
	result = system->initialize();
	if(result)
	{
		system->run();
	}

	// Shutdown and release the system object.
	system->shutdown();
	delete system;
	system = nullptr;

	return 0;
}
