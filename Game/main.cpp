#include "stdafx.h"
#include "RandomMapMaker.h"
//#include <charconv>
//#include "DemolisherWeapon/GraphicsAPI/DirectX12/DX12Test.h"
#include "WasdCamera.h"
#include "World.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	//�G���W���̏������ݒ�
	InitEngineParameter initparam;
	
	//TODO �t���X�N�Ή�
	//initparam.windowStyle = WS_POPUP | WS_BORDER ;		//�E�B���h�E�X�^�C��

	//�𑜓x
	//initparam.screenWidth = initparam.frameBufferWidth = initparam.frameBufferWidth3D = 1920;
	//initparam.screenHeight = initparam.frameBufferHeight = initparam.frameBufferHeight3D = 1080;

	initparam.useFpsLimiter = false;
	initparam.useVSync = false;
	initparam.isWindowMode = true;

	//�V�F�[�_�[
	//���f�o�b�O�p
	initparam.isShaderPathReplaceForEngineFilePath = true;
	
	//�G���W��������
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Obosh", initparam);	
	
	//����\��
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);

	//�f�o�b�O�\���E���́A�펞�L����
	SetIsDebugDraw(true);
	SetIsDebugInput(true);
	
	World world;

	GameObj::PerspectiveCamera c;
	SetMainCamera( &c );
	
	GetMainCamera()->SetPos( { 0, 0, 2 } );
	GetMainCamera()->SetTarget( { 0, 0, 0 } );
	WasdCamera camera;

	RandomMapMaker* rMM = NewGO<RandomMapMaker>();
	rMM->SetWorld( &world );
	rMM->Awake();
	//�Q�[�����[�v�B
	GetEngine().RunGameLoop();
}