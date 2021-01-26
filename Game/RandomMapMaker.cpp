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
#include "TreeGenerator.h"

RandomMapMaker::~RandomMapMaker()
{
	
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

	m_treeGenerator;
	m_treeGenerator.SetRandomMapMaker(this);
	m_treeGenerator.SetWorld(world);
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

			if (state == enBiome_Forest) {
				//��Ō��肵�����������Ƃɍō����x�̃u���b�N��ݒu�B
				chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_Grass));
				//�؂𐶂₷�B
				//m_treeGenerator.GenerateTree(wx, wy, wz);
			}
			else if (state == enBiome_Desert) {
				chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_OakLog));
			}
		
		
			//���肵���ō��n�_����Œፂ�x�܂Ńu���b�N�������߂Ă����B
			while( wy > m_minHeight ){

				wy--;

				{
					//�y
					EnCube bType = enCube_Soil;
					if (state == enBiome_Desert)
					{
						bType = enCube_OakLog;
					}
				
					//��ԉ��ɂ͊�Ղ�ݒu����
					if (wy == m_minHeight)
					{
						bType = enCube_Bedrock;
					}
					//�΃u���b�N�����Ȃ��
					else if (m_stoneMaxHeight >= wy && wy >= m_stoneMinHeight) {
						bType = enCube_Stone;
					}
					//�z�΃u���b�N�����Ńm�C�Y�������΍z��
					/*if( m_OreMaxHeight >= wy && wy >= m_OreMinHeight ){

						//�p�[�����m�C�Y
						float noise = GetPerlin().PerlinNoise( ( float( wx ) + m_seedX ) / m_relief2,
							( float( wy ) + m_seedY ) / m_relief2, ( float( wz ) + m_seedZ ) / m_relief2 );

						if( noise >= 0.7f ){
							bType = enCube_Soil;//�b��I�ɓy�ő�p
						}
					}*/

					//if (bType == enCube_Stone || bType == enCube_Soil)
						//continue;

					//chunk.SetBlock( cx, wy, cz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}
	
	//�z�ΐ���
	//CreateOre co;
	//co.SetMapMaker(this);
	//co.SetWorld(m_world);
	//co.Awake(chunk);

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
	y = m_maxHeight * noise + m_addHeight;
	y = std::round( y );

	return y;
}