#include "stdafx.h"
#include "GameCamera.h"


GameCamera::GameCamera()
{
}


GameCamera::~GameCamera()
{
}

bool GameCamera::Start()
{
	//ƒJƒƒ‰İ’èB
	m_camera = NewGO<GameObj::PerspectiveCamera>();
	m_camera->SetPos({ 0.f,250.f,-500.f });
	SetMainCamera(m_camera);

	return true;
}

void GameCamera::Update()
{

}