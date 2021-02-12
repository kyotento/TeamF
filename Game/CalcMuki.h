#pragma once
#include "Block.h"

Block::enMuki CalcMuki( CVector3 v ){
	v.y = 0;
	if( v.LengthSq() < 0.00001f ){
		return Block::enMuki::enNum;
	}

	if( std::abs( v.z ) > std::abs( v.x ) ){
		if( v.z > 0 ){
			return Block::enMuki::enZp;
		} else{
			return Block::enMuki::enZm;
		}
	} else{
		if( v.x > 0 ){
			return Block::enMuki::enXp;
		} else{
			return Block::enMuki::enXm;
		}
	}
}

Block::enMuki CalcMukiReverse(const CVector3& v ){
	switch( CalcMuki( v ) ){
	case Block::enMuki::enXm:
		return Block::enMuki::enXp;
	case Block::enMuki::enXp:
		return Block::enMuki::enXm;
	case Block::enMuki::enZm:
		return Block::enMuki::enZm;
	case Block::enMuki::enZp:
		return Block::enMuki::enZp;
	}
}