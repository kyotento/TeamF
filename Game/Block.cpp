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

	//AABBを回転する
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
	DW_ERRORBOX(m_muki < 0 || m_muki >= enMuki::enNum, "Block::GetMukiDir()\n異常な向きが指定されています")
	return mukiDir[m_muki];
}
const IntVector3& Block::GetMukiDir(enMuki muki) {
	DW_ERRORBOX(muki < 0 || muki >= enMuki::enNum, "Block::GetMukiDir()\n異常な向きが指定されています")
	return mukiDir[muki];
}

void Block::Init( const BlockInfo * bInfo, enMuki muki ){
	m_bInfo = bInfo;
	m_hp = bInfo->hp;

	//instanceMaxはすでにモデルがロードされている場合は使われないので値が何でも関係ない。
	m_model.Init( 0, bInfo->modelPath.c_str() );
	m_model.SetParamPtr( &m_lighting );//ライティング情報の設定

	//向き
	DW_ERRORBOX(muki < 0 || muki >= enMuki::enNum, "Block::Init()\n異常な向きが指定されています")
	m_muki = muki;
	m_model.SetRot( CQuaternion( CVector3::AxisY(), CMath::PI_HALF * m_muki ) );

	CalcAABB();

	//レイトレモデル
	m_raytraceModel.Init( m_model );

	//デフォの明るさ
	for( int _1 = 0; _1 < 4; _1++ ){
		for( int _2 = 0; _2 < 4; _2++ ){
			SetLightingData( _1, _2, 0 );
		}
	}
}

void Block::CalcAABB() {
	m_aabb.clear();

	for(const AABB& infoAabb : m_bInfo->aabbArray ){
		//ブロックAABB
		AABB& aabb = m_aabb.emplace_back( infoAabb );
		//回転
		RotAABB( CQuaternion( CVector3::AxisY(), CMath::PI_HALF * m_muki ), aabb );
		//位置
		aabb.min += GetModelPos();
		aabb.max += GetModelPos();
	}
}

IntVector3 Block::CalcBlockUnitPos()const {
	//このブロックの座標求める
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	return { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };
}

void Block::CalcAddLight(bool isDestroy) {
	//計算不要
	if (m_bInfo->id == enCube_None && !isDestroy) {
		return;
	}

	//ライト力(ぢから)
	char lightPower = 0;
	char skyLightPower = 0;
	if (!isDestroy) {
		lightPower = m_bInfo->light;
	}

	//ワールド
	World* world = m_sWorld;
	//このブロックの座標求める
	IntVector3 blockpos = CalcBlockUnitPos();
	//ライト取得
	auto light = world->GetLightData(blockpos);	
	auto skyLight = world->GetSkyLightData(blockpos);
	if (!light || !skyLight) {
		return;
	}

	//透明ブロック
	if (m_bInfo->isOpacity == false) {
		if (!isDestroy) {
			skyLightPower = *skyLight;
			lightPower = max(*light, lightPower);
		}
	}

	//更新前のライト力、記録
	char oldLightPower = *light;
	char oldSkyLightPower = *skyLight;

	//更新
	*light = lightPower;
	*skyLight = skyLightPower;

	//スカイライト計算
	if (m_bInfo->isOpacity || isDestroy) {//不透明または破壊時
		SkyLight sky(world);
		sky.CalcSkyLightThisPosition(blockpos, !isDestroy);
	}

	//負の伝播
	if (*light < oldLightPower) {//暗くなった
		int count =	LightUtil::SpreadDark(world, oldLightPower, blockpos, { 0,0,0 }, false);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop:%d \n", count)
	}
	if (*skyLight < oldSkyLightPower) {
		int count = LightUtil::SpreadDark(world, oldSkyLightPower, blockpos, { 0,0,0 }, true);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop(sky):%d \n", count)
	}

	//破壊時、周りから光伝播
	if (isDestroy) {
		//TODO いっかいでいい?
		for (int sb = 0; sb < 6; sb++) {
			//周りから光を伝搬させる
			auto pos = blockpos + LightUtil::spreadDir[sb];
			//ブロック
			char* samplight = world->GetLightData(pos);
			if (samplight) {
				LightUtil::SpreadLight(world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, false);
			}
			//スカイ
			samplight = world->GetSkyLightData(pos);
			if (samplight) {
				LightUtil::SpreadLight(world, *samplight - 1, pos, LightUtil::spreadDir[sb] * -1, true);
			}
		}
		lightPower = *light;
		skyLightPower = *skyLight;
	}

	//更新あるか?
	bool reflesh = oldLightPower != lightPower;
	bool refleshSky = oldSkyLightPower != skyLightPower;

	//自分と周囲のライティング描画更新
	RefleshDrawLighting(world, blockpos, lightPower, skyLightPower);

	//周りへ光を伝播
	if (reflesh) {
		//弱すぎる光は伝搬しない
		if (lightPower <= 1) {
			return;
		}
		//光を伝搬させる
		LightUtil::SpreadLight(world, lightPower - 1, blockpos, { 0,0,0 }, false);
	}
	if (refleshSky) {
		//弱すぎる光は伝搬しない
		if (skyLightPower <= 1) {
			return;
		}
		//光を伝搬させる
		LightUtil::SpreadLight(world, skyLightPower - 1, blockpos, { 0,0,0 }, true);
	}
}

void Block::RefleshDrawLighting(World* world, const IntVector3& blockpos, char lightPower, char skyLightPower) {
	//自分のライティング
	for (int sb = 0; sb < 6; sb++) {
		auto pos = blockpos + LightUtil::spreadDir[sb] * -1;
		//ブロックライト
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
		//スカイライト
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
	//上下左右前後のブロックのライティング
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

	//モデル
	m_model.SetPos( pos );

	//当たり判定
	if (m_collision) {
		for( int i = 0; i < m_aabb.size(); i++ ){
			CVector3 colpos = pos;
			colpos += m_aabb[i].min + ( m_aabb[i].max - m_aabb[i].min ) / 2.0f;
			m_collision[i].SetPosition( colpos );
		}
	}

	//光の計算 
	//TODO ここじゃない
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
			collision.SetIsHurtCollision(true);	//自分から判定をとらない。
			collision.SetName(L"Block");		//コリジョンに名前。
			collision.SetPointer(this);			//ポインタを設定。
			//m_collision[i.GetCollisionObject().setUserIndex(enBlock);
			//m_collision[i.GetCollisionObject().setUserPointer(this);		

			CVector3 pos = m_model.GetPos();
			pos += aabb.min + (aabb.max - aabb.min) / 2.0f;

			collision.SetPosition(pos);
		}
	}
}
