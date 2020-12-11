#include "stdafx.h"
#include "RecipeFiler.h"
#include "ItemType.h"
#include "RecipeManager.h"
#include "Item.h"
#include <jansson.h>

namespace std{
	//�X�}�[�g�|�C���^�̃f���[�^�[�ɓ��ꉻ��ǉ��B
	template<>
	struct default_delete<json_t>{
		void operator()( json_t* ptr ){
			json_decref( ptr );
		}
	};
}

//@brief ���V�s�ǂݍ��݂ŃG���[���b�Z�[�W���o���ċ����I������֐��I�u�W�F�N�g�B
class RecipeError{
public:
	void SetFileName( const std::string& name ){
		m_fileName = "file: " + name + '\n';
	}

	void operator()( const char* message ){
		std::string text = m_fileName + message;

		MessageBox( NULL, text.c_str(), "���V�s�t�@�C���ǂݍ��ݎ��s�B", MB_OK );
		abort();
	}

private:
	std::string m_fileName;
};

void RecipeFiler::LoadRecipe( RecipeManager & rm ){

	using namespace std::filesystem;

	path recipeDir = m_folder;

	//���V�s�t�@�C�������ׂď����B
	for( recursive_directory_iterator itr( recipeDir ), end; itr != end; itr++ ){

		//�ʏ�̃t�@�C��������Ώۂɂ���B
		if( ( *itr ).is_regular_file() ){

			path file = ( *itr ).path();

			//�啶������������ʂ����Ɋg���q��json���Ɗm�F�B
			if( _wcsicmp( file.extension().c_str(), L".json" ) == 0 ){

				//�G���[�̃_�C�A���O���o���I�u�W�F�N�g�B
				RecipeError error;
				error.SetFileName( file.filename().string() );

				//json�ǂݍ��݁B
				json_error_t jerror;
				std::unique_ptr<json_t> jObjPtr( json_load_file( file.string().c_str(), 0, &jerror ) );

				if( !jObjPtr ){
					std::stringstream str;
					str << jerror.text << std::endl
						<< jerror.source << std::endl
						<< "line: " << jerror.line << ", " << jerror.column << std::endl
						<< "bytePosition: " << jerror.position;
					error( str.str().c_str() );
				}

				//��������get()���Ăт����Ȃ��̂ŕ��ʂ̃|�C���^��錾�B
				json_t* jObj = jObjPtr.get();

				if( !json_is_object( jObj ) ){
					error( "���V�s�t�@�C�����I�u�W�F�N�g�̌`�ɂȂ��Ă��܂���B" );
				}

				//���V�s�p�^�[���̃L�[�Ǝ��ۂ̒l��R�Â���}�b�v
				std::unordered_map<char, int> keyMap;
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
					json_t* type = json_object_get( jObj, "type" );

					if( type == nullptr ){
						error( "type������܂���B" );
					}
					if( json_is_string( type ) == false ){
						error( "type�ɂ͕�������w�肵�Ă��������B" );
					}

					const char* typeStr = json_string_value( type );

					if( strcmp( typeStr, "crafting_shaped" ) == 0 ){
						//TODO �N���t�g�^�C�v��ݒ肷��B
					} else{
						error( "type���s���ł��B" );
					}
				}


				//�L�[�̓ǂݍ���
				{
					json_t* keyObj = json_object_get( jObj, "key" );

					if( keyObj == nullptr ){
						error( "�L�[������܂���B" );
					}
					if( json_is_object( keyObj ) == false ){
						error( "�L�[�ɂ̓I�u�W�F�N�g���w�肵�Ă��������B" );
					}

					const char *key;
					json_t *value;
					//�S�L�[�����[�v�B
					json_object_foreach( keyObj, key, value ){
						if( json_is_string( value ) == false ){
							error( "�L�[�̒l�͕�������w�肵�Ă��������B" );
						}
						if( key[0] == '\0' || key[1] != '\0' ){
							error( "�L�[�ɂ͔��p1�������w�肵�Ă��������B" );
						}

						//������ID�𐔒lID�ɕϊ�����B
						const char* itemStrID = json_string_value( value );
						int itemIntID;
						try{
							itemIntID = Item::GetItem( itemStrID ).GetID();
						} catch( std::out_of_range ){
							error( "�L�[�ɑ��݂��Ȃ��A�C�e��ID���w�肳��Ă��܂��B" );
						}

						keyMap.emplace( key[0], itemIntID );
					}
				}


				//�z�u�ǂݍ���
				{
					json_t* pattern = json_object_get( jObj, "pattern" );

					if( pattern == nullptr ){
						error( "�p�^�[��������܂���B" );
					}
					if( json_is_array( pattern ) == false ){
						error( "�p�^�[���ɂ͕�����̔z����w�肵�Ă��������B" );
					}

					size_t index;
					json_t* value;
					const int tableWidth = 3;

					//�p�^�[���z���1�s�����[�v�B
					json_array_foreach( pattern, index, value ){

						if( index == tableWidth ){
							error( "�p�^�[����4�s�ȏ゠��܂��B�p�^�[����3�s�܂łł��B" );
						}
						if( json_is_string( value ) == false ){
							error( "�p�^�[���z��̒��g�͕�����Ŏw�肵�Ă��������B" );
						}
						std::string patStr = json_string_value( value );

						//�擾�����s��1���������[�v�B
						int col = 0;
						for( char c : patStr ){
							if( col == tableWidth ){
								error( "�p�^�[����4��ȏ゠��܂��B�p�^�[����3��܂łł��B" );
							}
							col++;

							//�L�[�ɂ��������ăA�C�e���ԍ����Z�b�g�B
							try{
								itemArray.push_back( keyMap.at( c ) );
							} catch( std::out_of_range ){
								error( "�w�肳��Ă��Ȃ��L�[���p�^�[���Ŏg�p���Ă��܂��B" );
							}
						}
						//���V�s�̕����X�V�B
						width = max( width, col);
					}

					//���V�s�̍������X�V�B
					height = max( height, index );
				}

				//���ʕ��̓ǂݍ���
				{
					json_t* result = json_object_get( jObj, "result" );

					if( result == nullptr ){
						error( "���ʕ�������܂���B" );
					}
					if( json_is_string( result ) == false ){
						error( "���ʕ��ɂ͕�������w�肵�Ă��������B" );
					}

					//������ID�𐔒lID�ɕϊ�����B
					const char* itemStrID = json_string_value( result );
					try{
						resultItem = Item::GetItem( itemStrID ).GetID();
					} catch( std::out_of_range ){
						error( "���ʕ��ɑ��݂��Ȃ��A�C�e��ID���w�肳��Ă��܂��B" );
					}
				}

				//���V�s�����B
				auto recipe = std::make_unique<Recipe>( width, height, itemArray, Item::GetItem( resultItem ) );
				RecipeManager::Instance().AddRecipe( std::move( recipe ) );
			}
		}
	}

}
