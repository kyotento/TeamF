#pragma once

class Player;
class GameCamera;
class ItemDisplay : public IGameObject
{
public:
	ItemDisplay();
	~ItemDisplay();
	bool Start()override;
	void Update()override;
	/// <summary>
	/// プレイヤーから座標をもらう。
	/// </summary>
	/// <param name="selItem">プレイヤーのポジション。</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// プレイヤーから方向ももらうじぇ。
	/// </summary>
	/// <param name="selItem">プレイヤーの回転。</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	/// <summary>
	/// モデルの生成。
	/// </summary>
	void InitModel();
	/// <summary>
	/// 追従
	/// </summary>
	void Follow();
	/// <summary>
	/// モデルの切り替え
	/// </summary>
	void Switching();
	/// <summary>
	/// アイテムを斜め上に表示するために。
	/// </summary>
	void Rotation();
private:
	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//モデル。
	CVector3 m_position = CVector3::Zero();						//モデルを表示する座標。
	CVector3 m_forward = CVector3::Zero();						//前方。

	CQuaternion m_rotation = CQuaternion::Identity();			//モデルを回転させる。
	Player* m_player = nullptr;									//プレイヤーのインスタンス。
	GameCamera* m_gameCamera;									//ゲームカメライェア。
};

