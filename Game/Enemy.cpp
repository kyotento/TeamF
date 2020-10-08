#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
	m_skinModelRender = NewGO<GameObj::CSkinModelRender>();
}


Enemy::~Enemy()
{
	DeleteGO(m_skinModelRender);
}
