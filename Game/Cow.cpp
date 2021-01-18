#include "stdafx.h"
#include "Cow.h"

Cow::~Cow()
{

}


bool Cow::Start()
{
	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/Cow.tkm");
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//レイトレモデル初期化
	m_raytraceModel.Init(*m_skinModelRender);
	m_characon.SetPosition(m_position);

	m_hp = 10;					//体力の設定。

	m_damageVoice = L"Resource/soundData/enemy/cat1.wav";		//ボイス。
	m_deathVoice = L"Resource/soundData/enemy/cat2.wav";		//ボイス。

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
	}
	KnockBack();	//ノックバック。
	Death();		//死亡判定。
}