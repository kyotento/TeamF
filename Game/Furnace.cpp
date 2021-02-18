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

	//燃やすものがなくなったら火を消す。
	if( source.GetNumber() == 0 ){
		m_fireGo.reset();
		return;
	}

	//完成品スロットと違うIDでは燃やせない。
	if(result.GetID() != enCube_None && source.GetID() != result.GetID() ){
		m_fireGo.reset();
		return;
	}

	//スタック上限。
	if( result.GetID() != enCube_None && result.GetStackLimit() == result.GetNumber() ){
		m_fireGo.reset();
		return;
	}

	auto resultItem = RecipeManager::Instance().GetFurnaceResult( source.GetID() );

	//燃やせないアイテムなら火を消す。
	if( resultItem == nullptr ){
		m_fireGo.reset();
	}
}

void FurnaceFireGameObj::Update(){
	auto& result = m_inventory.GetItem( Furnace::RESULT );
	auto& fuel = m_inventory.GetItem( Furnace::FUEL );
	auto& source = m_inventory.GetItem( Furnace::SOURCE );

	//10秒でタイマーをひとつ減らす。
	m_fireTimer -= GetDeltaTimeSec() * 0.1f;
	m_bakeTimer -= GetDeltaTimeSec() * 0.1f;

	//焼けたときの処理。
	if( m_bakeTimer < 0 ){
		auto resultItem = RecipeManager::Instance().GetFurnaceResult( source->GetID() );

		//精錬不可能なアイテム
		if( resultItem == nullptr ){
			m_mePtr->reset();
			return;
		}

		//完成品を追加。
		if( result == nullptr ){
			result = std::move(resultItem);
		} else{
			result->AddNumber( 1 );
		}

		//素材を減らす。
		source->AddNumber( -1 );
		if( source->GetNumber() <= 0 ){
			source.reset();
			m_mePtr->reset();
			return;
		}

		//スタック上限
		if( result->GetNumber() >= result->GetStackLimit() ){
			m_mePtr->reset();
			return;
		}

		m_bakeTimer = 1.0f;
	}

	//次の燃料を消費する処理。
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
