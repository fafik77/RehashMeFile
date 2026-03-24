#pragma once

#include <string>
#include <regex>
#include <vector>
#include <sstream>

//#include <windows.h>
//#include <tchar.h>
//#include <strsafe.h>
//#include <fstream>

class RenameFilesMain
{
	std::regex regexNotMd5 = std::regex("^[0-9a-zA-Z]{32}\\..*");
	std::regex regexImage = std::regex("\\.(png|jpg|jpeg|gif|tif|tiff)$", std::regex_constants::syntax_option_type::icase);
	bool doNotRehash = true;
	int ParseCmdArgs(int argc, char* argv[]);

public:
	RenameFilesMain(int argc, char* argv[]) {
		int retVal = ParseCmdArgs(argc, argv);
		if (retVal) exit(retVal);
	}
	
};

