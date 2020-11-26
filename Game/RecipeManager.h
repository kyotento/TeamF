#pragma once

#include "Recipe.h"

class RecipeManager{
public:
	void AddRecipe( std::unique_ptr<Recipe> recipe ){
		m_recipeMap[recipe->GetWH()].push_back( std::move( recipe ) );
	}

private:
	using RecipeVector = std::vector<std::unique_ptr<Recipe>>;
	std::map<std::pair<int, int>, RecipeVector> m_recipeMap;
};

