#include "stdafx.h"
#include "gtest/gtest.h"

class SystemInfoCollector :public testing::Test
{
protected:
	SystemInfoCollector()
	{
	}

	virtual ~SystemInfoCollector()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(SystemInfoCollector, test_get_os_version)
{
	_mem_check_begin
	{
		HKEY key = nullptr;
		LONG res = ERROR_SUCCESS;
		DWORD dwType;
		DWORD dwBytes;
		char product_name[MAX_PATH];
		char release_id[MAX_PATH];

		res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						   L"software\\microsoft\\windows nt\\currentversion",
						   0,
						   KEY_ALL_ACCESS,
						   &key);
		ASSERT_TRUE(ERROR_SUCCESS == res);
		ASSERT_TRUE(nullptr != key);

		res = RegQueryValueEx(key,
							  L"ProductName",
							  0,
							  &dwType,
							  (LPBYTE)product_name,
							  &dwBytes);
		ASSERT_TRUE(ERROR_SUCCESS == res);

		res = RegQueryValueEx(key,
							  L"ReleaseId",
							  0,
							  &dwType,
							  (LPBYTE)release_id,
							  &dwBytes);
		ASSERT_TRUE(ERROR_SUCCESS == res);

		if (nullptr != key)
		{
			RegCloseKey(key);
			key = nullptr;
		}

	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_64bit_os)
{
	_mem_check_begin
	{
		typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
		LPFN_ISWOW64PROCESS fnIsWow64Process = nullptr;
		BOOL is_wow64 = FALSE;

		fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
		ASSERT_TRUE(nullptr != fnIsWow64Process);
		ASSERT_TRUE(TRUE == fnIsWow64Process(GetCurrentProcess(), &is_wow64));

		if (TRUE != is_wow64)
		{
			std::cout << "64bit operating system" << std::endl;
		}
		else
		{
			std::cout << "32bit operating system" << std::endl;
		}
	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_cpu_info)
{
	_mem_check_begin
	{
		HKEY key = nullptr;
		LONG res = ERROR_SUCCESS;
		DWORD dwType;
		DWORD dwBytes;
		wchar_t processor_name[MAX_PATH];

		res = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
						   L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
						   0,
						   KEY_ALL_ACCESS,
						   &key);
		ASSERT_TRUE(ERROR_SUCCESS == res);
		ASSERT_TRUE(nullptr != key);

		res = RegQueryValueEx(key,
								L"ProcessorNameString",
								0,
								&dwType,
								(LPBYTE)processor_name,
								&dwBytes);
		ASSERT_TRUE(ERROR_SUCCESS == res);

		if (nullptr != key)
		{
			RegCloseKey(key);
			key = nullptr;
		}
	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_memory_info)
{
	_mem_check_begin
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
	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_interface_info)
{
	_mem_check_begin
	{
		unsigned long stsize = sizeof(IP_ADAPTER_INFO);
		PIP_ADAPTER_INFO pIpAdapterInfo = nullptr;
		BYTE* pHeadIpAdapterInfo = new BYTE[stsize];
		DWORD dw = 0;
		IP_ADDR_STRING * ns = NULL;
		char imsi[10];
		pIpAdapterInfo = (PIP_ADAPTER_INFO)pHeadIpAdapterInfo;

		dw = ::GetAdaptersInfo(NULL, &stsize);

		if (dw == ERROR_BUFFER_OVERFLOW)
		{
			printf("Error Buffer Overflow\n");

			if (nullptr != pHeadIpAdapterInfo)
			{
				delete[] pHeadIpAdapterInfo;
				pHeadIpAdapterInfo = nullptr;
			}
			pHeadIpAdapterInfo = new BYTE[stsize];
			if (nullptr != pHeadIpAdapterInfo)
			{
				pIpAdapterInfo = (PIP_ADAPTER_INFO)pHeadIpAdapterInfo;
			}
		}

		ASSERT_TRUE(::GetAdaptersInfo(pIpAdapterInfo, &stsize) == ERROR_SUCCESS);

		while (nullptr != pIpAdapterInfo)
		{
			printf("Adapter [ %02d ]--------------------\n", pIpAdapterInfo->Index);
			printf("▷ Name: %s\n", pIpAdapterInfo->AdapterName);
			printf("▷ Description: %s\n", pIpAdapterInfo->Description);
			printf("▷ Type: %d\n", pIpAdapterInfo->Type);
			//	 참고: https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_info?f1url=%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(IPTYPES%252FPIP_ADAPTER_INFO);k(PIP_ADAPTER_INFO);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
			std::stringstream mac_addr;
			memset(imsi, NULL, sizeof(imsi));
			for (int i = 0; i < (int)pIpAdapterInfo->AddressLength; i++)
			{
				if (i == pIpAdapterInfo->AddressLength - 1)
					wsprintfA(imsi, "%02X", pIpAdapterInfo->Address[i]);
				else
					wsprintfA(imsi, "%02X-", pIpAdapterInfo->Address[i]);
				
				mac_addr << imsi ;
			}
			printf("▷ Hardware Address: %s\n", mac_addr.str().c_str());

			ns = &pIpAdapterInfo->IpAddressList;
			while (ns)
			{
				printf("▷ Ip : %s\n", ns->IpAddress.String);
				printf("▷ Mask: %s\n", ns->IpMask.String);
				ns = ns->Next;
			}

			printf("\n\n");
			pIpAdapterInfo = pIpAdapterInfo->Next;
		}

		if (nullptr != pHeadIpAdapterInfo)
		{
			delete[] pHeadIpAdapterInfo;
			pHeadIpAdapterInfo = nullptr;
		}
	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_disk_info)
{
	_mem_check_begin
	{
		HANDLE	hVolume = INVALID_HANDLE_VALUE;
		DWORD	RetnVal = 0;
		wchar_t	VolumeName[MAX_PATH + 1];
		wchar_t	Path[MAX_PATH + 1];

		// 버퍼 초기화
		ZeroMemory(VolumeName, (MAX_PATH + 1) * sizeof(TCHAR));
		ZeroMemory(Path, (MAX_PATH + 1) * sizeof(TCHAR));

		// 볼륨 이름 찾기 시작
		hVolume = FindFirstVolume(VolumeName, MAX_PATH);
		ASSERT_TRUE(hVolume != INVALID_HANDLE_VALUE);

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

	}_mem_check_end;
}

TEST_F(SystemInfoCollector, test_get_display_card_info)
{
	_mem_check_begin
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

		while (TRUE == EnumDisplayDevices(nullptr, deviceIndex, &dd, 0))
		{
			std::wstring devicename = dd.DeviceName;

			DISPLAY_DEVICE new_dd;
			new_dd.cb = sizeof(new_dd);
			int monitorIndex = 0;
			while (TRUE == EnumDisplayDevices(devicename.c_str(), monitorIndex, &new_dd, 0))
			{
				monitorIndex++;
				std::wcout << L"Device Name: " << new_dd.DeviceName << std::endl;
				std::wcout << L"Device String: " << new_dd.DeviceString << std::endl;
				std::wcout << L"Device ID: " << new_dd.DeviceID << std::endl;
				std::wcout << L"Device Key: " << new_dd.DeviceKey << std::endl;
			}
			deviceIndex++;
		}
	}_mem_check_end;
}
