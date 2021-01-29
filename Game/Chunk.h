//! @file
#pragma once
#include "Block.h"

/*! @brief World クラスは保持するべき Block オブジェクトをこのクラスごとに分けて保持する。
	@details #WIDTH * #HEIGHT * #WIDTH の3次元配列でブロックを保持する。
	@author Takayama */
class Chunk{
public:

	Chunk(){}

	Chunk( const Chunk& c ) = delete;
	const Chunk& operator=( const Chunk& c ) = delete;

	Chunk( Chunk&& c )noexcept{
		m_chunkX = c.m_chunkX;
		m_chunkZ = c.m_chunkZ;
		std::move( &c.m_blockArray[0][0][0], &c.m_blockArray[WIDTH - 1][HEIGHT - 1][WIDTH - 1], &m_blockArray[0][0][0] );
	}
	const Chunk& operator=( Chunk&& c )noexcept{
		m_chunkX = c.m_chunkX;
		m_chunkZ = c.m_chunkZ;
		std::move( &c.m_blockArray[0][0][0], &c.m_blockArray[WIDTH - 1][HEIGHT - 1][WIDTH - 1], &m_blockArray[0][0][0] );
	}

	//! @brief チャンク内の座標でライト情報を取得
	char* GetLightData(int x, int y, int z) {
		return &m_lightData[x][y][z];
	}
	//! @brief チャンク内の座標でライト情報を取得
	char* GetLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetLightData(x, y, z);
	}

	//! @brief チャンク内の座標でスカイライト情報を取得
	char* GetSkyLightData(int x, int y, int z) {
		return &m_skyLightData[x][y][z];
	}
	//! @brief チャンク内の座標でスカイライト情報を取得
	char* GetSkyLightData(const CVector3& pos) {
		int x = (int)std::floorf(pos.x);
		int y = (int)std::floorf(pos.y);
		int z = (int)std::floorf(pos.z);
		return GetLightData(x, y, z);
	}

	//! @brief チャンク内の座標でブロックを取得
	Block* GetBlock( int x, int y, int z ){
		return m_blockArray[x][y][z].get();
	}
	//! @brief チャンク内の座標でブロックを取得
	Block* GetBlock( const CVector3& pos ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		return GetBlock( x, y, z );
	}

	//! @brief チャンク内の座標でブロックを設定
	void SetBlock( int x, int y, int z, std::unique_ptr<Block> block ){
		m_blockArray[x][y][z] = std::move( block );
		m_blockArray[x][y][z]->SetPos( CalcWorldCoordX( x ), y, CalcWorldCoordZ( z ) );
	}

	//! @brief チャンク内の座標でブロックを設定
	void SetBlock( const CVector3& pos, std::unique_ptr<Block> block ){
		int x = (int)std::floorf( pos.x );
		int y = (int)std::floorf( pos.y );
		int z = (int)std::floorf( pos.z );
		SetBlock( x, y, z, std::move( block ) );
	}

	//チャンク内座標でブロックを設置、プレイヤー用。
	bool PlaceBlock(const int x, const int y, const int z, std::unique_ptr<Block> block)
	{
		
		if (m_blockArray[x][y][z] == nullptr) {
			m_blockArray[x][y][z] = std::move(block);
			m_blockArray[x][y][z]->SetPos(CalcWorldCoordX(x), y, CalcWorldCoordZ(z));
			return true;
		}
		return false;
	}
	
	//ブロックを削除 チャンク座標でブロックを削除。
	void DeleteBlock(const int x, const int y, const int z) {
		m_blockArray[x][y][z].reset();
	}

	//! @brief チャンク内座標を計算
	//! @param coord ワールド座標のxかz
	//! @return チャンク内座標のxかz
	static int CalcInChunkCoord( int coord ){
		if( coord < 0 ){
			coord -= Chunk::WIDTH - 1;
			return ( Chunk::WIDTH - 1 ) + coord % Chunk::WIDTH;
		}
		return coord % Chunk::WIDTH;
	}

	//! @brief ワールド座標のX値を計算
	int CalcWorldCoordX( int x ){
		return x + m_chunkX * Chunk::WIDTH;
	}

	//! @brief ワールド座標のZ値を計算
	int CalcWorldCoordZ( int z ){
		return z + m_chunkZ * Chunk::WIDTH;
	}

	//! @brief チャンク1個分を1としたときのこのチャンクの座標を設定。
	void SetChunkPos( int x, int z ){
		m_chunkX = x; m_chunkZ = z;
	}

	//! @brief チャンク座標のX。
	int GetX() const{
		return m_chunkX;
	}
	//! @brief チャンク座標のZ。
	int GetZ() const{
		return m_chunkZ;
	}

	//! @brief ブロック配列の先頭アドレスを取得。
	const auto& Data() const{
		return m_blockArray;
	}

	//! @brief ブロック配列の先頭アドレスを取得。
	auto& Data(){
		return m_blockArray;
	}

	//! @brief 地形生成完了フラグを立てる。
	void SetGenerated(){
		m_isGenerated = true;
	}

	//! @brief 地形生成が完了しているかを返す。
	bool IsGenerated() const{
		return m_isGenerated;
	}

	//! @brief チャンクのx,z方向の長さ。
	static constexpr int WIDTH = 16;
	//! @brief チャンクのy方向の長さ。
	static constexpr int HEIGHT = 64;

private:
	int m_chunkX = 0;
	int m_chunkZ = 0;

	//ライト情報
	char m_lightData[WIDTH][HEIGHT][WIDTH] = {};
	char m_skyLightData[WIDTH][HEIGHT][WIDTH] = {};

	std::unique_ptr<Block> m_blockArray[WIDTH][HEIGHT][WIDTH];

	bool m_isGenerated = false;
};

