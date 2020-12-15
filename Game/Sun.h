#pragma once

/// <summary>
/// ���z
/// </summary>
class Sun : public IGameObject {
	void Update() override{		
		//���Ԍo��
		CQuaternion rot(CVector3::AxisX(), 0.05f * GetDeltaTimeSec());
		rot.Multiply(m_direction);

		//���C�g����
		m_light.SetDirection(m_direction);
		//���C�g�F
		CVector3 color = CVector3::One() * -m_light.GetDirection().y;
		color.Max(0.0f);
		m_light.SetColor(color);
	}

	CVector3 m_direction = {0.0f,-1.0f,0.0f};
	GameObj::CDirectionLight m_light;
};
	