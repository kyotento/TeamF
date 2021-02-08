#include "stdafx.h"
#include "PlayerArmor.h"

namespace {
	static const wchar_t* ARMOR_FILE_PATH_ARRAY[4][4 + 1];
}

PlayerArmor::PlayerArmor()
{
	//���f���p�X�B
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
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_ChestPlate] = L"Resource/modelData/armor/Diamond_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_ChestPlate2] = L"Resource/modelData/armor/Diamond_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_Leggings] = L"Resource/modelData/armor/Diamond_Helmet.tkm";
	ARMOR_FILE_PATH_ARRAY[enArmorMaterial_Diamond][enArmorPart_Boots] = L"Resource/modelData/armor/Diamond_Helmet.tkm";
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
	GetBoneInfor();			//�����擾�B
	return true;
}

void PlayerArmor::Update()
{
	SetArmorPos();		//�A�[�}�[�̍��W�Ɖ�]�ݒ�B
	MaterialChange();	//���f���̑f�ނ�ύX�B
}

//�e���f���̏������B
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

//�A�[�}�[�̍��W�Ɖ�]�ݒ�B
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

//�����W���擾�B
void PlayerArmor::SetArmorPos()
{
	for (int i = 0; i < m_armorNum; i++) {
		m_position[i] = m_bone[i]->GetPosition();
		CQuaternion modelRot;						//���f���̉�]�ʁB
		modelRot = m_playerSkinModel->GetRot();		//���f���̉�]�ʂ��擾�B

		if (i >= 2 && i<= 3) {
			m_position[i].y += 5.f;
		}

		CQuaternion boneRot;
		boneRot.SetRotation(CVector3::AxisZ(), CMath::PI_HALF);//��]��␳
		boneRot.Concatenate(CQuaternion(CVector3::AxisX(), CMath::PI));//��]��␳
		boneRot.Concatenate(m_bone[i]->GetRotation());

		m_skinModelArmor[i]->SetPos(m_position[i]);
		m_skinModelArmor[i]->SetRot(boneRot);
	}
}

//���f���̑f�ޕύX�B
void PlayerArmor::MaterialChange()
{
	//todo �v���C���[����h������擾����B
	ArmorMaterial helmet = enArmorMaterial_Leather;
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
	m_skinModelArmor[7]->Init(ARMOR_FILE_PATH_ARRAY[boots][enArmorPart_Boots]);
	
}

//���f���̕`�������B
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
