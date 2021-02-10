#pragma once

template<class Stream>
class ChunkFlags{
public:
	ChunkFlags( Stream& stream ) : stream( stream ){
		static_assert( std::is_same<Stream, std::ifstream>::value ||
					   std::is_same<Stream, std::ofstream>::value||
					   std::is_same<Stream, std::fstream>::value );
	}

	//! �t���O��ǂݍ��݁B
	void ReadFlags(){
		stream.seekg( 0 );
		stream.read( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
	}

	//! �t���O���������݁BStream��fstream�̂Ƃ���p�B
	void WriteFlags(){
		if constexpr( std::is_same<Stream, std::ifstream>::value == false ){
			stream.seekp( 0 );
			stream.write( reinterpret_cast<char*>( &flags ), sizeof( flags ) );
		} else{
			_ASSERTE( false && "std::ifstream�ŏ���������Ƃ��̊֐��͎g���܂���B" );
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
	bool IsExist() const {
		return flags & GetFlagMask( Flag::Exist );
	}
	//! �`�����N�������ς݂̃t���O���擾�B
	bool IsGenerated() const {
		return flags & GetFlagMask( Flag::Generated );
	}

	enum Flag{
		Exist,
		Generated,
		Num,
	};

private:
	//! �e�t���O�̃`�����N�ԍ��ɍ��킹���}�X�N���擾�B
	int8_t GetFlagMask( Flag f) const {
		return  1 << f;
	}

	//! �ǂݏ����p�t�@�C���X�g���[��
	Stream& stream;

	//! �t���O�̊i�[���ꂽ���l
	int8_t flags = 0;
};

using ChunkFlagsEnum = ChunkFlags<std::fstream>::Flag;