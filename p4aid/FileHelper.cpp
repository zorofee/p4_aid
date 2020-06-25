#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <io.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <thread>

using namespace std;

#define MULTI_THREAD
static int index = 0;
static int submit_num = 0;
static int ignore_num = 0;
const short THREAD_NUM = 10;
vector<string> file_paths;

void search_file(string path,vector<string>& file_paths)
{
	intptr_t hFile = 0;
	struct _finddata_t fileInfo;
	string pathName;

	if ((hFile = _findfirst(pathName.assign(path).append("\\*").c_str(), &fileInfo)) == -1)
		return;

	do {
		if (strcmp(fileInfo.name, "..") && strcmp(fileInfo.name, ".") && fileInfo.attrib == _A_SUBDIR)
		{
			search_file(path + "/" + fileInfo.name, file_paths);
		}

		if (fileInfo.attrib != _A_SUBDIR)
		{
			string entirePath = path + "/" + fileInfo.name;
			//cout << path + "\\" + fileInfo.name << " is a file" << endl;
			file_paths.push_back(entirePath);
		}
	} 
	while (_findnext(hFile, &fileInfo) == 0);

	_findclose(hFile);
	return;
}

void write_file(const char* content)
{
	ofstream outfile;
	string file_name = "addcmd.sh";
	outfile.open(file_name, ios_base::out | ios_base::trunc);
	stringstream ss;
	ss << content;
	outfile << ss.str() << endl;
	outfile.close();
}


void files_search_thread(void *param)
{
	//分配到任务后,开始在线程中执行
	vector<int>* start_end = (vector<int>*)param;
	std::cout <<"excute file num is :"<< (*start_end)[0]<<"  "<<(*start_end)[1] << '\n';
	int start = (*start_end)[0];
	int end = (*start_end)[1];
	string command = "p4 add -d -c default ";
	for(size_t i = start;i<=end;i++)
	{
		if (i == file_paths.size())
		{
			break;
		}
		if (file_paths[i].find(".pdb") == string::npos)
		{
			//屏蔽pdb文件
			command.append("\"");
			command.append(file_paths[i]);
			command.append("\"");
			command.append("\ ");
			index++;
			submit_num++;

			//std::cout << (file_paths[i]) << '\n';
			if (index >= 50)
			{
				std::cout << "add " << index << " files\n";
				//每50个文件提交一次
				system(command.c_str());
				command = "p4 add -d -c default ";
				index = 0;
			}
		}
		else
		{
			ignore_num++;
		}
	}

	if (index > 0)
	{
		std::cout << "last add " << index << " files\n";
		system(command.c_str());
	}

	std::cout << "****************** add files :" << submit_num << "; ignore files : " << ignore_num << "*******************\n";

}

void files_search_multi_thread(const char* path)
{
	search_file(path, file_paths);
	std::cout <<"vector size is "<< file_paths.size()<<"  "<<file_paths[1]<<"\n";

	int num = file_paths.size() / THREAD_NUM + 1;

#ifdef MULTI_THREAD
	//多线程执行   157930,1133,18s
	std::cout << "***********multi thread**********";
	for (size_t i = 0; i < THREAD_NUM; i++)
	{
		vector<int> start_end;
		start_end.push_back(i * num);
		start_end.push_back((i + 1) * num - 1);
		std::thread t(files_search_thread,&start_end);
		t.join();
	}
#else
	//主线程中执行   157930,1133,18s
	std::cout << "***********single thread**********";
	vector<int> start_end;
	start_end.push_back(0);
	start_end.push_back(file_paths.size() - 1);
	files_search_thread(&start_end);
#endif // MULTI_THREAD
}

void start_files_search(const char* path)
{
	//vector<string> file_paths;
	search_file(path, file_paths);
	
	std::vector<string>::iterator it = file_paths.begin();
	string command = "p4 add -d -c default ";
	while (it != file_paths.end())
	{
		if ((*it).find(".pdb") == string::npos)
		{
			//屏蔽pdb文件
			command.append("\"");
			command.append(*it);
			command.append("\"");
			command.append("\ ");
			index++;
			submit_num++;

			//std::cout << (*it) << '\n';
			if (index >= 50)
			{
				std::cout << "add "<<index<<" files\n";
				//每50个文件提交一次
				system(command.c_str());
				command = "p4 add -d -c default ";
				index = 0;
			}
		}
		else
		{
			ignore_num++;
		}
		it++;
	}

	if (index > 0)
	{
		std::cout << "last add "<<index<<" files\n";
		system(command.c_str());
	}

	std::cout << "****************** add files :" << submit_num << "; ignore files : " << ignore_num << "*******************\n";

}