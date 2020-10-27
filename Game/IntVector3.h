#pragma once

struct IntVector3{
	int x, y, z;

	IntVector3( int x, int y, int z ) : x( x ), y( y ), z( z ){}
	explicit IntVector3(const CVector3& v) : x(std::floor(v.x)), y( std::floor( v.y ) ), z( std::floor( v.z ) ){}

	bool operator==( const IntVector3& rhs ) const{
		return rhs.x == x && rhs.y == y && rhs.z == z;
	}
	bool operator!=( const IntVector3& rhs ) const{
		return !operator==( rhs );
	}
};

namespace std{
	template<>
	class hash<IntVector3>{
	public:
		size_t operator () ( const IntVector3 &p ) const{
			auto hash = std::hash<int>();

			auto hash1 = hash( p.x );
			auto hash2 = hash( p.y );
			auto hash3 = hash( p.z );


			//重複しないようにハッシュ処理
			//コピペなので意味はわからんが、いいらしい。
			size_t seed = 0;
			seed = hash1 + 0x9e3779b9;
			seed ^= hash2 + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
			seed ^= hash3 + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
			return seed;
		}
	};
}
