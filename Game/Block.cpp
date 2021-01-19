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
	m_raytraceModel.Release();
}

void Block::CalcAddLight() {
	//計算不要
	if (m_state == enCube_None) {
		return;
	}

	int lightPower = 0;//ライト力(ぢから)

	//とりあえず丸石を松明として扱う
	if (m_state == enCube_CobbleStone) {
		lightPower = 14;
	}

	//ワールド
	World* world = FindGO<World>(L"World");

	//このブロックの座標求める
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	IntVector3 blockpos = { (int)std::round(pos.x),(int)std::round(pos.y), (int)std::round(pos.z) };

	DW_ERRORBOX((lightPower < 0 || lightPower > LightUtil::LIGHT_POWER_MAX), "明るさレベルが範囲外です")

	//このブロック自体の明るさを設定
	auto light = world->GetLightData(blockpos.x, blockpos.y, blockpos.z);

	//DW_ERRORBOX(light == nullptr, "です")
	if (!light) {
		return;
	}

	bool isReflesh = false;
	if (*light < lightPower) {
		isReflesh = true;
		*light = lightPower;		
	}
	for (int _1 = 0; _1 < 4; _1++) {
		for (int _2 = 0; _2 < 4; _2++) {
			Lighting(_1, _2, lightPower);
		}
	}

	//更新不要
	if (!isReflesh) {
		return;
	}

	//上下左右前後のブロックを照らす
	for (int sb = 0; sb < 6; sb++) {
		IntVector3 samplePos = blockpos + LightUtil::spreadDir[sb];
		Block* block = world->GetBlock(samplePos.x, samplePos.y, samplePos.z);
		if (block) {
			if (sb < 4) {
				block->Lighting(sb, 0, lightPower);
			}
			else {
				block->Lighting(sb - 4, 1, lightPower);
			}
		}
	}

	//弱すぎる光は伝搬しない
	if (lightPower <= 1) {
		return;
	}
	
	//光を伝搬させる
	LightUtil::SpreadLight(world, lightPower -1, blockpos, { 0,0,0 }, false);
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
		m_collision->SetIsHurtCollision(true);	//自分から判定をとらない。
		m_collision->SetName(L"Block");			//コリジョンに名前。
		m_collision->SetPointer(this);			//ポインタを設定。
		//m_collision->GetCollisionObject().setUserIndex(enBlock);
		//m_collision->GetCollisionObject().setUserPointer(this);		

		CVector3 pos = m_model.GetPos();
		pos.y += WIDTH * 0.5f;

		m_collision->SetPosition(pos);
	}
}
