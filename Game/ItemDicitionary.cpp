#include "stdafx.h"
#include "ItemDictionary.h"
#include "BlockInfoDictionary.h"
#include <headerOnlyLib/json.hpp>
#include <headerOnlyLib/nameof.hpp>
#include "Utf8ToSjis.h"

void messageAbort( std::filesystem::path filePath, const std::string& msg );
std::string AddResorcePath( std::string path );

Item& ItemDictionary::GetItem( const std::string & id ){
	return *( m_nameMap.at( id ) );
}

void ItemDictionary::LoadItems( std::filesystem::path folderPath ){
	//enum�̖��O->�l�̃}�b�v���쐬�B
	std::unordered_map<std::string_view, int> enumMap;
	{
		for( int i = enCube_None + 1; i < enCube_Num; i++ ){
			EnCube e = EnCube( i );
			enumMap.emplace( NAMEOF_ENUM( e ), e );
		}
		for( int i = enCube_Num; i < enAllItem_Num; i++ ){
			EnItem e = EnItem( i );
			enumMap.emplace( NAMEOF_ENUM( e ), e );
		}
	}

	using namespace std::filesystem;
	namespace nl = nlohmann;
	
	SetToolMap();

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
			const std::string strItemId = jObj["id"].get<std::string>();
			const int itemId = enumMap.at( strItemId );

			if( itemId < enCube_Num ){
				throw "�t�H���_�Ƀu���b�Njson���������Ă��܂��B\n�����Ă��������B";
			}

			//���O�̎擾
			std::wstring itemName;
			{
				const std::string strName = UTF8toSjis( jObj["name"].get<std::string>() );
				itemName = utf8toWide( strName );
			}

			//�X�v���C�g�p�X�̎擾
			path spritePath;
			if( jObj.find( "sprite" ) != jObj.end() ){
				spritePath = jObj["sprite"].get<std::string>();
				spritePath = AddResorcePath( spritePath.string() );
			}

			//���f���p�X�̎擾
			path modelPath = "Resource/modelData/tools/Wood_Sword.tkm";
			if( jObj.find( "model" ) != jObj.end() ){
				modelPath = jObj["model"].get<std::string>();
				modelPath = AddResorcePath( modelPath.string() );
			}

			//�X�^�b�N�����̎擾
			int stackLimit = 64;
			if( jObj.find( "stack_limit" ) != jObj.end() ){
				stackLimit = jObj["stack_limit"].get<int>();
			}

			//�A�C�e���̓o�^�B
			m_array[itemId] = Item( EnItem( itemId ), itemName.c_str(), stackLimit, spritePath, modelPath );

			//�A�C�e���̑��������߂�B
			EnTool toolId = DetermineToolId( strItemId );
			m_array[itemId].m_toolId = toolId;

			//�c�[���̃��x�����擾
			if( jObj.find( "tool_level" ) != jObj.end() ){
				m_array[itemId].m_toolLevel = jObj["tool_level"].get<int>();
			}

			//enum��->�A�C�e���A��Map�֓o�^�B
			m_nameMap.emplace( strItemId, &m_array[itemId] );

		} catch( nl::detail::exception& ex ){
			messageAbort( file, ex.what() );

		} catch( std::out_of_range ){
			messageAbort( file, "�w�肳�ꂽID��ItemType.h�ɑ��݂��܂���B" );

		} catch( const char* exMsg ){
			messageAbort( file, exMsg );
		}

	}
}

void ItemDictionary::LoadBlocks( const std::unordered_map<EnCube, BlockInfo>& blockMap ){

	for( const auto& entry : blockMap ){
		const BlockInfo& bInfo = entry.second;
		EnCube itemId = entry.first;

		//�u���b�N�A�C�e���̓o�^�B
		m_array[itemId] = Item( itemId, utf8toWide(bInfo.name).c_str(), 64, bInfo.modelPath );
		//enum��->�A�C�e���A��Map�֓o�^�B
		m_nameMap.emplace( NAMEOF_ENUM(itemId).data(), &m_array[itemId] );
	}
}

//! @brief �u���b�Njson�ǂݍ��ݗp�̃G���[���b�Z�[�W��\�����ė��Ƃ��B
static void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "�A�C�e��json�ǂݍ��ݎ��s�B", MB_OK );

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

void ItemDictionary::SetToolMap()
{
	m_toolMap[enTool_Sword] = "Sword";
	m_toolMap[enTool_Pickaxe] = "Pickaxe";
	m_toolMap[enTool_Shovel] = "Shovel";
	m_toolMap[enTool_Axe] = "Axe";
	m_toolMap[enTool_Hoe] = "Hoe";
}

EnTool ItemDictionary::DetermineToolId(std::string itemid)
{
	for (int i = 0; i < enTool_Num; i++)
	{
		int strPos = itemid.find(m_toolMap[i]);
		//�����񌟍����q�b�g������B
		if (strPos != std::string::npos)
		{
			return EnTool(i);
		}
	}
	//�����q�b�g���Ȃ�������B
	return enTool_None;
}