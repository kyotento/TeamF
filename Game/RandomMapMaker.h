#pragma once
class RandomMapMaker : public IGameObject
{
public:
	//初期化
	void Awake();
	/*void Update()
	{
		for (auto cube : m_cubeList)
		{
			cube->Update();
		}
	}
	void Draw()
	{
		for (auto cube : m_cubeList)
		{
			cube->Draw();
		}
	}*/
	void Update() override
	{
		/*m_cubeModel.BeginUpdateInstancingData();
		for (auto pos : m_posList)
		{
			m_cubeModel.UpdateInstancingData(pos);
		}*/
		CVector3 speed = CVector3::Zero();
		speed.x = Pad(0).GetStick(L).x;
		speed.z = Pad(0).GetStick(L).y;
		speed *= 5.0f;
		m_position += speed;
		m_target += speed;
		m_camera->SetPos(m_position);
		m_camera->SetTarget(m_target);
		//MainCamera().Update();
	} 
	//void Draw();
private:
	float SetY(const CVector3& pos);
private:
	struct Cube {
		CVector3 s_position;
		GameObj::CInstancingModelRender m_model;
	};
	float m_seedX, m_seedZ;
	const float m_width = 70;		//マップのサイズ
	const float m_depth = 70;
	float m_maxHeight = 20;			//マップの最大の高さ
	float m_relief = 20.f;			//起状の激しさ
	float m_mapSize = 1.f;			//マップの大きさ
	//GameObj::CSkinModelRender m_cubeModel;
	std::list<CVector3> m_posList;
	std::list<CVector3> m_posList2;
	std::list<CVector3> typedef PosList;
	std::vector<PosList> m_posVector;
	CVector3 m_position;
	CVector3 m_target;
	float m_minHeight = -5;
	GameObj::PerspectiveCamera* m_camera;                   //3Dカメラのインスタンス
	//std::list<CSkinModelRender*> m_cubeList;
	std::list<GameObj::CInstancingModelRender*> m_cubeModelList;

	std::list<Cube> typedef CubeList;
};

