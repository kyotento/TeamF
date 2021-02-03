#include "stdafx.h"
#include "BlockInfoDictionary.h"
#include "Block.h"
#include <headerOnlyLib/json.hpp>
#include <headerOnlyLib/nameof.hpp>
#include "Utf8ToSjis.h"

//! @brief ブロックjson読み込み用のエラーメッセージを表示して落とす。
static void messageAbort( std::filesystem::path filePath, const std::string& msg ){
	std::string fileName = "file: " + filePath.filename().string() + '\n';

	std::string text = fileName + msg;

	MessageBox( NULL, text.c_str(), "ブロックjson読み込み失敗。", MB_OK );

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

void BlockInfoDictionary::Load( const std::filesystem::path & folderPath ){
	//enumの名前->値のマップを作成。
	std::unordered_map<std::string_view, EnCube> enumMap;

	for( int i = enCube_None + 1; i < enCube_Num; i++ ){
		EnCube e = EnCube( i );
		enumMap.emplace( NAMEOF_ENUM( e ), e );
	}

	using namespace std::filesystem;
	namespace nl = nlohmann;

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
			const std::string blockIdStr = jObj["id"].get<std::string>();
			const EnCube blockId = enumMap.at( blockIdStr );

			//ブロック情報を新規作成。
			BlockInfo& bInfo = m_infoMap[blockId];
			bInfo.id = blockId;

			//名前の取得
			bInfo.name = UTF8toSjis( jObj["name"].get<std::string>() );

			//モデルパスの取得
			bInfo.modelPath = jObj["model"].get<std::string>();
			bInfo.modelPath = AddResorcePath( bInfo.modelPath.string() );

			//HPの取得
			bInfo.hp = jObj["hp"].get<int>();

			//有用ツールの取得(デフォルトでは空文字)
			if( jObj.find( "tool" ) != jObj.end() ){
				bInfo.usefulTool = jObj["tool"].get<std::string>();
			}

			//AABBの取得(デフォルトでは立方体1個)
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

			//明るさの取得(デフォルトでは0)
			if( jObj.find( "light" ) != jObj.end() ){
				bInfo.light = jObj["light"].get<int>();
			}

			//不透明の取得(デフォルトではfalse)
			if( jObj.find( "opacity" ) != jObj.end() ){
				bInfo.isOpacity = jObj["opacity"].get<bool>();
			}


		} catch( nl::detail::exception& ex ){
			messageAbort( file, ex.what() );

		} catch( std::out_of_range ){
			messageAbort( file, "指定されたIDがBlockType.hに存在しません。" );

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

		text += "に対応するjsonが存在しません。";

		MessageBox( NULL, text.c_str(), "jsonが存在しないブロックIDです。", MB_OK );

		abort();
	}
}
