#pragma once

/// <summary>
/// ‘¾—z
/// </summary>
class Sun : public IGameObject {
	void Update() override;
	void PostRender() override;

	void DebugInput();

	CVector3 m_direction = {0.0f,0.0f,1.0f};
	GameObj::CDirectionLight m_light;
	float m_timeScale = 1.0f;
	float m_seconds = 3600.0f * 12.0f;

public:
	void SetTimeScale(float timeScale = 1.0f) {
		m_timeScale = timeScale;
	}
	float GetTimeScale()const {
		return m_timeScale;
	}

	/// <summary>
	/// Œ»İ‚ğ•b‚Åæ“¾
	/// </summary>
	/// <returns></returns>
	float GetSecond()const {
		return m_seconds;
	}

	void SetSecond(float sec) {
		m_seconds = sec;
	}
};
	