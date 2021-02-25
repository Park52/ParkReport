#include "stdafx.h"
#include "gtest/gtest.h"

///	@brief
class EventCollector :public testing::Test
{
protected:
	EventCollector()
	{
	}

	virtual ~EventCollector()
	{
	}

	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

TEST_F(EventCollector, test_application_evt_export_file)
{
	_mem_check_begin
	{
		std::wstring snapshot_log_path = L".\\log.evtx";
		// Export all the events in the specified channel to the target log file.
		ASSERT_TRUE(TRUE == EvtExportLog(NULL,
										 L"Application",
										 L"*",
										 snapshot_log_path.c_str(),
										 EvtExportLogChannelPath));

		std::wstring query_log_path = L".\\application_error_log.evtx";
		std::wstring query_string = L"Event/System[EventID=1000] and Event/System[Provider[@Name='Application Error']]";
		ASSERT_TRUE(TRUE == EvtExportLog(NULL,
										 snapshot_log_path.c_str(),
										 query_string.c_str(),
										 query_log_path.c_str(),
										 EvtExportLogFilePath));

		DeleteFile(snapshot_log_path.c_str());
		DeleteFile(query_log_path.c_str());

		ASSERT_TRUE(TRUE != PathFileExists(snapshot_log_path.c_str()));
		ASSERT_TRUE(TRUE != PathFileExists(query_log_path.c_str()));
	}
	_mem_check_end;
}

TEST_F(EventCollector, test_system_evt_export_file)
{
	_mem_check_begin
	{
		std::wstring snapshot_log_path = L".\\log.evtx";
		// Export all the events in the specified channel to the target log file.
		ASSERT_TRUE(TRUE == EvtExportLog(NULL,
										 L"System",
										 L"*",
										 snapshot_log_path.c_str(),
										 EvtExportLogChannelPath));

		std::wstring query_log_path = L".\\system_log.evtx";
		std::wstring query_string = L"Event/System[EventID=1 or EventID=6 or EventID=7045] and Event/System[Provider[@Name='Service Control Manager' or @Name='Microsoft-Windows-FilterManager']]";
		ASSERT_TRUE(TRUE == EvtExportLog(NULL,
										 snapshot_log_path.c_str(),
										 query_string.c_str(),
										 query_log_path.c_str(),
										 EvtExportLogFilePath));

		DeleteFile(snapshot_log_path.c_str());
		DeleteFile(query_log_path.c_str());

		ASSERT_TRUE(TRUE != PathFileExists(snapshot_log_path.c_str()));
		ASSERT_TRUE(TRUE != PathFileExists(query_log_path.c_str()));
	}
	_mem_check_end;
}