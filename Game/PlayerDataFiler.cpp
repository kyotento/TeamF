#include "stdafx.h"
#include "PlayerDataFiler.h"
#include "Player.h"

//! @brief ファイル読みこみで、古いフォーマットを読まないために。
const int16_t PlayerDataFiler::VERSION = 1;

namespace{
	const char* filePath = "Save/playerData.player";
	const char* filePathConfig = "Save/playerConfig.player";

	template<class T>
	T readAs( std::ifstream& ifs ){
		T v;
		ifs.read( reinterpret_cast<char*>( &v ), sizeof( T ) );
		return v;
	}

	template<class T>
	void writeAs( std::ofstream& ofs, const T& v ){
		ofs.write( reinterpret_cast<const char*>( &v ), sizeof( T ) );
	}

	template<>
	CVector3 readAs<CVector3>( std::ifstream& ifs ){
		CVector3 v;
		ifs.read( reinterpret_cast<char*>( &v.x ), sizeof( float ) );
		ifs.read( reinterpret_cast<char*>( &v.y ), sizeof( float ) );
		ifs.read( reinterpret_cast<char*>( &v.z ), sizeof( float ) );
		return v;
	}

	template<>
	void writeAs<CVector3>( std::ofstream& ofs, const CVector3& v ){
		ofs.write( reinterpret_cast<const char*>( &v.x ), sizeof( float ) );
		ofs.write( reinterpret_cast<const char*>( &v.y ), sizeof( float ) );
		ofs.write( reinterpret_cast<const char*>( &v.z ), sizeof( float ) );
	}
}

void PlayerDataFiler::Load( Player* player ){
	std::ifstream file( filePath, std::ios::binary );
	if( !file ){
		return;
	}

	//バージョン違いはエラー
	if( readAs<int16_t>(file) != VERSION ){
		MessageBoxA( NULL, "プレイヤーデータのバージョンが違います。\n"
					 "プレイヤーデータを削除してください。",
					 "プレイヤーデータ読み込み失敗", MB_OK );
		abort();
	}

	//現在位置
	player->SetPos( readAs<CVector3>(file) );

	//リスポーン位置
	player->SetRespawnPos( readAs<CVector3>( file ) );

	//HP
	player->SetHP( readAs<float>( file ) );

	//スタミナ
	player->SetStamina( readAs<float>( file ) );

	//経験値
	player->SetExp( readAs<float>( file ) );

	//インベントリ
	player->GetInventory().ReadData( file );

	//視線の向き。
	player->SetRadianY( readAs<float>( file ) );
	player->SetRadianXZ( readAs<float>( file ) );

	m_loadSuccess = true;
}
void PlayerDataFiler::Save( const Player* player ){
	std::ofstream file( filePath, std::ios::binary );

	//バージョンを書き込む。
	writeAs<int16_t>( file, VERSION );

	//現在位置
	writeAs<CVector3>( file, player->GetPos() );

	//リスポーン位置
	writeAs<CVector3>( file, player->GetRespawnPos() );

	//HP
	writeAs<float>( file, player->GetHP() );

	//スタミナ
	writeAs<float>( file, player->GetStamina() );

	//スタミナ
	writeAs<float>( file, player->GetExp() );

	//インベントリ
	player->GetInventory().WriteData( file );

	//視線の向き。
	writeAs<float>( file, player->GetRadianY() );
	writeAs<float>( file, player->GetRadianXZ() );
}


void PlayerConfigDataFiler::Load(Player* player) {
	std::ifstream file(filePathConfig, std::ios::binary);
	if (!file) {
		return;
	}
	//バージョン違いはエラー
	if (readAs<int16_t>(file) != VERSION) {
		return;
	}

	//視点感度。
	player->SetTurnMult(readAs<float>(file));
	//視点操作反転。
	player->SetReverseTurnXZ(readAs<bool>(file));

	m_loadSuccess = true;
}
void PlayerConfigDataFiler::Save(const Player* player) {
	std::ofstream file(filePathConfig, std::ios::binary);

	//バージョンを書き込む。
	writeAs<int16_t>(file, VERSION);

	//視点感度。
	writeAs<float>(file, player->GetTurnMult());
	//視点操作反転。
	writeAs<bool>(file, player->GetReverseTurnXZ());
}