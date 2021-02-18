#include "stdafx.h"
#include "Furnace.h"
#include "FurnaceInventory.h"
#include "Player.h"
#include "ItemStack.h"
#include "RecipeManager.h"

bool Furnace::OnClick( Player * player ){
	player->OpenGUI( std::make_unique<GUI::FurnaceInventory>( player, m_inventory, m_fire ) );
	return true;
}

void Furnace::ReadExData( std::ifstream & ifs ){
	m_inventory.ReadData( ifs );
	m_fire.ReadData( ifs );
}

void Furnace::WriteExData( std::ofstream & ofs ){
	m_inventory.WriteData( ofs );
	m_fire.WriteData( ofs );
}

FurnaceFire::FurnaceFire( Inventory & inv ): m_inventory(inv){}

void FurnaceFire::SourceChange(){
	auto& result = m_inventory.GetNullableItem( Furnace::RESULT );
	auto& source = m_inventory.GetNullableItem( Furnace::SOURCE );

	//�R�₷���̂��Ȃ��Ȃ�����΂������B
	if( source.GetNumber() == 0 ){
		m_fireGo.reset();
		return;
	}

	//�����i�X���b�g�ƈႤID�ł͔R�₹�Ȃ��B
	if(result.GetID() != enCube_None && source.GetID() != result.GetID() ){
		m_fireGo.reset();
		return;
	}

	//�X�^�b�N����B
	if( result.GetID() != enCube_None && result.GetStackLimit() == result.GetNumber() ){
		m_fireGo.reset();
		return;
	}

	auto resultItem = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

	//�R�₹�Ȃ��A�C�e���Ȃ�΂������B
	if( resultItem == nullptr ){
		m_fireGo.reset();
	}
}

void FurnaceFireGameObj::Update(){
	auto& result = m_inventory.GetItem( Furnace::RESULT );
	auto& fuel = m_inventory.GetItem( Furnace::FUEL );
	auto& source = m_inventory.GetItem( Furnace::SOURCE );

	//10�b�Ń^�C�}�[���ЂƂ��炷�B
	m_fireTimer -= GetDeltaTimeSec() * 0.1f;
	m_bakeTimer -= GetDeltaTimeSec() * 0.1f;

	//�Ă����Ƃ��̏����B
	if( m_bakeTimer < 0 ){
		auto resultItem = RecipeManager::Instance().GetFurnaceResult( source->GetID() );

		//���B�s�\�ȃA�C�e��
		if( resultItem == nullptr ){
			m_mePtr->reset();
			return;
		}

		//�����i��ǉ��B
		if( result == nullptr ){
			result = std::move(resultItem);
		} else{
			result->AddNumber( 1 );
		}

		//�f�ނ����炷�B
		source->AddNumber( -1 );
		if( source->GetNumber() <= 0 ){
			source.reset();
			m_mePtr->reset();
			return;
		}

		//�X�^�b�N���
		if( result->GetNumber() >= result->GetStackLimit() ){
			m_mePtr->reset();
			return;
		}

		m_bakeTimer = 1.0f;
	}

	//���̔R��������鏈���B
	if( m_fireTimer < 0 ){
		if( fuel == nullptr ){
			m_mePtr->reset();
			return;
		}

		fuel->AddNumber( -1 );
		m_fireTimer = fuel->GetFuelLevel();
		m_fireTimerMax = m_fireTimer;

		if( fuel->GetNumber() <= 0 ){
			fuel.reset();
		}
	}
}

void FurnaceFireGameObj::ReadData( std::ifstream& ifs ){
	ifs.read( reinterpret_cast<char*>( &m_bakeTimer ), sizeof( m_bakeTimer ) );
	ifs.read( reinterpret_cast<char*>( &m_fireTimer ), sizeof( m_fireTimer ) );
	ifs.read( reinterpret_cast<char*>( &m_fireTimerMax ), sizeof( m_fireTimerMax ) );
}

void FurnaceFireGameObj::WriteData( std::ofstream& ofs ){
	ofs.write( reinterpret_cast<const char*>( &m_bakeTimer ), sizeof( m_bakeTimer ) );
	ofs.write( reinterpret_cast<const char*>( &m_fireTimer ), sizeof( m_fireTimer ) );
	ofs.write( reinterpret_cast<const char*>( &m_fireTimerMax ), sizeof( m_fireTimerMax ) );
}
