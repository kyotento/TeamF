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
		//CalcDestroyLight();//やらない場合(判定の方)
		CalcAddLight(true);
	}
}

void Block::CalcAddLight(bool isDestroy) {
	//計算不要
	if (m_state == enCube_None && !isDestroy) {
		return;
	}

	int lightPower = 0;//ライト力(ぢから)

	if (!isDestroy) {
		//とりあえず丸石を松明として扱う
		if (m_state == enCube_CobbleStone) {
			lightPower = 14;
		}
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
	auto light = world->GetLightData(blockpos);

	//DW_ERRORBOX(light == nullptr, "です")
	if (!light) {
		return;
	}

	bool isLight = *light < lightPower;
	char oldLightPower = *light;
	//更新
	*light = lightPower;

	if (!isLight) {//暗くなった
		//負の伝播
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
			//周りから光を伝搬させる
			auto pos = blockpos + LightUtil::spreadDir[sb];
			char* light = world->GetLightData(pos);
			LightUtil::SpreadLight(world, *light - 1, pos, LightUtil::spreadDir[sb]*-1, false);
			//TODO いっかいでいい?
		}
		lightPower = *light;
	}

	bool reflesh = oldLightPower != lightPower;

	//自分のライティング
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
	//上下左右前後のブロックのライティング
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

	//更新不要
	if (!reflesh) {
		return;
	}	

	//弱すぎる光は伝搬しない
	if (lightPower <= 1) {
		return;
	}
	//光を伝搬させる
	LightUtil::SpreadLight(world, lightPower - 1, blockpos, { 0,0,0 }, false);
	
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
