#include "Console.h"



void CConsole::createConsoleWindow()
{
	AllocConsole();
	hConsole = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT);
	*stdout = *_fdopen(hConsole, "w");
	setvbuf(stdout, NULL, _IONBF, 0);
}

void CConsole::closeConsoleWindow()
{
	_close(hConsole);
}
