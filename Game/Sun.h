#pragma once

/// <summary>
/// 太陽
/// </summary>
class Sun : public IGameObject {
	void Update() override{		
		//時間経過
		CQuaternion rot(CVector3::AxisX(), 0.05f * GetDeltaTimeSec());
		rot.Multiply(m_direction);

		//ライト方向
		m_light.SetDirection(m_direction);
		//ライト色
		CVector3 color = CVector3::One() * -m_light.GetDirection().y;
		color.Max(0.0f);
		m_light.SetColor(color);
	}

	CVector3 m_direction = {0.0f,-1.0f,0.0f};
	GameObj::CDirectionLight m_light;
};
	