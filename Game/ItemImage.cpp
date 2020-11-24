#include "stdafx.h"
#include "ItemImage.h"

ItemImage::ItemImage( bool isModel, const wchar_t * filepath ){
	if( isModel ){
		SkinModel model;
		model.Init( filepath );
		CQuaternion rot = CQuaternion( CVector3::AxisX(), CMath::PI_HALF );
		rot.Concatenate( CQuaternion( CVector3::AxisY(), CMath::PI_QUARTER ) );
		rot.Concatenate( CQuaternion( CVector3::AxisX(), CMath::PI_QUARTER ) );
		m_sprite.Init( filepath, model, 130, rot );
	} else{
		m_sprite.Init( filepath );
	}
}

void ItemImage::Draw( const CVector2 & pos, const CVector2 & parentScale ){
	m_sprite.DrawScreenPos( pos, parentScale * st_initialScale, m_pivot );
}
