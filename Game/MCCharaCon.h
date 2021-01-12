#pragma once

class World;

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

private:
	//描画AABBの更新
	void UpdateAABBRender() {
		if (m_aabbReender) {
			CVector3 min = m_position - m_colSize, max = m_position + m_colSize;
			min.y = m_position.y;
			max.y = m_position.y + m_colSize.y;

			m_aabbReender->Init(min, max, {1.0f,0.0f,1.0f,1.0f});
		}
	}

private:
	bool				m_isInited = false;				//初期化済み？

	CVector3 			m_position = CVector3::Zero();	//座標。

	bool 				m_isJump = false;				//ジャンプ中？
	bool				m_isOnGround = true;			//地面の上にいる？
	bool				m_isContactWall = false;		//壁に接触してる？

	CVector3			m_colSize;						//コリジョンの大きさ
	float				m_offset = 0.05f;				//ブロックから余分に押し出す距離

	std::unique_ptr<GameObj::CAABBRender> m_aabbReender;//AABBを描画するやつ

	World* m_world = nullptr;							//ワールドのポインタ
};
