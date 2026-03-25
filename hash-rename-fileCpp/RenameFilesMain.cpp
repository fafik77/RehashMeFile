#include "RenameFilesMain.h"
#include "quickdigest5.hpp"
#include "AutoHandle.hpp"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


int RenameFilesMain::ParseCmdArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i) {
		string arg(argv[i]);
		if (arg == "/?" || arg == "/help") {
			printf("Hash rename files by fafik77 on github. On 24.03.2026:\n"
				" /rehash\t -by default files named with MD5 hash will not be renamed, this option will rename the file always\n"
				" /r\t\t -recurse all subfolders\n"
				" /depth <int>\t -recurse subfolders up to depth\n"
				" /ext \"str\"\t -provide file extensions to work on. Default is \"png,jpg,jpeg,gif,tif,tiff\"\n"
			);
			return 1;
		}
		else if (arg == "/rehash") {
			doNotRehash = false;
		}
		else if (arg == "/r") {
			scanDepth = -1;
		}
		else if (arg == "/depth") {
			if (!(i + 1 < argc)) {
				fprintf(stderr, "Error: /depth requires a number but none was provided.");
				return 2;
			}
			string ArgStr(argv[++i]);
			if (!std::all_of(ArgStr.begin(), ArgStr.end(), ::isdigit)) {
				fprintf(stderr, "Error: /depth requires a number. \"%s\" is not a valid number.\n", ArgStr.c_str());
				return 1;
			}
			scanDepth = stoi(ArgStr);
		}

		else if (arg == "/ext") {
			if (i + 1 < argc) {
				regex wordRegex("[a-zA-Z0-9_!@#$~-]+");
				string extArgStr(argv[++i]);
				vector<string> fileExtensions;

				for (smatch sm; regex_search(extArgStr, sm, wordRegex);)
				{
					fileExtensions.push_back(sm.str());
					extArgStr = sm.suffix();
				}

				if (fileExtensions.empty()) {
					fprintf(stderr, "Error: /ext requires a list of file extensions.\n  The provided \"%s\" does not qualify.\n", extArgStr.c_str());
					return 2;
				}

				stringstream fileExtensionsSS;
				fileExtensionsSS << ".*\\.(";
				auto fileExtensionsPtr = fileExtensions.begin();
				while (fileExtensionsPtr != fileExtensions.end()) {
					fileExtensionsSS << *fileExtensionsPtr;
					++fileExtensionsPtr;
					if (fileExtensionsPtr == fileExtensions.end()) break;
					fileExtensionsSS << "|";
				}
				fileExtensionsSS << ")$";
				auto str = fileExtensionsSS.str();
				const std::wstring ws(str.begin(), str.end());
				regexExtensionMatch = wregex(ws, regex_constants::syntax_option_type::icase);
			}
			else {
				fprintf(stderr, "Error: /ext requires a list of file extensions.\n");
				return 1;
			}
		}
	}
	return 0;
}

static const uint64_t MB = 1 << 20;

void RenameFilesMain::ProcessFile(const std::wstring& directoryPath, const WIN32_FIND_DATAW& data)
{
	std::wstring fileName(data.cFileName);
	if (fileName.empty()) throw std::exception("File name is empty, that is illegal. Failed to assign?");
	if (doNotRehash && regex_match(fileName, regexMd5Named)) //already hash named and "no rehash" is selected
		return;
	if (!regex_match(fileName, regexExtensionMatch)) //not our extension
		return;

	
	threadPool.enqueue([directoryPath, fileName]() {	//capture a copy of those to process in pararell
		Hash::MD5 md5;
		fs::path pathOldName(directoryPath);
		pathOldName.append(fileName);
		AutoHandle hFile = CreateFileW(pathOldName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return;
		
		vector<uint8_t> buf(MB, 0x00);
		DWORD read;
		while (ReadFile(hFile, &buf.front(), MB, &read, NULL) && read != 0) {
			md5.update(&buf.front(), read);
		}
		hFile.Close();	//we have to close it to rename it
		auto hash = md5.FinalizeHash();
		wstring hashW(hash.begin(), hash.end());
		fs::path pathNewName(directoryPath);
		pathNewName.append(hashW + pathOldName.extension().wstring());
		fs::rename(pathOldName, pathNewName);
	});
	wprintf(L"File: %s\n", fileName.c_str());
}
