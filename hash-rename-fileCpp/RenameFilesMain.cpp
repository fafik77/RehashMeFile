#include "RenameFilesMain.h"

using namespace std;

int RenameFilesMain::ParseCmdArgs(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i) {
		string arg(argv[i]);
		if (arg == "/?" || arg == "/help") {
			printf("Hash rename files by fafik77 on github. On 24.03.2026:\n "
				"/rehash\t by default files named with MD5 hash will not be renamed, this option will rename the file always\n"
				"/ext \"comma or space separated string\"\t provide file extensions to work on. Default is (png,jpg,jpeg,gif,tif,tiff)\n"
			);
			return 1;
		}
		else if (arg == "/rehash") {
			doNotRehash = false;
		}
		else if (arg == "/ext") {
			if (i + 1 < argc) {
				regex wordRegex("[a-zA-Z0-9_-]+");
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
				fileExtensionsSS << "\\.(";
				auto fileExtensionsPtr = fileExtensions.begin();
				while (fileExtensionsPtr != fileExtensions.end()) {
					fileExtensionsSS << *fileExtensionsPtr;
					++fileExtensionsPtr;
					if (fileExtensionsPtr == fileExtensions.end()) break;
					fileExtensionsSS << "|";
				}
				fileExtensionsSS << ")$";
				regexImage = regex(fileExtensionsSS.str(), regex_constants::syntax_option_type::icase);
			}
			else {
				fprintf(stderr, "Error: /ext requires a list of file extensions.\n");
				return 1;
			}
		}
	}
	return 0;
}