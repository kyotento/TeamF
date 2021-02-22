#pragma once

#include "AABB.h"

class World;
class Block;

/// <summary>
/// マインクラフト用キャラクターコントローラー
/// </summary>
class MCCharaCon {
public:
	//初期化
	void Init(float width, float height, const CVector3& position);
	//実行
	const CVector3& Execute(CVector3& moveSpeed, float deltaTime = 1.0f / GetStandardFrameRate());

	//このキャラコンの当たり判定を描画するか
	void SetIsDrawCollider(bool b) {
		if (b && GetIsDebugDraw()) {
			m_aabbReender = std::make_unique<GameObj::CAABBRender>();
			UpdateAABBRender();
		}
		else {
			m_aabbReender.reset();
		}
	}

	//シフト移動中か設定
	void SetIsShiftMove(bool isShifting) {
		m_isShiftMove = isShifting;
	}
	bool IsShiftMove()const {
		return m_isShiftMove;
	}

	//座標取得
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	//座標設定
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}

	//ブロックから余分に押し出す距離を設定
	void SetOffset(float offset) {
		m_offset = offset;
	}
	
	//ジャンプ中か
	bool IsJump() const
	{
		return m_isJump;
	}
	//地面にいるか
	bool IsOnGround() const
	{		
		return m_isOnGround;
	}
	//壁に接触しているか？
	bool IsContactWall() const
	{
		return m_isContactWall;
	}

	const AABB& GetAABB()const {
		return m_aabb;
	}

private:
	//描画AABBの更新
	void UpdateAABBRender() {
		m_aabb.min = m_position - m_colSize, m_aabb.max = m_position + m_colSize;
		m_aabb.min.y = m_position.y;
		m_aabb.max.y = m_position.y + m_colSize.y;

		if (m_aabbReender) {
			m_aabbReender->Init(m_aabb.min, m_aabb.max, {1.0f,0.0f,1.0f,1.0f});
		}
	}

	//一定範囲付近のブロックの取得
	bool GetBlocks(const CVector3& aabbmin, const CVector3& aabbmax, std::vector<Block*>& return_blocks);

private:
	bool				m_isInited = false;				//初期化済み？

	CVector3 			m_position = CVector3::Zero();	//座標。

	bool 				m_isJump = false;				//ジャンプ中？
	bool				m_isOnGround = true;			//地面の上にいる？
	bool				m_isContactWall = false;		//壁に接触してる？

	CVector3			m_colSize;						//コリジョンの大きさ
	float				m_offset = 0.05f;				//ブロックから余分に押し出す距離

	bool				m_isShiftMove = false;			//シフト移動中?

	std::unique_ptr<GameObj::CAABBRender> m_aabbReender;//AABBを描画するやつ
	//GameObj::CAABBRender m_aabbReender2[16];

	AABB				m_aabb;

	World* m_world = nullptr;							//ワールドのポインタ
};
