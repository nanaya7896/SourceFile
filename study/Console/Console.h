//=====================================================================
//Console�̍쐬
//		
//=====================================================================

//includeGurad
#pragma once
#include"../Global/Global.h"
#include<io.h>
#include<Fcntl.h>
class CConsole
{
	int hConsole = 0;
public:
	void createConsoleWindow();
	void closeConsoleWindow();
};

