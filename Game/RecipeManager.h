#pragma once

#include "Recipe.h"

class RecipeManager{
public:

	static RecipeManager& Instance(){
		static RecipeManager st_manager;
		return st_manager;
	}

	void AddRecipe( std::unique_ptr<Recipe> recipe ){
		m_recipeMap[recipe->GetWH()].push_back( std::move( recipe ) );
	}

private:
	RecipeManager(){}
	~RecipeManager(){}

	using RecipeVector = std::vector<std::unique_ptr<Recipe>>;
	std::map<std::pair<int, int>, RecipeVector> m_recipeMap;
};

