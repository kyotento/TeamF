#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include "World.h"

#include "BlockFactory.h"

void RandomMapMaker::Init( World* world ){
	m_world = world;

	int loadEdge = m_world->GetChunkLoadRange() * 2 * Chunk::WIDTH;

	//�u���b�N�t�@�N�g����������
	BlockFactory::LoadInstancingModels(loadEdge * loadEdge * ( int(m_maxHeight) + 1 ) );

	std::random_device rand;
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h����
	m_seedX = rand() % 101;
	m_seedZ = rand() % 101;
	m_seedY = rand() % 101;

	m_seedX2 = rand() % 101 + 50;
	m_seedY2 = rand() % 101 + 50;
	m_seedZ2 = rand() % 101 + 50;
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

			//�n�\�̍���������B
			CVector3 pos = CVector3( float(wx), 0, float(wz) );
			pos.y = SetY( pos );
			int wy = int( pos.y );

			//��Ō��肵�����������Ƃɍō����x�̃u���b�N��ݒu�B
			chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock( enCube_Grass ) );
			//�؂𐶂₷�B
			Tree( wx, wy, wz );

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

					chunk.SetBlock( cx, wy, cz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}

	//�`�����N�𐶐��ς݂ɂ���B
	chunk.SetGenerated();
}

float RandomMapMaker::SetY( const CVector3& pos ){
	float y = 0;

	float xSample = ( pos.x + m_seedX ) / m_relief;
	float zSample = ( pos.z + m_seedZ ) / m_relief;

	float noise = GetPerlin().PerlinNoise( xSample * 2, 0.0f, zSample * 2 );
	noise += GetPerlin().PerlinNoise( xSample, 0.0f, zSample ) * 3;
	noise /= 4;
	y = m_maxHeight * noise;
	y = std::round( y );

	return y;
}



void RandomMapMaker::Tree(const int x, const int y, const int z)
{
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h����
	float xSample = (x + m_seedX2) / m_relief2;
	float ySample = (y + m_seedY2) / m_relief2;
	float zSample = (z + m_seedZ2) / m_relief2;

	float noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

	if (noise > 0.5f && noise < 0.507f) {
		
		int xx = x;
		int yy = y + 1;
		int zz = z;
		noise *= 100.f;
		int height = rand() * int(noise) % 3 + 3;
		for (int i = 0; i < height; i++) {
			m_world->SetBlock(xx, yy, zz, BlockFactory::CreateBlock(enCube_Soil));
			yy += 1;
		}



		int reafHeight = 5;
		int reafWidth = 3;
		int reafDepth = 3;
		yy -= rand() * int(noise * 100) % 2 + 1;

		float seed = 13;

		float a = 1.f;
	
		int xxx = xx % 10;
		int yyy = yy % 10;
		int zzz = zz % 10;


		xSample = (xxx + seed) / m_relief3 * a;
		ySample = (yyy + seed) / m_relief3;
		zSample = (zzz + seed) / m_relief3 * a;

		noise = GetPerlin().PerlinNoise(xSample, ySample, zSample);

		for (int i = 0; i < reafHeight; i++) {
			for (int j = -2; j < reafWidth; j++) {
				for (int p = -2; p < reafDepth; p++) {
					int rm = rand() * int(noise * 10000) % 2;
					int rm2 = rand() * int(noise * 10000) % 2;
					int b = 15;
					float xSample = (xxx + j + seed + rm2) / m_relief3 * a;
					float ySample = (yyy + i * b + seed) / m_relief3;
					float zSample = (zzz + p + seed + rm) / m_relief3 * a;
					float noise2 = GetPerlin().PerlinNoise(xSample, ySample, zSample);

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
					if (abs(noise - noise2) > a || abs(noise - noise3) > a || abs(noise - noise4) > a || abs(noise - noise5) > a)
						continue;
					m_world->SetBlock(xx + j, yy + i, zz + p, BlockFactory::CreateBlock(enCube_Leaf));
				}
			}
		}
	}
}