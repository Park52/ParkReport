#include "stdafx.h"
#include "CppUnitTest.h"

#include "ExportEvtFile.h"
#include "SystemInfo.h"

#include "File.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CommonLib
{
	TEST_CLASS(ExportEvent)
	{
	public:
		
		TEST_METHOD(Test_ExportEvtFile1)
		{
			Assert::IsTrue(application_evt_export_file());
			Assert::IsTrue(is_file_exist(L".\\application_error_log.evtx"));
		}

		TEST_METHOD(Test_ExportEvtFile2)
		{
			Assert::IsTrue(system_evt_export_file());
			Assert::IsTrue(is_file_exist(L".\\system_log.evtx"));
		}
	};

	TEST_CLASS(Systeminfo)
	{
	public:
		TEST_METHOD(Test_GetOSVersion)
		{
			Assert::IsTrue(get_os_version());
		}
		TEST_METHOD(Test_Get64bitos)
		{
			Assert::IsTrue(get_64bit_os());
		}
		TEST_METHOD(Test_GetCpuinfo)
		{
			Assert::IsTrue(get_cpu_info());
		}
		TEST_METHOD(Test_GetMeminfo)
		{
			Assert::IsTrue(get_memory_info());
		}
		TEST_METHOD(Test_Getinterface)
		{
			Assert::IsTrue(get_interface_info());
		}
		TEST_METHOD(Test_Getdiskinfo)
		{
			Assert::IsTrue(get_disk_info());
		}
		TEST_METHOD(Test_Getdisplayinfo)
		{
			Assert::IsTrue(get_display_card_info());
		}

	};
}
