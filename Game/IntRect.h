//! @file
#pragma once
#include "IntVector3.h"

//! @brief �����^�Œ����̂�\������N���X�B
//! @details ���W�������Ɋ܂܂�邩�m���߂���A�����̍��W���C�e���[�^�[�Ń��[�v����������ł���B
//! @author Takayama
class IntRect{
public:
	IntVector3 minPos;
	IntVector3 edgeLength;

	//! @param minPos �����̓��̍ŏ��̍��W�B
	//! @param edgeLength �e�ӂ̒����B
	IntRect( const IntVector3& minPos, const IntVector3& edgeLength )
		: minPos( minPos ), edgeLength( edgeLength ){
		_ASSERTE( edgeLength.x > 0 && edgeLength.y > 0 && edgeLength.z > 0 );
	}

	//! @param minPos �����̓��̍ŏ��̍��W�B
	//! @param edgeLength ���ׂĂ̕ӂ̒����B
	IntRect( const IntVector3& minPos, int edgeLength )
		: IntRect( minPos, IntVector3(edgeLength, edgeLength, edgeLength)){
		_ASSERTE( edgeLength > 0 );
	}

	//! @param center �����̍��W�B
	//! @param halfEdge �e�ӂ̒����̔����B�ӂ̒����� halfEdge.x * 2 + 1�B
	static IntRect CreateWithCenter( IntVector3 center, IntVector3 halfEdge );

	//! @param center �����̍��W�B
	//! @param halfEdge ���ׂĂ̕ӂ̒����̔����B
	static IntRect CreateWithCenter(const IntVector3& center, int halfEdge ){
		IntVector3 halfEdgeVec( halfEdge, halfEdge, halfEdge );
		return CreateWithCenter( center, halfEdgeVec );
	}

	//! @brief ���W�������̂Ɋ܂܂�邩�𒲂ׂ�B
	bool IsInclude( int _x, int _y, int _z ) const;

	//! @brief ���W�������̂Ɋ܂܂�邩�𒲂ׂ�B
	bool IsInclude( const IntVector3& vec ) const{
		return IsInclude( vec.x, vec.y, vec.z );
	}

	//! @brief �����̓��̐������W���g���ă��[�v�������s���B
	void for_each( std::function<void( int, int, int )> func ) const;
};

