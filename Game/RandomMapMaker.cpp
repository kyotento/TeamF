#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Perlin.h"
#include "World.h"

#include "BlockFactory.h"

void RandomMapMaker::Awake(){

	//�u���b�N�t�@�N�g����������
	BlockFactory::LoadInstancingModels( m_width * m_depth * ( m_maxHeight + 1 ) );

	std::random_device rand;
	//�����}�b�v�𐶐����Ȃ��悤�ɃV�[�h����
	m_seedX = rand() % 101;
	m_seedZ = rand() % 101;
	m_seedY = rand() % 101;

	//�L���[�u����
	for( int x = 0; x < m_width; x++ ){
		for( int z = 0; z < m_depth; z++ ){

			CVector3 pos = CVector3( x, 0, z );
			pos.y = SetY( pos );
			int xx = int( pos.x );
			int yy = int( pos.y );
			int zz = int( pos.z );

			//��Ō��肵�����������Ƃɍō����x�̃u���b�N��ݒu�B
			m_world->SetBlock( xx, yy, zz, BlockFactory::CreateBlock( enCube_Grass ) );

			//���肵���ō��n�_����Œፂ�x�܂Ńu���b�N�������߂Ă����B
			while( yy > m_minHeight ){

				yy--;
				pos.y = float( yy ) * 10.f;

				{
					//�y
					EnCube bType = enCube_Soil;
					//�΃u���b�N�����Ȃ��
					if( m_stoneMaxHeight >= yy && yy >= m_stoneMinHeight ){
						bType = enCube_Stone;
					}

					//�z�΃u���b�N�����Ńm�C�Y�������΍z��
					if( m_OreMaxHeight >= yy && yy >= m_OreMinHeight ){

						//�p�[�����m�C�Y
						float noise = GetPerlin().PerlinNoise( ( float( xx ) + m_seedX ) / m_relief2,
							( float( yy ) + m_seedY ) / m_relief2, ( float( zz ) + m_seedZ ) / m_relief2 );

						if( noise >= 0.7f ){
							bType = enCube_Soil;//�b��I�ɓy�ő�p
						}
					}

					m_world->SetBlock( xx, yy, zz, BlockFactory::CreateBlock( bType ) );

				}
			}
		}
	}

	m_world->GenerateEndCulling();
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