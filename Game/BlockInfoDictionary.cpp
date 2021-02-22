#include "stdafx.h"
#include "BlockInfoDictionary.h"
#include "Block.h"
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

enum Job{
	LOAD,
	TOOL,
};

void BlockInfoDictionary::Load( const std::filesystem::path & folderPath ){
	//enum�̖��O->�l�̃}�b�v���쐬�B
	std::unordered_map<std::string_view, EnCube> blockEnumMap;
	std::unordered_map<std::string_view, EnTool> toolEnumMap;
	std::unordered_map<std::string_view, int> enumMap;

	for( int i = enCube_None + 1; i < enCube_Num; i++ ){
		EnCube e = EnCube( i );
		blockEnumMap.emplace( NAMEOF_ENUM( e ), e );
	}
	for( int i = enTool_None + 1; i < enTool_Num; i++ ){
		EnTool e = EnTool( i );
		toolEnumMap.emplace( NAMEOF_ENUM( e ), e );
	}
	for (int i = enCube_None + 1; i < enCube_Num; i++) {
		EnCube e = EnCube(i);
		enumMap.emplace(NAMEOF_ENUM(e), e);
	}
	for (int i = enCube_Num; i < enAllItem_Num; i++) {
		EnItem e = EnItem(i);
		enumMap.emplace(NAMEOF_ENUM(e), e);
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

		Job job = LOAD;

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
			const EnCube blockId = blockEnumMap.at( blockIdStr );

			//�u���b�N����V�K�쐬�B
			BlockInfo& bInfo = m_infoMap[blockId];
			bInfo.id = blockId;

			//���O�̎擾
			bInfo.name = UTF8toSjis( jObj["name"].get<std::string>() );

			//���f���p�X�̎擾
			bInfo.modelPath = jObj["model"].get<std::string>();
			bInfo.modelPath = AddResorcePath( bInfo.modelPath.string() );

			//HP�̎擾
			bInfo.hp = jObj["hp"].get<int>();

			//�L�p�c�[���̎擾(�f�t�H���g�ł͋󕶎�)
			job = TOOL;
			if( jObj.find( "tool" ) != jObj.end() ){
				bInfo.usefulTool = toolEnumMap.at( jObj["tool"].get<std::string>() );
			}

			//AABB�̎擾(�f�t�H���g�ł͗�����1��)
			if( jObj.find( "aabb" ) != jObj.end() ){
				nl::json posArray = jObj["aabb"];
				
				for( nl::json::iterator itr = posArray.begin(); itr != posArray.end(); itr++ ){
					AABB& aabb = bInfo.aabbArray.emplace_back();

					aabb.min.x = ( *itr )[0].get<float>();
					aabb.min.y = ( *itr )[1].get<float>();
					aabb.min.z = ( *itr )[2].get<float>();
					aabb.min *= Block::WIDTH;

					itr++;

					aabb.max.x = ( *itr )[0].get<float>();
					aabb.max.y = ( *itr )[1].get<float>();
					aabb.max.z = ( *itr )[2].get<float>();
					aabb.max *= Block::WIDTH;
				}
			} else{
				AABB& aabb = bInfo.aabbArray.emplace_back();
				aabb.min = CVector3(-0.5f, 0, -0.5f);
				aabb.min *= Block::WIDTH;
				aabb.max = CVector3( 0.5f, 1, 0.5f );
				aabb.max *= Block::WIDTH;
			}

			//���邳�̎擾(�f�t�H���g�ł�0)
			if( jObj.find( "light" ) != jObj.end() ){
				bInfo.light = jObj["light"].get<int>();
			}

			//�s�����̎擾(�f�t�H���g�ł�false)
			if( jObj.find( "opacity" ) != jObj.end() ){
				bInfo.isOpacity = jObj["opacity"].get<bool>();
			}

			//�e�N�X�`���ɓ����v�f���邩�擾(�f�t�H���g�ł�false)
			if (jObj.find("transTex") != jObj.end()) {
				bInfo.isTransTexture = jObj["transTex"].get<bool>();
			}	

			//�j�󎞂ɗ��Ƃ��A�C�e�����擾
			if (jObj.find("dropItem") != jObj.end()) {
				//�A�C�e��ID
				const std::string strItemId = jObj["dropItem"].get<std::string>();
				const int itemId = enumMap.at(strItemId);
				bInfo.dropItem = (EnItem)itemId;
			}
			else {
				bInfo.dropItem = (EnItem)blockId;
			}

			//�X�v���C�g�p�X�̎擾
			if (jObj.find("sprite") != jObj.end()) {
				bInfo.spritePath = jObj["sprite"].get<std::string>();
				bInfo.spritePath = AddResorcePath(bInfo.spritePath.string());
			}

			//�h���b�v���f���p�X�̎擾
			if (jObj.find("dropModel") != jObj.end()) {
				bInfo.dropModelPath = jObj["dropModel"].get<std::string>();
				bInfo.dropModelPath = AddResorcePath(bInfo.dropModelPath.string());
			}

			//�R�����x��(�Ă��鐔)���擾
			if( jObj.find( "fuel_level" ) != jObj.end() ){
				bInfo.fuel_level = jObj["fuel_level"].get<float>();
			}

			//�L�����R���ƏՓ˂��邩
			if (jObj.find("isColision") != jObj.end()) {
				bInfo.isColision = jObj["isColision"].get<bool>();
			}

		} catch( nl::detail::exception& ex ){
			messageAbort( file, ex.what() );

		} catch( std::out_of_range& ex){
			switch( job ){
			case LOAD:
				messageAbort( file, "�w�肳�ꂽID��BlockType.h�ɑ��݂��܂���B" );
				break;
			case TOOL:
				messageAbort( file, "�w�肳�ꂽ�c�[����ItemType.h��EnTool�ɑ��݂��܂���B" );
				break;
			default:
				messageAbort( file, std::string("�s���ȃG���[") + ex.what());
			}

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
