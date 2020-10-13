#pragma once

class Chunk;

class ChunkFiler{
public:

	void Read( Chunk& chunk );

	void Write( const Chunk& chunk );
};