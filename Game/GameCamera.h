#pragma once
#include "Block.h"
#include "CameraCollisionSolver.h"
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

	//カメラの高さ
	static constexpr float height = 1.50f * Block::WIDTH;
	//カメラをプレイヤーの正面に
	static constexpr float front = 0.05f * Block::WIDTH;

	//カメラのモードを取得
	EnCameraMode GetCameraMode()
	{
		return m_mode;
	}

	//座標を取得。
	const CVector3& GetPos() const
	{
		return m_position;
	}
	//FPS視点位置
	const CVector3& GetPosFPS()const {
		return m_positionFPS;
	}

	/// <summary>
	/// ロール軸回転角度を設定
	/// </summary>
	/// <param name="deg">角度</param>
	/// <param name="isLock">ロックするか</param>
	void SetRollDeg(float deg, bool isLock = false) {
		m_rollDeg = deg;
		m_isRollLock = isLock;
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
	CVector3 m_positionFPS = CVector3::One() * 10.0f;	//FPS視点座標
	CVector3 m_target = CVector3::Zero();				//注視点
	const float m_radius = 4.f* Block::WIDTH;			//注視点と座標の距離
	float m_radianY = 0.0f;								//Y軸の回転(ラジアン)
	float m_radianXZ = 0.0f;							//XZ軸の回転(ラジアン)
	EnCameraMode m_mode = EnMode_FPS;					//カメラのモード
	float m_viewAngleDeg = 90.0f;						//視野角
	float m_rollDeg = -0.5f;								//カメラの回転角度(ロール軸)
	bool m_isRollLock = false;							//ロール軸回転をロックするか
	CCameraCollisionSolver m_cameraCollisionSolver;
};

