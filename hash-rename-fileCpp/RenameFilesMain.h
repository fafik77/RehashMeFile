#pragma once

#include <regex>
#include <sstream>
#include "DirectoryExplorer.hpp"

/**
 * @brief This class is the main point for discovering and filtering files to be renamed by their MD5 hash
 */
class RenameFilesMain
{
//Data
	/**
	 * @brief The Md5 hash as 32 hex chars in file name
	 */
	std::wregex regexMd5Named = std::wregex(L"^[0-9a-zA-Z]{32}\\..*");
	/**
	 * @brief Extensions to process, by default: Images
	 */
	std::wregex regexExtensionMatch = std::wregex(L".*\\.(png|jpg|jpeg|gif|tif|tiff)$", std::regex_constants::syntax_option_type::icase);
	bool doNotRehash = true;
	int scanDepth = 0;


	/**
	 * @brief Parses Cmd args and prepares the enviroment
	 * @param argc 
	 * @param argv 
	 * @return status code to exit if its non Zero
	 */
	int ParseCmdArgs(int argc, char* argv[]);
	/**
	 * @brief The callback action from DirectoryExplorer.GetFilesIn() that handles each file
	 * @param directoryPath 
	 * @param data 
	 */
	void ProcessFile(const std::wstring& directoryPath, const WIN32_FIND_DATAW& data);

public:
	RenameFilesMain(int argc, char* argv[]) {
		int retVal = ParseCmdArgs(argc, argv);
		if (retVal) exit(retVal);
	}


	void run() {
		DirectoryExplorer<RenameFilesMain> dirExp(this);
		dirExp.GetFilesIn(L".", scanDepth, &RenameFilesMain::ProcessFile);
	}
};

