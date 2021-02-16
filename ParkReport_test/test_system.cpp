#include "stdafx.h"
#include "test_list.h"

bool 
test_get_os_version()
{
	HKEY key = NULL;
	LONG res = ERROR_SUCCESS;
	DWORD dwType;
	DWORD dwBytes;
	wchar_t product_name[MAX_PATH];
	wchar_t release_id[MAX_PATH];

	res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					   L"software\\microsoft\\windows nt\\currentversion",
					   0,
					   KEY_ALL_ACCESS,
					   &key);
	if (ERROR_SUCCESS != res)
	{
		std::cout << "RegOpenKeyEx Failed. Registry path: "
			<< "HKLM\\software\\microsoft\\windows nt\\currentversion "
			<< std::endl
			<< "GetLastError :"
			<< res
			<< std::endl;
		return false;
	}

	do
	{
		res = RegQueryValueEx(key,
							  L"ProductName",
							  0,
							  &dwType,
							  (LPBYTE)product_name,
							  &dwBytes);
		if (ERROR_SUCCESS != res)
		{
			std::cout << "RegQueryValueEx Failed. Registry Name "
				<< "ProductName"
				<< "GetLastError :"
				<< res
				<< std::endl;
			break;
		}

		res = RegQueryValueEx(key,
							  L"ReleaseId",
							  0,
							  &dwType,
							  (LPBYTE)release_id,
							  &dwBytes);
		if (ERROR_SUCCESS != res)
		{
			std::cout << "RegQueryValueEx Failed. Registry Name "
				<< "ReleaseId"
				<< "GetLastError :"
				<< res
				<< std::endl;
			break;
		}

	} while (false);

	if (NULL != key)
	{
		RegCloseKey(key);
		key = NULL;
	}

	if (ERROR_SUCCESS != res)
	{
		return false;
	}

	std::wcout 
		<< L"Product Name: " << product_name
		<< L" Release Id: " << release_id
		<< std::endl;

	return true;
}

bool 
test_get_64bit_os()
{
	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	BOOL is_wow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
	if (NULL != fnIsWow64Process)
	{
		if (TRUE != fnIsWow64Process(GetCurrentProcess(), &is_wow64))
		{
			std::cout << "fnIsWow64Process failed." << std::endl;
			return false;
		}
	}

	if (TRUE != is_wow64)
	{
		std::cout << "64bit operating system" << std::endl;
	}
	else
	{
		std::cout << "32bit operating system" << std::endl;
	}
	return true;
}

bool 
test_get_cpu_info()
{
	HKEY key = NULL;
	LONG res = ERROR_SUCCESS;
	DWORD dwType;
	DWORD dwBytes;
	wchar_t processor_name[MAX_PATH];

	res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
					   L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
					   0,
					   KEY_ALL_ACCESS,
					   &key);
	if (ERROR_SUCCESS != res)
	{
		std::cout << "RegOpenKeyEx Failed. Registry path: "
			<< "HKLM\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
			<< std::endl
			<< "GetLastError :"
			<< res
			<< std::endl;
		return false;
	}

	do
	{
		res = RegQueryValueEx(key,
							  L"ProcessorNameString",
							  0,
							  &dwType,
							  (LPBYTE)processor_name,
							  &dwBytes);
		if (ERROR_SUCCESS != res)
		{
			std::cout << "RegQueryValueEx Failed. Registry Name "
				<< "ProcessorNameString "
				<< "GetLastError :"
				<< res
				<< std::endl;
			break;
		}

		std::wcout << "Processor Name: " << processor_name << std::endl;
	} while (false);

	if (NULL != key)
	{
		RegCloseKey(key);
		key = NULL;
	}

	if (ERROR_SUCCESS != res)
	{
		return false;
	}

	return true;
}

bool 
test_get_memory_info()
{
	MEMORYSTATUSEX   mem;
	mem.dwLength = sizeof(mem);

	GlobalMemoryStatusEx(&mem);

	_tprintf(_T("There is  %ld percent of memory in use.\n"), mem.dwMemoryLoad);
	_tprintf(_T("There are %I64d total MB of physical memory.\n"), mem.ullTotalPhys/(1024*1024));
	_tprintf(_T("There are %I64d free  MB of physical memory.\n"), mem.ullAvailPhys/ (1024 * 1024));
	_tprintf(_T("There are %I64d total MB of paging file.\n"), mem.ullTotalPageFile/ (1024 * 1024));
	_tprintf(_T("There are %I64d free  MB of paging file.\n"), mem.ullAvailPageFile/ (1024 * 1024));
	_tprintf(_T("There are %I64d total MB of virtual memory.\n"), mem.ullTotalVirtual/ (1024 * 1024));
	_tprintf(_T("There are %I64d free  MB of virtual memory.\n"), mem.ullAvailVirtual/ (1024 * 1024));

	_tprintf(_T("There are %I64d free  MB of extended memory.\n"), mem.ullAvailExtendedVirtual / (1024 * 1024));

	return true;
}