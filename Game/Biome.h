#pragma once
class Chunk;
class Biome
{
public:
	Biome() {}
	virtual ~Biome() {}

	//�u���b�N��ݒu����B
	virtual void InstallBlock(const Chunk& chunk, const int x, const int y, const int z) = 0;
	//�u���b�N��Y���W��ݒ肷��B
	virtual int SetY(const int x, const int y, const int z) = 0;
protected:

};

