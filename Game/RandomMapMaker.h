#pragma once

class Block;
class World;

class RandomMapMaker
{
public:
	//初期化
	void Init( World* world );

private:
	float SetY(const CVector3& pos);
	void Tree(const int x, const int y, const int z);
private:

	float m_seedX, m_seedZ, m_seedY;
	const float m_width = 32;		//マップのサイズ
	const float m_depth = 32;
	float m_maxHeight = 16;			//マップの最大の高さ
	float m_relief = 20.f;			//起状の激しさ
	float m_mapSize = 1.f;			//マップの大きさ
	float m_minHeight = 0;

	//std::list<CVector3> m_posList;
	//std::list<CVector3> m_posList2;
	//std::list<CVector3> typedef PosList;
	//std::vector<PosList> m_posVector;
	//CVector3 m_position;
	//CVector3 m_target;
	//GameObj::PerspectiveCamera* m_camera;                   //3Dカメラのインスタンス
	//std::list<GameObj::CInstancingModelRender*> m_cubeModelList;

	const int m_stoneMaxHeight = 3;
	const int m_stoneMinHeight = 0;
	const int m_OreMaxHeight = 2;
	const int m_OreMinHeight = 0;

	World* m_world = nullptr;
	float m_seedX2, m_seedZ2, m_seedY2;
	float m_relief2 = 1.5f;			//起状の激しさ
	float m_relief3 = 1000.0f;
};

