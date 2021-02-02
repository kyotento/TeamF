#include "stdafx.h"
#include "BlockInfoDictionary.h"
#include <headerOnlyLib/json.hpp>
#include <headerOnlyLib/nameof.hpp>
#include "Utf8ToSjis.h"

//! @brief �u���b�Njson�ǂݍ��ݗp�̃G���[���b�Z�[�W��\�����ė��Ƃ��B
static void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "�u���b�Njson�ǂݍ��ݎ��s�B", MB_OK );

	abort();
}

//! @brief �p�X��"Resource/"��t��������B
static std::string AddResorcePath( std::string path ){
	if( path.substr( 0, 1 ) == "/" ){
		path = path.substr( 1 );
	}

	if( path.substr( 0, 9 ) != "Resource/" ){
		path = "Resource/" + path;
	}

	return path;
}

void BlockInfoDictionary::Load( const std::filesystem::path & folderPath ){
	//enum�̖��O->�l�̃}�b�v���쐬�B
	std::unordered_map<std::string_view, EnCube> enumMap;

	for( int i = enCube_None + 1; i < enCube_Num; i++ ){
		EnCube e = EnCube( i );
		enumMap.emplace( NAMEOF_ENUM( e ), e );
	}

	using namespace std::filesystem;
	namespace nl = nlohmann;

	//�t�H���_���̃t�@�C�������ׂď����B
	for( recursive_directory_iterator itr( folderPath ), end; itr != end; itr++ ){

		path file = ( *itr ).path();

		//����ȃt�@�C���̓X�L�b�v�B
		if( ( *itr ).is_regular_file() == false ){
			continue;
		}

		//�啶������������ʂ��Ȃ��������r�B�g���q��json�łȂ���΃X�L�b�v�B
		if( _wcsicmp( file.extension().c_str(), L".json" ) != 0 ){
			continue;
		}

		try{
			//�t�@�C�����J��
			std::ifstream ifs( file );
			if( ifs.good() == false ){
				throw "�t�@�C�����J���܂���ł����B";
			}

			//json�ǂݍ���
			nl::json jObj;
			ifs >> jObj;

			//�u���b�NID���m�F�B
			const std::string blockIdStr = jObj["id"].get<std::string>();
			const EnCube blockId = enumMap.at( blockIdStr );

			//�u���b�N����V�K�쐬�B
			BlockInfo& bInfo = m_infoMap[blockId];

			//���O�̎擾
			bInfo.name = UTF8toSjis( jObj["name"].get<std::string>() );

			//���f���p�X�̎擾
			bInfo.modelPath = jObj["model"].get<std::string>();
			bInfo.modelPath = AddResorcePath( bInfo.modelPath.string() );

			//HP�̎擾
			bInfo.hp = jObj["hp"].get<int>();

			//�L�p�c�[���̎擾(�f�t�H���g�ł͋󕶎�)
			if( jObj.find( "tool" ) != jObj.end() ){
				bInfo.usefulTool = jObj["tool"].get<std::string>();
			}


		} catch( nl::detail::exception& ex ){
			messageAbort( file, ex.what() );

		} catch( std::out_of_range ){
			messageAbort( file, "�w�肳�ꂽID��BlockType.h�ɑ��݂��܂���B" );

		} catch( const char* exMsg ){
			messageAbort( file, exMsg );
		}

	}
}

const BlockInfo & BlockInfoDictionary::GetInfo( EnCube id ) const{
	try{
		return m_infoMap.at( id );
	} catch( std::out_of_range ){
		std::string text = NAMEOF_ENUM( id ).data();

		text += "�ɑΉ�����json�����݂��܂���B";

		MessageBox( NULL, text.c_str(), "json�����݂��Ȃ��u���b�NID�ł��B", MB_OK );

		abort();
	}
}
