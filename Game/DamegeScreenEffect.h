#pragma once

/// <summary>
/// �_���[�W�G�t�F�N�g
/// ��ʑS�̂���u�Ԃ����邾�낤��
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

