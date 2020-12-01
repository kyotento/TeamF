#include "stdafx.h"
#include "RecipeFiler.h"
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
	void SetFileName(const std::string& name ){
		m_name = name + '\n';
	}

	void operator()(const char* message){
		std::string text = m_name + message;

		MessageBox( NULL, text.c_str(), "���V�s�t�@�C���ǂݍ��ݎ��s�B", MB_OK );
		abort();
	}

private:
	std::string m_name;
};

void RecipeFiler::LoadRecipe( RecipeManager & rm ){
	//using namespace std::filesystem;

	//path recipeDir( ".\Recipes" );

	////���V�s�t�@�C�������ׂď����B
	//for( directory_iterator itr( recipeDir ), end; itr != end; itr++ ){
	//	//�ʏ�̃t�@�C��������Ώۂɂ���B
	//	if( ( *itr ).is_regular_file() ){
	//		path file = ( *itr ).path();

	//		//�啶������������ʂ����Ɋg���q��json���Ɗm�F�B
	//		if(wcsicmp( file.extension().c_str(), L"json")){

	//			RecipeError error;
	//			error.SetFileName( file.filename.string() );

	//			//json�ǂݍ��݁B
	//			json_error_t jerror;
	//			std::unique_ptr<json_t> jObjPtr(json_load_file( file.string().c_str(), 0, &jerror ));

	//			if( !jObjPtr ){
	//				std::stringstream str;
	//				str << jerror.text << std::endl
	//					<< jerror.source << std::endl
	//					<< "line: " << jerror.line << ", " << jerror.column << std::endl
	//					<< "bytePosition: " << jerror.position;
	//				error( str.str().c_str() );
	//			}

	//			//��������get()���Ăт����Ȃ��̂ŕ��ʂ̃|�C���^��錾�B
	//			json_t* jObj = jObjPtr.get();

	//			if( !json_is_object( jObj ) ){
	//				error( "���V�s�t�@�C�����I�u�W�F�N�g�̌`�ɂȂ��Ă��܂���B" );
	//			}

	//			//�^�C�v�ǂݍ���
	//			{
	//				json_t* type = json_object_get( jObj, "type" );

	//				if( type == nullptr ){
	//					error( "type������܂���B" );
	//				}
	//				if( json_is_string( type ) == false ){
	//					error( "type�ɂ͕�������w�肵�Ă��������B" );
	//				}

	//				const char* typeStr = json_string_value( jObj );

	//				if( strcmp( typeStr, "crafting_shaped" ) ){
	//					//TODO �N���t�g�^�C�v��ݒ肷��B
	//				} else{
	//					error( "type���s���ł��B" );
	//				}
	//			}

	//			//�z�u�ǂݍ���
	//			{
	//				json_t* type = json_object_get( jObj, "pattern" );

	//				if( type == nullptr ){
	//					error( "pattern������܂���B" );
	//				}
	//				if( json_is_array( type ) == false ){
	//					error( "type�ɂ͕�����z����w�肵�Ă��������B" );
	//				}

	//			}

	//		}
	//	}
	//}
}
