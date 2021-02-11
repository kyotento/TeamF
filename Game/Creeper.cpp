#include "stdafx.h"
#include "Creeper.h"

namespace {
	constexpr int bombArea = 10;
	constexpr float ATTACK_DISTANCE = Block::WIDTH * 2.0f;
}

bool Creeper::Start()
{
	//アニメーションの初期化。
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/zombie_tracking.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);

	//スキンモデル　の初期化。
	m_skinModelRender->Init(L"Resource/modelData/zombie.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);
	//レイトレモデル初期化
	m_raytraceModel.Init(*m_skinModelRender);

	m_characon.SetPosition(m_position);

	m_hp = 1;					//体力の設定。
	m_attackPow = 30;			//攻撃力の設定。
	m_exp = 0.7f;				//取得経験値量。
	m_knockBack = 0.5f;			//ノックバック倍率。

	m_damageVoice = L"Resource/soundData/enemy/zombiedamage.wav";
	m_deathVoice = L"Resource/soundData/enemy/zombiedeath.wav";

	//アイテムドロップ
	SetDropItemId(enCube_CoalOre);
	SetChanceDropping(100);
	//レアドロップ
	SetDropItemId(enCube_WoGBlock, true);
	SetChanceDropping(5, true);

	return true;
}

void Creeper::Update()
{	
	//EscMenuが開いているとき更新を止める。
	if (m_game->GetIsEscMenu()) {
		return;
	}

	//エネミーが存在しているときのみ。
	if (m_enemyState != enEnemy_death) {
		//挙動
		{
			//サバイバルのとき。
			if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
				if ((m_player->GetPos() - GetPos()).LengthSq() < ATTACK_DISTANCE * ATTACK_DISTANCE) {
					m_enemyState = enEnemy_attack;
				}
				else if (m_enemyState != enEnemy_attack) {
					m_enemyState = enEnemy_idle;
				}
			}
			//モデル、キャラコン、当たり判定用の座標を更新。
			m_characonMove = CVector3::Down()*10.0f* m_moveSpeed;
			m_position = m_characon.Execute(m_characonMove);
			m_skinModelRender->SetPos(m_position);
			CVector3 colPos = { m_position.x, m_position.y + Block::WIDTH, m_position.z };	//当たり判定の座標。
			m_damageCollision->SetPosition(colPos);
		}
		//StateManagement();		//状態管理。

		//サバイバルのとき。
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();				//攻撃。
		}
	}
	KnockBack();	//ノックバック処理。
	Death();		//死亡判定
	AttackSun();	//太陽によるダメージ。
}

void Creeper::Attack() {
	if (m_enemyState != enEnemy_attack) {	//攻撃状態のとき。
		return;
	}

	//爆発までのカウントダウン
	if (m_bombCount == 0) {
		SuicideObj::CSE* voice;
		voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/beam5.wav");
		voice->Play();
	}
	m_bombCount++;
	if (m_bombCount % 10 < 5) {
		//モデルの色を元に戻す。
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			//mat->SetAlbedoScale(CVector4::White());
			mat->SetEmissive(1.0f);
		});
	}
	else {
		//モデルの色を白く。
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			//mat->SetAlbedoScale({ 10.0f, 10.0f, 10.0f, 1.0f });
			mat->SetEmissive(10.0f);
		});
	}

	if (m_bombCount < 120) {
		return;
	}

	//爆発
	SuicideObj::CSE* voice;
	voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/destruction1.wav");
	voice->Play();

	//ブロック破壊
	IntVector3 pos = { (int)(GetPos().x / Block::WIDTH),(int)(GetPos().y / Block::WIDTH),(int)(GetPos().z / Block::WIDTH) };
	for (int x = -bombArea; x < bombArea; x++) {
		for (int y = -bombArea; y < bombArea; y++) {
			for (int z = -bombArea; z < bombArea; z++) {
				if ((CMath::RandomZeroToOne()+0.5f) * (float)bombArea * (float)bombArea > CVector3((float)abs(x), (float)abs(y), (float)abs(z)).LengthSq()) {
					if (CMath::RandomZeroToOne() > 0.005f) {
						m_world->DestroyBlockNoDrop(pos + IntVector3(x, y, z));
					}
					else {
						m_world->DestroyBlock(pos + IntVector3(x, y, z));
					}
				}
			}
		}
	}

	//ダメージ判定
	SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
	attackCol->CreateSphere(GetPos(), m_rot, Block::WIDTH * bombArea);
	attackCol->SetTimer(0);//寿命１フレーム。
	attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
		if (param.EqualName(L"CPlayer")) {//名前検索。
			CVector3 direction = m_player->GetPos() - GetPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//距離減衰
			m_player->TakenDamage((int)(m_attackPow * damegeScale), direction, true);//プレイヤーにダメージを与える。
		}
		if (param.EqualName(L"CEnemy")) {//名前検索。
			auto enemy = param.GetClass<Enemy>();
			CVector3 direction = enemy->GetPos() - GetPos();
			float damegeScale = 1.0f - (direction.Length() / (Block::WIDTH * bombArea));//距離減衰
			enemy->TakenDamage((int)(m_attackPow * damegeScale));//ダメージを与える。
		}
	});

	//自死
	TakenDamage(10000);
}