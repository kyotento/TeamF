#pragma once

class Player;
class PlayerParameter : public IGameObject
{
public:
	PlayerParameter();
	~PlayerParameter();

	bool Start() override;
	void Update() override;
	void PostRender() override;

	/// <summary>
	/// プレイヤーのインスタンスを設定する。
	/// </summary>
	/// <param name="pl">プレイヤーのインスタンス</param>
	void SetPlayerIns(Player* pl)
	{
		m_player = pl;
	}

private:

	/// <summary>
	/// パラメータ画像の基盤を生成。
	/// </summary>
	void SetParamFound();

	/// <summary>
	/// 体力を変更する。
	/// </summary>
	void ChangeHP();

	/// <summary>
	/// スタミナを変更する。
	/// </summary>
	void ChangeStamina();

	/// <summary>
	/// 防御を変更する。
	/// </summary>
	void ChangeArmor();

	static const int m_paramNum = 10;				//パラメーターの画像の数。
	int m_oldHP = 0;								//プレイヤーの1フレーム前のHPを格納。
	int m_oldStamina = 0;							//プレイヤーの1フレーム前のスタミナを格納。
	int m_oldArmor = 0;								//プレイヤーの１フレーム前の防御力を格納。

	const float m_scale = 3.f;						//HPとスタミナのスケール。

	CVector2 m_hpPosition[m_paramNum] = { CVector2::Zero() };		//HP画像の座標。
	CVector2 m_staminaPosition[m_paramNum] = { CVector2::Zero() };	//スタミナ画像の座標。
	CVector2 m_armorPos[m_paramNum] = { CVector2::Zero() };			//防御力画像の座標。

	CSpriteRender m_spriteRenderHP[m_paramNum];				//HP画像。
	CSpriteRender m_spriteRenderStamina[m_paramNum];		//スタミナ画像。
	CSpriteRender m_spriteRenderArmor[m_paramNum];			//防御力画像。
	CSpriteRender* m_spriteRenderOnHand = nullptr;			//手持ちアイテム画像基盤。
	CSpriteRender* m_spriteRenderExp = nullptr;				//経験値。

	Player* m_player = nullptr;			//プレイヤー。
	CFont m_font;						//フォント。
};

