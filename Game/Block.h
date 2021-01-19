//! @file
#pragma once
#include "../BlockType.h"
#include "Light.h"

class Player;
class World;

//! @brief ブロックの情報とモデルレンダーを持つクラス。
//! @details 生成には BlockFactory を使用してほしい。
//! @details これの派生クラスを作ったら BlockFactory::CreateBlock 関数に追加する。
//! @author Takayama
class Block{
public:
	Block();
	virtual ~Block();

	//! @brief ポジションをセット。
	//! @details Worldは1ブロック1単位で座標を運用しているため、モデルにはブロックの幅を乗算した値を設定している。
	void SetPos( int x, int y, int z );
		
	//! @brief ワールド座標を使ってポジションをセット。
	//! ※いずれなくなる関数かも
	void SetPosWithWorldPos(const CVector3& worldpos);

	//! @brief モデルのポジションを取得
	const CVector3& GetModelPos()const {
		return m_model.GetPos();
	}

	//! @brief モデルを初期化
	void InitModel(const wchar_t* filePath);

	//! @brief ブロックの種類を設定。
	//! @details この関数はモデルを変更しない。
	void SetBlockType(EnCube enCube)
	{
		m_state = enCube;
	}
	//! @brief ブロックの種類を取得。
	EnCube GetBlockType() const
	{
		return m_state;
	}

	//! @brief モデルの描画をするかどうかを設定。
	//! @details 埋まっていて見えないブロックの描画を止めるために使用される。
	void SetIsDraw(bool isDraw){
		m_model.SetIsDraw( isDraw );
	}

	//! @brief 当たり判定が有効か取得。
	bool IsCollisionEnabled() const{
		return m_collision.operator bool();
	}

	//! @brief ブロックを右クリックしたときの動作。
	//! @return true 何かを実行した。(ブロックの設置をしない)
	//! @return false 何も実行しなかった。(ブロックの設置をする)
	virtual bool OnClick( Player* player ){
		return false;
	}

	//! @brief 当たり判定の有効化。
	void EnableCollision();

	//! @brief 当たり判定の無効化。
	void DisableCollision(){
		m_collision.reset();
	}

	//! @brief ライティング状態の設定。
	void SetLightingData(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "明るさレベルが範囲外です")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		m_lighting.m[row][column] = LightUtil::DRAWING_LIGHT[lightpower];
	}
	//! @brief ライティングを行う。
	void Lighting(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "明るさレベルが範囲外です")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		//より明るい方を使用
		m_lighting.m[row][column] = max(m_lighting.m[row][column], LightUtil::DRAWING_LIGHT[lightpower]);
	}

	//! @brief ブロックの幅、奥行き、高さ。
	static constexpr float WIDTH = 140;

private:
	//! @brief ライティング計算する
	void CalcAddLight();

private:
	//モデル
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;//レイトレ用

	//! @brief ブロックの種類。
	EnCube m_state = enCube_None;

	//明るさ
	CMatrix m_lighting = CMatrix::Zero();
	//int m_blockLighting[6] = {};
	//int m_skyLighting[6] = {};
	//このブロックが影響範囲内にある光源のリスト
	//std::list<Block*> m_lightSourceList;
	//このブロックが光源として影響を与えているブロックのリスト
	//std::list<Block*> m_lightingBlockList;

	//コリジョン
	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;
};

