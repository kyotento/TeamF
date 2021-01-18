#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"

namespace {
	constexpr float half = Block::WIDTH * 0.5f;
	constexpr int LIGHT_POWER_MAX = 15;//���邳�̍ő�l
	//�\����̖��邳
	constexpr float DRAWING_LIGHT[LIGHT_POWER_MAX] = { 
		0.04398046511104f, 0.0549755813888f, 0.068719476736f, 0.08589934592f, 0.1073741824f,
		0.134217728f, 0.16777216f, 0.2097152f, 0.262144f, 0.32768f,
		0.4096f, 0.512f, 0.64f, 0.8f, 1.0f
	};

	//�����`���������
	constexpr IntVector3 spreadDir[6] = {
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1},
	};
}

Block::Block(){
}

Block::~Block(){
	m_raytraceModel.Release();
}

void Block::CalcAddLight() {
	//�v�Z�s�v
	if (m_state == enCube_None) {
		return;
	}

	int lightPower = 0;//���C�g��(������)

	//�Ƃ肠�����ې΂������Ƃ��Ĉ���
	if (m_state == enCube_CobbleStone) {
		lightPower = 14;
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
	bool isReflesh = false;
	for (int _1 = 0; _1 < 4; _1++) {
		for (int _2 = 0; _2 < 4; _2++) {
			auto light = world->GetLightData(blockpos.x, blockpos.y, blockpos.z);
			if (*light < lightPower) {
				isReflesh = true;
				*light = lightPower;
				m_lighting.m[_1][_2] = max(m_lighting.m[_1][_2], DRAWING_LIGHT[lightPower]);
			}
		}
	}

	//�X�V�s�v
	if (!isReflesh) {
		return;
	}

	//�㉺���E�O��̃u���b�N���Ƃ炷
	for (int sb = 0; sb < 6; sb++) {
		IntVector3 samplePos = blockpos + spreadDir[sb];
		Block* block = world->GetBlock(samplePos.x, samplePos.y, samplePos.z);
		if (block) {
			if (sb < 4) {
				block->m_lighting.m[sb][0] = DRAWING_LIGHT[lightPower];
			}
			else {
				block->m_lighting.m[sb - 4][1] = DRAWING_LIGHT[lightPower];
			}
		}
	}

	//�シ������͓`�����Ȃ�
	if (lightPower <= 1) {
		return;
	}
	
	//����`��������
	SpreadLight(world, lightPower -1, blockpos, { 0,0,0 });
}

void Block::SpreadLight(World* world, int lightPower, const IntVector3& pos, const IntVector3& fromDir) {
	//�シ������͓`�����Ȃ�
	if (lightPower < 1) {
		return;
	}
	for (int i = 0; i < 6; i++) {//6�����ɓ`��(�O��㉺���E)
		//���������ɂ͖߂�Ȃ�
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//�T���v���ʒu�Z�o
		IntVector3 nowPos = pos + spreadDir[i];

		//���C�g���L�^
		auto light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
		if (*light < lightPower) {
			*light = lightPower;
		}
		else {
			//���̕����ւ̓`�d�͏I���(�������邢����)
			continue;
		}
		
		//�㉺���E�O��̃u���b�N���Ƃ炷
		for (int sb = 0; sb < 6; sb++) {
			IntVector3 samplePos = nowPos + spreadDir[sb];
			Block* block = world->GetBlock(samplePos.x, samplePos.y, samplePos.z);
			if (block) {
				if (sb < 4) {
					block->m_lighting.m[sb][0] = DRAWING_LIGHT[lightPower];
				}
				else {
					block->m_lighting.m[sb - 4][1] = DRAWING_LIGHT[lightPower];
				}
			}
		}

		//�u���b�N���T���v��
		Block* block = world->GetBlock(nowPos.x, nowPos.y, nowPos.z);
		if (block) {//TODO �Ȃ����s�����u���b�N			

			//TODO ���X�g�ɒǉ�

			//���̕����ւ̓`�d�͏I���(�u���b�N�ƏՓ˂�������)
		}
		else {
			//�`���𑱂���
			SpreadLight(world, lightPower - 1, nowPos, spreadDir[i]);
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
			m_lighting.m[_1][_2] = DRAWING_LIGHT[0];
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
