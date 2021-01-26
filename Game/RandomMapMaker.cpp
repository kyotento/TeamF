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

	//ブロックファクトリを初期化
	BlockFactory::LoadInstancingModels(loadEdge * loadEdge * ( int(m_maxHeight) + 1 ) );

	//同じマップを生成しないようにシード生成
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


	//wx,wz <- ワールド座標。cx,cz <- チャンク内座標。
	//チャンク内のx,z座標をループ。
	for( int wx = xStart, cx = 0; wx < xEnd; wx++, cx++ ){
		for( int wz = zStart, cz = 0; wz < zEnd; wz++, cz++ ){

			//どのバイオームに属するか決定。
			enBiome state = GetBiomeManager().DecideBiome(wx, 0, wz);



			//地表の高さを決定。
			CVector3 pos = CVector3( float(wx), 0, float(wz) );
			pos.y = SetY( pos );
			int wy = int( pos.y );

			if (state == enBiome_Forest) {
				//上で決定した高さをもとに最高高度のブロックを設置。
				chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_Grass));
				//木を生やす。
				//m_treeGenerator.GenerateTree(wx, wy, wz);
			}
			else if (state == enBiome_Desert) {
				chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_OakLog));
			}
		
		
			//決定した最高地点から最低高度までブロックをしきつめていく。
			while( wy > m_minHeight ){

				wy--;

				{
					//土
					EnCube bType = enCube_Soil;
					if (state == enBiome_Desert)
					{
						bType = enCube_OakLog;
					}
				
					//一番下には岩盤を設置する
					if (wy == m_minHeight)
					{
						bType = enCube_Bedrock;
					}
					//石ブロック圏内なら石
					else if (m_stoneMaxHeight >= wy && wy >= m_stoneMinHeight) {
						bType = enCube_Stone;
					}
					//鉱石ブロック圏内でノイズが許せば鉱石
					/*if( m_OreMaxHeight >= wy && wy >= m_OreMinHeight ){

						//パーリンノイズ
						float noise = GetPerlin().PerlinNoise( ( float( wx ) + m_seedX ) / m_relief2,
							( float( wy ) + m_seedY ) / m_relief2, ( float( wz ) + m_seedZ ) / m_relief2 );

						if( noise >= 0.7f ){
							bType = enCube_Soil;//暫定的に土で代用
						}
					}*/

					//if (bType == enCube_Stone || bType == enCube_Soil)
						//continue;

					//chunk.SetBlock( cx, wy, cz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}
	
	//鉱石生成
	//CreateOre co;
	//co.SetMapMaker(this);
	//co.SetWorld(m_world);
	//co.Awake(chunk);

	//チャンクを生成済みにする。
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