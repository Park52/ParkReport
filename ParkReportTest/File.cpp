#include "stdafx.h"
#include "File.h"

bool
is_file_exist(_In_ const char* path)
{
	if (TRUE != PathFileExistsA(path))
	{
		return false;
	}

	return true;
}

bool
is_file_exist(_In_ const wchar_t* path)
{
	if (TRUE != PathFileExistsW(path))
	{
		return false;
	}

	return true;
}