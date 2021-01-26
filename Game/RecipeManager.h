#pragma once

#include "Recipe.h"

class ItemStack;

class RecipeManager{
public:

	static RecipeManager& Instance(){
		static RecipeManager st_manager;
		return st_manager;
	}

	void AddRecipe( std::unique_ptr<Recipe> recipe ){
		m_recipeMap[recipe->GetWH()].push_back( std::move( recipe ) );
	}

	//! @brief クラフトテーブルの配列から成果物アイテムを取得。
	std::unique_ptr<ItemStack> GetResult( const int( &itemArray )[9] ){
		return GetResult( 3, itemArray );
	}

	//! @brief クラフトテーブルの配列から成果物アイテムを取得。
	std::unique_ptr<ItemStack> GetResult( const int( &itemArray )[4] ){
		return GetResult( 2, itemArray );
	}

	bool GetInited(){
		return m_inited;
	}

	void SetInited(){
		m_inited = true;
	}

private:
	RecipeManager(){}
	~RecipeManager(){}

	//! @param width クラフトテーブルの幅
	std::unique_ptr<ItemStack> GetResult(const int width, const int * const itemArray );

	using RecipeVector = std::vector<std::unique_ptr<Recipe>>;
	std::map<std::pair<int, int>, RecipeVector> m_recipeMap;

	bool m_inited = false;
};

