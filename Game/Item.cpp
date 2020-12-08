#include "stdafx.h"
#include "Item.h"
#include "ItemImage.h"
#include "ItemDictionary.h"

Item& Item::GetItem( unsigned id ){
	return ItemDictionary::Instance().GetItem( id );
}

Item & Item::GetItem( const std::string & strId ){
	return ItemDictionary::Instance().GetItem( strId );
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

Item::Item( EnCube enCube, const wchar_t* itemName, int limitNumber, const std::filesystem::path& modelPath )
	: m_id( enCube ), m_itemName( itemName ), m_limitNumber( limitNumber ),
	m_image( std::make_unique<ItemImage>( true, modelPath ) ){}

Item::Item( EnItem enItem, const wchar_t* itemName, int limitNumber, const std::filesystem::path& spritePath )
	: m_id( enItem ), m_itemName( itemName ), m_limitNumber( limitNumber ),
	m_image( std::make_unique<ItemImage>( false, spritePath ) ){}
