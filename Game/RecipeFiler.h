#pragma once

class RecipeManager;

class RecipeFiler{

	void SetFolder( std::filesystem::path folderPath ){
		std::filesystem::create_directories( folderPath );
		m_folder = folderPath;
	}

	void LoadRecipe( RecipeManager& rm );

private:
	std::filesystem::path m_folder;
};

