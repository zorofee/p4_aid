#pragma once

#include <iostream>
#include <thread>
#include <string>
#include "Timer.h"

using namespace std;



void create_p4_command(char* command);
void create_file_search_thread(char* command);

void start_files_search(const char* path);


int main(int argc, char *argv[])
{
	if (argv[1])
	{
		//µ÷ÓÃp4ÃüÁî
		std::cout << "start p4 command\n";
		start_files_search(argv[1]);


	}
	else
	{

		std::cout << "no path input\n";
		//string path = "F:/CPPWorks/p4aid/test/A";
		//start_files_search((char *)path.c_str());
	}
	
	return 0;
}