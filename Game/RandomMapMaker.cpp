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

			
			//洞窟生成
			bool isCave = false;
			int caveMinY = m_minHeight+1, caveMaxY = wy;
			{
				//小穴
				float caveNoise = GetPerlin().OctavePerlin(wx / 15.7f + m_seedX2, 0.0f, wz / 15.7f + m_seedZ2, 2);
				
				if (
					caveNoise < 0.33f || caveNoise > 0.67f 
				) {
					//ノイズ
					float noise = GetPerlin().PerlinNoise(
						(float(wx) + m_seedX2) / 12.5f,
						0.0f,
						(float(wz) + m_seedZ2) / 12.5f
					);
					float noise2 = GetPerlin().PerlinNoise(
						(float(wx) + m_seedZ) / 37.3f,
						0.0f,
						(float(wz) + m_seedX) / 37.3f
					);

					isCave = true;
					
					//洞窟空間生成
					caveMinY = (int)(max(1.0f,(float)wy * noise2));
					caveMaxY = caveMinY + (int)(4.0f * noise) + 2;

					//地盤沈下
					if (caveMaxY < wy && wy - caveMaxY <= 1) {
						caveMaxY = wy;
					}
				}
			}		

			if (!isCave || !(wy >= caveMinY && wy <= caveMaxY)) {//洞窟範囲でなければ
				if (state == enBiome_Forest) {
					//上で決定した高さをもとに最高高度のブロックを設置。
					chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_Grass));
					//木を生やす。
					m_treeGenerator.GenerateTree(wx, wy, wz);
				}
				else if (state == enBiome_Desert) {
					chunk.SetBlock(cx, wy, cz, BlockFactory::CreateBlock(enCube_Sand));
				}
			}

			//決定した最高地点から最低高度までブロックをしきつめていく。
			while( wy > m_minHeight ){

				wy--;

				//洞窟生成
				if (isCave && wy >= caveMinY && wy <= caveMaxY) {
					continue;
				}

				{
					//土
					EnCube bType = enCube_Soil;
					if (state == enBiome_Desert)
					{
						bType = enCube_Sand;
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

					chunk.SetBlock( cx, wy, cz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}
	
	//鉱石生成
	CreateOre co;
	co.SetMapMaker(this);
	co.SetWorld(m_world);
	co.Awake(chunk);

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