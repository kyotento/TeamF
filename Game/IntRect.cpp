#include "stdafx.h"
#include "IntRect.h"

IntRect IntRect::CreateWithCenter( IntVector3 center, IntVector3 halfEdge ){
	center.x -= halfEdge.x; center.y -= halfEdge.y; center.z -= halfEdge.z;
	halfEdge *= 2;
	halfEdge.x += 1; halfEdge.y += 1; halfEdge.z += 1;
	return IntRect( center, halfEdge );
}

bool IntRect::IsInclude( int _x, int _y, int _z ) const{
	const bool include = 
		minPos.x  <= _x && _x < minPos.x + edgeLength.x &&
		minPos.y  <= _y && _y < minPos.y + edgeLength.y &&
		minPos.z  <= _z && _z < minPos.z + edgeLength.z;

	return include;
}

void IntRect::for_each( std::function<void( int, int, int )> func ) const{
	const int xStart = minPos.x, yStart = minPos.y, zStart = minPos.z;
	const int xEnd = xStart + edgeLength.x, yEnd = yStart + edgeLength.y, zEnd = zStart + edgeLength.z;

	for(int x = xStart; x < xEnd; x++ ){
		for(int y = yStart; y < yEnd; y++ ){
			for(int z = zStart; z < zEnd; z++ ){
				func( x, y, z );
			}
		}
	}
}
