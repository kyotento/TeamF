#include "stdafx.h"
#include "RandomMapMaker.h"
#include "Game.h"
//#include <charconv>
//#include "DemolisherWeapon/GraphicsAPI/DirectX12/DX12Test.h"
#include "WasdCamera.h"
#include "World.h"

namespace {
	bool isStart = false;		//ゲームが実行されて一度だけ呼ばれる。
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	if (isStart == false)
	{
		NewGO<Game>();
		isStart = true;
	}

	//エンジンの初期化設定
	InitEngineParameter initparam;	

	//解像度
	//initparam.screenWidth = initparam.frameBufferWidth = initparam.frameBufferWidth3D = 1920;
	//initparam.screenHeight = initparam.frameBufferHeight = initparam.frameBufferHeight3D = 1080;

	//initparam.useFpsLimiter = false;//フレームレート上限無効
	initparam.limitFps = 60;		//フレームレート上限
	initparam.standardFps = 60;		//動作フレームレート※これいじるとゲームスピードが変わる(処理実行回数が増える)

	initparam.useVSync = false;
	initparam.isWindowMode = true;
	//initparam.windowStyle = WS_POPUP | WS_BORDER ;		//ウィンドウスタイル

	//シェーダー
	//※デバッグ用
	initparam.isShaderPathReplaceForEngineFilePath = true;
	
	//エンジン初期化
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Minecraft2", initparam);	
	
	//判定表示
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);

	//デバッグ表示・入力、常時有効化
	SetIsDebugDraw(true);
	SetIsDebugInput(true);
	
	World world;

	//GameObj::PerspectiveCamera c;
	//SetMainCamera( &c );
	
	GetMainCamera()->SetPos( { 0, 0, 2 } );
	GetMainCamera()->SetTarget( { 0, 0, 0 } );
	//WasdCamera camera;

	RandomMapMaker* rMM = NewGO<RandomMapMaker>();
	rMM->SetWorld( &world );
	rMM->Awake();
	MouseCursor().SetLockMouseCursor(true);			//マウスカーソルを中央に固定。
	//ゲームループ。
	GetEngine().RunGameLoop();
}