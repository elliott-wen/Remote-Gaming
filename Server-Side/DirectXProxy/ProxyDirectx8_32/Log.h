#pragma once
#include <tchar.h>

#include <iostream>
#include <fstream>
#include <string>
#include <xstring>
using namespace std;
//============Log====================
#undef DEBUG
#ifdef DEBUG
#define LOGFILE(text)		\
							{ofstream log;\
							log.open("D:\\log1.txt", ios::app);\
							log << (text)<<endl;\
							log.close();}
#define DXLOG(text)		\
							{ofstream log;\
							log.open("D:\\log1.txt", ios::app);\
							log << (text)<<endl;\
							log.close();}

#else
#define LOGFILE(text)
#define DXLOG(text)	
#endif
//===========Log========================
