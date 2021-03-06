#include "stdafx.h"
#include "Cow.h"

namespace {
	CVector3 setScale = { 0.75f,0.75f,0.75f };
	const int KomuraNum = 10;
}

Cow::Cow() : Animals(enEntity_Cow)
{
}

Cow::~Cow()
{

}


bool Cow::Start()
{
	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/Cow.tkm");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(setScale);
	m_skinModelRender->SetRot(m_rot);
	//レイトレモデル初期化
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = KomuraNum;					//体力の設定。

	m_damageVoice = L"Resource/soundData/enemy/cow.wav";		//ボイス。
	m_deathVoice = L"Resource/soundData/enemy/cow.wav";		//ボイス。

	SetDropItemId(enItem_Raw_Meat);
	SetChanceDropping(100);
	SetDropItemId(enItem_Leather,true);
	SetChanceDropping(35,true);
	
	return true;
}

void Cow::Update()
{
	if (m_position.y <= 3.f) {
		return;
	}
	//存在しているとき。
	if (m_animalState != enAnimals_death) {
		//まだ動かさない。
		//実質動かないよ。
		m_damageCollision->Update();
		CVector3 colPos = { m_position.x, m_position.y + m_characonHeight + m_characonRadius, m_position.z };	//当たり判定の座標。
		m_damageCollision->SetPosition(colPos);
		m_skinModelRender->SetPos(m_position);
		m_characon.SetPosition(m_position);
		Jump();
	}	
	m_characonMove = m_direction * m_moveSpeed;
	m_position = m_characon.Execute(m_characonMove);
	KnockBack();	//ノックバック。
	Death();		//死亡判定。
}