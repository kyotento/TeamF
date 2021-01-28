#include "stdafx.h"
#include "RecipeFiler.h"
#include "ItemType.h"
#include "RecipeManager.h"
#include "Item.h"
#include <nlohmann_json/json.hpp>

//! @brief ���V�s�t�@�C���ǂݍ��ݗp�̃G���[���b�Z�[�W��\�����ė��Ƃ��B
void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "���V�s�t�@�C���ǂݍ��ݎ��s�B", MB_OK );

	abort();
}

enum class Job{

	LOAD_TYPE,
	LOAD_KEY,
	LOAD_PATTERN,
	LOAD_RESULT,
};

void RecipeFiler::LoadRecipe( RecipeManager & rm ){

	if( rm.GetInited() ){
		return;
	}

	using namespace std::filesystem;
	namespace nl = nlohmann;

	path recipeDir = m_folder;

	//���V�s�t�@�C�������ׂď����B
	for( recursive_directory_iterator itr( recipeDir ), end; itr != end; itr++ ){

		path file = ( *itr ).path();

		//����ȃt�@�C���̓X�L�b�v�B
		if( ( *itr ).is_regular_file() == false ){
			continue;
		}

		//�啶������������ʂ��Ȃ��������r�B�g���q��json�łȂ���΃X�L�b�v�B
		if( _wcsicmp( file.extension().c_str(), L".json" ) != 0 ){
			continue;
		}

		Job nowJob;

		try{
			//�t�@�C�����J��
			std::ifstream ifs( file );
			if( ifs.good() == false ){
				throw "�t�@�C�����J���܂���ł����B";
			}

			//json�ǂݍ���
			nl::json jObj;
			ifs >> jObj;

			//���V�s�p�^�[���̃L�[�Ǝ��ۂ̒l��R�Â���}�b�v
			std::unordered_map<char , int> keyMap;
			//���V�s�̋󔒂����A���炩���ߐݒ肵�Ă����B
			keyMap.emplace( ' ', enCube_None );

			//�z�u�����A�C�e���̔z��B
			std::vector<int> itemArray;
			//���V�s�̕��ƍ����B
			int width = 0, height = 0;
			//���ʕ�
			int resultItem = 0;


			//�^�C�v�ǂݍ���
			{
				nowJob = Job::LOAD_TYPE;

				std::string type = jObj["type"].get<std::string>();

				if( type.compare( "crafting_shaped" ) == 0 ){
					//TODO �N���t�g�^�C�v��ݒ肷��B
				} else{
					throw "�s����type=>\"" + type + "\"";
				}
			}


			//�L�[�̓ǂݍ���
			{
				nowJob = Job::LOAD_KEY;

				nl::json key = jObj["key"];

				for( auto& entry : key.items() ){
					//������ID�𐔒lID�ɕϊ�����B
					std::string itemStrID = entry.value().get<std::string>();

					if( entry.key().length() != 1 ){
						throw "�L�[��1�����Ŏw�肵�Ă��������B";
					}

					int itemIntID = Item::GetItem( itemStrID ).GetID();

					keyMap.emplace( entry.key()[0] , itemIntID );
				}
			}


			//�z�u�ǂݍ���
			{
				nowJob = Job::LOAD_PATTERN;

				nl::json pattern = jObj["pattern"];

				if( pattern.is_array() == false ){
					throw "�p�^�[���ɂ͕�����̔z����w�肵�Ă��������B";
				}

				const int tableWidth = 3;

				if( pattern.size() > tableWidth ){
					throw "�p�^�[����4�s�ȏ゠��܂��B�p�^�[����3�s�܂łł��B";
				}

				//���V�s�̍������X�V�B
				height = max( height, int(pattern.size()) );

				for( nl::json& ptRow : pattern ){
					std::string ptStr = ptRow.get<std::string>();

					if( ptStr.length() > tableWidth ){
						throw "�p�^�[����4��ȏ゠��܂��B�p�^�[����3��܂łł��B";
					}
					//���V�s�̕����X�V�B
					width = max( width, int(ptStr.length()) );

					for( char c : ptStr ){
						//�L�[�ɂ��������ăA�C�e���ԍ����Z�b�g�B
						itemArray.push_back( keyMap.at( c ) );
					}
				}
			}

			//���ʕ��̓ǂݍ���
			{
				nowJob = Job::LOAD_RESULT;

				std::string result = jObj["result"].get<std::string>();

				//������ID�𐔒lID�ɕϊ�����B
				resultItem = Item::GetItem( result ).GetID();
			}

			//���V�s�����B
			auto recipe = std::make_unique<Recipe>( width, height, itemArray, Item::GetItem( resultItem ) );
			RecipeManager::Instance().AddRecipe( std::move( recipe ) );

			rm.SetInited();

		} catch( nl::detail::exception& ex ){

			messageAbort( file, ex.what() );

		} catch( const char* exMsg ){

			messageAbort( file, exMsg );

		} catch( std::out_of_range& ex ){

			switch( nowJob ){
			case Job::LOAD_KEY:
				messageAbort( file, "�L�[�ɑ��݂��Ȃ��A�C�e��ID���w�肳��Ă��܂��B" );
				break;
			case Job::LOAD_PATTERN:
				messageAbort( file, "�w�肳��Ă��Ȃ��L�[���p�^�[���Ŏg�p���Ă��܂��B" );
				break;
			case Job::LOAD_RESULT:
				messageAbort( file, "���ʕ��ɑ��݂��Ȃ��A�C�e��ID���w�肳��Ă��܂��B" );
				break;
			default:
				messageAbort( file, std::string("�s����out_of_range�G���[�B\n") + ex.what() );
				break;
			}

		}

	}

}
