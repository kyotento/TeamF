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
	void PostRender() override;
	//右方向を取得
	const CVector3& GetRight()
	{
		return m_right;
	}
	//正面方向を取得
	const CVector3& GetFront()
	{
		return m_front;
	}
	//座標を取得
	const CVector3& GetPosition()
	{
		return m_position;
	}
	//Y軸の回転を取得
	float GetRadianY()
	{
		return m_radianY;
	}
	//XZ軸の回転を取得
	float GetRadianXZ()
	{
		return m_radianXZ;
	}
	//インベントリの長さ
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;
private:
	//移動
	void Move();
	//回転
	void Turn();
public:
	CVector3 m_position = CVector3::One() * 15.0f;				//座標
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//右方向
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//正面
	GameCamera* m_gameCamera;									//ゲームカメラ
	float m_degreeY = 0.0f;										//Y軸の回転
	float m_degreeXZ = 0.0f;									//XZ軸の回転
	float m_radianY = 0.0f;										//Y軸の回転(ラジアン)
	float m_radianXZ = 0.0f;									//XZ軸の回転(ラジアン)
	CQuaternion m_rotation = CQuaternion::Identity();			//クォータニオン
	GameObj::CSkinModelRender m_model;							//モデル
	CCharacterController m_characon;							//キャラコン
	float m_characonRadius = 50.f;			//キャラコンの半径。
	float m_characonHeight = 100.f;			//キャラコンの高さ。
	CFont m_font;												//フォント
	std::vector<Inventory*> m_inventoryList;					//インベントリ
};

