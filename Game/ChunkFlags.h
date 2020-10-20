#pragma once

template<class Stream>
class ChunkFlags{
public:
	ChunkFlags( Stream& stream , int32_t chunkNo ) : stream( stream ) , chunkNo(chunkNo){
		static_assert( std::is_same<Stream, std::ifstream>::value ||
					   std::is_same<Stream, std::fstream>::value );
	}

	//! �t���O��ǂݍ��݁B
	void ReadFlags(){
		stream.seekg( chunkNo * Flag::Num / 8 );
		stream.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	}

	//! �t���O���������݁BStream��fstream�̂Ƃ���p�B
	void WriteFlags(){
		if constexpr( std::is_same<Stream, std::fstream>::value ){
			stream.seekp( chunkNo * Flag::Num / 8 );
			stream.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
		}
	}

	//! �`�����N�����݂���t���O�𗧂Ă�B
	void SetExist(){
		flags |= GetFlagMask( Flag::Exist );
	}
	//! �`�����N�������ς݂̃t���O�𗧂Ă�B
	void SetGenerated(){
		flags |= GetFlagMask( Flag::Generated );
	}
	//! �`�����N�����݂���t���O���擾�B
	bool IsExist(){
		return flags & GetFlagMask( Flag::Exist );
	}
	//! �`�����N�������ς݂̃t���O���擾�B
	bool IsGenerated(){
		return flags & GetFlagMask( Flag::Generated );
	}

	enum Flag{
		Exist,
		Generated,
		Num,
	};

private:
	//! �e�t���O�̃`�����N�ԍ��ɍ��킹���}�X�N���擾�B
	int8_t GetFlagMask( Flag f){
		//1�`�����N���Ƃ�Num�̃t���O������B
		//�����̃`�����N�������ꏊ�Ɉړ������̂��A�ړI�̃t���O " f " �Ɉړ�����B
		return  1 << ( chunkNo * Flag::Num % 8 + f );
	}

	//! �`�����N�ԍ�
	int32_t chunkNo;

	//! �ǂݏ����p�t�@�C���X�g���[��
	Stream& stream;

	//! �t���O�̊i�[���ꂽ���l
	int8_t flags;
};

using ChunkFlagsEnum = ChunkFlags<std::fstream>::Flag;