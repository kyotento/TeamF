#pragma once
class GameCamera;
class Player : public IGameObject
{
public:
	Player() {}
	~Player() {}
	bool Start() override;
	void Update() override;
	const CVector3& GetRight()
	{
		return m_right;
	}
	const CVector3& GetFront()
	{
		return m_front;
	}
	const CVector3& GetPosition()
	{
		return m_position;
	}
	void SetGameCamera(GameCamera* gameCamera)
	{
		m_gameCamera = gameCamera;
	}
private:
	void Move();
	void Turn();
public:
	CVector3 m_position = CVector3::One() * 15.0f;
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);
	GameCamera* m_gameCamera;
	float m_radian = 0.0f;
	float m_degree = 0.0f;
	CQuaternion m_rotation = CQuaternion::Identity();
	GameObj::CSkinModelRender m_model;
};

