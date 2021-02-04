#include "stdafx.h"
#include "test_list.h"

int main(int argc, char* argv[])
{
	if (true != test_application_evt_export_file())
	{
		std::cout << "test_application_evt_export_file failed." << std::endl;
	}

	return 0;
}