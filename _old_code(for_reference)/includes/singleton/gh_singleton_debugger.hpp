#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>

class Debugger
{
public:
	Debugger(const Debugger&) = delete;

	static Debugger& Get()
	{
		static Debugger INSTANCE;
		return INSTANCE;
	}

	bool GetInitialized() const { return initialized; }
	bool GetDebugEnabled() const { return debug_enabled; }

	int EnableDebug()
	{
		if (GetDebugEnabled())
			return 1;

		debug_enabled = true;

		if (!GetInitialized())
			start_debugger();

		return 0;
	}

	int DisableDebug()
	{
		if (!GetDebugEnabled())
			return 1;

		debug_enabled = false;

		if (GetInitialized())
			end_debugger();

		return 0;
	}

	void DebugTimeStart()
	{
		if (debug_enabled)
			debug_timepoint = std::chrono::high_resolution_clock::now();
	}

	long DebugTimeGetElapsedTime() const
	{
		if (debug_enabled)
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() -
																		debug_timepoint)
				.count();
		return 0;
	}

	void Log(const std::string& message)
	{
		if (GetDebugEnabled())
		{
			auto now = get_current_tm();

			const int hour   = now.tm_hour;
			const int minute = now.tm_min;
			const int second = now.tm_sec;

			log_file << std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second) + "   \t- " +
							message + "\n";

			log_file.flush();
		}
	}

	void LogErr(const std::string& message) { Log("!ERROR! - " + message); }

private:
	Debugger() = default;

	bool debug_enabled = false;
	bool initialized   = false;
	std::ofstream log_file;

	std::chrono::_V2::system_clock::time_point debug_timepoint;

	std::tm get_current_tm() const
	{
		// Get the current system time
		auto currentTime = std::chrono::system_clock::now();

		// Convert the system time to a time_point representing the current date
		// and time
		std::time_t currentTimeT = std::chrono::system_clock::to_time_t(currentTime);

		// Convert the time_t to a tm structure
		std::tm toRet;
		localtime_r(&currentTimeT, &toRet);

		return toRet;
	}

	int start_debugger()
	{
		if (GetInitialized())
			return 1;

		auto now = get_current_tm();

		const int day    = now.tm_mday;
		const int month  = now.tm_mon + 1;      // First month is zero
		const int year   = now.tm_year + 1900;  // Years since 1900
		const int hour   = now.tm_hour;
		const int minute = now.tm_min;
		const int second = now.tm_sec;

		std::string filename("debug_log_" + std::to_string(hour) + "-" + std::to_string(minute) + "-" +
							 std::to_string(second) + "_" + std::to_string(day) + "-" + std::to_string(month) + "-" +
							 std::to_string(year) + ".txt");

		std::filesystem::create_directory("debugger_logs/");

		log_file.open("debugger_logs/" + filename);

		if (!log_file.is_open())
			return 2;

		initialized = true;
		return 0;
	}

	void end_debugger()
	{
		log_file.close();
		initialized = false;
	}
};
