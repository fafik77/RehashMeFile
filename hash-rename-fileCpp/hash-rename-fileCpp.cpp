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
	Hash::MD5 md5, test, test2;
	//auto res = md5.fileToHash(R"(V:\a\b4abc8a33be779e9fc4c2c740462c580.jpg)");	//b4abc8a33be779e9fc4c2c740462c580
	//printf("%s", res.c_str());

	std::string testStr = "test";
	test.update(&testStr.front(), testStr.size());
	auto res2 = test.FinalizeHash();	//098f6bcd4621d373cade4e832627b4f6

	test2.update(&testStr.at(0), 1);
	test2.update(&testStr.at(1), 1);
	test2.update(&testStr.at(2), 1);
	test2.update(&testStr.at(3), 1);
	auto res3 = test2.FinalizeHash();	//098f6bcd4621d373cade4e832627b4f6
}


//void FindFilesIn(const std::wstring& path)
//{
//	WIN32_FIND_DATA data;
//	HANDLE hFind = FindFirstFileW(path.c_str(), &data);      // DIRECTORY
//
//}
