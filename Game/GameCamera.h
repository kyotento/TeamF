#pragma once
class Player;
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}
	bool Start() override;
	void Update() override;
private:
	void TPS();
public:
	Player* m_player = nullptr;
	GameObj::PerspectiveCamera* m_camera;
	CVector3 m_position = CVector3::One() * 100.0f;
	CVector3 m_target = CVector3::Zero();
	const float m_radius = 40.0f;
	float m_degreeY = 0.0f;
	float m_degreeXZ = 0.0f;

};

