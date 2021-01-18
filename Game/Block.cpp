#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"

namespace {
	constexpr float half = Block::WIDTH * 0.5f;
	constexpr int LIGHT_POWER_MAX = 15;//明るさの最大値
	//表示上の明るさ
	constexpr float DRAWING_LIGHT[LIGHT_POWER_MAX] = { 
		0.04398046511104f, 0.0549755813888f, 0.068719476736f, 0.08589934592f, 0.1073741824f,
		0.134217728f, 0.16777216f, 0.2097152f, 0.262144f, 0.32768f,
		0.4096f, 0.512f, 0.64f, 0.8f, 1.0f
	};

	//光が伝搬する方向
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

	//このブロック自体の明るさを設定
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

	//更新不要
	if (!isReflesh) {
		return;
	}

	//上下左右前後のブロックを照らす
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

	//弱すぎる光は伝搬しない
	if (lightPower <= 1) {
		return;
	}
	
	//光を伝搬させる
	SpreadLight(world, lightPower -1, blockpos, { 0,0,0 });
}

void Block::SpreadLight(World* world, int lightPower, const IntVector3& pos, const IntVector3& fromDir) {
	//弱すぎる光は伝搬しない
	if (lightPower < 1) {
		return;
	}
	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVector3 nowPos = pos + spreadDir[i];

		//ライト情報記録
		auto light = world->GetLightData(nowPos.x, nowPos.y, nowPos.z);
		if (*light < lightPower) {
			*light = lightPower;
		}
		else {
			//この方向への伝播は終わり(もう明るいから)
			continue;
		}
		
		//上下左右前後のブロックを照らす
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

		//ブロックをサンプル
		Block* block = world->GetBlock(nowPos.x, nowPos.y, nowPos.z);
		if (block) {//TODO なおかつ不透明ブロック			

			//TODO リストに追加

			//この方向への伝播は終わり(ブロックと衝突したから)
		}
		else {
			//伝搬を続ける
			SpreadLight(world, lightPower - 1, nowPos, spreadDir[i]);
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
