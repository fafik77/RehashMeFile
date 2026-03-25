#pragma once

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <string>
#include <functional>

/**
 * @brief Template class that explores directory tree.
 * @tparam T Type of the object instance stored and used by callbacks to invoke actions on persistent data.
 */
template <typename T>
class DirectoryExplorer
{
	T* instace;
public:
	/**
	 * @brief Makes the concrete implementation with instace that can persist your data,
	 * @param instace !!! the instance is managed by the caller, it can be a ref to self (this)
	 */
	DirectoryExplorer(T* instace):
		instace(instace)
	{ }
	
	using ActionOnFile = std::function<void(T& instance, const std::wstring& directoryPath, const WIN32_FIND_DATAW& data)>;

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
