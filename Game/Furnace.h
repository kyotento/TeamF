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
	float m_fireTimer = 0.0f;//今の火が消えるまでのタイマー
	float m_fireTimerMax = -1.0f;//今の火の最大値。
	float m_bakeTimer = 1.0f;//1個焼けるまでのタイマー
	Inventory& m_inventory;
	std::unique_ptr<FurnaceFireGameObj>* m_mePtr = nullptr;
};

class FurnaceFire{
public:
	FurnaceFire(Inventory& inv);

	//! @brief 火がついているか。
	bool isFire(){
		return m_fireGo != nullptr;
	}

	//! @brief 火をつける。
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

	//! @brief ブロック特有の情報を、チャンクファイルから読み込む。
	virtual void ReadExData( std::ifstream& ifs ) override;

	//! @brief ブロック特有の情報を、チャンクファイルに書き込む。
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

