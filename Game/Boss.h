#pragma once
#include "Enemy.h"

class Boss : public Enemy
{
public:
	Boss();
	~Boss();

	bool Start()override;
	void Update()override;
	void PostUpdate()override;
	void PostRender()override;

	void Attack();

	void TakenDamage(int attackDamage)override;

	void SetEnableEntityInner2(bool enable)override {
		m_spriteRenderHp.SetEnable(enable);
		m_spriteRenderHpGauge.SetEnable(enable);
		
		m_weaponModelRender.SetEnable(enable);

		if (enable) {
			if (!m_isPlayBGM) {
				if (!m_game->GetIsStopBGM()) {
					m_bgm = NewGO<SuicideObj::CBGM>(L"Resource/soundData/enemy/boss/bossbgm.wav");
					m_bgm->SetVolume(0.3f);
					m_bgm->Play(false, true);
				}
				m_game->StopBGM(true);
			}
		}
		else {
			if (m_isPlayBGM) {
				m_game->StopBGM(false);
			}
			if (m_bgm) {
				DeleteGO(m_bgm);
				m_bgm = nullptr;
			}
		}
		m_isPlayBGM = enable;
	}

	bool AddItem(std::unique_ptr<ItemStack>& item);

private:
	void Move();
	CVector3 GetDamageColPos();		//ダメージ判定の座標。		

	int m_damageTimer = 0;	//ダメージ判定生成タイマー。

	//HPバー
	CFont m_dotFont;
	GameObj::CSpriteRender m_spriteRenderHp;
	GameObj::CSpriteRender m_spriteRenderHpGauge;

	//BGM
	bool m_isPlayBGM = false;
	SuicideObj::CBGM* m_bgm = nullptr;

	//ステート
	enum enBossState {
		enBoss_tracking,
		enBoss_guard,
		enBoss_preAttack,					
		enBoss_attack,							
		enBoss_run,
		enBoss_damege,

		enBoss_num,	
	};
	enBossState mBossState = enBoss_tracking;
	float m_stateTimer = 0.0f;

	//歩き
	CVector3 m_posOffset;
	float m_footSoundTimer = 0.0f;

	Bone* m_rightHandBone = nullptr;
	GameObj::CSkinModelRender m_weaponModelRender;//武器モデル

	int m_getItemPoint = 0;
	int m_destroyItemDamege = 0;
	float m_destroyBlockSec = 0.0f;

	//個体番号
	int m_kotainum = 0;
};