#include "stdafx.h"
#include "DebugTimer.h"

using namespace std::chrono;

void DebugTimer::Start(){
	m_start = system_clock::now();
}

void DebugTimer::Stop(){
	m_duration = duration_cast<decltype(m_duration)>( std::chrono::operator-( system_clock::now(), m_start ) );
}

void DebugTimer::OutputDebug(){
	std::stringstream str;
	str << m_duration.count() << "ƒ~ƒŠ";
}
