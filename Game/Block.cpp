#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"
#include "Light.h"

namespace {
	constexpr float half = Block::WIDTH * 0.5f;
}

Block::Block(){
}

Block::~Block(){
	if (m_state != enCube_None) {
		m_state = enCube_None;
		//CalcDestroyLight();//���Ȃ��ꍇ(����̕�)
		CalcAddLight(true);
	}
}

void Block::CalcAddLight(bool isDestroy) {
	//�v�Z�s�v
	if (m_state == enCube_None && !isDestroy) {
		return;
	}

	int lightPower = 0;//���C�g��(������)

	if (!isDestroy) {
		//�Ƃ肠�����ې΂������Ƃ��Ĉ���
		if (m_state == enCube_CobbleStone) {
			lightPower = 14;
		}
	}

	//���[���h
	World* world = FindGO<World>(L"World");

	//���̃u���b�N�̍��W���߂�
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	IntVector3 blockpos = { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };

	DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "���邳���x�����͈͊O�ł�")

	//���̃u���b�N���̖̂��邳��ݒ�
	auto light = world->GetLightData(blockpos);

	//DW_ERRORBOX(light == nullptr, "�ł�")
	if (!light) {
		return;
	}

	bool isLight = *light < lightPower;
	char oldLightPower = *light;
	//�X�V
	*light = lightPower;

	if (!isLight) {//�Â��Ȃ���
		//���̓`�d
		//for (int sb = 0; sb < 2; sb++) {
		int count =	LightUtil::SpreadDark(world, oldLightPower, blockpos, { 0,0,0 }, false);
		DW_WARNING_MESSAGE(true, "SpreadDark_Loop:%d \n", count)
		//}
		/*for (int sb = 0; sb < 6; sb++) {
			IntVector3 samplePos = blockpos + LightUtil::spreadDir[sb];
			char* lihght = world->GetLightData(samplePos);
			if (lihght && *lihght > 1) {
				LightUtil::SpreadLight(world, *lihght - 1, samplePos, { 0,0,0 }, false);
			}
		}*/
	}

	if (isDestroy) {
		for (int sb = 0; sb < 6; sb++) {
			//���肩�����`��������
			auto pos = blockpos + LightUtil::spreadDir[sb];
			char* light = world->GetLightData(pos);
			LightUtil::SpreadLight(world, *light - 1, pos, LightUtil::spreadDir[sb]*-1, false);
			//TODO ���������ł���?
		}
		lightPower = *light;
	}

	bool reflesh = oldLightPower != lightPower;

	//�����̃��C�e�B���O
	for (int sb = 0; sb < 6; sb++) {		
		char setpow = lightPower;
		auto pos = blockpos + LightUtil::spreadDir[sb] * -1;
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
	//�㉺���E�O��̃u���b�N�̃��C�e�B���O
	for (int sb = 0; sb < 6; sb++) {
		IntVector3 samplePos = blockpos + LightUtil::spreadDir[sb];
		Block* block = world->GetBlock(samplePos);
		if (block) {
			if (sb < 4) {
				block->SetLightingData(sb, 0, lightPower);
			}
			else {
				block->SetLightingData(sb - 4, 1, lightPower);
			}
		}
	}

	//�X�V�s�v
	if (!reflesh) {
		return;
	}	

	//�シ������͓`�����Ȃ�
	if (lightPower <= 1) {
		return;
	}
	//����`��������
	LightUtil::SpreadLight(world, lightPower - 1, blockpos, { 0,0,0 }, false);
	
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

void Block::SetPosWithWorldPos(const CVector3& worldpos) {
	m_model.SetPos(worldpos);

	if (m_collision) {
		m_collision->SetPosition(worldpos + CVector3::Up() * (WIDTH * 0.5f));
	}
}

void Block::EnableCollision(){
	if( !m_collision ){
		m_collision = std::make_unique<SuicideObj::CCollisionObj>();
		m_collision->SetIsStaticObject( true );
		m_collision->CreateBox( CVector3::Zero(), CQuaternion::Identity(), CVector3::One() * Block::WIDTH );
		m_collision->SetTimer( enNoTimer );
		m_collision->SetIsHurtCollision(true);	//�������画����Ƃ�Ȃ��B
		m_collision->SetName(L"Block");			//�R���W�����ɖ��O�B
		m_collision->SetPointer(this);			//�|�C���^��ݒ�B
		//m_collision->GetCollisionObject().setUserIndex(enBlock);
		//m_collision->GetCollisionObject().setUserPointer(this);		

		CVector3 pos = m_model.GetPos();
		pos.y += WIDTH * 0.5f;

		m_collision->SetPosition(pos);
	}
}
