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

	int minDownPos = -30;
	int maxDownPos = 0;

	int Limit = 0;
	int yPos = 0;

	float UpPosY = 25;
	float foodHasHandLRPos = 0.0f;
	float foodHasHandUDPos = 0.0f;

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

	//とりあえずの変数。
	int a = 0;
	bool flag = false;
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
	m_player = FindGO<Player>();
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

		//切り替えの処理。
		Switching();

		//クリックしたときのモーション処理。
		LeftClickMouseToMoveHand();

		//右クリック
		RightClickMouseToEat();
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
	m_isItemChangeFlag = true;
	Switching();
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
		//追従処理。
		Follow();
		//回転
		Rotation();
	}
	else
	{
		//右手のボーンの処理。
		NotFPSCameraOfRightHandDraw();
		//右手の回転処理
		NotFPSCameraOfItemRotation();
	}
}
//再度。
void ItemDisplay::BuildAgain()
{
	auto& item = m_player->GetInventory().GetItem(m_player->GetSelectItemNum() - 1);

	//一度だけ生成しなおす。
	//何も持ってない時。
	if (initItem_flag && type == enHand)
	{
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(L"Resource/modelData/playerhand.tkm");
		initItem_flag = false;
	}	
	else if (initItem_flag && item->GetItem().GetModelPath() == "Resource/modelData/2DFound.tkm") {
		DeleteGO(m_skinModelRender);
		m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
		m_skinModelRender->Init(item->GetItem().GetModelPath().c_str());//モデルの初期化。
		m_skinModelRender->GetSkinModel().InitMaterialSetting();
		m_skinModelRender->GetSkinModel().FindMaterialSetting([&](MaterialSetting* mts) {
			mts->SetAlbedoTexture(item->GetItem().GetImage().GetTextueData());
		});
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
	m_mullFornt = toolMullFront + foodHasHandUDPos;;
	m_mullCrossProduct = toolMullCross + foodHasHandLRPos;
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
	m_mullFornt = itemMullFront + foodHasHandUDPos;
	m_mullCrossProduct = itemMullCross + foodHasHandLRPos;
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
//食べるよん。
void ItemDisplay::RightClickMouseToEat()
{
	if (m_player->GetIsEating())
	{
		foodHasHandLRPos = -35.0f;
		foodHasHandUDPos = -5.0f;
		m_isUpDownFlag = true;
		UpDown();	//実際に上下に動かしてるのはここ。
	}
	else
	{
		foodHasHandLRPos = 0.0f;
		foodHasHandUDPos = 0.0f;
		m_isUpDownFlag = false;
	}
}
//雑に食べてる表現。
void ItemDisplay::UpDown()
{
	const float UPDOWN = 0.5f;
	const int maxA = 5;
	if (m_isUpDownFlag && !flag)
	{
		UpPosY += UPDOWN;
		a++;
		if (a > maxA && !flag)
		{
			flag = true; 
			a = 0;

			//SE
			SuicideObj::CSE* eatse = nullptr;
			eatse = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/eat.wav");
			eatse->SetVolume(0.25f);
			eatse->Play();
		}
	}
	else if (m_isUpDownFlag && flag)
	{
		UpPosY -= UPDOWN;
		a++;
		if (a > maxA && flag)
		{
			flag = false;
			a = 0;

			//SE
			SuicideObj::CSE* eatse = nullptr;
			eatse = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/eat.wav");
			eatse->SetVolume(0.25f);
			eatse->Play();
		}
	}	
}
//FPS以外の時のモデル処理。
void ItemDisplay::NotFPSCameraOfRightHandDraw()
{
	m_position = m_player->GetRightHandPos();
	m_skinModelRender->SetPos(m_position);
	if (type == enHand)
	{
		m_skinModelRender->SetIsDraw(false);
	}
}
//FPS以外の時の回転
void ItemDisplay::NotFPSCameraOfItemRotation()
{
	//プレイヤーの回転を持ってくる
	m_radianY = m_player->GetRadianY();
	//ここで斜めにずらします。
	m_rotation = m_player->GetRot();
	CQuaternion m_rotationX;
	m_rotationX.SetRotationDeg(CVector3::AxisX(), m_radianY);
	m_rotation.Multiply(m_rotationX);
	//モデルへ。
	m_skinModelRender->SetRot(m_rotation);
	//スケールの変更
	m_scale = { 0.35f,0.35f,0.35f };
}