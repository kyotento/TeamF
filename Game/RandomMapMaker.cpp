#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include <random>

void RandomMapMaker::Awake()
{


	std::random_device rand;
	//同じマップを生成しないようにシード生成
	m_seedX = rand() % 101;
	m_seedZ = rand() % 101;


	//キューブ生成
	for (int x = 0; x < m_width; x++) {
		for (int z = 0; z < m_depth; z++) {
			/*ModelRender* cube = new ModelRender;
			cube->Init("Assets/modelData/cube.tkm");
			cube->SetPosition(Vector3(x, 0, z));
			//cube->SetScale(Vector3::One * 0.1f);
			//Y座標を設定
			SetY(cube);
			m_cubeList.push_back(cube);*/
			CVector3 pos = CVector3(x, 0, z);
			pos.y = SetY(pos);
			pos *= 10.f;
			m_posList.push_back(pos);

			int y = int(pos.y);
			while (y > m_minHeight) {
				y--;
				pos.y = y;
				m_posList2.push_back(pos);
			}
		}
	}

	m_position = CVector3(00.0f, 200.0f, 100.0f);
	m_camera = NewGO<GameObj::PerspectiveCamera>();
	//m_camera = FindGO<GameObj::PerspectiveCamera>(L"Camera");
	//Vector3 pos = Vector3(-400.0f, 400.0f, 400.0f);
	m_target = CVector3(0.0f, 100.0f, 0.0f);
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
	SetMainCamera(m_camera);
	//m_cubeModel.Init(m_posList.size(),L"Resource/modelData/cube.cmo");

	for (auto pos : m_posList) {
		GameObj::CInstancingModelRender* cube = new GameObj::CInstancingModelRender();
		cube->Init(m_width * m_depth, L"Resource/modelData/GrassBlock.cmo");
		cube->SetScale(CVector3::One() * 0.1);
		cube->SetPos(pos);
		m_cubeModelList.push_back(cube);
	}

	for (auto pos : m_posList2)
	{
		GameObj::CInstancingModelRender* cube = new GameObj::CInstancingModelRender();
		cube->Init(m_posList2.size(), L"Resource/modelData/soilBlock.cmo");
		cube->SetPos(pos);
		cube->SetScale(CVector3::One() * 0.1);
		m_cubeModelList.push_back(cube);
	}
}

float RandomMapMaker::SetY(const CVector3& pos)
{
	float y = 0;

	float xSample = (pos.x + m_seedX) / m_relief;
	float zSample = (pos.z + m_seedZ) / m_relief;

	float noise = GetPerlin().PerlinNoise(xSample * 2, 0.0f, zSample * 2);
	noise += GetPerlin().PerlinNoise(xSample, 0.0f, zSample) * 3;
	noise /= 4;
	y = m_maxHeight * noise;
	y = std::round(y);

	return y;
}