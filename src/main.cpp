#include "systemclass.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* system = nullptr;
	SystemClassDesc system_desc{};
	bool result;
	
	// Config system
	system_desc.show_cursor = true;
	
	// Create the system object.
	system = new SystemClass();

	// Initialize and run the system object.
	result = system->initialize(system_desc);
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
