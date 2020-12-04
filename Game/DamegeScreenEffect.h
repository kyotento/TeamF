#pragma once

/// <summary>
/// ダメージエフェクト
/// 画面全体を一瞬赤くするだろうか
/// </summary>
class DamegeScreenEffect : public IGameObject
{
public:
	bool Start()override;
	void Update()override;

private:
	float m_cnt = 0.0f;
	GameObj::CSpriteRender m_sprite;
};

