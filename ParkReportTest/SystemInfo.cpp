#include "stdafx.h"
#include "SystemInfo.h"

bool
get_os_version()
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
get_64bit_os()
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
get_cpu_info()
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
get_memory_info()
{
	MEMORYSTATUSEX   mem;
	mem.dwLength = sizeof(mem);

	GlobalMemoryStatusEx(&mem);

	_tprintf(_T("There is  %ld percent of memory in use.\n"), mem.dwMemoryLoad);
	_tprintf(_T("There are %I64d total MB of physical memory.\n"), mem.ullTotalPhys / (1024 * 1024));
	_tprintf(_T("There are %I64d free  MB of physical memory.\n"), mem.ullAvailPhys / (1024 * 1024));
	_tprintf(_T("There are %I64d total MB of paging file.\n"), mem.ullTotalPageFile / (1024 * 1024));
	_tprintf(_T("There are %I64d free  MB of paging file.\n"), mem.ullAvailPageFile / (1024 * 1024));
	_tprintf(_T("There are %I64d total MB of virtual memory.\n"), mem.ullTotalVirtual / (1024 * 1024));
	_tprintf(_T("There are %I64d free  MB of virtual memory.\n"), mem.ullAvailVirtual / (1024 * 1024));

	_tprintf(_T("There are %I64d free  MB of extended memory.\n"), mem.ullAvailExtendedVirtual / (1024 * 1024));

	return true;
}

bool
get_interface_info()
{
	PIP_ADAPTER_INFO p = NULL;
	ULONG len = 0;
	DWORD dw = 0;
	IP_ADDR_STRING * ns = NULL;
	char imsi[10];

	len = sizeof(IP_ADAPTER_INFO);
	p = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));

	dw = ::GetAdaptersInfo(NULL, &len);

	if (dw == ERROR_BUFFER_OVERFLOW)
	{
		printf("Error Buffer Overflow\n");

		free(p);
		p = (IP_ADAPTER_INFO*)malloc(len);
	}


	if (dw = ::GetAdaptersInfo(p, &len) == ERROR_SUCCESS)
	{
		while (p)
		{
			printf("Adapter [ %02d ]--------------------\n", p->Index);
			printf("▷ Name: %s\n", p->AdapterName);
			printf("▷ Description: %s\n", p->Description);
			printf("▷ Type: %d\n", p->Type);
			//	 참고: https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_info?f1url=%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(IPTYPES%252FPIP_ADAPTER_INFO);k(PIP_ADAPTER_INFO);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
			std::stringstream mac_addr;
			memset(imsi, NULL, sizeof(imsi));
			for (int i = 0; i < (int)p->AddressLength; i++)
			{
				if (i == p->AddressLength - 1)
					wsprintfA(imsi, "%02X", p->Address[i]);
				else
					wsprintfA(imsi, "%02X-", p->Address[i]);

				mac_addr << imsi;
			}
			printf("▷ Hardware Address: %s\n", mac_addr.str().c_str());

			ns = &p->IpAddressList;
			while (ns)
			{
				printf("▷ Ip : %s\n", ns->IpAddress.String);
				printf("▷ Mask: %s\n", ns->IpMask.String);
				ns = ns->Next;
			}
			printf("\n\n");
			p = p->Next;
		}
	}
	else
	{
		printf("Call to GetAdaptersInfo failed.\n");
		LPVOID lpMsgBuf;
		if (FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL)) {
			printf("\tError: %s", lpMsgBuf);
		}
		LocalFree(lpMsgBuf);
	}

	free(p);
	return true;
}

bool
get_disk_info()
{
	HANDLE	hVolume = NULL;
	DWORD	RetnVal = 0;
	wchar_t	VolumeName[MAX_PATH + 1];
	wchar_t	Path[MAX_PATH + 1];

	// 버퍼 초기화
	ZeroMemory(VolumeName, (MAX_PATH + 1) * sizeof(TCHAR));
	ZeroMemory(Path, (MAX_PATH + 1) * sizeof(TCHAR));

	// 볼륨 이름 찾기 시작
	hVolume = FindFirstVolume(VolumeName, MAX_PATH);
	if (hVolume == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	GetVolumePathNamesForVolumeName(VolumeName, Path, MAX_PATH, &RetnVal);
	if (_wcsicmp(Path, L"") != 0)
	{
		wprintf(L"%s\n", Path);
		ULARGE_INTEGER avail_memory, total_memory, free_memory;

		memset(&avail_memory, 0, sizeof(avail_memory));
		memset(&total_memory, 0, sizeof(total_memory));
		memset(&free_memory, 0, sizeof(free_memory));

		GetDiskFreeSpaceEx(Path, &avail_memory, &total_memory, &free_memory);

		//MB단위로 환산
		LONGLONG nDiskFree = (UINT)(free_memory.QuadPart / 1024 / 1024 / 1024);
		LONGLONG nDiskTotal = (UINT)(total_memory.QuadPart / 1024 / 1024 / 1024);

		wprintf(L"Total disk size : %llu GB\n", nDiskTotal);
		wprintf(L"Free disk size : %llu GB\n", nDiskFree);
	}

	// 버퍼 초기화
	ZeroMemory(VolumeName, (MAX_PATH + 1) * sizeof(TCHAR));
	ZeroMemory(Path, (MAX_PATH + 1) * sizeof(TCHAR));

	// 반복
	while (FindNextVolume(hVolume, VolumeName, MAX_PATH))
	{
		GetVolumePathNamesForVolumeName(VolumeName, Path, MAX_PATH, &RetnVal);
		if (_wcsicmp(Path, L"") != 0)
		{
			wprintf(L"%s\n", Path);
			ULARGE_INTEGER avail_memory, total_memory, free_memory;

			memset(&avail_memory, 0, sizeof(avail_memory));
			memset(&total_memory, 0, sizeof(total_memory));
			memset(&free_memory, 0, sizeof(free_memory));

			GetDiskFreeSpaceEx(Path, &avail_memory, &total_memory, &free_memory);

			//MB단위로 환산
			LONGLONG nDiskFree = (UINT)(free_memory.QuadPart / 1024 / 1024 / 1024);
			LONGLONG nDiskTotal = (UINT)(total_memory.QuadPart / 1024 / 1024 / 1024);

			wprintf(L"Total disk size : %llu GB\n", nDiskTotal);
			wprintf(L"Free disk size : %llu GB\n", nDiskFree);
		}
		ZeroMemory(Path, (MAX_PATH + 1) * sizeof(TCHAR));
		ZeroMemory(VolumeName, (MAX_PATH + 1) * sizeof(TCHAR));

	}

	// 볼륨 찾기 종료
	FindVolumeClose(hVolume);

	// 핸들 초기화
	hVolume = NULL;
	RetnVal = 0;

	return true;
}

bool
get_display_card_info()
{
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(dd);
	int deviceIndex = 0;
	if (TRUE == EnumDisplayDevices(nullptr, deviceIndex, &dd, 0))
	{
		std::wcout << L"Device Name: " << dd.DeviceName << std::endl;
		std::wcout << L"Device String: " << dd.DeviceString << std::endl;
		std::wcout << L"Device ID: " << dd.DeviceID << std::endl;
		std::wcout << L"Device Key: " << dd.DeviceKey << std::endl;
	}
	//while (TRUE == EnumDisplayDevices(nullptr, deviceIndex, &dd, 0))
	//{
		//std::wstring devicename = dd.DeviceName;

		//DISPLAY_DEVICE new_dd;
		//new_dd.cb = sizeof(new_dd);
		//int monitorIndex = 0;
		//while (TRUE == EnumDisplayDevices(devicename.c_str(), monitorIndex, &new_dd, 0))
		//{
		//	monitorIndex++;
		//	std::wcout << L"Device Name: " << new_dd.DeviceName << std::endl;
		//	std::wcout << L"Device String: " << new_dd.DeviceString << std::endl;
		//	std::wcout << L"Device ID: " << new_dd.DeviceID << std::endl;
		//	std::wcout << L"Device Key: " << new_dd.DeviceKey << std::endl;
		//}
		//deviceIndex++;
	//}

	return true;
}