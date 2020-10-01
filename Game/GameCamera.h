#pragma once
class Player;
enum EnCameraMode {
	EnMode_FPS,
	EnMode_TPS,
	EnMode_ReverseTPS,
	EnMode_Num
};
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}
	bool Start() override;
	void Update() override;
	bool GetIsFPS()
	{
		return m_isFPS;
	}
	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	void SetDegreeY(float degreeY)
	{
		m_degreeY = degreeY;
	}
	void SetDegreeXZ(float degreeXZ)
	{
		m_degreeXZ = degreeXZ;
	}
	EnCameraMode GetCameraMode()
	{
		return m_mode;
	}
private:
	void TPS();
	void FPS();
	void ReverseTPS();
public:
	Player* m_player = nullptr;
	GameObj::PerspectiveCamera* m_camera;
	CVector3 m_position = CVector3::One() * 10.0f;
	CVector3 m_target = CVector3::Zero();
	const float m_radius = 4.0f;
	float m_degreeY = 0.0f;
	float m_degreeXZ = 0.0f;
	bool m_isFPS = true;
	EnCameraMode m_mode = EnMode_FPS;
};

