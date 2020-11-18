#include "stdafx.h"
#include "Item.h"
#include "ItemImage.h"
#include "BlockFactory.h"

Item& Item::GetItem( unsigned id ){
	struct ItemArray{
		ItemArray(){
			m_array[enCube_Grass] = Item( enCube_Grass, L"ëê", 64, model(enCube_Grass));
			m_array[enCube_Soil] = Item( enCube_Soil, L"ìy", 64, model(enCube_Soil) );
			m_array[enCube_Stone] = Item( enCube_Stone, L"êŒ", 64, model(enCube_Stone) );
			m_array[enCube_OakLeaf] = Item(enCube_OakLeaf, L"ót", 64 , model(enCube_OakLeaf));
			m_array[enCube_IronOre] = Item( enCube_IronOre, L"ìSçzêŒ", 64 , model(enCube_IronOre));
			m_array[enCube_GoldOre] = Item( enCube_GoldOre, L"ã‡çzêŒ", 64, model( enCube_GoldOre ) );
			m_array[enItem_GoldenPickaxe] = 
				Item( enItem_GoldenPickaxe, L"ã‡ÇÃÉcÉãÉnÉV", 1, sprite(L"Resource/spriteData/Golden_Pickaxe.dds") );
			m_array[enItem_DiamondHoe] =
				Item( enItem_DiamondHoe, L"É_ÉCÉÑÇÃÉNÉè", 16, sprite( L"Resource/spriteData/Diamond_Hoe.dds" ) );
		}

		Item& operator[]( int n ){
			return m_array[n];
		}

	private:
		std::array<Item, enAllItem_Num> m_array{};

		static std::unique_ptr<ItemModelImage> model( EnCube blockType ){
			return std::make_unique<ItemModelImage>( BlockFactory::GetModelPath(blockType) );
		}
		static std::unique_ptr<ItemSpriteImage> sprite( const wchar_t* path ){
			return std::make_unique<ItemSpriteImage>( path );
		}
	};

	static ItemArray st_itemArray;

	return st_itemArray[id];
}

void Item::Draw( const CVector2 & pos, const CVector2 & scale ){
	m_image->Draw( pos, scale );
}

Item::~Item(){}

const Item & Item::operator=( Item && item ){
	m_id = item.m_id;
	m_limitNumber = item.m_limitNumber;
	m_itemName = item.m_itemName;
	m_image = std::move( item.m_image );
	return *this;
}

Item::Item(){}

Item::Item( Item&& item ) : m_id( item.m_id ), m_limitNumber( item.m_limitNumber ), m_itemName( item.m_itemName ),
m_image( std::move( item.m_image ) ){}

Item::Item( EnCube enCube, const wchar_t* itemName, int limitNumber, std::unique_ptr<ItemImage> image )
	:m_id( enCube ), m_itemName( itemName ), m_limitNumber( limitNumber ), m_image( std::move( image ) ){}

Item::Item( EnItem enItem, const wchar_t* itemName, int limitNumber, std::unique_ptr<ItemImage> image )
	:m_id( enItem ), m_itemName( itemName ), m_limitNumber( limitNumber ), m_image( std::move( image ) ){}
