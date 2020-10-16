//! @file
#pragma once

class Chunk;

//! @brief �t�@�C���ɑ΂��� Chunk �I�u�W�F�N�g�̓ǂݏ������s���B
//! @author Takayama
class ChunkFiler{
public:

	//! @param[out] chunk ���̃`�����N�Ƀt�@�C������ǂݍ��ށB
	//! @retval true �t�@�C���𐳏�ɓǂݍ��񂾁B
	//! @retval false �`�����N��񂪑��݂��Ȃ������Ȃǂ̗��R�œǂݍ��݂Ɏ��s�����B
	bool Read( Chunk& chunk );

	//! @param[in] chunk ���̃`�����N���t�@�C���ɏ����o���B
	void Write( const Chunk& chunk );
};