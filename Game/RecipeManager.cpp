#include "stdafx.h"
#include "RecipeManager.h"

std::unique_ptr<ItemStack> RecipeManager::GetFurnaceResult( int itemID ){
	if( m_furnaceMap.count( itemID ) == 0 ){
		return nullptr;
	}
	int id = m_furnaceMap[itemID];
	return std::make_unique<ItemStack>( Item::GetItem( id ) );
}

std::unique_ptr<ItemStack> RecipeManager::GetResult( const int width, const int * const itemArray ){

	int up = width, bottom = -1, left = width, right = -1;

	//[0, 1, 2,
	// 3, 4, 5,
	// 6, 7, 8]
	//アイテム配列をこう並べて、左上が座標(0,0)とみたてて計算。
	for( int i = 0; i < width*width; i++ ){

		if( itemArray[i] != enCube_None ){

			left = min( left, i % width);
			right = max( right, i % width);

			up = min( up, i / width);
			bottom = max( bottom, i / width);
		}
	}

	//一つもアイテムが無い場合。
	if( up == width ){
		return nullptr;
	}

	//AABBでアイテムリストを生成。
	std::vector<int> aabbArray;
	for( int y = up; y <= bottom; y++ ){
		for( int x = left; x <= right; x++ ){
			aabbArray.push_back( itemArray[x + y * width] );
		}
	}

	//縦横幅でレシピリストを取得。
	const int aabbW = right - left + 1;
	const int aabbH = bottom - up + 1;

	try{
		auto& recipeList = m_recipeMap.at( std::make_pair( aabbW, aabbH ) );

		//一致するレシピを調べる。
		for( auto& recipe : recipeList ){
			if( recipe->ArrayEquals( aabbArray ) ){
				return recipe->GetResult();
			}
		}

		if( aabbW > 1 ){

			//左右反転で一致するレシピを調べる。
			for( auto& recipe : recipeList ){
				if( recipe->ArrayEqualsMirror( aabbArray ) ){
					return recipe->GetResult();
				}
			}

		}

	} catch( std::out_of_range ){
		return nullptr;
	}

	return nullptr;
}
