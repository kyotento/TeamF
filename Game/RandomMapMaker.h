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
private:

	float m_seedX, m_seedZ, m_seedY;
	const float m_width = 70;		//�}�b�v�̃T�C�Y
	const float m_depth = 70;
	float m_maxHeight = 15;			//�}�b�v�̍ő�̍���
	float m_relief = 20.f;			//�N��̌�����
	float m_mapSize = 1.f;			//�}�b�v�̑傫��
	float m_minHeight = 0;

	//std::list<CVector3> m_posList;
	//std::list<CVector3> m_posList2;
	//std::list<CVector3> typedef PosList;
	//std::vector<PosList> m_posVector;
	//CVector3 m_position;
	//CVector3 m_target;
	//GameObj::PerspectiveCamera* m_camera;                   //3D�J�����̃C���X�^���X
	//std::list<GameObj::CInstancingModelRender*> m_cubeModelList;

	const int m_stoneMaxHeight = 3;
	const int m_stoneMinHeight = 0;
	const int m_OreMaxHeight = 2;
	const int m_OreMinHeight = 0;
	float m_relief2 = 10.f;

	World* m_world = nullptr;
};

