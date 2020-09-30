#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include <random>
#include "Block.h"
#include "World.h"

void RandomMapMaker::Awake()
{
	m_filePathList.push_back(L"Resource/modelData/GrassBlock.cmo");
	m_filePathList.push_back(L"Resource/modelData/soilBlock.cmo");
	m_filePathList.push_back(L"Resource/modelData/stoneBlock.cmo");

	std::random_device rand;
	//同じマップを生成しないようにシード生成
	m_seedX = rand() % 101;
	m_seedZ = rand() % 101;
	m_seedY = rand() % 101;

	/*m_cubeList.resize(m_width);
	//m_cubeList[0].push_back();
	for (int i = 0; i < m_width; i++) {
		m_cubeList[i].resize(m_maxHeight + 1);
		for (int j = 0; j < m_maxHeight + 1; j++) {
			m_cubeList[i][j].resize(m_depth);
		}
	}*/
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

			int xx = int(pos.x) / 10;
			int yy = int(pos.y) / 10;
			int zz = int(pos.z) / 10;

			{
				auto block = std::make_unique<Block>();
				block->GetModel().Init( m_width * m_depth * ( m_maxHeight + 1 ), m_filePathList[enCube_Grass] );
				block->GetModel().SetScale( CVector3::One() * 0.0075f );
				m_world->SetBlock( xx, yy, zz, std::move( block ) );
			}

			/*m_cubeList[xx][yy][zz].s_position = pos;
			m_cubeList[xx][yy][zz].s_model = new GameObj::CInstancingModelRender();
			m_cubeList[xx][yy][zz].s_state = enCube_Grass;
			m_cubeList[xx][yy][zz].s_model->Init(m_width * m_depth * (m_maxHeight + 1), m_filePathList[m_cubeList[xx][yy][zz].s_state]);
			m_cubeList[xx][yy][zz].s_model->SetPos(pos);
			m_cubeList[xx][yy][zz].s_model->SetScale(CVector3::One() * 0.07f);*/
		

			m_posList.push_back(pos);

			while (yy > m_minHeight) {
				yy--;
				pos.y = float(yy) * 10.f;

				{
					auto block = std::make_unique<Block>();
					block->GetModel().SetScale( CVector3::One() * 0.0075f );
					Soil(xx, yy, zz, block.get());
					Stone(xx, yy, zz, block.get());
					Ore(xx, yy, zz, block.get());
					block->GetModel().Init( m_width * m_depth * ( m_maxHeight + 1 ), m_filePathList[block->GetBlockType()] );
					m_world->SetBlock( xx, yy, zz, std::move( block ) );

				}

				/*m_cubeList[xx][yy][zz].s_position = pos;
				m_cubeList[xx][yy][zz].s_model = new GameObj::CInstancingModelRender();
				Soil(xx, yy, zz);
				Stone(xx, yy, zz);
				Ore(xx, yy, zz);
				m_cubeList[xx][yy][zz].s_model->Init(m_width * m_depth * (m_maxHeight + 1), m_filePathList[m_cubeList[xx][yy][zz].s_state]);
				m_cubeList[xx][yy][zz].s_model->SetPos(pos);
				m_cubeList[xx][yy][zz].s_model->SetScale(CVector3::One() * 0.07f);*/
				//m_posList2.push_back(pos);
			}
		}
	}

	/*m_position = CVector3(00.0f, 200.0f, 100.0f);
	m_camera = NewGO<GameObj::PerspectiveCamera>();*/
	//m_camera = FindGO<GameObj::PerspectiveCamera>(L"Camera");
	//Vector3 pos = Vector3(-400.0f, 400.0f, 400.0f);
	/*m_target = CVector3(0.0f, 100.0f, 0.0f);
	m_camera->SetPos(m_position);
	m_camera->SetTarget(m_target);
	SetMainCamera(m_camera);*/
	//m_cubeModel.Init(m_posList.size(),L"Resource/modelData/cube.cmo");

	/*for (auto pos : m_posList) {
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
	}*/
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

void RandomMapMaker::Soil(const int x, const int y, const int z, Block* b)
{
	b->SetBlockType(enCube_Soil);
}

void RandomMapMaker::Stone(const int x, const int y, const int z, Block* b)
{
	if (m_stoneMaxHeight >= y && y >= m_stoneMinHeight) {
		b->SetBlockType(enCube_Stone);
	}
}

void RandomMapMaker::Ore(const int x, const int y, const int z, Block* b)
{
	if (m_OreMaxHeight >= y && y >= m_OreMinHeight) {
		float noise = GetPerlin().PerlinNoise((float(x) + m_seedX) / m_relief2, (float(y) + m_seedY) / m_relief2, (float(z) + m_seedZ) / m_relief2);
		if (noise >= 0.7f) {
			b->SetBlockType(enCube_Soil);
		}
	}
}