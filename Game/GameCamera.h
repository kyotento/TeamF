#pragma once

class GameCamera :public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

	/// <summary>
	/// �J�����̒����_��ݒ肷��B
	/// </summary>
	/// <param name="targetPos">�����_</param>
	void SetTarget(CVector3 targetPos)
	{
		m_target = targetPos;
	}

private:

	CVector3 m_position = CVector3::Zero();				//�J�����̍��W�B
	CVector3 m_target = CVector3::Zero();				//�J�����̒����_�B

	GameObj::PerspectiveCamera* m_camera = nullptr;		//�J�����B

};

