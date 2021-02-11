#pragma once
#include "Block.h"
#include "Inventory.h"

class FurnaceFireGameObj : IGameObject{
public:
	FurnaceFireGameObj(Inventory& inv) : m_inventory(inv){}

	void Update() override;

	void SetMe( std::unique_ptr<FurnaceFireGameObj>* ptr){
		m_mePtr = ptr;
	}

	float GetArrowPer(){
		return 1.0f - m_bakeTimer;
	}

	float GetFirePer(){
		return m_fireTimer / m_fireTimerMax;
	}

	void ReadData( std::ifstream& ifs );

	void WriteData( std::ofstream& ofs );


private:
	float m_fireTimer = 0.0f;//���̉΂�������܂ł̃^�C�}�[
	float m_fireTimerMax = -1.0f;//���̉΂̍ő�l�B
	float m_bakeTimer = 1.0f;//1�Ă���܂ł̃^�C�}�[
	Inventory& m_inventory;
	std::unique_ptr<FurnaceFireGameObj>* m_mePtr = nullptr;
};

class FurnaceFire{
public:
	FurnaceFire(Inventory& inv);

	//! @brief �΂����Ă��邩�B
	bool isFire(){
		return m_fireGo != nullptr;
	}

	//! @brief �΂�����B
	void setFire(){
		m_fireGo = std::make_unique<FurnaceFireGameObj>( m_inventory );
		m_fireGo->SetMe( &m_fireGo );
	}

	void SourceChange();

	float GetArrowPer(){
		if( m_fireGo ){
			return m_fireGo->GetArrowPer();
		}
		return 0;
	}

	float GetFirePer(){
		if( m_fireGo ){
			return m_fireGo->GetFirePer();
		}
		return 0;
	}

	void ReadData( std::ifstream& ifs ){
		int8_t haveFire;
		ifs.read( reinterpret_cast<char*>( &haveFire ), sizeof( haveFire ) );
		if( haveFire ){
			setFire();
			m_fireGo->ReadData( ifs );
		}
	}

	void WriteData( std::ofstream& ofs ){
		if( m_fireGo ){
			int8_t haveFire = 1;
			ofs.write( reinterpret_cast<const char*>( &haveFire ), sizeof( haveFire ) );
			m_fireGo->WriteData( ofs );
			return;
		}
		int8_t haveFire = 0;
		ofs.write( reinterpret_cast<const char*>( &haveFire ), sizeof( haveFire ) );
	}

private:
	std::unique_ptr<FurnaceFireGameObj> m_fireGo;
	Inventory& m_inventory;
};

class Furnace :public Block{
public:
	bool OnClick( Player* player ) override;

	//! @brief �u���b�N���L�̏����A�`�����N�t�@�C������ǂݍ��ށB
	virtual void ReadExData( std::ifstream& ifs ) override;

	//! @brief �u���b�N���L�̏����A�`�����N�t�@�C���ɏ������ށB
	virtual void WriteExData( std::ofstream& ofs ) override;

	enum Slot{
		FUEL,
		SOURCE,
		RESULT,
		NUM
	};
private:
	Inventory m_inventory{Slot::NUM};
	FurnaceFire m_fire{m_inventory};
};

