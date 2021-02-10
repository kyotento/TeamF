#pragma once

class Player;
class ItemDisplay;
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

	/// <summary>
	/// 選択されているアイテム番号を取得する。
	/// </summary>
	/// <returns>選択されている番号</returns>
	int GetSelectItem()
	{
		return m_selectNum;
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

	/// <summary>
	/// 経験値ゲージを変更する。
	/// </summary>
	void ChangeExp();

	/// <summary>
	/// 手持ちのアイテムを指定する。
	/// </summary>
	void SelectItem();

	/// <summary>
	//todo  1~9ボタンによるアイテムセレクト(ごり押しの極み)。
	/// </summary>
	void KariItemS();

	/// <summary>
	/// 右手の生成。
	/// </summary>
	void InitRightHand();

	/// <summary>
	/// アイテムがインベントリーで切り替わった時の処理。
	/// </summary>
	void StopMoveToChange();

	void PlayerLevelUp();
	static const int m_paramNum = 10;				//パラメーターの画像の数。
	int m_oldHP = 0;								//プレイヤーの1フレーム前のHPを格納。
	int m_oldStamina = 0;							//プレイヤーの1フレーム前のスタミナを格納。
	int m_oldArmor = 0;								//プレイヤーの1フレーム前の防御力を格納。
	int m_selectNum = 1;							//何番目のアイテムをセレクトしているか 1~9(プレイヤー格納用)。
	int m_selectNumOld = 0;							//１フレーム前の↑。
	int m_idOld = 0;								//1フレーム前のID

	const float m_scale = 3.f;						//HPとスタミナのスケール。
	const float m_selectPosX = 0.260f;				//アイテムセレクト0番目のX座標。
	const float m_expScale = 1.5f;					//経験値系画像のスケール。

	bool m_isItemChangeFlag = false;				//持ってるもの切り替えたかどうか。

	bool m_isStopMoveToChangeFlag = false;			//インベントリー開いてるときに持ち物が切り替わったか。
	CVector2 m_hpPosition[m_paramNum] = { CVector2::Zero() };		//HP画像の座標。
	CVector2 m_staminaPosition[m_paramNum] = { CVector2::Zero() };	//スタミナ画像の座標。
	CVector2 m_armorPos[m_paramNum] = { CVector2::Zero() };			//防御力画像の座標。
	CVector2 m_sItemPos = CVector2::Zero();							//アイテムセレクト座標。

	GameObj::CSpriteRender m_spriteRenderHP[m_paramNum];			//HP画像。
	GameObj::CSpriteRender m_spriteRenderStamina[m_paramNum];		//スタミナ画像。
	GameObj::CSpriteRender m_spriteRenderArmor[m_paramNum];			//防御力画像。
	GameObj::CSpriteRender m_spriteRenderOnHand;					//手持ちアイテム画像基盤。
	GameObj::CSpriteRender m_spriteRenderExp;						//経験値。
	GameObj::CSpriteRender m_spriteRenderExpGauge;					//経験値(ゲージの増減)。
	GameObj::CSpriteRender m_spriteRenderSelectItem;				//度のアイテムを選択しているか。

	Player* m_player = nullptr;			//プレイヤー。
	CFont m_font;						//フォント。

	ItemDisplay* m_rightHandDisplay = nullptr;					//右手表示。
};

