/// Author: fafik77 on github
/// This program renames files after their MD5 hash, there is an extension filter and a check to not rename already hash named files.
/// This program comes with no warranty and is presented as is.
/// License: GNU 3.0.  Publish of modified version of this software under the original Authors name is not allowed.
/// 
/// hash-rename-fileCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
///

#include "RenameFilesMain.h"
#include "DirectoryExplorer.h"


int main(int argc, char* argv[])
{
	RenameFilesMain renameFiles(argc, argv);
	DirectoryExplorer dirExp;
	dirExp.GetFilesIn<int>(L".", 0);
}

