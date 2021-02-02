//! @file
#pragma once
#include "BlockType.h"

//! @brief ブロックの種類ごとに共通の情報。
struct BlockInfo{
	std::string name;
	int hp;
	std::string usefulTool;
	std::filesystem::path modelPath;
};

//! @brief ブロック情報をロード、格納するクラス。
class BlockInfoDictionary{
public:

	void Load( const std::filesystem::path& filepath );

	const BlockInfo& GetInfo( EnCube id ) const;

	const std::unordered_map<EnCube, BlockInfo>& GetMap() const{
		return m_infoMap;
	}

private:
	std::unordered_map<EnCube, BlockInfo> m_infoMap;
};

