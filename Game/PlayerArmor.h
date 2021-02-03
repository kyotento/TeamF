#pragma once

class PlayerArmor : public IGameObject
{
public:
	PlayerArmor();
	~PlayerArmor();

	bool Start()override;
	void Update()override;

	/// <summary>
	/// 各モデルの初期化。
	/// </summary>
	void InitModel();

	/// <summary>
	/// 骨情報取得。
	/// </summary>
	void GetBoneInfor();

	/// <summary>
	/// アーマーの座標と回転設定。
	/// </summary>
	void SetArmorPos();

	/// <summary>
	/// プレイヤーのスキンモデルを設定する。
	/// </summary>
	/// <param name="skin">SkinModel</param>
	void SetPlayerSkinModel(GameObj::CSkinModelRender* skin) {
		m_playerSkinModel = skin;
	}

private:

	static const int m_armorNum = 8;
	
	float m_scale = 1.5f;											//スケール。
	CVector3 m_position[m_armorNum] = { CVector3::Zero() };			//座標。
	CQuaternion m_rot[m_armorNum] = { CQuaternion::Identity() };	//回転。

	GameObj::CSkinModelRender* m_playerSkinModel = nullptr;				//プレイヤーのSkinModel。

	GameObj::CSkinModelRender* m_skinModelArmor[m_armorNum] = { nullptr };		//防具のモデル。

	Bone* m_bone[m_armorNum] = { nullptr };				//骨。

	/// <summary>
	/// アーマー番号詳細。
	/// </summary>
	//0		ヘルメット。
	//1		胴体チェストプレート。
	//2		右手チェストプレート。
	//3		左手チェストプレート。
	//4		右レギンス。
	//5		左レギンス。
	//6		右ブーツ。
	//7		左ブーツ。
};

