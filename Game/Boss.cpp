#include "stdafx.h"
#include "Boss.h"
#include "BlockFactory.h"
#include "ItemDictionary.h"
#include "ItemStack.h"

Boss::Boss() : Enemy(enEntity_Boss)
{
}

Boss::~Boss()
{
	if (m_isPlayBGM) {
		m_game->StopBGM(false);
	}
	if (m_bgm) {
		DeleteGO(m_bgm);
		m_bgm = nullptr;
	}
	if (m_world) {
		m_world->RemoveBoss(this);
	}
}

namespace {
	constexpr int HP_MAX = 20;
	constexpr int NAME_CNT = 5;
	constexpr wchar_t NAME[NAME_CNT][12] = {
		L"KOMURA",
		L"SAEKI",
		L"TAKAYAMA",
		L"YAMANAKA",
		L"HIRANO"
	};
	constexpr float PRE_ATTACK_SEC = 0.35f;//�U����������
	constexpr int RETURN_ITEM_POINT = 10;//�A�҂ɕK�v�ȃ|�C���g
	constexpr float KNOCK_BACK_GUARD = 3.75f;
	constexpr float KNOCK_BACK_DAMEGE = 10.7f;
	constexpr float ATTACK_RENGE_Y_HIGH = Block::WIDTH * 2.0f;
	constexpr float ATTACK_RENGE_Y_LOW = Block::WIDTH * -0.6f;

	//�j�󂵂Ă����u���b�N��
	bool CanDestroyBlock(EnCube type) {
		switch (type)
		{
		case enCube_Bedrock:
		case enCube_CraftingTable:
		case enCube_WoGBlock:
		//case enCube_DoorUp:
		//case enCube_DoorDown:
		case enCube_TorchBlock:
		case enCube_Torch:
		case enCube_LearnTorch:
		case enCube_BedHead:
		case enCube_BedLeg:
		case enCube_Chest:
		case enCube_Furnace:
		case enCube_TNT:
			return false;
			break;
		default:
			break;
		}
		return true;
	}
}

bool Boss::Start()
{
	m_world->SetBoss(this);

	//�A�j���[�V�����̏������B
	m_animationClips[enAnimationClip_idle].Load(L"Resource/animData/zombie_idle.tka", true);
	m_animationClips[enAnimationClip_move].Load(L"Resource/animData/zombie_move.tka", true);
	m_animationClips[enAnimationClip_tracking].Load(L"Resource/animData/player_move.tka", true);
	m_animationClips[enAnimationClip_attack].Load(L"Resource/animData/zombie_attack.tka", true);
	m_animationClips[enAniamtionClip_fan].Load(L"Resource/animData/zombie_fan.tka", true);
	m_interpolateTimeSec = 0.0f;

	m_scale *= 1.15f;

	//�X�L�����f���@�̏������B
	m_skinModelRender->Init(L"Resource/modelData/player.tkm", m_animationClips, enAnimationClip_num);
	m_skinModelRender->SetPos(m_position);
	m_skinModelRender->SetScale(m_scale);
	m_skinModelRender->SetRot(m_rot);

	//���C�g�����f��������
	m_raytraceModel.Init(*m_skinModelRender);

	//���탂�f��
	m_weaponModelRender.Init(ItemDictionary::Instance().GetItem(enItem_Diamond_Sword).GetModelPath().c_str());

	//�{�[��
	m_rightHandBone = m_skinModelRender->FindBone(L"Bone009");

	m_characon.SetPosition(m_position);

	m_hp = HP_MAX;				//�̗͂̐ݒ�B
	m_attackPow = 10;			//�U���͂̐ݒ�B
	m_exp = 7.0f;				//�擾�o���l�ʁB
	m_knockBack = KNOCK_BACK_GUARD;	//�m�b�N�o�b�N�{���B
	m_moveSpeed *= 7.0f;		//�ړ����x
	//m_attackRenge *= 2.0f;
	m_fallSpeed *= 5.0f;

	//�A�C�e���h���b�v
	SetDropItemId(enCube_Bedrock);
	SetChanceDropping(100);
	SetDropCount(10);

	//�̔ԍ�
	m_kotainum = 0;// CMath::IntUniDist(NAME_CNT - 1);

	//HP�o�[
	m_dotFont.LoadFont(L"Resource/BestTenDot.spritefont");
	m_spriteRenderHp.Init(L"Resource/spriteData/Experience_Found.dds");
	m_spriteRenderHp.SetPos({ 0.5f,0.10f });
	m_spriteRenderHp.SetScale(1.5f);
	m_spriteRenderHpGauge.Init(L"Resource/spriteData/Experience_Gauge.dds");
	m_spriteRenderHpGauge.SetPos({ 0.288f,0.10f });
	m_spriteRenderHpGauge.SetScale(1.5f);
	m_spriteRenderHpGauge.SetPivot({ 0.f, 0.5f });
	m_spriteRenderHpGauge.SetColor({ 0.5f, 0.0f, 0.5f, 0.2f });

	//BGM
	if (!m_game->GetIsStopBGM()) {
		m_bgm = NewGO<SuicideObj::CBGM>(L"Resource/soundData/enemy/boss/bossbgm.wav");
		m_bgm->SetVolume(0.3f);
		m_bgm->Play(false, true);
	}
	m_game->StopBGM(true);
	m_isPlayBGM = true;

	//�G���g���[�{�C�X
	SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/entry.wav");
	voice->Play();

	//�{�C�X
	m_damageVoice = L"Resource/soundData/enemy/boss/damege3.wav";
	m_deathVoice = L"Resource/soundData/enemy/boss/death2.wav";

	return true;
}

void Boss::Update()
{
	//EscMenu���J���Ă���Ƃ��X�V���~�߂�B
	if (m_game->GetIsEscMenu()){// ||GetKeyInput(VK_RETURN)) {
		return;
	}

	//�G�l�~�[�����݂��Ă���Ƃ��̂݁B
	if (m_enemyState != enEnemy_death) {

		CVector3 move;

		//�X�e�[�g
		if (m_stateTimer > 0.0f) {
			m_stateTimer -= GetDeltaTimeSec();
		}		
		CVector3 distance = (m_player->GetPos() - m_position);
		switch (mBossState) {
		case enBoss_tracking:
			m_enemyState = enEnemy_tracking;
			m_animSpeed = 10.0f;

			//����
			m_footSoundTimer += GetDeltaTimeSec();
			if (m_footSoundTimer > 0.1f) {
				m_footSoundTimer = 0.0f;
				
				SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/walk.wav");
				voice->SetVolume(0.125f);
				voice->SetPos(m_position);
				voice->SetDistance(1000.0f);
				voice->Play(true);

				if (m_posOffset.y > 0.0f) {
					m_posOffset.y = 0.0f;
				}
				else {
					m_posOffset.y = 15.0f;
				}
			}

			//�o��
			//�T�o�C�o���̂Ƃ��B
			if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
				if ((distance * CVector3(1.0f, 0.0f, 1.0f)).LengthSq() < CMath::Square(m_attackRenge)
					&& distance.y < ATTACK_RENGE_Y_HIGH
					&& distance.y > ATTACK_RENGE_Y_LOW
					) {
					mBossState = enBoss_preAttack;
					m_stateTimer = PRE_ATTACK_SEC;//�U����������

					SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/preAttack.wav");
					//voice->SetVolume(0.5f); 
					voice->SetPos(m_position);
					voice->SetDistance(1000.0f);
					voice->Play(true);
				}
			}
			break;
		case enBoss_guard:
			m_enemyState = enEnemy_idle;
			m_animSpeed = 1.0f;
			//�o��
			if (m_stateTimer < FLT_EPSILON) {
				mBossState = enBoss_tracking;
				//�F�߂�
				m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
					mat->SetEmissive(0.0f);
				});
			}
			break;
		case enBoss_preAttack:
			m_enemyState = enEnemy_fan;
			m_animSpeed = 1.5f;

			//�o��
			if (m_stateTimer < FLT_EPSILON) {
				mBossState = enBoss_attack;
				m_stateTimer = 0.3f;//�U������
				m_damageTimer = 0;

				SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/attack.wav");
				//voice->SetVolume(0.5f);
				voice->SetPos(m_position);
				voice->SetDistance(1000.0f);
				voice->Play(true);
			}
			break;
		case enBoss_attack:
			m_enemyState = enEnemy_attack;
			m_animSpeed = 1.0f;

			//�o��
			if (m_stateTimer < FLT_EPSILON) {
				mBossState = enBoss_tracking;
			}
			break;
		case enBoss_damege:
			m_enemyState = enEnemy_move;
			m_animSpeed = 3.0f;
			m_knockBack = KNOCK_BACK_DAMEGE;

			//�o��
			if (!m_isTakenDamage) {
				mBossState = enBoss_tracking;
				m_knockBack = KNOCK_BACK_GUARD;
			}
			break;
		default:
			break;
		}


		Move();//����

		//�T�o�C�o���̂Ƃ��B
		if (m_gameMode->GetGameMode() == GameMode::enGameModeSurvival) {
			Attack();//�U���B
		}

		StateManagement();		//��ԊǗ��B
	}

	KnockBack();	//�m�b�N�o�b�N�����B

	//�K�[�h���̃��f���F
	if (mBossState == enBoss_guard) {
		m_skinModelRender->GetSkinModel().FindMaterialSetting([](MaterialSetting* mat) {
			mat->SetAlbedoScale(CVector4::White());
			mat->SetEmissive(5.0f);
		});
	}

	Death();		//���S����
}

//�U�������B
void Boss::Attack()
{
	//�U����Ԃ̂Ƃ��B
	if (m_enemyState == enEnemy_attack) {
		m_damageTimer += 1;//�_���[�W����p�^�C�}�[�����Z�B
		if (m_damageTimer == 2) {//�_���[�W����p�̃t���[���ɓ��B�������B
			SuicideObj::CCollisionObj* attackCol = NewGO<SuicideObj::CCollisionObj>();
			attackCol->CreateBox(GetDamageColPos(), m_rot, Block::WIDTH * 1.5f);
			attackCol->SetTimer(0);	//�����P�t���[���B
			attackCol->SetCallback([&](SuicideObj::CCollisionObj::SCallbackParam& param) {
				if (param.EqualName(L"CPlayer")) {
					float beforeHP = m_player->GetHP();

					CVector3 direction = GetDamageColPos() - GetPos();
					m_player->TakenDamage(m_attackPow, direction, true);

					//�����{�C�X
					if (beforeHP > 0.0f && m_player->GetHP() < FLT_EPSILON) {
						constexpr wchar_t winVoicePath[3][64] = {
							L"Resource/soundData/enemy/boss/win.wav",
							L"Resource/soundData/enemy/boss/win2.wav",
							L"Resource/soundData/enemy/boss/win3.wav"
						};
						SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(winVoicePath[CMath::IntUniDist(2)]);
						voice->Play();
						DeleteGO(this);
					}
				}
				if (param.EqualName(L"CEnemy")) {
					Enemy* enemy = param.GetClass<Enemy>();
					if (enemy != this) {
						enemy->TakenDamage(m_attackPow);
					}
				}
			});
		}
	}
	else {
		//�U����ԂłȂ��Ƃ��B
		m_damageTimer = 0;
	}
}

//�]���r�̃_���[�W����̍��W���擾�B
CVector3 Boss::GetDamageColPos()
{
	return m_rightHandBone->GetPosition();
}

void Boss::TakenDamage(int attackDamage)
{
	if (mBossState == enBoss_preAttack && m_stateTimer < 0.1f) {
		//�_���[�W
		constexpr float defensePower = 20.0f;
		Enemy::TakenDamage((int)(attackDamage * (1.f - defensePower * 0.04f)));
		mBossState = enBoss_damege;
	}
	else {
		//�K�[�h
		mBossState = enBoss_guard;
		m_stateTimer = 0.25f;
		m_isTakenDamage = true;	//�_���[�W�t���O��Ԃ�(�m�b�N�o�b�N�̂��߂�)

		SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/guard.wav");
		voice->Play();
	}
}

void Boss::Move()
{
	bool nonDestroyMode = true, isCanPlaceBlock =false;

	m_direction = (m_player->GetPos() - m_position);
	m_oldDirection = m_direction;
	m_direction = CVector3::Zero();

	//����
	if (!m_characon.IsOnGround()) {
		m_direction.y -= m_fallSpeed;
	}

	//�Ǐ]����
	if ((mBossState == enBoss_tracking || mBossState == enBoss_attack || mBossState == enBoss_preAttack && m_stateTimer > 0.1f) && !m_isTakenDamage) {
		//�Ǐ]
		CVector3 tracking = m_oldDirection;
		tracking.y = 0.0f;
		tracking.Normalize();

		CVector3 XZ = m_oldDirection; XZ.y = 0.0f;
		float XZLengthSq = XZ.LengthSq();
		//���ʒǐ�
		if (XZLengthSq > CMath::Square(m_attackRenge))
		{
			m_direction += tracking;
		}

		//���ꂪ�K�v�ȍ�����
		bool isHighUp = false;
		if (XZLengthSq < CMath::Square(Block::WIDTH * 6.0f) && m_oldDirection.y > ATTACK_RENGE_Y_HIGH)
		{
			isHighUp = true;
		}
		//�����@��K�v�������邩
		bool isDig = false;
		if (XZLengthSq < CMath::Square(Block::WIDTH * 6.0f) && m_oldDirection.y < ATTACK_RENGE_Y_LOW)
		{
			isDig = true;
		}

		if (mBossState == enBoss_tracking) {
			//�u���b�N�j��
			if (m_characon.IsContactWall() || isHighUp || isDig) {
				auto breakP = IntVector3(m_position / Block::WIDTH);

				if (isDig) {//���@��
					breakP.y -= 1;
					for (int i = 0; i <= 8; i++) {
						IntVector3 sampP = breakP;
						if (i >= 1 && i <= 3) {
							sampP.x++;
							if (i == 2) { sampP.z++; }
							if (i == 3) { sampP.z--; }
						}
						if (i >= 4 && i <= 6) {
							sampP.x--;
							if (i == 5) { sampP.z++; }
							if (i == 6) { sampP.z--; }
						}
						if (i == 7) { sampP.z++; }
						if (i == 8) { sampP.z--; }

						Block* block = m_world->GetBlock(sampP);
						if (block && CanDestroyBlock(block->GetBlockType())) {
							breakP = sampP;
							break;
						}
					}
				}
				else if (isHighUp) {//�V��j��
					bool isBreak = false;
					for (int i2 = 0; i2 <= 2; i2++) {
						breakP.y++;
						for (int i = 0; i <= 8; i++) {
							IntVector3 sampP = breakP;
							if (i >= 1 && i <= 3) {
								sampP.x++;
								if (i == 2) { sampP.z++; }
								if (i == 3) { sampP.z--; }
							}
							if (i >= 4 && i <= 6) {
								sampP.x--;
								if (i == 5) { sampP.z++; }
								if (i == 6) { sampP.z--; }
							}
							if (i == 7) { sampP.z++; }
							if (i == 8) { sampP.z--; }

							Block* block = m_world->GetBlock(sampP);
							if (block && CanDestroyBlock(block->GetBlockType())) {
								breakP = sampP;
								isBreak = true;
								break;
							}
						}
						if (isBreak) {
							break;
						}
					}
				}
				else {//�ǔj��
					breakP = breakP + IntVector3((int)std::round(tracking.x), (int)std::floor(tracking.y), (int)std::round(tracking.z));
					for (int i = 0; i < 15; i++) {
						IntVector3 sampP = breakP;

						if (i >= 1 && i <= 2) {//�c
							sampP.y += i;
						}
						if (i >= 4 && i <= 5) {//�c
							sampP.y += i - 3;
						}
						if (i >= 7 && i <= 8) {//�c
							sampP.y += i - 6;
						}
						if (i >= 10 && i <= 11) {//�c
							sampP.y += i - 9;
						}
						if (i >= 13 && i <= 14) {//�c
							sampP.y += i - 12;
						}
						if (i >= 3) {//��
							IntVector3 side = IntVector3::Zero();
							if (i >= 3 && i <= 5) {
								side.x += -1;
							}
							if (i >= 6 && i <= 8) {
								side.x += 1;
							}
							if (i >= 9 && i <= 11) {
								side.z += -1;
							}
							if (i >= 12 && i <= 14) {
								side.z += 1;
							}
							sampP += side;
						}

						Block* block = m_world->GetBlock(sampP);
						if (block && CanDestroyBlock(block->GetBlockType())) {
							breakP = sampP;
							break;
						}
					}
				}

				//0.3�a�Ɉ��10����
				m_destroyBlockSec += GetDeltaTimeSec();
				if (m_destroyBlockSec > 0.15f) {
					m_destroyBlockSec = 0.0f;
					m_destroyItemDamege += 10;
				}
				Block* block = m_world->GetBlock(breakP);
				if (!block) {
					isCanPlaceBlock = true;
				}
				if (block && CanDestroyBlock(block->GetBlockType()) && block->GetHP() < m_destroyItemDamege)
				{
					m_world->DestroyBlock(breakP);
					m_destroyItemDamege = 0;
				}

				nonDestroyMode = false;

				/*SuicideObj::CSE* destroy;
				destroy = NewGO<SuicideObj::CSE>(L"Resource/soundData/block/blockdestroy.wav");
				destroy->SetVolume(0.5f);
				destroy->SetPos(m_position);
				destroy->SetDistance(1000.0f);
				destroy->Play(true);*/
			}

			//�u���b�N�ݒu
			if (isHighUp && (nonDestroyMode || isCanPlaceBlock)) {
				if (m_world->PlaceBlock((m_position + CVector3::Up() * Block::WIDTH * 0.5f) / Block::WIDTH, BlockFactory::CreateBlock(EnCube::enCube_OakLeaf)))
				{
					/*SuicideObj::CSE* se;
					se = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/put.wav");
					se->SetVolume(0.5f);
					se->SetPos(m_position);
					se->SetDistance(1000.0f);
					se->Play(true);		*/
				}
			}
		}

		//�W�����v�����B
		if ((m_characon.IsContactWall() || m_oldDirection.y > Block::WIDTH) && m_characon.IsOnGround())
		{
			m_jumpFlag = true;
			m_jmpInitialVelocity = 13.f * 0.5f;
		}
		if (m_jumpFlag) {
			m_direction.y = m_jmpInitialVelocity;
			m_jmpInitialVelocity -= m_gravity;
			if (m_characon.IsOnGround() && m_jmpInitialVelocity < m_gravity) {
				m_jumpFlag = false;
			}
		}
	}

	//���f���A�L�����R���A�����蔻��p�̍��W���X�V�B
	m_characonMove = m_direction * m_moveSpeed;
	m_characonMove.y = m_direction.y * m_moveSpeed;
	m_position = m_characon.Execute(m_characonMove);
	m_skinModelRender->SetPos(m_position + m_posOffset);
	m_damageCollision->SetPosition({ m_position.x, m_position.y + Block::WIDTH, m_position.z });

	//�v���C���[�̕��������������B
	if (mBossState != enBoss_attack && mBossState != enBoss_preAttack) {
		m_oldDirection.Normalize();
		m_rot.SetRotation(CVector3::AxisY(), atan2f(m_oldDirection.x, m_oldDirection.z));
		m_skinModelRender->SetRot(m_rot);
	}

	if(nonDestroyMode) {
		//�u���b�N�j�󃊃Z�b�g
		m_destroyBlockSec = 0.0f;
		m_destroyItemDamege = 0;
	}
}

void Boss::PostRender()
{
	//HUD
	m_spriteRenderHpGauge.SetScale({ ((float)m_hp / HP_MAX) * 1.5f , 1.5f });
	m_dotFont.Draw(NAME[m_kotainum], { 0.5f,0.05f }, { 0.75f, 0.75f, 0.75f, 1.0f }, { 0.75f, 0.75f }, { 0.5f,0.5f });
	
	std::wstringstream str;
	str << "GIVE ME ITEM : " << m_getItemPoint << "/" << RETURN_ITEM_POINT << "\n";
	m_dotFont.Draw(str.str().c_str(), { 0.5f,0.125f }, { 0.75f, 0.75f, 0.75f, 1.0f }, { 0.75f, 0.75f }, {0.5f,0.5f});
}

void Boss::PostUpdate()
{
	//����ʒu
	m_weaponModelRender.SetPos(m_rightHandBone->GetPosition());
	
	//�����]
	CQuaternion rot = m_rightHandBone->GetRotation();
	if (mBossState != enBoss_preAttack) {
		rot.Multiply(CQuaternion(CVector3::AxisY(), CMath::DegToRad(90.0f)));
	}
	else {
		rot.Multiply(CQuaternion(CVector3::AxisX(), CMath::PI2 * CMath::RandomZeroToOne()));
	}
	m_weaponModelRender.SetRot(rot);
}

bool Boss::AddItem(std::unique_ptr<ItemStack>& item) {
	int point = (int)(item->GetFoodLevel() + item->GetFuelLevel() / 5 + item->GetToolLevel() - 1);
	point = max(0, point);
	point *= 2;

	switch (item->GetID())
	{	
	case enCube_CoalOre:		//�ΒY�z�΁B
		point += 1;
		break;
	case enCube_CoalBlock:		//�ΒY�u���b�N�B
		point += 10;
		break;
	case enCube_IronOre:		//�S�z�΁B
		point += 2;
		break;
	case enCube_IronBlock:		//�S�u���b�N�B
		point += 10;
		break;
	case enCube_GoldOre:		//���z�΁B
		point += 5;
		break;
	case enCube_GoldBlock:		//���u���b�N�B
		point += 10;
		break;
	case enCube_DiamondOre:		//�_�C�A�����h�z�΁B
		point += 5;
		break;
	case enCube_DiamondBlock:	//�_�C�A�����h�u���b�N
		point += 10;
		break;

	case enItem_Iron_Ingot:
		point += 2;
		break;
	case enItem_Gold_Ingot:
		point += 5;
		break;
	case enItem_Diamond:
		point += 5;
		break;
	case enItem_Leather:
		point += 1;
		break;

	default:
		break;
	}

	if (point > 0) {
		SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/itemOK2.wav");
		voice->Play();

		//���|�C���g�l���ŋA�҂���
		m_getItemPoint += point;
		if (m_getItemPoint >= RETURN_ITEM_POINT) {
			SuicideObj::CSE* voice = NewGO<SuicideObj::CSE>(L"Resource/soundData/enemy/boss/return.wav");
			voice->Play();
			DeleteGO(this);
		}
	}
	else {
		SuicideObj::CSE* se;
		se = NewGO<SuicideObj::CSE>(L"Resource/soundData/player/get.wav");
		se->SetVolume(0.3f);
		se->SetPos(m_position);
		se->SetDistance(1000.0f);
		se->Play(true);
	}

	item.reset();
	return true;
}
