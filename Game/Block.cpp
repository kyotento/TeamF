#include "stdafx.h"
#include "Block.h"
#include "CollisionIndex.h"
#include "World.h"

namespace {
	constexpr float half = Block::WIDTH * 0.5f;
	constexpr float LIGHT_POWER_MAX = 15.0f;//明るさの最大値

	//光が伝搬する方向
	constexpr IntVec3 spreadDir[6] = {
		{1,0,0},
		{-1,0,0},
		{0,1,0},
		{0,-1,0},
		{0,0,1},
		{0,0,-1},
	};

	int count = 0;
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

	//このブロック自体の明るさを設定
	for (int _1 = 0; _1 < 4; _1++) {
		for (int _2 = 0; _2 < 4; _2++) {
			m_blockLighting[_1 + min(2, _2)] = max(m_blockLighting[_1 + min(2, _2)], lightPower);
			m_lighting.m[_1][_2] = max(m_lighting.m[_1][_2], lightPower / LIGHT_POWER_MAX);
		}
	}

	//弱すぎる光は伝搬しない
	if (lightPower <= 1) {
		return;
	}

	//このブロックの座標求める
	CVector3 pos = m_model.GetPos();
	pos.x -= half;
	pos.z -= half;
	pos /= WIDTH;
	IntVec3 blockpos;
	blockpos.x = (int)std::round(pos.x), blockpos.y = (int)std::round(pos.y), blockpos.z = (int)std::round(pos.z);

	//光を伝搬させる
	World* world = FindGO<World>(L"World");
	count = 0;
	SpreadLight(world, lightPower -1, blockpos, { 0,0,0 });
}

void Block::SpreadLight(World* world, int lightPower, IntVec3 pos, IntVec3 fromDir) {
	//弱すぎる光は伝搬しない
	if (lightPower < 1) {
		return;
	}
	count ++;
	for (int i = 0; i < 6; i++) {//6方向に伝搬(前後上下左右)
		//来た方向には戻らない
		if (fromDir.x * -1 == spreadDir[i].x && fromDir.y * -1 == spreadDir[i].y && fromDir.z * -1 == spreadDir[i].z) {
			continue;
		}

		//サンプル位置算出
		IntVec3 nowPos = pos;
		nowPos.x += spreadDir[i].x;
		nowPos.y += spreadDir[i].y;
		nowPos.z += spreadDir[i].z;

		//ブロックをサンプル
		Block* block = world->GetBlock(nowPos.x, nowPos.y, nowPos.z);

		if (block) {//TODO なおかつ不透明ブロック
			//ブロックにヒットしたらヒットしてる面を照らして次のループへ
			block->m_blockLighting[i] = max(block->m_blockLighting[i], lightPower);
			if (i < 4) {
				block->m_lighting.m[i][0] = max(block->m_lighting.m[i][0], lightPower / LIGHT_POWER_MAX);
			}
			else {
				block->m_lighting.m[i - 4][1] = max(block->m_lighting.m[i - 4][1], lightPower / LIGHT_POWER_MAX);
			}

			//TODO リストに追加

			//この方向への伝播は終わり
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

	for (int _1 = 0; _1 < 4; _1++) {
		for (int _2 = 0; _2 < 4; _2++) {
			m_lighting.m[_1][_2] = max(m_lighting.m[_1][_2], 0.05f);
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
