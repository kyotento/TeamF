#include "stdafx.h"
#include "RespawnPointFiler.h"

namespace
{
	const char* filePath = "Save/PlayerRespawnPoint.rspwn";
}

bool RespawnPointFiler::Load(CVector3& respawnP) {
	std::ifstream file(filePath);
	if (!file){
		return false;
	}

	file >> respawnP.x;
	file >> respawnP.y;
	file >> respawnP.z;

	return true;
}
void RespawnPointFiler::Save(const CVector3& respawnP) {
	std::ofstream file;
	file.open(filePath, std::ios::trunc);

	file << respawnP.x << std::endl;
	file << respawnP.y << std::endl;
	file << respawnP.z << std::endl;

	file.close();
}