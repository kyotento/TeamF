#include "stdafx.h"
#include "RecipeFiler.h"
#include <jansson.h>

namespace std{
	//スマートポインタのデリーターに特殊化を追加。
	template<>
	struct default_delete<json_t>{
		void operator()( json_t* ptr ){
			json_decref( ptr );
		}
	};
}

//@brief レシピ読み込みでエラーメッセージを出して強制終了する関数オブジェクト。
class RecipeError{
public:
	void SetFileName(const std::string& name ){
		m_name = name + '\n';
	}

	void operator()(const char* message){
		std::string text = m_name + message;

		MessageBox( NULL, text.c_str(), "レシピファイル読み込み失敗。", MB_OK );
		abort();
	}

private:
	std::string m_name;
};

void RecipeFiler::LoadRecipe( RecipeManager & rm ){
	//using namespace std::filesystem;

	//path recipeDir( ".\Recipes" );

	////レシピファイルをすべて処理。
	//for( directory_iterator itr( recipeDir ), end; itr != end; itr++ ){
	//	//通常のファイルだけを対象にする。
	//	if( ( *itr ).is_regular_file() ){
	//		path file = ( *itr ).path();

	//		//大文字小文字を区別せずに拡張子がjsonだと確認。
	//		if(wcsicmp( file.extension().c_str(), L"json")){

	//			RecipeError error;
	//			error.SetFileName( file.filename.string() );

	//			//json読み込み。
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

	//			//いちいちget()を呼びたくないので普通のポインタを宣言。
	//			json_t* jObj = jObjPtr.get();

	//			if( !json_is_object( jObj ) ){
	//				error( "レシピファイルがオブジェクトの形になっていません。" );
	//			}

	//			//タイプ読み込み
	//			{
	//				json_t* type = json_object_get( jObj, "type" );

	//				if( type == nullptr ){
	//					error( "typeがありません。" );
	//				}
	//				if( json_is_string( type ) == false ){
	//					error( "typeには文字列を指定してください。" );
	//				}

	//				const char* typeStr = json_string_value( jObj );

	//				if( strcmp( typeStr, "crafting_shaped" ) ){
	//					//TODO クラフトタイプを設定する。
	//				} else{
	//					error( "typeが不正です。" );
	//				}
	//			}

	//			//配置読み込み
	//			{
	//				json_t* type = json_object_get( jObj, "pattern" );

	//				if( type == nullptr ){
	//					error( "patternがありません。" );
	//				}
	//				if( json_is_array( type ) == false ){
	//					error( "typeには文字列配列を指定してください。" );
	//				}

	//			}

	//		}
	//	}
	//}
}
