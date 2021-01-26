#include "stdafx.h"
#include "RecipeFiler.h"
#include "ItemType.h"
#include "RecipeManager.h"
#include "Item.h"
#include <nlohmann_json/json.hpp>

//! @brief レシピファイル読み込み用のエラーメッセージを表示して落とす。
void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "レシピファイル読み込み失敗。", MB_OK );

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

	//レシピファイルをすべて処理。
	for( recursive_directory_iterator itr( recipeDir ), end; itr != end; itr++ ){

		path file = ( *itr ).path();

		//特殊なファイルはスキップ。
		if( ( *itr ).is_regular_file() == false ){
			continue;
		}

		//大文字小文字を区別しない文字列比較。拡張子がjsonでなければスキップ。
		if( _wcsicmp( file.extension().c_str(), L".json" ) != 0 ){
			continue;
		}

		Job nowJob;

		try{
			//ファイルを開く
			std::ifstream ifs( file );
			if( ifs.good() == false ){
				throw "ファイルを開けませんでした。";
			}

			//json読み込み
			nl::json jObj;
			ifs >> jObj;

			//レシピパターンのキーと実際の値を紐づけるマップ
			std::unordered_map<char , int> keyMap;
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
				nowJob = Job::LOAD_TYPE;

				std::string type = jObj["type"].get<std::string>();

				if( type.compare( "crafting_shaped" ) == 0 ){
					//TODO クラフトタイプを設定する。
				} else{
					throw "不正なtype=>\"" + type + "\"";
				}
			}


			//キーの読み込み
			{
				nowJob = Job::LOAD_KEY;

				nl::json key = jObj["key"];

				for( auto& entry : key.items() ){
					//文字列IDを数値IDに変換する。
					std::string itemStrID = entry.value().get<std::string>();

					if( entry.key().length() != 1 ){
						throw "キーは1文字で指定してください。";
					}

					int itemIntID = Item::GetItem( itemStrID ).GetID();

					keyMap.emplace( entry.key()[0] , itemIntID );
				}
			}


			//配置読み込み
			{
				nowJob = Job::LOAD_PATTERN;

				nl::json pattern = jObj["pattern"];

				if( pattern.is_array() == false ){
					throw "パターンには文字列の配列を指定してください。";
				}

				const int tableWidth = 3;

				if( pattern.size() > tableWidth ){
					throw "パターンが4行以上あります。パターンは3行までです。";
				}

				//レシピの高さを更新。
				height = max( height, int(pattern.size()) );

				for( nl::json& ptRow : pattern ){
					std::string ptStr = ptRow.get<std::string>();

					if( ptStr.length() > tableWidth ){
						throw "パターンが4列以上あります。パターンは3列までです。";
					}
					//レシピの幅を更新。
					width = max( width, int(ptStr.length()) );

					for( char c : ptStr ){
						//キーにしたがってアイテム番号をセット。
						itemArray.push_back( keyMap.at( c ) );
					}
				}
			}

			//成果物の読み込み
			{
				nowJob = Job::LOAD_RESULT;

				std::string result = jObj["result"].get<std::string>();

				//文字列IDを数値IDに変換する。
				resultItem = Item::GetItem( result ).GetID();
			}

			//レシピ生成。
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
				messageAbort( file, "キーに存在しないアイテムIDが指定されています。" );
				break;
			case Job::LOAD_PATTERN:
				messageAbort( file, "指定されていないキーをパターンで使用しています。" );
				break;
			case Job::LOAD_RESULT:
				messageAbort( file, "成果物に存在しないアイテムIDが指定されています。" );
				break;
			default:
				messageAbort( file, std::string("不明なout_of_rangeエラー。\n") + ex.what() );
				break;
			}

		}

	}

}
