#pragma once
#include "BlockType.h"
#include "Block.h"

class BlockFactory{
public:

	static void LoadInstancingModels(int instanceMax);

	static std::unique_ptr<Block> CreateBlock( EnCube blockType );

private:

};

