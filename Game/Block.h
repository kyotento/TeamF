//! @file
#pragma once
#include "../BlockType.h"
#include "Light.h"
#include "BlockInfo.h"

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

	enum enMuki {
		enXm, enZm, enXp, enZp//-X,-Z...
	};

	//! @brief 向きをセット
	void SetMuki(enMuki muki) {
		m_muki = muki;
		m_model.SetRot(CQuaternion(CVector3::AxisY(), CMath::PI_HALF * m_muki));
		CalcAABB();
		DisableCollision();
		EnableCollision();
	}
	//! @brief 向きを取得
	enMuki GetMuki()const {
		return m_muki;
	}

	//! @brief このブロックのAABBを取得
	const AABB& GetAABB( int index )const{
	 	return m_aabb[index];
	}

	//! @brief このブロックのAABBの数を取得
	int GetAABBNum() const{
		return m_bInfo->aabbArray.size();
	}

	//! @brief ブロックの種類を取得。
	EnCube GetBlockType() const
	{
		return m_bInfo->id;
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
	bool GetIsOpacity()const{
		return m_bInfo->isOpacity;
	}

	//! @brief 初期化。
	void Init( const BlockInfo* bInfo );

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
		m_hp = m_bInfo->hp;
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

	//! @brief ワールドを設定
	static void SetWorldPtr(World* world) {
		m_sWorld = world;
	}

	//デストロイモード
	//デストラクタの挙動を変える
	static bool m_sDestroyMode;

	//! @brief ブロックの幅、奥行き、高さ。
	static constexpr float WIDTH = 140;
	//! @brief ブロックAABBの最大数
	static constexpr int BLOCK_AABB_MAXNUM = 2;

private:
	//! @brief ライティング計算する
	void CalcAddLight(bool isDestroy = false);

	//! @brief AABBを計算する
	void CalcAABB();

protected:
	//! @brief ブロック単位の座標を計算する
	IntVector3 CalcBlockUnitPos()const;

private:
	//モデル
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;//レイトレ用

	//! @brief ブロックの種類ごとに共通の情報。
	const BlockInfo* m_bInfo;

	//向き
	enMuki m_muki = enXm;

	//明るさ
	CMatrix m_lighting = CMatrix::Zero();

	//AABB
	std::vector<AABB> m_aabb;
	//コリジョン
	std::unique_ptr<SuicideObj::CCollisionObj[]> m_collision;

	int m_hp = 10;

protected:
	//ワールドポインタ
	static World* m_sWorld;
};

