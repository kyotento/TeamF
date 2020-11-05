#pragma once
#include "Block.h"
class Player;
//カメラのモード
enum EnCameraMode {
	EnMode_FPS,   //FPS
	EnMode_TPS,   //TPS
	EnMode_ReverseTPS,    //正面からのカメラ
	EnMode_Num
};
class GameCamera : public IGameObject
{
public:
	GameCamera() {}
	~GameCamera() {}
	bool Start() override;
	void Update() override;
	//カメラのモードを取得
	EnCameraMode GetCameraMode()
	{
		return m_mode;
	}

	//座標を取得。
	CVector3 GetPos() const
	{
		return m_position;
	}
private:
	//TPS
	void TPS();
	//FPS
	void FPS();
	//正面からのカメラ
	void ReverseTPS();
public:
	Player* m_player = nullptr;							//プレイヤー
	GameObj::PerspectiveCamera* m_camera;				//ゲームカメラ
	CVector3 m_position = CVector3::One() * 10.0f;		//座標
	CVector3 m_target = CVector3::Zero();				//注視点
	const float m_radius = 4.f* Block::WIDTH;						//注視点と座標の距離
	float m_radianY = 0.0f;								//Y軸の回転(ラジアン)
	float m_radianXZ = 0.0f;							//XZ軸の回転(ラジアン)
	EnCameraMode m_mode = EnMode_FPS;					//カメラのモード
};

