#include "stdafx.h"
#include "Title.h"


Title::Title()
{
}


Title::~Title()
{
}

bool Title::Start()
{
	m_spriteRender = NewGO<CSpriteRender>();
	m_spriteRender->Init();

	return true;
}

void Title::Update()
{

}