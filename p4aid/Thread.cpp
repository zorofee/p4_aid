
#include <iostream>
#include <cstdio>
#include <thread>

using namespace std;
void start_files_search(const char* path);

void excute_shell(void *param)
{
	std::cout << (char*)param << '\n';
	system((char*)param);
}

void create_p4_command(char* command)
{
	char & param = *command;
	std::thread t(excute_shell, &param);
	t.join();
}

void create_file_search_thread(char* command)
{
	char& param = *command;
	std::thread t(start_files_search, &param);
	t.join();
}

