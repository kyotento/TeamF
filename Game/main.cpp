#include "stdafx.h"
#include "resource.h"
#include "Title.h"

namespace {
	bool isStart = false;		//�Q�[�������s����Ĉ�x�����Ă΂��B
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	//�G���W���̏������ݒ�
	InitEngineParameter initparam;	

	initparam.SDUnityChanScale = 2.5f;//�X�P�[���ݒ�
	initparam.isAtmosphericFog = true;//��C�t�H�O�L����
	initparam.isPointFiltering = true;//�e�N�X�`���|�C���g�t�B���^�����O�L����
	//initparam.isCreateMipmap = false; //�~�b�v�}�b�v����������

	//�𑜓x
	//initparam.screenWidth = initparam.frameBufferWidth = initparam.frameBufferWidth3D = 1920;
	//initparam.screenHeight = initparam.frameBufferHeight = initparam.frameBufferHeight3D = 1080;

	//initparam.useFpsLimiter = false;//�t���[�����[�g�������
	initparam.limitFps = 60;		//�t���[�����[�g���
	initparam.standardFps = 60;		//����t���[�����[�g�����ꂢ����ƃQ�[���X�s�[�h���ς��(�������s�񐔂�������)

	initparam.useVSync = false;
	initparam.isWindowMode = true;
	//initparam.windowStyle = WS_POPUP | WS_BORDER ;		//�E�B���h�E�X�^�C��

	//�V�F�[�_�[
	//���f�o�b�O�p
	initparam.isShaderPathReplaceForEngineFilePath = true;
	
	//�G���W��������
	GetEngine().InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, "Minecraft2", initparam, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)));
	
	//����\��
	//SetPhysicsDebugDrawMode(btIDebugDraw::DBG_DrawWireframe);

	if (isStart == false)
	{
		NewGO<Title>();
		isStart = true;
	}

	//�f�o�b�O�\���E���́A�펞�L����
	SetIsDebugDraw(true);
	SetIsDebugInput(false);//Esc�ŏI�����Ȃ��Ȃ�܂��B

	//�t�H�O�̐ݒ�
	SetEnableFog(true);				//�L��
	SetFogDistance(3000.0f);		//�t�H�O�����S�ɂ����肫�鋗��
	SetFogStartDistance(2800.0f);	//�t�H�O���n�܂鋗��
	SetFogHeightScale(0.1f);		//���������ւ̃t�H�O�̂�����	

	//�Q�[�����[�v�B
	GetEngine().RunGameLoop();
	return 0;
}