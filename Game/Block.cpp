#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"
#include "Light.h"
#include "BlockFactory.h"

namespace {
	constexpr float half = Block::WIDTH * 0.5f;
}

Block::Block(){
}

Block::~Block(){
	if (m_state != enCube_None) {
		m_state = enCube_None;
		CalcAddLight(true);
	}
}

AABB Block::GetAABB()const {
	//�u���b�NAABB
	AABB aabb = BlockFactory::GetAABB(m_state);
	aabb.min += GetModelPos();
	aabb.max += GetModelPos();
	return aabb;
}

bool Block::GetIsOpacity()const {
	return BlockFactory::GetIsOpacity(m_state);
}

void Block::CalcAddLight(bool isDestroy) {
	//�v�Z�s�v
	if (m_state == enCube_None && !isDestroy) {
		return;
	}

	//���C�g��(������)
	char lightPower = 0;
	char skyLightPower = 0;
	if (!isDestroy) {
		lightPower = BlockFactory::GetLight(m_state);
	}

	//���[���h
	World* world = FindGO<World>(L"World");

	//���̃u���b�N�̍��W���߂�
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	IntVector3 blockpos = { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };

	//���̃u���b�N���̖̂��邳��ݒ�
	auto light = world->GetLightData(blockpos);
	if (!light) {
		return;
	}
	auto skyLight = world->GetSkyLightData(blockpos);

	//�����u���b�N
	if (BlockFactory::GetIsOpacity(m_state) == false) {
		skyLightPower = *skyLight;
		if (!isDestroy && lightPower < *light) {
			lightPower = *light;
		}
	}

	bool isLight = *light < lightPower;//���Ƃ�薾�邢������?
	char oldLightPower = *light;//�X�V�O�̃��C�g��
	char oldSkyLightPower = *skyLight;//�X�V�O�̃X�J�C���C�g��

	//�X�V
	*light = lightPower;
	*skyLight = skyLightPower;

	//���̓`�d
	if (!isLight) {//�Â��Ȃ���
		int count =	LightUtil::SpreadDark(world, oldLightPower, blockpos, { 0,0,0 }, false);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop:%d \n", count)
	}
	if (*skyLight != oldSkyLightPower) {
		int count = LightUtil::SpreadDark(world, oldSkyLightPower, blockpos, { 0,0,0 }, true);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop(sky):%d \n", count)
	}

	//�X�J�C���C�g�v�Z
	SkyLight sky(world);
	sky.CalcSkyLightThisPosition(blockpos, !isDestroy);

	//�j��
	if (isDestroy) {
		//TODO ���������ł���?
		for (int sb = 0; sb < 6; sb++) {
			//���肩�����`��������
			auto pos = blockpos + LightUtil::spreadDir[sb];
			//�u���b�N
			char* samplight = world->GetLightData(pos);
			if (samplight) {
				LightUtil::SpreadLight(world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, false);
			}
			//�X�J�C
			samplight = world->GetSkyLightData(pos);
			if (samplight) {
				LightUtil::SpreadLight(world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, true);
			}
		}
		lightPower = *light;
		skyLightPower = *skyLight;
	}

	//�X�V���邩?
	bool reflesh = oldLightPower != lightPower;
	bool refleshSky = oldSkyLightPower != skyLightPower;

	//�����Ǝ��͂̃��C�e�B���O�`��X�V
	RefleshDrawLighting(world, blockpos, lightPower, skyLightPower);

	if (reflesh) {
		//�シ������͓`�����Ȃ�
		if (lightPower <= 1) {
			return;
		}
		//����`��������
		LightUtil::SpreadLight(world, lightPower - 1, blockpos, { 0,0,0 }, false);
	}
	if (refleshSky) {
		//�シ������͓`�����Ȃ�
		if (skyLightPower <= 1) {
			return;
		}
		//����`��������
		LightUtil::SpreadLight(world, skyLightPower - 1, blockpos, { 0,0,0 }, true);
	}
}

void Block::RefleshDrawLighting(World* world, const IntVector3& blockpos, char lightPower, char skyLightPower) {
	//�����̃��C�e�B���O
	for (int sb = 0; sb < 6; sb++) {
		auto pos = blockpos + LightUtil::spreadDir[sb] * -1;
		//�u���b�N���C�g
		if(lightPower >= 0){
			char setpow = lightPower;
			char* lihght = world->GetLightData(pos);
			if (lihght) {
				setpow = max(setpow, *lihght);
			}
			if (sb < 4) {
				SetLightingData(sb, 0, setpow);
			}
			else {
				SetLightingData(sb - 4, 1, setpow);
			}
		}
		//�X�J�C���C�g
		if(skyLightPower >= 0){
			char setpow = skyLightPower;
			char* lihght = world->GetSkyLightData(pos);
			if (lihght) {
				setpow = max(setpow, *lihght);
			}
			if (sb < 4) {
				SetLightingData(sb, 2, setpow);
			}
			else {
				SetLightingData(sb - 4, 3, setpow);
			}
		}
	}
	//�㉺���E�O��̃u���b�N�̃��C�e�B���O
	for (int sb = 0; sb < 6; sb++) {
		IntVector3 samplePos = blockpos + LightUtil::spreadDir[sb];
		Block* block = world->GetBlock(samplePos);
		if (block) {
			if (sb < 4) {
				if (lightPower >= 0)block->SetLightingData(sb, 0, lightPower);
				if (skyLightPower >= 0)block->SetLightingData(sb, 2, skyLightPower);
			}
			else {
				if (lightPower >= 0)block->SetLightingData(sb - 4, 1, lightPower);
				if (skyLightPower >= 0)block->SetLightingData(sb - 4, 3, skyLightPower);
			}
		}
	}
}

void Block::InitModel(const wchar_t* filePath) {
	//instanceMax�͂��łɃ��f�������[�h����Ă���ꍇ�͎g���Ȃ��̂Œl�����ł��֌W�Ȃ��B
	m_model.Init(0, filePath);
	m_model.SetRot(CQuaternion(CVector3::AxisY(), ((CMath::RandomInt() % 4) * CMath::PI_HALF)));//���f�������_���ŉ�]
	m_model.SetParamPtr(&m_lighting);//���C�e�B���O���̐ݒ�

	//���C�g�����f��
	m_raytraceModel.Init(m_model);

	//�f�t�H�̖��邳
	for (int _1 = 0; _1 < 4; _1++) {
		for (int _2 = 0; _2 < 4; _2++) {
			SetLightingData(_1, _2, 0);
		}
	}
}

void Block::SetPos( int x, int y, int z ){
	CVector3 pos{ x * WIDTH + half, y * WIDTH, z * WIDTH + half };

	m_model.SetPos( pos );

	if( m_collision ){
		pos.y += half;
		m_collision->SetPosition( pos );
	}

	//���̌v�Z ��������Ȃ�
	CalcAddLight();
}

void Block::EnableCollision(){
	if( !m_collision ){
		AABB aabb = BlockFactory::GetAABB(m_state);

		m_collision = std::make_unique<SuicideObj::CCollisionObj>();
		m_collision->SetIsStaticObject( true );
		m_collision->CreateBox(CVector3::Zero(), CQuaternion::Identity(), aabb.max - aabb.min);
		m_collision->SetTimer( enNoTimer );
		m_collision->SetIsHurtCollision(true);	//�������画����Ƃ�Ȃ��B
		m_collision->SetName(L"Block");			//�R���W�����ɖ��O�B
		m_collision->SetPointer(this);			//�|�C���^��ݒ�B
		//m_collision->GetCollisionObject().setUserIndex(enBlock);
		//m_collision->GetCollisionObject().setUserPointer(this);		

		CVector3 pos = m_model.GetPos();
		pos.y += aabb.max.y * 0.5f;

		m_collision->SetPosition(pos);
	}
}
