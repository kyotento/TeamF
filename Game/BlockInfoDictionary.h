//! @file
#pragma once
#include "BlockInfo.h"

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

