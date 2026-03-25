#include <Windows.h>

struct AutoHandle
{
	HANDLE handle;
	AutoHandle(HANDLE handle):
		handle(handle)
	{}
	~AutoHandle() {
		Close();
	}
	operator HANDLE() const {
		return handle;			
	}
	BOOL Close() {
		BOOL retVal = false;
		if (handle != INVALID_HANDLE_VALUE) retVal = CloseHandle(handle);
		handle = INVALID_HANDLE_VALUE;
		return retVal;
	}
};