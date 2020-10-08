#pragma once



class Block;
class World;

class RandomMapMaker : public IGameObject
{
public:
	//������
	void Awake();

	void SetWorld( World* world ){
		m_world = world;
	}

private:
	float SetY(const CVector3& pos);
	void Tree(const int x, const int y, const int z);
private:

	float m_seedX, m_seedZ, m_seedY;
	const float m_width = 20;		//�}�b�v�̃T�C�Y
	const float m_depth = 20;
	float m_maxHeight = 16;			//�}�b�v�̍ő�̍���
	float m_relief = 20.f;			//�N��̌�����
	float m_mapSize = 1.f;			//�}�b�v�̑傫��
	float m_minHeight = 0;

	const int m_stoneMaxHeight = 3;
	const int m_stoneMinHeight = 0;
	const int m_OreMaxHeight = 2;
	const int m_OreMinHeight = 0;

	World* m_world = nullptr;
	float m_seedX2, m_seedZ2, m_seedY2;
	float m_relief2 = 1.5f;			//�N��̌�����
	float m_relief3 = 1000.0f;
};

