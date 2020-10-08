#pragma once
#include "BlockType.h"


class Player;
class Box;
class Drop : public IGameObject
{
public:
	Drop() {}
	~Drop() 
	{
		DeleteGO(m_model);
	}
	bool Start() override;
	void Update() override;
	void SetPosition(const CVector3& position)
	{
		m_position = position;
	}
	void SetNumber(int number)
	{
		m_number = number;
	}
	void SetEnCube(EnCube enCube)
	{
		m_state = enCube;
	}
private:
	void Distance();
private:
	GameObj::CSkinModelRender* m_model;
	CVector3 m_position = CVector3::Zero();
	int m_number = 0;
	EnCube m_state = enCube_None;
	Player* m_player = nullptr;
	Box* m_box = nullptr;
};

