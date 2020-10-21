#pragma once

/// @brief ���Ԃ��v��f�o�b�O�p�X�g�b�v�E�H�b�`�B�g���ĂȂ��̂ŏ����Ă��悢�B
/// @author Takayama
class DebugTimer{
public:
	void Start();
	void Stop();

	void OutputDebug();

private:
	std::chrono::system_clock::time_point m_start;
	std::chrono::milliseconds m_duration;
};

