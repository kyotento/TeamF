#include "stdafx.h"
#include "ItemDisplay.h"
#include "GameCamera.h"
#include "Player.h"
#include "BlockFactory.h"
#include "ItemStack.h"

static const wchar_t* FILE_PATH_ARRAY[enCube_Num]{};

namespace {
	const int endBlockNum = 11;					//ボックス系の最後の番号。
	const int startToolNum = 16;					//ツールの初めの番号。

	int DownPosY = 0;
	int DownPosZ = 0;

	int minDownPos = -50;
	int maxDownPos = 0;

	int Limit = 0;

	float UpPosY = 25;

	const float handMullFront = 45.0f;
	const float blockMullFront = 45.0f;
	const float toolMullFront = 30.0f;
	const float itemMullFront = 15.0f;

	const float handMullCross = 45.0f;
	const float BlockMullCross= 45.0f;
	const float toolMullCross = 30.0f;
	const float itemMullCross = 15.0f;

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

	if (!m_player->GetIsPlayerDead()) {
		CameraModeChangeToDisplay();
		//追従処理。
		Follow();

		//回転
		Rotation();

		//切り替えの処理。
		Switching();

		//クリックしたときのモーション処理。
		LeftClickMouseToMoveHand();

		m_skinModelRender->SetScale(m_scale);
	}
	else if (m_player->GetIsPlayerDead())
	{
		m_skinModelRender->SetIsDraw(false);
	}
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


	//次にプレイヤーの正面取得。
	m_forward = m_player->GetFront();
	m_forward *= m_mullFornt;

	//右方向にずらす。
	CVector3 right = m_player->GetRight();
	m_forward += (right * m_mullCrossProduct);
	
	//下方向にずらす。
	m_forward.y -= blockPulsPosY + UpPosY;

	//上下方向に回転させる。
	CQuaternion upDownRot;
	upDownRot.SetRotation(right, m_player->GetRadianXZ());
	upDownRot.Multiply(m_forward);

	//カメラの座標取得。
	m_position = m_gameCamera->GetPos();
	m_position += m_forward;

	m_position.y += DownPosY;
	////座標をモデルへ。
	m_skinModelRender->SetPos(m_position);
}
//切り替え
void ItemDisplay::Switching()
{

	SwitchItemType();		//持つアイテムの情報の処理。

	//切り替え
	if (m_isItemChangeFlag && !swich_flag)
	{
		DownPosY = minDownPos;
		initItem_flag = true;
		swich_flag = true;
	}
	else if (m_isItemChangeFlag)
	{
		DownPosY = minDownPos;
		initItem_flag = true;
		swich_flag = true;
	}
	else if (swich_flag)
	{
		DownPosY++;
		BuildAgain();
		//元の位置へ。
		m_isItemChangeFlag = false;
		if (DownPosY >= maxDownPos)
		{
			DownPosY = maxDownPos;
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
	case ItemDisplay::enItem:
		ItemRotation();
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
	//何も持ってない時。
	if (initItem_flag && type == enHand)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
	}
	//ブロック系のアイテム。
	else if (initItem_flag && type == enBlock)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
	//ツール系のアイテム。
	else if (initItem_flag && type == enTool)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
	//それ以外のアイテム。
	else if (initItem_flag && type == enItem)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(m_modelPath.wstring().c_str());
		initItem_flag = false;
	}
}
//手の回転。
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
	//サイズと位置をずらす。
	UpPosY = 0;
	m_scale = { 0.40f,0.40f,0.40f };
	m_mullFornt = handMullFront;
	m_mullCrossProduct = BlockMullCross;
}
//ブロック系の回転。
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
	//サイズと位置をずらす。
	m_scale = { 0.25f,0.25f,0.25f };
	m_mullFornt = blockMullFront;
	m_mullCrossProduct = BlockMullCross;
	UpPosY = 0;
}
//ツール系の回転。
void ItemDisplay::ToolRotation()
{
	const float m_rotZ = 10.0f;						//ツール系が増えたときに前に倒すための変数。
	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisZ(), m_rotZ);
	m_rotation.Multiply(m_rotationX);
	//ここでプレイヤーの上下にそってずらします。
	CQuaternion upDownRot;
	upDownRot.SetRotation(CVector3::AxisX(), -m_player->GetRadianXZ());
	m_rotation.Multiply(upDownRot);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
	//サイズと位置をずらす。
	m_scale = { 0.2f,0.225f,0.2f };
	m_mullFornt = toolMullFront;
	m_mullCrossProduct = toolMullCross;
	UpPosY = -20;
}
//木の棒やインゴット等の回転。
void ItemDisplay::ItemRotation()
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
	//サイズと位置をずらす。
	UpPosY = -40;
	m_scale = { 0.10f,0.10f,0.10f };
	m_mullFornt = itemMullFront;
	m_mullCrossProduct = itemMullCross;
}
//インベントリに合わせて切り替え(ry。
void ItemDisplay::SwitchItemType()
{
	//簡易処理
	//アイテムの参照。
	auto& item = m_player->GetInventory().GetItem(m_player->GetSelectItemNum()-1);
	if (item == nullptr) {
		type = enHand;
	}
	else if (item != nullptr) {
		if (item->GetID() >=endBlockNum && item->GetID() <= startToolNum) {
			type = enItem;
			m_modelPath = item->GetModelPath();
		}
		else if (!item->GetIsBlock() && item->GetID() > startToolNum)
		{
			type = enTool;
			m_modelPath = item->GetModelPath();
		}
		else if (item->GetIsBlock()) {
			type = enBlock;
			m_modelPath = item->GetModelPath();
		}
	}
}

//クリックに合わせて動かすよん。
void ItemDisplay::LeftClickMouseToMoveHand()
{
	const int minDown = -75;
	const int plus = 5;
	if (m_player->GetIsBlockDestruction() && Limit >= maxDownPos)
	{
		Limit = minDown;
	}
	if (Limit <= maxDownPos)
	{
		Limit += plus;
	}
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), Limit);
	m_rotation.Multiply(m_rotationX);
	m_skinModelRender->SetRot(m_rotation);
}