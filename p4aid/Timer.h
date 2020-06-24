#pragma once

#include <iostream>
#include <string>
#include <time.h>
using namespace std;

string getTime()
{
	time_t timep;
	tm t;
	time(&timep);
	char tmp[64];
	localtime_s(&t, &timep);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",&t);
	return tmp;
}

int print_time(){
	string   time = getTime();
	cout <<"******** current time ***********"<< time << endl;

	return 0;
}
