#include "stdafx.h"
#include "ItemImage.h"

ItemModelImage::ItemModelImage( const wchar_t * modelpath ){
	m_model.Init( modelpath );
}

void ItemModelImage::Draw( const CVector2 & pos, const CVector2 & parentScale ){
	m_model.UpdateWorldMatrix( pos, CQuaternion::Identity(), parentScale * st_initialScale );
	m_model.Draw( false, 1 );
}

ItemSpriteImage::ItemSpriteImage( const wchar_t * spritePath ){
	m_sprite.Init( spritePath );
}

void ItemSpriteImage::Draw( const CVector2 & pos, const CVector2 & parentScale ){
	m_sprite.DrawScreenPos( pos, parentScale * st_initialScale, m_pivot);
}
