#pragma once

class GameCamera :public IGameObject
{
public:
	GameCamera();
	~GameCamera();

	bool Start();
	void Update();

	/// <summary>
	/// カメラの注視点を設定する。
	/// </summary>
	/// <param name="targetPos">注視点</param>
	void SetTarget(CVector3 targetPos)
	{
		m_target = targetPos;
	}

private:

	CVector3 m_position = CVector3::Zero();				//カメラの座標。
	CVector3 m_target = CVector3::Zero();				//カメラの注視点。

	GameObj::PerspectiveCamera* m_camera = nullptr;		//カメラ。

};

