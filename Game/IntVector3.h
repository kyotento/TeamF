#pragma once

struct IntVector3{
	int x, y, z;

	constexpr IntVector3( int x, int y, int z ) : x( x ), y( y ), z( z ){}
	explicit IntVector3(const CVector3& v) : x(int(std::floor(v.x))), y( int(std::floor( v.y )) ), z( int(std::floor( v.z )) ){}

	bool operator==( const IntVector3& rhs ) const{
		return rhs.x == x && rhs.y == y && rhs.z == z;
	}
	bool operator!=( const IntVector3& rhs ) const{
		return !operator==( rhs );
	}

	const IntVector3& operator+=( const IntVector3& v ){
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	IntVector3 operator+( IntVector3 v )const{
		v.operator+=( *this );
		return v;
	}

	IntVector3 operator-()const{
		IntVector3 v = *this;
		v.x *= -1; v.y *= -1; v.z *= -1;
		return v;
	}

	const IntVector3& operator-=( const IntVector3& v ){
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	IntVector3 operator-( const IntVector3& v )const{
		IntVector3 vec = *this;
		vec -= v;
		return vec;
	}

	const IntVector3& operator*=( int v ){
		x *= v; y *= v; z *= v;
		return *this;
	}

	IntVector3 operator*( int v )const{
		IntVector3 vec = *this;
		vec *= v;
		return vec;
	}

	float LengthSq()const {
		return x * x + y * y + z * z;
	}

	[[nodiscard]] 
	static constexpr IntVector3 Zero() noexcept
	{
		return{ 0, 0, 0 };
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
