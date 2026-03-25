#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <functional>

// Examples:
// Count matching files in directory
//  int sum = 0;
//  DirectoryExplorer<int> dirExpInt(&sum);
//  dirExpInt.GetFilesIn(L".", 0, [](int& sum, const std::wstring& directoryPath, const WIN32_FIND_DATAW& data) {sum++;});
// Use own class to implement actuall logic:
//  DirectoryExplorer<RenameFilesMain> dirExp(this);
//  dirExp.GetFilesIn(L".", 0, &RenameFilesMain::ProcessFile);
//   where: void RenameFilesMain.ProcessFile(const std::wstring& directoryPath, const WIN32_FIND_DATAW& data);
// 


/**
 * @brief Template class that explores directory tree.
 * @tparam T Type of the object instance stored and used by callbacks to invoke actions on persistent data. It can be your Class with methods to handle `ActionOnFile`
 */
template <typename T>
class DirectoryExplorer
{
	T* instace;
public:
	/**
	 * @brief init with instace that can persist your data.
	 * @param instace !!! the instance is managed by the caller, it can be a ref to self (this)
	 */
	DirectoryExplorer(T* instace):
		instace(instace)
	{ }
	/**
	 * @brief Action to perform on each file in GetFilesIn
	 */
	using ActionOnFile = std::function<void(T& instance, const std::wstring& directoryPath, const WIN32_FIND_DATAW& data)>;

	/**
	 * @brief Get all items (Files and Folders) inside a directory, run actionOnFile on each File(only)
	 * @param path -start at given path, should be path only without slash at the end and no wildcards
	 * @param recursiveDepth -enter sub directories up to said depth(stops at == 0)
	 * @param actionOnFile -perform action on file
	 */
	void GetFilesIn(const std::wstring& path, const int recursiveDepth = 0, ActionOnFile actionOnFile = nullptr);
};

template <typename T>
void DirectoryExplorer<T>::GetFilesIn(const std::wstring& path, const int recursiveDepth, ActionOnFile actionOnFile)
{
	WIN32_FIND_DATAW data;
	HANDLE hFind = FindFirstFileW((path + L"\\*.*").c_str(), &data);	// DIRECTORY
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFile failed (%d)\n", GetLastError());
		return;
	}
	FindNextFileW(hFind, &data);	//this removes the "." and ".." that appear to be always first (as they dont fully exist)
	while (FindNextFileW(hFind, &data)) {
		if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { // it is a Dir
			if (recursiveDepth == 0) continue;
			if (data.dwFileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) { // it is a Link to a Dir
				//wprintf(L"! Link to Dir: %s\n", data.cFileName);
			}
			//wprintf(L"Entering Dir: %s\n", data.cFileName);
			GetFilesIn(path + L"\\" + data.cFileName, recursiveDepth - 1);
			//wprintf(L"Leaving Dir: %s\n", data.cFileName);
			continue;
		}
		//it is a File of some sort
		if (actionOnFile) actionOnFile(*instace, path, data);
		//wprintf(L"File: %s\n", data.cFileName);
	}
	FindClose(hFind);
}
