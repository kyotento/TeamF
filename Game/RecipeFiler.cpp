#include "stdafx.h"
#include "RecipeFiler.h"
#include "ItemType.h"
#include "RecipeManager.h"
#include "Item.h"
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
	void SetFileName( const std::string& name ){
		m_fileName = "file: " + name + '\n';
	}

	void operator()( const char* message ){
		std::string text = m_fileName + message;

		MessageBox( NULL, text.c_str(), "レシピファイル読み込み失敗。", MB_OK );
		abort();
	}

private:
	std::string m_fileName;
};

void RecipeFiler::LoadRecipe( RecipeManager & rm ){

	using namespace std::filesystem;

	path recipeDir = m_folder;

	//レシピファイルをすべて処理。
	for( recursive_directory_iterator itr( recipeDir ), end; itr != end; itr++ ){

		//通常のファイルだけを対象にする。
		if( ( *itr ).is_regular_file() ){

			path file = ( *itr ).path();

			//大文字小文字を区別せずに拡張子がjsonだと確認。
			if( _wcsicmp( file.extension().c_str(), L".json" ) == 0 ){

				//エラーのダイアログを出すオブジェクト。
				RecipeError error;
				error.SetFileName( file.filename().string() );

				//json読み込み。
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

				//いちいちget()を呼びたくないので普通のポインタを宣言。
				json_t* jObj = jObjPtr.get();

				if( !json_is_object( jObj ) ){
					error( "レシピファイルがオブジェクトの形になっていません。" );
				}

				//レシピパターンのキーと実際の値を紐づけるマップ
				std::unordered_map<char, int> keyMap;
				//レシピの空白だけ、あらかじめ設定しておく。
				keyMap.emplace( ' ', enCube_None );

				//配置されるアイテムの配列。
				std::vector<int> itemArray;
				//レシピの幅と高さ。
				int width = 0, height = 0;
				//成果物
				int resultItem = 0;


				//タイプ読み込み
				{
					json_t* type = json_object_get( jObj, "type" );

					if( type == nullptr ){
						error( "typeがありません。" );
					}
					if( json_is_string( type ) == false ){
						error( "typeには文字列を指定してください。" );
					}

					const char* typeStr = json_string_value( type );

					if( strcmp( typeStr, "crafting_shaped" ) == 0 ){
						//TODO クラフトタイプを設定する。
					} else{
						error( "typeが不正です。" );
					}
				}


				//キーの読み込み
				{
					json_t* keyObj = json_object_get( jObj, "key" );

					if( keyObj == nullptr ){
						error( "キーがありません。" );
					}
					if( json_is_object( keyObj ) == false ){
						error( "キーにはオブジェクトを指定してください。" );
					}

					const char *key;
					json_t *value;
					//全キーをループ。
					json_object_foreach( keyObj, key, value ){
						if( json_is_string( value ) == false ){
							error( "キーの値は文字列を指定してください。" );
						}
						if( key[0] == '\0' || key[1] != '\0' ){
							error( "キーには半角1文字を指定してください。" );
						}

						//文字列IDを数値IDに変換する。
						const char* itemStrID = json_string_value( value );
						int itemIntID;
						try{
							itemIntID = Item::GetItem( itemStrID ).GetID();
						} catch( std::out_of_range ){
							error( "キーに存在しないアイテムIDが指定されています。" );
						}

						keyMap.emplace( key[0], itemIntID );
					}
				}


				//配置読み込み
				{
					json_t* pattern = json_object_get( jObj, "pattern" );

					if( pattern == nullptr ){
						error( "パターンがありません。" );
					}
					if( json_is_array( pattern ) == false ){
						error( "パターンには文字列の配列を指定してください。" );
					}

					size_t index;
					json_t* value;
					const int tableWidth = 3;

					//パターン配列を1行ずつループ。
					json_array_foreach( pattern, index, value ){

						if( index == tableWidth ){
							error( "パターンが4行以上あります。パターンは3行までです。" );
						}
						if( json_is_string( value ) == false ){
							error( "パターン配列の中身は文字列で指定してください。" );
						}
						std::string patStr = json_string_value( value );

						//取得した行を1文字ずつループ。
						int col = 0;
						for( char c : patStr ){
							if( col == tableWidth ){
								error( "パターンが4列以上あります。パターンは3列までです。" );
							}
							col++;

							//キーにしたがってアイテム番号をセット。
							try{
								itemArray.push_back( keyMap.at( c ) );
							} catch( std::out_of_range ){
								error( "指定されていないキーをパターンで使用しています。" );
							}
						}
						//レシピの幅を更新。
						width = max( width, col);
					}

					//レシピの高さを更新。
					height = max( height, index );
				}

				//成果物の読み込み
				{
					json_t* result = json_object_get( jObj, "result" );

					if( result == nullptr ){
						error( "成果物がありません。" );
					}
					if( json_is_string( result ) == false ){
						error( "成果物には文字列を指定してください。" );
					}

					//文字列IDを数値IDに変換する。
					const char* itemStrID = json_string_value( result );
					try{
						resultItem = Item::GetItem( itemStrID ).GetID();
					} catch( std::out_of_range ){
						error( "成果物に存在しないアイテムIDが指定されています。" );
					}
				}

				//レシピ生成。
				auto recipe = std::make_unique<Recipe>( width, height, itemArray, Item::GetItem( resultItem ) );
				RecipeManager::Instance().AddRecipe( std::move( recipe ) );
			}
		}
	}

}
