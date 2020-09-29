#include "stdafx.h"
#include "RandomMapMaker.h"
//#include <charconv>
//#include "DemolisherWeapon/GraphicsAPI/DirectX12/DX12Test.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	//エンジンの初期化設定
	InitEngineParameter initparam;
	
	//TODO フルスク対応
	//initparam.windowStyle = WS_POPUP | WS_BORDER ;		//ウィンドウスタイル

	//解像度
	//initparam.screenWidth = initparam.frameBufferWidth = initparam.frameBufferWidth3D = 1920;
	//initparam.screenHeight = initparam.frameBufferHeight = initparam.frameBufferHeight3D = 1080;

	initparam.useFpsLimiter = false;
	initparam.useVSync = false;
	initparam.isWindowMode = true;

	//シェーダー
	//※デバッグ用
	initparam.isShaderPathReplaceForEngineFilePath = true;
	
	//エンジン初期化
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Obosh", initparam);	
	
	//判定表示
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);

	//デバッグ表示・入力、常時有効化
	SetIsDebugDraw(true);
	SetIsDebugInput(true);

	RandomMapMaker* rMM = NewGO<RandomMapMaker>();
	rMM->Awake();
	//ゲームループ。
	GetEngine().RunGameLoop();
}