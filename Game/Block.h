//! @file
#pragma once
#include "../BlockType.h"

class Player;

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

	/// <summary>
	/// ワールド座標を使ってポジションをセット。
	/// </summary>
	/// <param name="worldpos">ワールド座標</param>
	void SetPosWithWorldPos(const CVector3& worldpos);

	//! @brief モデルのポジションを取得
	const CVector3& GetModelPos()const {
		return m_model.GetPos();
	}

	//! @brief モデルを初期化
	void InitModel(const wchar_t* filePath) {
		//instanceMaxはすでにモデルがロードされている場合は使われないので値が何でも関係ない。
		m_model.Init(0, filePath);
		m_model.SetRot(CQuaternion(CVector3::AxisY(),((CMath::RandomInt() % 4) * CMath::PI_HALF)));//モデルランダムで回転
		m_raytraceModel.Init(m_model);
	}

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

	bool IsCollisionEnabled() const{
		return m_collision.operator bool();
	}

	//! @brief ブロックを右クリックしたときの動作。
	//! @return true 何かを実行した。(ブロックの設置をしない)
	//! @return false 何も実行しなかった。(ブロックの設置をする)
	virtual bool OnClick( Player* player ){
		return false;
	}

	void EnableCollision();

	void DisableCollision(){
		m_collision.reset();
	}

	//! @brief ブロックの幅、奥行き、高さ。
	static constexpr float WIDTH = 140;
private:
	GameObj::CInstancingModelRender m_model;
	CRayTracingModelRender m_raytraceModel;

	//! @brief ブロックの種類。
	EnCube m_state = enCube_None;

	std::unique_ptr<SuicideObj::CCollisionObj> m_collision;
};

