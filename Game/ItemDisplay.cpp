#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

namespace {
	int DownPosY = 0;
	int DownPosZ = 0;
	bool swich_flag = false;
	bool initItem_flag = false;
}

ItemDisplay::ItemDisplay()
{
}

ItemDisplay::~ItemDisplay()
{
	DeleteGO(m_skinModelRender);
}

bool ItemDisplay::Start()
{
	//モデル生成
	InitModel();
	return true;
}

void ItemDisplay::Update()
{
	if (m_gameCamera == nullptr) {
		m_gameCamera = FindGO<GameCamera>();
		return;
	}
	if (m_player == nullptr) {
		m_player = FindGO<Player>();
		return;
	}

	CameraModeChangeToDisplay();
	//追従処理。
	Follow();

	//回転
	Rotation();

	//切り替えの処理。
	Switching();

	m_skinModelRender->SetScale(m_scale);
}

//モデル生成。
void ItemDisplay::InitModel()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
	m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
	type = enHand;
}
//モデルの追従。
void ItemDisplay::Follow()
{	
	const float blockPulsPosY = 60.0;				//Yは別で足してずらします。
	const float mullFornt = 45.0f;					//前ベクトルの数値を大きくする変数。
	const float mullCrossProduct = 45.0f;			//外積の数値を大きくする変数

	//次にプレイヤーの正面取得。
	m_forward = m_player->GetFront();
	m_forward *= mullFornt;

	//右方向にずらす。
	CVector3 right = m_player->GetRight();
	m_forward += (right * mullCrossProduct);
	
	//下方向にずらす。
	m_forward.y -= blockPulsPosY ;

	//上下方向に回転させる。
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//カメラの座標取得。
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	m_position.y += DownPosY;
	m_position.z -= DownPosZ;
	////座標をモデルへ。
	m_skinModelRender->SetPos(m_position);
}
//切り替え
void ItemDisplay::Switching()
{
	int minDownPos = -50;
	int maxDownPos = 0;
	//切り替え(後でif文を変えます。)
	if (GetKeyDown('G'))
	{
		//下に落とすよ。
		DownPosY--;
		DownPosZ--;
		if (DownPosY >= minDownPos)
		{
			DownPosY = minDownPos-1;
			DownPosZ = minDownPos-1;
			initItem_flag = true;
			swich_flag = true;
		}
	}
	else if (swich_flag)
	{
		DownPosY++;
		DownPosZ++;
		BuildAgain();
		//元の位置へ。
		if (DownPosY >= maxDownPos)
		{
			DownPosY = maxDownPos;
			DownPosZ = maxDownPos;
			swich_flag = false;
		}
	}
}
//腕の回転処理
void ItemDisplay::Rotation()
{
	switch (type)
	{
	case ItemDisplay::enHand:
		HandRotation();
		break;
	case ItemDisplay::enBlock:
		BlockRotation();
		break;
	case ItemDisplay::enTool:
		ToolRotation();
		break;
	}
}
//カメラのモードに合わせた処理
void ItemDisplay::CameraModeChangeToDisplay()
{
	if (m_gameCamera->GetCameraMode() == m_cameraDisplayMode)
	{
		m_skinModelRender->SetIsDraw(true);
	}
	else
	{
		m_skinModelRender->SetIsDraw(false);
	}
}
//再度。
void ItemDisplay::BuildAgain()
{
	//一度だけ生成しなおす。
	//if文は後に変更します。
	if (initItem_flag && type == enHand)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/GrassBlock.tkm");
		initItem_flag = false;
		type = enBlock;
	}
	else if (initItem_flag && type == enBlock)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
		type = enHand;
	}
}
//手の回転
void ItemDisplay::HandRotation()
{
	const float m_rotX = 25.0f;						//ツール系が増えたときに前に倒すための変数。
	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), -m_rotX);
	m_rotation.Multiply(m_rotationX);
	//ここでプレイヤーの上下にそってずらします。
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
	//サイズ。
	m_scale = { 0.40f,0.40f,0.40f };
}
//ブロック系の回転
void ItemDisplay::BlockRotation()
{
	const float m_rotY = 0.0f;						//ツール系が増えたときに前に倒すための変数。
	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisY(), m_rotY);
	m_rotation.Multiply(m_rotationX);
	//ここでプレイヤーの上下にそってずらします。
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
	//サイズ。
	m_scale = { 0.25f,0.25f,0.25f };
}
//ツール系の回転
void ItemDisplay::ToolRotation()
{
	const float m_rotX = 0.0f;						//ツール系が増えたときに前に倒すための変数。
	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), -m_rotX);
	m_rotation.Multiply(m_rotationX);
	//ここでプレイヤーの上下にそってずらします。
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
	//サイズ。
	m_scale = { 0.40f,0.40f,0.40f };
}