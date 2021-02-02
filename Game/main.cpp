#include "stdafx.h"
#include "resource.h"
#include "Title.h"

namespace {
	bool isStart = false;		//ゲームが実行されて一度だけ呼ばれる。
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	//エンジンの初期化設定
	InitEngineParameter initparam;	

	initparam.SDUnityChanScale = 2.5f;//スケール設定
	initparam.isAtmosphericFog = true;//大気フォグ有効化
	initparam.isPointFiltering = true;//テクスチャポイントフィルタリング有効化
	//initparam.isCreateMipmap = false; //ミップマップ生成無効化

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
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Minecraft2", initparam, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
	
	//判定表示
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);

	if (isStart == false)
	{
		NewGO<Title>();
		isStart = true;
	}

	//デバッグ表示・入力、常時有効化
	SetIsDebugDraw(true);
	SetIsDebugInput(false);//Escで終了しなくなります。

	//フォグの設定
	SetEnableFog(true);				//有効
	SetFogDistance(3000.0f);		//フォグが完全にかかりきる距離
	SetFogStartDistance(2800.0f);	//フォグが始まる距離
	SetFogHeightScale(0.1f);		//高さ方向へのフォグのかかり具合	

	//ゲームループ。
	GetEngine().RunGameLoop();
	return 0;
}