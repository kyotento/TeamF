#include "stdafx.h"
#include "PlayerArmor.h"


PlayerArmor::PlayerArmor()
{
}


PlayerArmor::~PlayerArmor()
{
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
