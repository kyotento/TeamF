//! @file
#pragma once
#include "IntVector3.h"

//! @brief 正数型で直方体を表現するクラス。
//! @details 座標が内部に含まれるか確かめたり、内部の座標をイテレーターでループ処理したりできる。
//! @author Takayama
class IntRect{
public:
	IntVector3 minPos;
	IntVector3 edgeLength;

	//! @param minPos 直方体内の最小の座標。
	//! @param edgeLength 各辺の長さ。
	IntRect( const IntVector3& minPos, const IntVector3& edgeLength )
		: minPos( minPos ), edgeLength( edgeLength ){
		_ASSERTE( edgeLength.x > 0 && edgeLength.y > 0 && edgeLength.z > 0 );
	}

	//! @param minPos 直方体内の最小の座標。
	//! @param edgeLength すべての辺の長さ。
	IntRect( const IntVector3& minPos, int edgeLength )
		: IntRect( minPos, IntVector3(edgeLength, edgeLength, edgeLength)){
		_ASSERTE( edgeLength > 0 );
	}

	//! @param center 中央の座標。
	//! @param halfEdge 各辺の長さの半分。辺の長さは halfEdge.x * 2 + 1。
	static IntRect CreateWithCenter( IntVector3 center, IntVector3 halfEdge );

	//! @param center 中央の座標。
	//! @param halfEdge すべての辺の長さの半分。
	static IntRect CreateWithCenter(const IntVector3& center, int halfEdge ){
		IntVector3 halfEdgeVec( halfEdge, halfEdge, halfEdge );
		return CreateWithCenter( center, halfEdgeVec );
	}

	//! @brief 座標が直方体に含まれるかを調べる。
	bool IsInclude( int _x, int _y, int _z ) const;

	//! @brief 座標が直方体に含まれるかを調べる。
	bool IsInclude( const IntVector3& vec ) const{
		return IsInclude( vec.x, vec.y, vec.z );
	}

	//! @brief 直方体内の整数座標を使ってループ処理を行う。
	void for_each( std::function<void( int, int, int )> func ) const;
};

