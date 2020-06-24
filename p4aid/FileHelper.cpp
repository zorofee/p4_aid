#include <iostream>
#include <sstream>
#include <fstream>

#include <string>
#include <io.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

using namespace std;

void search_file(string path,vector<string>& file_paths)
{
	long hFile = 0;
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

static int index = 0;
static int submit_num = 0;
static int ignore_num = 0;
void start_files_search(const char* path)
{
	vector<string> file_paths;
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

			std::cout << (*it) << '\n';
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