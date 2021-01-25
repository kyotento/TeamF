#pragma once

/// <summary>
/// ‘¾—z
/// </summary>
class Sun : public IGameObject {
	void Update() override;

	CVector3 m_direction = {0.0f,-1.0f,0.0f};
	GameObj::CDirectionLight m_light;
};
	