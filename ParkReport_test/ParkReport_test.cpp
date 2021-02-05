#include "stdafx.h"
#include "test_list.h"

int main(int argc, char* argv[])
{
	//if (true != test_application_evt_export_file())
	//{
	//	std::cout << "test_application_evt_export_file failed." << std::endl;
	//}

	//if (true != test_system_evt_export_file())
	//{
	//	std::cout << "test_system_evt_export_file failed." << std::endl;
	//}

	if (true != test_get_os_version())
	{
		std::cout << "test_get_os_version failed." << std::endl;
	}

	if (true != test_get_64bit_os())
	{
		std::cout << "test_get_64bit_os failed." << std::endl;
	}

	return 0;
}