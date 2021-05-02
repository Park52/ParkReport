#ifndef STDAFX_H
#define STDAFX_H

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>

#include <winevt.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "wevtapi.lib")

//
//	Network interface header
//
#include <Iprtrmib.h>
#include <TlHelp32.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#endif
