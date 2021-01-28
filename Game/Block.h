//! @file
#pragma once
#include "../BlockType.h"
#include "Light.h"
#include "AABB.h"

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

	//! @brief モデルのポジションを取得
	const CVector3& GetModelPos()const {
		return m_model.GetPos();
	}

	//! @brief このブロックのAABBを取得
	const AABB& GetAABB(int index)const;
	//! @brief このブロックのAABBの数を取得
	int GetAABBNum() const;

	//! @brief モデルを初期化
	void InitModel(const wchar_t* filePath);

	//! @brief ブロックの種類を設定。
	//! @details この関数はモデルを変更しない。
	void SetBlockType(EnCube enCube)
	{
		if (m_state != enCube) {
			m_state = enCube;
			CalcAABB();//AABBの計算
		}
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

	//! @brief 不透明かどうか取得
	bool GetIsOpacity()const;

	//HPを設定
	void SetHP(const int hp)
	{
		m_maxHP = hp;
		m_hp = hp;
	}
	//HPを取得
	const int GetHP() const
	{
		return m_hp;
	}
	//HPを減らす
	void ReduceHP(const int attack)
	{
		m_hp -= attack;
	}
	//ブロックの耐久値を全快させる
	void RestoresBlockDurabilityValue()
	{
		m_hp = m_maxHP;
	}

	//! @brief ライティング状態の設定。
	void SetLightingData(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "???????x???????O???")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		if (column < 2) {
			m_lighting.m[row][column] = LightUtil::DRAWING_LIGHT[lightpower];//ブロックライト
		}
		else {
			m_lighting.m[row][column] = LightUtil::DRAWING_SKY_LIGHT[lightpower];//スカイライト
		}
	}
	//! @brief ライティングを行う。
	void Lighting(int row, int column, char lightpower) {
		DW_WARNING_BOX((lightpower < 0 || lightpower > LightUtil::LIGHT_POWER_MAX), "???????x???????O???")
		lightpower = min(max(lightpower, 0), LightUtil::LIGHT_POWER_MAX);
		//より明るい方を使用
		if (column < 2) {
			m_lighting.m[row][column] = max(m_lighting.m[row][column], LightUtil::DRAWING_LIGHT[lightpower]);//ブロックライト
		}
		else {
			m_lighting.m[row][column] = max(m_lighting.m[row][column], LightUtil::DRAWING_SKY_LIGHT[lightpower]);//スカイライト
		}
	}
	//! @brief ライティング描画の更新
	void RefleshDrawLighting(World* world, const IntVector3& blockPos, char lightPower, char skyLightPower);

	//! @brief ブロックの幅、奥行き、高さ。
	static constexpr float WIDTH = 140;

	//ブロックAABBの最大数
	static constexpr int BLOCK_AABB_MAXNUM = 2;

private:
	//! @brief ライティング計算する
	void CalcAddLight(bool isDestroy = false);

	//! @brief AABBを計算する
	void CalcAABB();

private:
	//モデル
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;//レイトレ用

	//! @brief ブロックの種類。
	EnCube m_state = enCube_None;

	//向き
	enum enMuki {
		enXm,enZm,enXp,enZp//-X,-Z...
	};
	enMuki m_muki = enXm;

	//明るさ
	CMatrix m_lighting = CMatrix::Zero();

	//AABB
	std::unique_ptr<AABB[]> m_aabb;
	//コリジョン
	std::unique_ptr<SuicideObj::CCollisionObj[]> m_collision;

	int m_maxHP = 0;
	int m_hp = 10;
};

