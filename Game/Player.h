#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"


class GameCamera;
class Item;
struct Inventory {
	Item* s_item = nullptr;
	int s_number = 0;
};

class Player : public IGameObject
{
public:
	Player() {}
	~Player() {}
	bool Start() override;
	void Update() override;

	/// <summary>
	/// プレイヤーの右方向を取得。
	/// </summary>
	/// <returns>プレイヤーの右方向</returns>
	const CVector3& GetRight()
	{
		return m_right;
	}

	/// <summary>
	/// プレイヤーの正面方向を取得。
	/// </summary>
	/// <returns>プレイヤーの正面方向</returns>
	const CVector3& GetFront()
	{
		return m_front;
	}
	
	/// <summary>
	/// プレイヤーの座標を取得。
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	const CVector3& GetPosition()
	{
		return m_position;
	}

	/// <summary>
	/// プレイヤーのY軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのY軸の回転</returns>
	float GetRadianY()
	{
		return m_radianY;
	}

	/// <summary>
	/// プレイヤーのXZ軸の回転を取得。
	/// </summary>
	/// <returns>プレイヤーのXZ軸の回転</returns>
	float GetRadianXZ()
	{
		return m_radianXZ;
	}
	//インベントリの長さ
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;
private:
	/// <summary>
	/// 移動。
	/// </summary>
	void Move();

	/// <summary>
	/// 回転
	/// </summary>
	void Turn();
//public:
	float m_degreeY = 0.0f;										//Y軸の回転。
	float m_degreeXZ = 0.0f;									//XZ軸の回転。
	float m_radianY = 0.0f;										//Y軸の回転(ラジアン)。
	float m_radianXZ = 0.0f;									//XZ軸の回転(ラジアン)。
	float m_characonRadius = 50.f;								//キャラコンの半径。
	float m_characonHeight = 100.f;								//キャラコンの高さ。

	CVector3 m_position = CVector3::One() * 15.0f;				//プレイヤーの座標。
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//右方向。
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//正面。

	CQuaternion m_rotation = CQuaternion::Identity();			//クォータニオン。

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//モデル。
	CCharacterController m_characon;							//キャラコン。
	CFont m_font;												//フォント。
	std::vector<Inventory*> m_inventoryList;					//インベントリ。
	GameCamera* m_gameCamera = nullptr;							//ゲームカメラ。
};

