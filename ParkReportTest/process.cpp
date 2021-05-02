#include "stdafx.h"
#include "process.h"

bool 
get_my_process_name(
	_Out_ std::wstring& process_name)
{
	wchar_t buffer[MAX_PATH] = {};
	DWORD buffer_size = MAX_PATH;

	if (TRUE != QueryFullProcessImageNameW(GetCurrentProcess(),
										   0,
										   buffer,
										   &buffer_size))
	{
		return false;
	}

	process_name = std::wstring(buffer);
	return true;
}
