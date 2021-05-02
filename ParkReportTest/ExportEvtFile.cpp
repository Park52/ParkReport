#include "stdafx.h"
#include "ExportEvtFile.h"

bool
application_evt_export_file()
{
	std::wstring snapshot_log_path = L".\\log.evtx";

	if (TRUE != EvtExportLog(NULL,
							 L"Application",
							 L"*",
							 snapshot_log_path.c_str(),
							 EvtExportLogChannelPath))
	{
		wprintf(L"EvtExportLog failed for initial export with %lu.\n", GetLastError());
		return false;
	}

	std::wstring query_log_path = L".\\application_error_log.evtx";
	std::wstring query_string = L"Event/System[EventID=1000] and Event/System[Provider[@Name='Application Error']]";
	if (TRUE != EvtExportLog(NULL,
							 snapshot_log_path.c_str(),
							 query_string.c_str(),
							 query_log_path.c_str(),
							 EvtExportLogFilePath))
	{
		wprintf(L"EvtExportLog failed for relog with %lu.\n", GetLastError());
		return false;
	}

	DeleteFile(snapshot_log_path.c_str());

	return true;
}

bool
system_evt_export_file()
{
	std::wstring snapshot_log_path = L".\\log.evtx";
	
	if (TRUE != EvtExportLog(NULL,
							 L"System",
							 L"*",
							 snapshot_log_path.c_str(),
							 EvtExportLogChannelPath))
	{
		wprintf(L"EvtExportLog failed for initial export with %lu.\n", GetLastError());
		return false;
	}

	std::wstring query_log_path = L".\\system_log.evtx";
	std::wstring query_string = L"Event/System[EventID=1 or EventID=6 or EventID=7045] and Event/System[Provider[@Name='Service Control Manager' or @Name='Microsoft-Windows-FilterManager']]";
	if (TRUE != EvtExportLog(NULL,
							 snapshot_log_path.c_str(),
							 query_string.c_str(),
							 query_log_path.c_str(),
							 EvtExportLogFilePath))
	{
		wprintf(L"EvtExportLog failed for relog with %lu.\n", GetLastError());
		return false;
	}

	DeleteFile(snapshot_log_path.c_str());

	return true;
		DeleteFile(snapshot_log_path.c_str());
		DeleteFile(query_log_path.c_str());

		ASSERT_TRUE(TRUE != PathFileExists(snapshot_log_path.c_str()));
		ASSERT_TRUE(TRUE != PathFileExists(query_log_path.c_str()));
	}
	_mem_check_end;
}