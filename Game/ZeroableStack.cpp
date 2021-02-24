#include "stdafx.h"
#include "ZeroableStack.h"

#undef min;
#undef max;

void ZeroableStack::SetNumber( int num ){
	m_num = num;
	if( m_num <= 0 ){
		m_num = 0;
		m_stack = nullptr;
		return;
	}
	if( m_stack == nullptr ){
		m_stack = std::make_unique<ItemStack>( *m_item, 0 );
	}
	m_stack->SetNumber( num );
}

int ZeroableStack::GetCanTakeFrom( ZeroableStack & stack ){
	if( stack == false || ( ( *this ) && GetID() != stack.GetID() ) ){
		return 0;
	}
	SetItemToNull( stack.GetItem() );
	return std::min(stack.GetNumber(), GetItem()->GetStackLimit() - GetNumber() );
}

int ZeroableStack::TakeFrom( ZeroableStack & stack, int count ){
	const int moveCount = std::min(count, GetCanTakeFrom( stack ));
	if( moveCount == 0 ){
		return 0;
	}
	stack.SetNumber( stack.GetNumber() - moveCount );
	SetNumber( GetNumber() + moveCount );
	return moveCount;
}

void ZeroableStack::swap( ZeroableStack & stack ){
	m_stack.swap( stack.m_stack );
	std::swap( m_num, stack.m_num );
	std::swap( m_item, stack.m_item );
}
