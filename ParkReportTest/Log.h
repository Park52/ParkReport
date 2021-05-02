#pragma once
class Log
{
public:
	Log();
	virtual ~Log();

public:
	bool initialize();
	void finalize();

private:
	bool _initialized;

	HANDLE _log_file;
	std::wstringstream _log_file_path;
};

