#pragma once

class Chunk;

class ChunkFiler{
public:

	void Read( Chunk& chunk );

	void Write( const Chunk& chunk );

private:
	std::filesystem::path m_filePath = R"(.\Save\World\region.regi)";
};