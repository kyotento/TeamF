#pragma once
#include "ItemType.h"

class Player;
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
	/// 防具の素材変更。
	/// </summary>
	void MaterialChange();

	//それぞれ防具変更。
	void MaterialChangeHelmet(unsigned itemId);
	void MaterialChangeChestPlate(unsigned itemId);
	void MaterialChangeLeggings(unsigned itemId);
	void MaterialChangeBoots(unsigned itemId);


	/// <summary>
	/// モデルを描画するかどうか。
	/// </summary>
	/// <param name="draw">描画するかどうか</param>
	void IsDraw(bool draw);

	/// <summary>
	/// プレイヤーのスキンモデルを設定する。
	/// </summary>
	/// <param name="skin">SkinModel</param>
	void SetPlayerSkinModel(GameObj::CSkinModelRender* skin) {
		m_playerSkinModel = skin;
	}

	void SetPlayer(Player* player)
	{
		m_player = player;
	}
	/// <summary>
	/// 防具の材質。
	/// </summary>
	enum ArmorMaterial
	{
		enArmorMaterial_Leather,
		enArmorMaterial_Iron,
		enArmorMaterial_Goiden,
		enArmorMaterial_Diamond,
		enArmorMaterial_Num
	};

	/// <summary>
	/// アーマーのパーツ。
	/// </summary>
	enum ArmorPart
	{
		enArmorPart_Helmet,
		enArmorPart_ChestPlate,
		enArmorPart_ChestPlate2,
		enArmorPart_Leggings,
		enArmorPart_Boots,
		enArmorPart_Num
	};

private:

	static const int m_ArmorPutNum = 4;					//アーマーの部位の数。
	static const int m_armorNum = 8;					//アーマーのモデルの数。

	ArmorMaterial m_armorMaterial[m_ArmorPutNum] = { enArmorMaterial_Num,enArmorMaterial_Num,enArmorMaterial_Num,enArmorMaterial_Num };		//アーマーの種類。
	ArmorPart m_armorPart[m_ArmorPutNum + 1] = { enArmorPart_Num ,enArmorPart_Num ,enArmorPart_Num ,enArmorPart_Num };		//アーマーの部位の種類。


	bool m_isPutArmor[m_ArmorPutNum] = { false,false,false,false };
	
	float m_scale = 1.5f;											//スケール。
	CVector3 m_position[m_armorNum] = { CVector3::Zero() };			//座標。
	CQuaternion m_rot[m_armorNum] = { CQuaternion::Identity() };	//回転。

	GameObj::CSkinModelRender* m_playerSkinModel = nullptr;				//プレイヤーのSkinModel。

	GameObj::CSkinModelRender* m_skinModelArmor[m_armorNum] = { nullptr };		//防具のモデル。

	Bone* m_bone[m_armorNum] = { nullptr };				//骨。

	unsigned m_helmetId = enItem_Iron_Helmet;		//兜のアイテム。
	unsigned m_chestPlateId = enItem_Iron_ChestPlate;	//鎧のアイテム。
	unsigned m_leggingsId = enItem_Iron_Leggings;	//ズボンのアイテム。
	unsigned m_bootsId = enItem_Iron_Boots;		//靴のアイテム。
	Player* m_player = nullptr;
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

