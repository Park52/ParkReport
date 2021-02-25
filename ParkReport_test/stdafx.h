#pragma once

#include <iostream>
#include <sstream>
#include <Windows.h>
#pragma comment(lib, "user32.lib")
#include <winevt.h>
#pragma comment(lib, "wevtapi.lib")
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <tchar.h>
#include <strsafe.h>

//
//	Network interface header
//
#include <Iprtrmib.h>
#include <TlHelp32.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#include <json/json.h>

//	Google test macro
#define _mem_check_begin \
	_CrtMemState memoryState = { 0 }; \
	_CrtMemCheckpoint(&memoryState); 

#define _mem_check_break(ord) \
	_CrtSetBreakAlloc(ord);

#define _mem_check_end \
	_CrtMemDumpAllObjectsSince(&memoryState)
