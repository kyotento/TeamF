#include "stdafx.h"
#include "BlockInfo.h"

namespace{
	BlockInfo GetNoneBlock(){
		BlockInfo b;
		b.isOpacity = false;
		return b;
	}
}

const BlockInfo BlockInfo::NONE_BLOCK = GetNoneBlock();
