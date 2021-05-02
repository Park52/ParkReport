#include "stdafx.h"
#include "Log.h"

#include "File.h"

Log::Log()
	:
	_initialized(false),
	_log_file(INVALID_HANDLE_VALUE)
{
}

Log::~Log()
{
}

bool 
Log::initialize()
{
	if(_initialized)
	{ 
		return true;
	}

	
	_initialized = true;
	return true;
}

void Log::finalize()
{
}
