#include "stdafx.h"
#include "PlayerArmor.h"
#include "Player.h"
#include "Item.h"
#include "ItemStack.h"

namespace {
	static const wchar_t* ARMOR_FILE_PATH_ARRAY[4][4 + 1];

	const int HelmetSlots = 36;
	const int ChestPlateSlots = 37;
	const int LegggingsSlots = 38;
	const int BootsSlots = 39;
}

PlayerArmor::PlayerArmor()
{
	//モデルパス。
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Leather][enArmorPart_Helmet] = L"Resource/modelData/armor/Leather_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Leather][enArmorPart_ChestPlate] = L"Resource/modelData/armor/Leather_ChestPlate.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Leather][enArmorPart_ChestPlate2] = L"Resource/modelData/armor/Leather_ChestPlate2.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Leather][enArmorPart_Leggings] = L"Resource/modelData/armor/Leather_leggings.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Leather][enArmorPart_Boots] = L"Resource/modelData/armor/Leather_Boots.tkm";

	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Iron][enArmorPart_Helmet] = L"Resource/modelData/armor/Iron_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Iron][enArmorPart_ChestPlate] = L"Resource/modelData/armor/Iron_ChestPlate.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Iron][enArmorPart_ChestPlate2] = L"Resource/modelData/armor/Iron_ChestPlate2.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Iron][enArmorPart_Leggings] = L"Resource/modelData/armor/Iron_leggings.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Iron][enArmorPart_Boots] = L"Resource/modelData/armor/Iron_Boots.tkm";

	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Goiden][enArmorPart_Helmet] = L"Resource/modelData/armor/Golden_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Goiden][enArmorPart_ChestPlate] = L"Resource/modelData/armor/Golden_ChestPlate.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Goiden][enArmorPart_ChestPlate2] = L"Resource/modelData/armor/Golden_ChestPlate2.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Goiden][enArmorPart_Leggings] = L"Resource/modelData/armor/Golden_leggings.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Goiden][enArmorPart_Boots] = L"Resource/modelData/armor/Golden_Boots.tkm";

	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_Helmet] = L"Resource/modelData/armor/Diamond_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_ChestPlate] = L"Resource/modelData/armor/Diamond_ChestPlate.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_ChestPlate2] = L"Resource/modelData/armor/Diamond_ChestPlate2.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_Leggings] = L"Resource/modelData/armor/Diamond_leggings.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_Boots] = L"Resource/modelData/armor/Diamond_Boots.tkm";
}


PlayerArmor::~PlayerArmor()
{
	for (int i = 0; i < m_armorNum; i++) {
		DeleteGO(m_skinModelArmor[i]);
	}
}

bool PlayerArmor::Start()
{
	InitModel();
	GetBoneInfor();			//骨情報取得。
	return true;
}

void PlayerArmor::Update()
{
	SetArmorPos();		//アーマーの座標と回転設定。
	MaterialChange();	//モデルの素材を変更。
}

//各モデルの初期化。
void PlayerArmor::InitModel()
{
	for (int i = 0; i < m_armorNum; i++) {
		m_skinModelArmor[i] = NewGO<GameObj::CSkinModelRender>();
		m_skinModelArmor[i]->SetScale(m_scale);
	}
	m_skinModelArmor[0]->Init(L"Resource/modelData/armor/Iron_Helmet.tkm");
	m_skinModelArmor[1]->Init(L"Resource/modelData/armor/Iron_ChestPlate.tkm");
	m_skinModelArmor[2]->Init(L"Resource/modelData/armor/Iron_ChestPlate2.tkm");
	m_skinModelArmor[3]->Init(L"Resource/modelData/armor/Iron_ChestPlate2.tkm");
	m_skinModelArmor[4]->Init(L"Resource/modelData/armor/Iron_leggings.tkm");
	m_skinModelArmor[5]->Init(L"Resource/modelData/armor/Iron_leggings.tkm");
	m_skinModelArmor[6]->Init(L"Resource/modelData/armor/Iron_boots.tkm");
	m_skinModelArmor[7]->Init(L"Resource/modelData/armor/Iron_boots.tkm");
}

//アーマーの座標と回転設定。
void PlayerArmor::GetBoneInfor()
{
	m_bone[0] = m_playerSkinModel->FindBone(L"Bone002");
	m_bone[1] = m_playerSkinModel->FindBone(L"Bone010");
	m_bone[2] = m_playerSkinModel->FindBone(L"Bone008");
	m_bone[3] = m_playerSkinModel->FindBone(L"Bone005");
	m_bone[4] = m_playerSkinModel->FindBone(L"Bone015");
	m_bone[5] = m_playerSkinModel->FindBone(L"Bone012");
	m_bone[6] = m_playerSkinModel->FindBone(L"Bone016");
	m_bone[7] = m_playerSkinModel->FindBone(L"Bone013");
}

//骨座標を取得。
void PlayerArmor::SetArmorPos()
{
	for (int i = 0; i < m_armorNum; i++) {
		m_position[i] = m_bone[i]->GetPosition();
		CQuaternion modelRot;						//モデルの回転量。
		modelRot = m_playerSkinModel->GetRot();		//モデルの回転量を取得。

		if (i >= 2 && i<= 3) {
			m_position[i].y += 5.f;
		}

		CQuaternion boneRot;
		boneRot.SetRotation(CVector3::AxisZ(), CMath::PI_HALF);//回転を補正
		boneRot.Concatenate(CQuaternion(CVector3::AxisX(), CMath::PI));//回転を補正
		boneRot.Concatenate(m_bone[i]->GetRotation());

		m_skinModelArmor[i]->SetPos(m_position[i]);
		m_skinModelArmor[i]->SetRot(boneRot);
	}
}

//モデルの素材変更。
void PlayerArmor::MaterialChange()
{
	//装備中のアイテムIDを取得。
	auto helmetId = m_player->GetInventory().GetNullableItem(HelmetSlots).GetID();
	auto chestPlateId = m_player->GetInventory().GetNullableItem(ChestPlateSlots).GetID();
	auto leggingsId = m_player->GetInventory().GetNullableItem(LegggingsSlots).GetID();
	auto bootsId = m_player->GetInventory().GetNullableItem(BootsSlots).GetID();

	//ID違ってたら。
	if (m_helmetId != helmetId && helmetId != enCube_None)
	{
		//防具変更。
	
		MaterialChangeHelmet(helmetId);
	}

	if (m_chestPlateId != chestPlateId && chestPlateId != enCube_None)
	{
		//防具変更。
	
		MaterialChangeChestPlate(chestPlateId);

	}

	if (m_leggingsId != leggingsId && leggingsId != enCube_None)
	{
		//防具変更。
	
		MaterialChangeLeggings(leggingsId);

	}

	if (m_bootsId != bootsId && bootsId != enCube_None)
	{
		//防具変更。
	
		MaterialChangeBoots(bootsId);

	}

	m_helmetId = helmetId;
	m_chestPlateId = chestPlateId;
	m_leggingsId = leggingsId;
	m_bootsId = bootsId;

	if (m_helmetId == enCube_None)
	{
		m_skinModelArmor[0]->SetIsDraw(false);
	}
	if (m_chestPlateId == enCube_None)
	{
		m_skinModelArmor[1]->SetIsDraw(false);
		m_skinModelArmor[2]->SetIsDraw(false);
		m_skinModelArmor[3]->SetIsDraw(false);
	}
	if (m_leggingsId == enCube_None)
	{
		m_skinModelArmor[4]->SetIsDraw(false);
		m_skinModelArmor[5]->SetIsDraw(false);
	}
	if (m_bootsId == enCube_None)
	{
		m_skinModelArmor[6]->SetIsDraw(false);
		m_skinModelArmor[7]->SetIsDraw(false);

	}
	



	//todo プレイヤーから防具情報を取得する。
	/*ArmorMaterial helmet = enArmorMaterial_Leather;
	ArmorMaterial chestPlate = enArmorMaterial_Leather;
	ArmorMaterial leggings = enArmorMaterial_Leather;
	ArmorMaterial boots = enArmorMaterial_Leather;

	m_skinModelArmor[0]->Init(ARMOR_FILE_PATH_ARRAY[helmet][enArmorPart_Helmet]);
	m_skinModelArmor[1]->Init(ARMOR_FILE_PATH_ARRAY[chestPlate][enArmorPart_ChestPlate]);
	m_skinModelArmor[2]->Init(ARMOR_FILE_PATH_ARRAY[chestPlate][enArmorPart_ChestPlate2]);
	m_skinModelArmor[3]->Init(ARMOR_FILE_PATH_ARRAY[chestPlate][enArmorPart_ChestPlate2]);
	m_skinModelArmor[4]->Init(ARMOR_FILE_PATH_ARRAY[leggings][enArmorPart_Leggings]);
	m_skinModelArmor[5]->Init(ARMOR_FILE_PATH_ARRAY[leggings][enArmorPart_Leggings]);
	m_skinModelArmor[6]->Init(ARMOR_FILE_PATH_ARRAY[boots][enArmorPart_Boots]);
	m_skinModelArmor[7]->Init(ARMOR_FILE_PATH_ARRAY[boots][enArmorPart_Boots]);*/
	
}

void PlayerArmor::MaterialChangeHelmet(unsigned itemId)
{
	DeleteGO(m_skinModelArmor[0]);
	m_skinModelArmor[0] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[0]->SetScale(m_scale);
	m_skinModelArmor[0]->Init(Item::GetItem(itemId).GetModelPath().c_str());
}

void PlayerArmor::MaterialChangeChestPlate(unsigned itemId)
{
	DeleteGO(m_skinModelArmor[1]);
	DeleteGO(m_skinModelArmor[2]);
	DeleteGO(m_skinModelArmor[3]);
	m_skinModelArmor[1] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[1]->SetScale(m_scale);
	m_skinModelArmor[1]->Init(Item::GetItem(itemId).GetModelPath().c_str());
	std::filesystem::path path = Item::GetItem(itemId+1).GetModelPath();
	m_skinModelArmor[2] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[2]->SetScale(m_scale);
	m_skinModelArmor[2]->Init(path.c_str());
	m_skinModelArmor[3] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[3]->SetScale(m_scale);
	m_skinModelArmor[3]->Init(path.c_str());
	
}

void PlayerArmor::MaterialChangeLeggings(unsigned itemId)
{
	DeleteGO(m_skinModelArmor[4]);
	DeleteGO(m_skinModelArmor[5]);
	m_skinModelArmor[4] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[4]->SetScale(m_scale);
	m_skinModelArmor[4]->Init(Item::GetItem(itemId).GetModelPath().c_str());
	m_skinModelArmor[5] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[5]->SetScale(m_scale);
	m_skinModelArmor[5]->Init(Item::GetItem(itemId).GetModelPath().c_str());
}

void PlayerArmor::MaterialChangeBoots(unsigned itemId)
{
	DeleteGO(m_skinModelArmor[6]);
	DeleteGO(m_skinModelArmor[7]);
	m_skinModelArmor[6] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[6]->SetScale(m_scale);
	m_skinModelArmor[6]->Init(Item::GetItem(itemId).GetModelPath().c_str());
	m_skinModelArmor[7] = NewGO<GameObj::CSkinModelRender>();
	m_skinModelArmor[7]->SetScale(m_scale);
	m_skinModelArmor[7]->Init(Item::GetItem(itemId).GetModelPath().c_str());
}

//モデルの描画をする。
void PlayerArmor::IsDraw(bool draw)
{
	if (draw) {
		for (int i = 0; i < m_armorNum; i++) {
			m_skinModelArmor[i]->SetIsDraw(true);
		}
	}
	else {
		for (int i = 0; i < m_armorNum; i++) {
			m_skinModelArmor[i]->SetIsDraw(false);
		}
	}
}

