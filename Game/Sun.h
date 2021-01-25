#pragma once

/// <summary>
/// ‘¾—z
/// </summary>
class Sun : public IGameObject {
	void Update() override;

	CVector3 m_direction = {0.0f,-1.0f,0.0f};
	GameObj::CDirectionLight m_light;
	float m_timeScale = 1.0f;

public:
	void SetTimeScale(float timeScale = 1.0f) {
		m_timeScale = timeScale;
	}
	float GetTimeScale()const {
		return m_timeScale;
	}
};
	