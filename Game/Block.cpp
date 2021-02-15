#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"
#include "Light.h"
#include "BlockFactory.h"

bool Block::m_sDestroyMode = false;
World* Block::m_sWorld = nullptr;

namespace {
	constexpr float half = Block::WIDTH * 0.5f;

	constexpr IntVector3 mukiDir[] = {
		{ 1,0, 0},
		{ 0,0,-1},
		{-1,0, 0},
		{ 0,0, 1}
	};

	//AABB����]����
	void RotAABB(const CQuaternion& rot, AABB& aabb) {
		CVector3 A = aabb.min, B = aabb.max;
		rot.Multiply(A);
		rot.Multiply(B);
		aabb.min = A; aabb.min.Min(B);
		aabb.max = A; aabb.max.Max(B);
	}
}

Block::Block(){
}

Block::~Block(){
	if (m_sDestroyMode) {
		return;
	}
	if (m_bInfo->id != enCube_None) {
		m_bInfo = &BlockInfo::NONE_BLOCK;
		CalcAddLight(true);

	}
}

const IntVector3& Block::GetMukiDir()const {
	DW_ERRORBOX(m_muki < 0 || m_muki >= enMuki::enNum, "Block::GetMukiDir()\n�ُ�Ȍ������w�肳��Ă��܂�")
	return mukiDir[m_muki];
}
const IntVector3& Block::GetMukiDir(enMuki muki) {
	DW_ERRORBOX(muki < 0 || muki >= enMuki::enNum, "Block::GetMukiDir()\n�ُ�Ȍ������w�肳��Ă��܂�")
	return mukiDir[muki];
}

void Block::Init( const BlockInfo * bInfo, enMuki muki ){
	m_bInfo = bInfo;
	m_hp = bInfo->hp;

	//instanceMax�͂��łɃ��f�������[�h����Ă���ꍇ�͎g���Ȃ��̂Œl�����ł��֌W�Ȃ��B
	m_model.Init( 0, bInfo->modelPath.c_str() );
	m_model.SetParamPtr( &m_lighting );//���C�e�B���O���̐ݒ�

	//����
	DW_ERRORBOX(muki < 0 || muki >= enMuki::enNum, "Block::Init()\n�ُ�Ȍ������w�肳��Ă��܂�")
	m_muki = muki;
	m_model.SetRot( CQuaternion( CVector3::AxisY(), CMath::PI_HALF * m_muki ) );

	CalcAABB();

	//���C�g�����f��
	m_raytraceModel.Init( m_model );

	//�f�t�H�̖��邳
	for( int _1 = 0; _1 < 4; _1++ ){
		for( int _2 = 0; _2 < 4; _2++ ){
			SetLightingData( _1, _2, 0 );
		}
	}
}

void Block::CalcAABB() {
	m_aabb.clear();

	for(const AABB& infoAabb : m_bInfo->aabbArray ){
		//�u���b�NAABB
		AABB& aabb = m_aabb.emplace_back( infoAabb );
		//��]
		RotAABB( CQuaternion( CVector3::AxisY(), CMath::PI_HALF * m_muki ), aabb );
		//�ʒu
		aabb.min += GetModelPos();
		aabb.max += GetModelPos();
	}
}

IntVector3 Block::CalcBlockUnitPos()const {
	//���̃u���b�N�̍��W���߂�
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	return { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };
}

void Block::CalcAddLight(bool isDestroy) {
	//�v�Z�s�v
	if (m_bInfo->id == enCube_None && !isDestroy) {
		return;
	}

	//���C�g��(������)
	char lightPower = 0;
	char skyLightPower = 0;
	if (!isDestroy) {
		lightPower = m_bInfo->light;
	}

	//���[���h
	World* world = m_sWorld;
	//���̃u���b�N�̍��W���߂�
	IntVector3 blockpos = CalcBlockUnitPos();
	//���C�g�擾
	auto light = world->GetLightData(blockpos);	
	auto skyLight = world->GetSkyLightData(blockpos);
	if (!light || !skyLight) {
		return;
	}

	//�����u���b�N
	if (m_bInfo->isOpacity == false) {
		if (!isDestroy) {
			skyLightPower = *skyLight;
			lightPower = max(*light, lightPower);
		}
	}

	//�X�V�O�̃��C�g�́A�L�^
	char oldLightPower = *light;
	char oldSkyLightPower = *skyLight;

	//�X�V
	*light = lightPower;
	*skyLight = skyLightPower;

	//�X�J�C���C�g�v�Z
	if (m_bInfo->isOpacity || isDestroy) {//�s�����܂��͔j��
		SkyLight sky(world);
		sky.CalcSkyLightThisPosition(blockpos, !isDestroy);
	}

	//���̓`�d
	if (*light < oldLightPower) {//�Â��Ȃ���
		int count =	LightUtil::SpreadDark(world, oldLightPower, blockpos, { 0,0,0 }, false);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop:%d \n", count)
	}
	if (*skyLight < oldSkyLightPower) {
		int count = LightUtil::SpreadDark(world, oldSkyLightPower, blockpos, { 0,0,0 }, true);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop(sky):%d \n", count)
	}

	//�j�󎞁A���肩����`�d
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

	//����֌���`�d
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
			char blockPow = lightPower;
			char skyPow = skyLightPower;
			char* lightPtr = world->GetLightData(samplePos);
			if (lightPtr) {
				blockPow = max(blockPow, *lightPtr);
			}
			lightPtr = world->GetSkyLightData(samplePos);
			if (lightPtr) {
				skyPow = max(skyPow, *lightPtr);
			}

			if (sb < 4) {
				if (blockPow >= 0)block->SetLightingData(sb, 0, blockPow);
				if (skyPow >= 0)block->SetLightingData(sb, 2, skyPow);
			}
			else {
				if (blockPow >= 0)block->SetLightingData(sb - 4, 1, blockPow);
				if (skyPow >= 0)block->SetLightingData(sb - 4, 3, skyPow);
			}
		}
	}
}

void Block::SetPos( int x, int y, int z ){
	CVector3 pos{ x * WIDTH + half, y * WIDTH, z * WIDTH + half };

	//AABB
	if (m_aabb.empty() == false) {
		CVector3 move = pos - m_model.GetPos();
		for( AABB& aabb : m_aabb ){
			aabb.min += move;
			aabb.max += move;
		}
	}

	//���f��
	m_model.SetPos( pos );

	//�����蔻��
	if (m_collision) {
		for( int i = 0; i < m_aabb.size(); i++ ){
			CVector3 colpos = pos;
			colpos += m_aabb[i].min + ( m_aabb[i].max - m_aabb[i].min ) / 2.0f;
			m_collision[i].SetPosition( colpos );
		}
	}

	//���̌v�Z 
	//TODO ��������Ȃ�
	CalcAddLight();
}

void Block::EnableCollision(){
	if( m_collision == nullptr ){
		m_collision = std::make_unique<SuicideObj::CCollisionObj[]>( m_aabb.size() );
		for ( int i = 0; i < m_aabb.size(); i++ ) {
			AABB aabb = m_aabb[i];
			aabb.min -= m_model.GetPos();
			aabb.max -= m_model.GetPos();

			SuicideObj::CCollisionObj& collision = m_collision[i];
			
			collision.SetIsStaticObject(true);
			collision.CreateBox(CVector3::Zero(), CQuaternion::Identity(), aabb.max - aabb.min);
			collision.SetTimer(enNoTimer);
			collision.SetIsHurtCollision(true);	//�������画����Ƃ�Ȃ��B
			collision.SetName(L"Block");		//�R���W�����ɖ��O�B
			collision.SetPointer(this);			//�|�C���^��ݒ�B
			//m_collision[i.GetCollisionObject().setUserIndex(enBlock);
			//m_collision[i.GetCollisionObject().setUserPointer(this);		

			CVector3 pos = m_model.GetPos();
			pos += aabb.min + (aabb.max - aabb.min) / 2.0f;

			collision.SetPosition(pos);
		}
	}
}
