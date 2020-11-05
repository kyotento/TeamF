#include "stdafx.h"
#include "WorldInfoFile.h"

namespace{
	std::filesystem::path GetFilePath(){
		std::filesystem::path path( "./Save/World/worldInfo.winfo" );
		return path;
	}
}

bool WorldInfoFile::Read(){
	std::ifstream ifs( GetFilePath() , std::ios::binary );

	if( !ifs ){
		return false;
	}

	ifs.read( reinterpret_cast<char*>( &m_info ), sizeof( m_info ) );

	m_random.seed( m_info.randomSeed );

	return true;
}

void WorldInfoFile::Write(){
	std::filesystem::path path = GetFilePath();
	//ディレクトリが無い場合に作成する。
	std::filesystem::create_directories( path.parent_path() );
	std::ofstream ofs( path, std::ios::binary);

	if( !ofs ){
		abort();
	}

	ofs.write( reinterpret_cast<char*>( &m_info ), sizeof( m_info ) );
}

void WorldInfoFile::SetRandomSeed(){
	std::random_device ra;
	m_info.randomSeed = ra();
}
