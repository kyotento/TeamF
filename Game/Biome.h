#pragma once
class Chunk;
class Biome
{
public:
	Biome() {}
	virtual ~Biome() {}

	//ブロックを設置する。
	virtual void InstallBlock(const Chunk& chunk, const int x, const int y, const int z) = 0;
	//ブロックのY座標を設定する。
	virtual int SetY(const int x, const int y, const int z) = 0;
protected:

};

