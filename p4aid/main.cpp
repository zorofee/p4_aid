#pragma once

#include <iostream>
#include <thread>
#include <string>
#include "Timer.h"

using namespace std;



void start_files_search(const char* path);
void files_search_multi_thread(const char* path);

int main(int argc, char *argv[])
{
	if (argv[1])
	{
		//µ÷ÓÃp4ÃüÁî
		std::cout << "start p4 command\n";
		print_time();
		//start_files_search(argv[1]);
		files_search_multi_thread(argv[1]);
		print_time();
	}
	else
	{
		std::cout << "no path input\n";
		print_time();
		string path = "C:\\Users\\56984\\Desktop\\p4\\UnrealEngine";
		//files_search_multi_thread((char *)path.c_str());
		print_time();
	}
	
	return 0;
}