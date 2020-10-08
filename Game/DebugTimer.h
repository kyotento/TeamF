#pragma once
class DebugTimer{
public:
	void Start();
	void Stop();

	void OutputDebug();

private:
	std::chrono::system_clock::time_point m_start;
	std::chrono::milliseconds m_duration;
};

