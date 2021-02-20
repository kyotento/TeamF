#include "stdafx.h"
#include "PlayerDataFiler.h"
#include "Player.h"

//! @brief �t�@�C���ǂ݂��݂ŁA�Â��t�H�[�}�b�g��ǂ܂Ȃ����߂ɁB
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

	//�o�[�W�����Ⴂ�̓G���[
	if( readAs<int16_t>(file) != VERSION ){
		MessageBoxA( NULL, "�v���C���[�f�[�^�̃o�[�W�������Ⴂ�܂��B\n"
					 "�v���C���[�f�[�^���폜���Ă��������B",
					 "�v���C���[�f�[�^�ǂݍ��ݎ��s", MB_OK );
		abort();
	}

	//���݈ʒu
	player->SetPos( readAs<CVector3>(file) );

	//���X�|�[���ʒu
	player->SetRespawnPos( readAs<CVector3>( file ) );

	//HP
	player->SetHP( readAs<float>( file ) );

	//�X�^�~�i
	player->SetStamina( readAs<float>( file ) );

	//�o���l
	player->SetExp( readAs<float>( file ) );

	//�C���x���g��
	player->GetInventory().ReadData( file );

	//�����̌����B
	player->SetRadianY( readAs<float>( file ) );
	player->SetRadianXZ( readAs<float>( file ) );

	m_loadSuccess = true;
}
void PlayerDataFiler::Save( const Player* player ){
	std::ofstream file( filePath, std::ios::binary );

	//�o�[�W�������������ށB
	writeAs<int16_t>( file, VERSION );

	//���݈ʒu
	writeAs<CVector3>( file, player->GetPos() );

	//���X�|�[���ʒu
	writeAs<CVector3>( file, player->GetRespawnPos() );

	//HP
	writeAs<float>( file, player->GetHP() );

	//�X�^�~�i
	writeAs<float>( file, player->GetStamina() );

	//�X�^�~�i
	writeAs<float>( file, player->GetExp() );

	//�C���x���g��
	player->GetInventory().WriteData( file );

	//�����̌����B
	writeAs<float>( file, player->GetRadianY() );
	writeAs<float>( file, player->GetRadianXZ() );
}


void PlayerConfigDataFiler::Load(Player* player) {
	std::ifstream file(filePathConfig, std::ios::binary);
	if (!file) {
		return;
	}
	//�o�[�W�����Ⴂ�̓G���[
	if (readAs<int16_t>(file) != VERSION) {
		return;
	}

	//���_���x�B
	player->SetTurnMult(readAs<float>(file));
	//���_���씽�]�B
	player->SetReverseTurnXZ(readAs<bool>(file));

	m_loadSuccess = true;
}
void PlayerConfigDataFiler::Save(const Player* player) {
	std::ofstream file(filePathConfig, std::ios::binary);

	//�o�[�W�������������ށB
	writeAs<int16_t>(file, VERSION);

	//���_���x�B
	writeAs<float>(file, player->GetTurnMult());
	//���_���씽�]�B
	writeAs<bool>(file, player->GetReverseTurnXZ());
}