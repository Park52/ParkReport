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

bool 
test_get_interface_info()
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
			printf("¢¹ Name: %s\n", p->AdapterName);
			printf("¢¹ Description: %s\n", p->Description);
			printf("¢¹ Type: %d\n", p->Type);
			//	 Âü°í: https://docs.microsoft.com/en-us/windows/win32/api/iptypes/ns-iptypes-ip_adapter_info?f1url=%3FappId%3DDev15IDEF1%26l%3DEN-US%26k%3Dk(IPTYPES%252FPIP_ADAPTER_INFO);k(PIP_ADAPTER_INFO);k(DevLang-C%252B%252B);k(TargetOS-Windows)%26rd%3Dtrue
			std::stringstream mac_addr;
			memset(imsi, NULL, sizeof(imsi));
			for (int i = 0; i < (int)p->AddressLength; i++)
			{
				if (i == p->AddressLength - 1)
					wsprintfA(imsi, "%02X", p->Address[i]);
				else
					wsprintfA(imsi, "%02X-", p->Address[i]);
				
				mac_addr << imsi ;
			}
			printf("¢¹ Hardware Address: %s\n", mac_addr.str().c_str());

			ns = &p->IpAddressList;
			while (ns)
			{
				printf("¢¹ Ip : %s\n", ns->IpAddress.String);
				printf("¢¹ Mask: %s\n", ns->IpMask.String);
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
test_get_disk_info()
{
	return true;
}

bool 
test_get_display_card_info()
{
	return true;
}
