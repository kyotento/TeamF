#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include "World.h"

#include "BlockFactory.h"
#include "Zombie.h"
#include "Enemy.h"
#include "Block.h"
#include "CreateOre.h"
#include "BiomeManager.h"

namespace {
	std::mt19937 random((std::random_device())());
}

void RandomMapMaker::Init( World* world, std::mt19937& seedGenerator ){
	m_world = world;

	int loadEdge = m_world->GetChunkLoadRange() * 2 * Chunk::WIDTH;

	//�u���b�N�t�@�N�g����������
	BlockFactory::LoadInstancingModels(loadEdge * loadEdge * ( int(m_maxHeight) + 1 ) );

	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h����
	m_seedX = seedGenerator() % 101;
	m_seedZ = seedGenerator() % 101;
	m_seedY = seedGenerator() % 101;

	m_seedX2 = seedGenerator() % 101 + 50;
	m_seedY2 = seedGenerator() % 101 + 50;
	m_seedZ2 = seedGenerator() % 101 + 50;
}

void RandomMapMaker::GenerateChunk( Chunk & chunk ){
	const int xStart = chunk.CalcWorldCoordX( 0 );
	const int xEnd = chunk.CalcWorldCoordX( Chunk::WIDTH );
	const int zStart = chunk.CalcWorldCoordZ( 0 );
	const int zEnd = chunk.CalcWorldCoordZ( Chunk::WIDTH );


	//wx,wz <- ���[���h���W�Bcx,cz <- �`�����N�����W�B
	//�`�����N����x,z���W�����[�v�B
	for( int wx = xStart, cx = 0; wx < xEnd; wx++, cx++ ){
		for( int wz = zStart, cz = 0; wz < zEnd; wz++, cz++ ){

			//�ǂ̃o�C�I�[���ɑ����邩����B
			enBiome state = GetBiomeManager().DecideBiome(wx, 0, wz);



			//�n�\�̍���������B
			CVector3 pos = CVector3( float(wx), 0, float(wz) );
			pos.y = SetY( pos );
			int wy = int( pos.y );

			//��Ō��肵�����������Ƃɍō����x�̃u���b�N��ݒu�B
			chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock( enCube_Grass ) );
			//�؂𐶂₷�B
			if (!Tree(wx, wy, wz)) {
				//�؂𐶂₹�Ȃ�������A�]���r�c�N�[���B
				CreateZombie(wx, wy, wz);
			}
		
			//���肵���ō��n�_����Œፂ�x�܂Ńu���b�N�������߂Ă����B
			while( wy > m_minHeight ){

				wy--;

				{
					//�y
					EnCube bType = enCube_Soil;
					//�΃u���b�N�����Ȃ��
					if( m_stoneMaxHeight >= wy && wy >= m_stoneMinHeight ){
						bType = enCube_Stone;
					}

					//�z�΃u���b�N�����Ńm�C�Y�������΍z��
					if( m_OreMaxHeight >= wy && wy >= m_OreMinHeight ){

						//�p�[�����m�C�Y
						float noise = GetPerlin().PerlinNoise( ( float( wx ) + m_seedX ) / m_relief2,
							( float( wy ) + m_seedY ) / m_relief2, ( float( wz ) + m_seedZ ) / m_relief2 );

						if( noise >= 0.7f ){
							bType = enCube_Soil;//�b��I�ɓy�ő�p
						}
					}

					//if (bType == enCube_Stone || bType == enCube_Soil)
						//continue;

					chunk.SetBlock( cx, wy, cz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}
	
	//�z�ΐ���
	CreateOre co;
	co.SetMapMaker(this);
	co.SetWorld(m_world);
	co.Awake(chunk);

	//�`�����N�𐶐��ς݂ɂ���B
	chunk.SetGenerated();
}

float RandomMapMaker::SetY( const CVector3& pos ){
	float y = 0;

	float xSample = (pos.x + m_seedX) / m_relief;
	float zSample = (pos.z + m_seedZ) / m_relief;

	//float xSample = ( pos.x + m_seedX ) / m_relief;
	//float zSample = ( pos.z + m_seedZ ) / m_relief;

	float noise = GetPerlin().PerlinNoise( xSample * 2, 0.0f, zSample * 2 );
	noise += GetPerlin().PerlinNoise( xSample, 0.0f, zSample ) * 3;
	noise /= 4;
	y = m_maxHeight * noise;
	y = std::round( y );

	return y;
}



bool RandomMapMaker::Tree(const int x, const int y, const int z)
{
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h�����B
	float xSample = (x + m_seedX2) / m_relief2;
	float ySample = (y + m_seedY2) / m_relief2;
	float zSample = (z + m_seedZ2) / m_relief2;

	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//�m�C�Y�����ȓ���������؂𐶐��B
	if (noise < 0.6f || noise > 0.607f) {
		return false;
	}

	int xx = x;
	int yy = y + 1;
	int zz = z;
	noise *= 100.f;
	int height = rand() * int(noise) % 3 + 3;
	//�؂̊��������Ă����B
	for (int i = 0; i < height; i++) {
		m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube_OakLog));
		yy += 1;
	}


	//�t�̃p�����[�^
	int reafHeight = 5;
	int reafWidth = 3;
	int reafDepth = 3;
	yy -= rand() * int(noise * 100) % 2 + 1;
	float seed = 13;
	float a = 1.f;

	//xyz�̒l��0�`13�̒l�ɂ���B
	int xxx = xx % 14;
	int yyy = yy % 14;
	int zzz = zz % 14;


	xSample = (xxx + seed) / m_relief3 * a;
	ySample = (yyy + seed) / m_relief3;
	zSample = (zzz + seed) / m_relief3 * a;

	//�؂̒��S�̃m�C�Y�����߂�
	noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	//�t�𐶐����Ă����B
	for (int i = 0; i < reafHeight; i++) {
		for (int j = -2; j < reafWidth; j++) {
			for (int p = -2; p < reafDepth; p++) {

				//�m�C�Y�𐶐����Ă����B
				//��̃m�C�Y�������ƕ΂�̂ŁB
				//�����̃m�C�Y�𐶐����Ă����B
				int rm = rand() * int(noise * 10000) % 2;
				int rm2 = rand() * int(noise * 10000) % 2;
				int b = 15;
				float xSample = (xxx + j + seed + rm2) / m_relief3 * a;
				float ySample = (yyy + i * b + seed) / m_relief3;
				float zSample = (zzz + p + seed + rm) / m_relief3 * a;
				float noise2 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				//
				xSample = (xxx + j + seed + rm2) / m_relief3 * a;
				ySample = (yyy + i * b + seed) / m_relief3;
				zSample = (zzz - p + seed - rm) / m_relief3 * a;
				float noise3 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				xSample = (xxx - j + seed - rm2) / m_relief3 * a;
				ySample = (yyy + i * b + seed) / m_relief3;
				zSample = (zzz + p + seed + rm) / m_relief3 * a;
				float noise4 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				xSample = (xxx - j + seed - rm2) / m_relief3 * a;
				ySample = (yyy + i * b + seed) / m_relief3;
				zSample = (zzz - p + seed - rm) / m_relief3 * a;
				float noise5 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

				float a = 0.0015f;
				//�؂̒��S�̃m�C�Y�Ƃ̍������ȓ��Ȃ�t�𐶐�����B
				if (abs(noise - noise2) > a || abs(noise - noise3) > a || abs(noise - noise4) > a || abs(noise - noise5) > a)
					continue;
				m_world->SetBlock(xx + j, yy + i, zz + p, BlockFactory::CreateBlock(enCube_OakLeaf));
			}
		}
	}
	return true;
}

void RandomMapMaker::CreateZombie(const int x, const int y, const int z)
{
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h����
	float xSample = (x + m_seedX2) / m_relief2;
	float ySample = (y + m_seedY2) / m_relief2;
	float zSample = (z + m_seedZ2) / m_relief2;

	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	float minNoise = 0.701;
	float maxNoise = 0.7017;
	//�m�C�Y�����ȓ��������]���r�����B
	if (noise < minNoise || noise > maxNoise) {
		return;
	}

	Enemy* zombie = NewGO<Zombie>();
	zombie->SetPos(CVector3(x * Block::WIDTH, (float(y) + 1.0f) * Block::WIDTH, z * Block::WIDTH));
	zombie->SetScale(CVector3::One() * 1.f);
	m_world->AddEntity(zombie);
}