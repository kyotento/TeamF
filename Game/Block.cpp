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
	//ブロックAABB
	AABB aabb = BlockFactory::GetAABB(m_state);
	aabb.min += GetModelPos();
	aabb.max += GetModelPos();
	return aabb;
}

bool Block::GetIsOpacity()const {
	return BlockFactory::GetIsOpacity(m_state);
}

void Block::CalcAddLight(bool isDestroy) {
	//計算不要
	if (m_state == enCube_None && !isDestroy) {
		return;
	}

	//ライト力(ぢから)
	char lightPower = 0;
	char skyLightPower = 0;
	if (!isDestroy) {
		lightPower = BlockFactory::GetLight(m_state);
	}

	//ワールド
	World* world = FindGO<World>(L"World");

	//このブロックの座標求める
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	IntVector3 blockpos = { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };

	//このブロック自体の明るさを設定
	auto light = world->GetLightData(blockpos);
	if (!light) {
		return;
	}
	auto skyLight = world->GetSkyLightData(blockpos);

	//透明ブロック
	if (BlockFactory::GetIsOpacity(m_state) == false) {
		skyLightPower = *skyLight;
		if (!isDestroy && lightPower < *light) {
			lightPower = *light;
		}
	}

	bool isLight = *light < lightPower;//もとより明るい光源か?
	char oldLightPower = *light;//更新前のライト力
	char oldSkyLightPower = *skyLight;//更新前のスカイライト力

	//更新
	*light = lightPower;
	*skyLight = skyLightPower;

	//負の伝播
	if (!isLight) {//暗くなった
		int count =	LightUtil::SpreadDark(world, oldLightPower, blockpos, { 0,0,0 }, false);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop:%d \n", count)
	}
	if (*skyLight != oldSkyLightPower) {
		int count = LightUtil::SpreadDark(world, oldSkyLightPower, blockpos, { 0,0,0 }, true);
		DW_WARNING_MESSAGE(count >= 2, "SpreadDark_Loop(sky):%d \n", count)
	}

	//スカイライト計算
	SkyLight sky(world);
	sky.CalcSkyLightThisPosition(blockpos, !isDestroy);

	//破壊時
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
	//instanceMaxはすでにモデルがロードされている場合は使われないので値が何でも関係ない。
	m_model.Init(0, filePath);
	m_model.SetRot(CQuaternion(CVector3::AxisY(), ((CMath::RandomInt() % 4) * CMath::PI_HALF)));//モデルランダムで回転
	m_model.SetParamPtr(&m_lighting);//ライティング情報の設定

	//レイトレモデル
	m_raytraceModel.Init(m_model);

	//デフォの明るさ
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

	//光の計算 ここじゃない
	CalcAddLight();
}

void Block::EnableCollision(){
	if( !m_collision ){
		AABB aabb = BlockFactory::GetAABB(m_state);

		m_collision = std::make_unique<SuicideObj::CCollisionObj>();
		m_collision->SetIsStaticObject( true );
		m_collision->CreateBox(CVector3::Zero(), CQuaternion::Identity(), aabb.max - aabb.min);
		m_collision->SetTimer( enNoTimer );
		m_collision->SetIsHurtCollision(true);	//自分から判定をとらない。
		m_collision->SetName(L"Block");			//コリジョンに名前。
		m_collision->SetPointer(this);			//ポインタを設定。
		//m_collision->GetCollisionObject().setUserIndex(enBlock);
		//m_collision->GetCollisionObject().setUserPointer(this);		

		CVector3 pos = m_model.GetPos();
		pos.y += aabb.max.y * 0.5f;

		m_collision->SetPosition(pos);
	}
}
