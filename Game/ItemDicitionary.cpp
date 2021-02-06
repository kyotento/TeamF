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
	//enumの名前->値のマップを作成。
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

	//フォルダ内のファイルをすべて処理。
	for( recursive_directory_iterator itr( folderPath ), end; itr != end; itr++ ){

		path file = ( *itr ).path();

		//特殊なファイルはスキップ。
		if( ( *itr ).is_regular_file() == false ){
			continue;
		}
		
		//大文字小文字を区別しない文字列比較。拡張子がjsonでなければスキップ。
		if( _wcsicmp( file.extension().c_str(), L".json" ) != 0 ){
			continue;
		}

		try{
			//ファイルを開く
			std::ifstream ifs( file );
			if( ifs.good() == false ){
				throw "ファイルを開けませんでした。";
			}
			
			//json読み込み
			nl::json jObj;
			ifs >> jObj;

			//ブロックIDを確認。
			const std::string strItemId = jObj["id"].get<std::string>();
			const int itemId = enumMap.at( strItemId );

			if( itemId < enCube_Num ){
				throw "フォルダにブロックjsonが混じっています。\n分けてください。";
			}

			//名前の取得
			std::wstring itemName;
			{
				const std::string strName = UTF8toSjis( jObj["name"].get<std::string>() );
				itemName = utf8toWide( strName );
			}

			//スプライトパスの取得
			path spritePath;
			if( jObj.find( "sprite" ) != jObj.end() ){
				spritePath = jObj["sprite"].get<std::string>();
				spritePath = AddResorcePath( spritePath.string() );
			}

			//モデルパスの取得
			path modelPath = "Resource/modelData/tools/Wood_Sword.tkm";
			if( jObj.find( "model" ) != jObj.end() ){
				modelPath = jObj["model"].get<std::string>();
				modelPath = AddResorcePath( modelPath.string() );
			}

			//スタック制限の取得
			int stackLimit = 64;
			if( jObj.find( "stack_limit" ) != jObj.end() ){
				stackLimit = jObj["stack_limit"].get<int>();
			}

			//アイテムの登録。
			m_array[itemId] = Item( EnItem( itemId ), itemName.c_str(), stackLimit, spritePath, modelPath );

			//アイテムの属性を決める。
			EnTool toolId = DetermineToolId( strItemId );
			m_array[itemId].m_toolId = toolId;

			//ツールのレベルを取得
			if( jObj.find( "tool_level" ) != jObj.end() ){
				m_array[itemId].m_toolLevel = jObj["tool_level"].get<int>();
			}

			//enum名->アイテム、のMapへ登録。
			m_nameMap.emplace( strItemId, &m_array[itemId] );

		} catch( nl::detail::exception& ex ){
			messageAbort( file, ex.what() );

		} catch( std::out_of_range ){
			messageAbort( file, "指定されたIDがItemType.hに存在しません。" );

		} catch( const char* exMsg ){
			messageAbort( file, exMsg );
		}

	}
}

void ItemDictionary::LoadBlocks( const std::unordered_map<EnCube, BlockInfo>& blockMap ){

	for( const auto& entry : blockMap ){
		const BlockInfo& bInfo = entry.second;
		EnCube itemId = entry.first;

		//ブロックアイテムの登録。
		m_array[itemId] = Item( itemId, utf8toWide(bInfo.name).c_str(), 64, bInfo.modelPath );
		//enum名->アイテム、のMapへ登録。
		m_nameMap.emplace( NAMEOF_ENUM(itemId).data(), &m_array[itemId] );
	}
}

//! @brief ブロックjson読み込み用のエラーメッセージを表示して落とす。
static void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "アイテムjson読み込み失敗。", MB_OK );

	abort();
}

//! @brief パスに"Resource/"を付け加える。
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
		//文字列検索がヒットしたら。
		if (strPos != std::string::npos)
		{
			return EnTool(i);
		}
	}
	//何もヒットしなかったら。
	return enTool_None;
}