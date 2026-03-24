/// Author: fafik77 on github
/// This program renames files after their MD5 hash, there is an extension filter and a check to not rename already hash named files.
/// This program comes with no warranty and is presented as is.
/// License: GNU 3.0.  Publish of modified version of this software under the original Authors name is not allowed.
/// 
/// hash-rename-fileCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
///

#include "RenameFilesMain.h"
#include "quickdigest5.hpp"


int main(int argc, char* argv[])
{
	RenameFilesMain renameFiles(argc, argv);
	Hash::MD5 md5;
	auto res = md5.fileToHash(R"(V:\a\b4abc8a33be779e9fc4c2c740462c580.jpg)");
	printf("%s", res.c_str());
}


//void FindFilesIn(const std::wstring& path)
//{
//	WIN32_FIND_DATA data;
//	HANDLE hFind = FindFirstFileW(path.c_str(), &data);      // DIRECTORY
//
//}
