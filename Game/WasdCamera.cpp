#include "stdafx.h"
#include "WasdCamera.h"

static const float MOVE_SPEED = 7;
static const float EYE_SPEED = 10;

void WasdCamera::Update(){
	GameObj::ICamera* camera = GetMainCamera();

	const float deltaTime = GetRealDeltaTimeSec();

	//wasdキーによるカメラの移動。
	CVector3 front = camera->GetFront();
	front.y = 0;
	front.Normalize();
	front *= deltaTime * MOVE_SPEED;
	CVector3 left = camera->GetLeft() * deltaTime * MOVE_SPEED;
	CVector3 up = CVector3::Up() * deltaTime * MOVE_SPEED;

	CVector3 pos = camera->GetPos();
	{
		if( GetKeyInput( 'W' ) ){
			pos += front;
		}
		if( GetKeyInput( 'A' ) ){
			pos += left;
		}
		if( GetKeyInput( 'S' ) ){
			pos -= front;
		}
		if( GetKeyInput( 'D' ) ){
			pos -= left;
		}
		if( GetKeyInput( VK_SHIFT ) ){
			pos -= up;
		}
		if( GetKeyInput( VK_SPACE ) ){
			pos += up;
		}
	}
	camera->SetPos( pos );

	//マウスによる視点回転。
	if( GetKeyDown( VK_LBUTTON ) ){
		MouseCursor().SetLockMouseCursor( true );
	}
	if( GetKeyUp( VK_LBUTTON ) ){
		MouseCursor().SetLockMouseCursor( false );
	}

	if( GetKeyInput( VK_LBUTTON ) ){

		CVector2 mouse = MouseCursor().GetMouseMove();
		CQuaternion rotY;
		rotY.SetRotationDeg( CVector3::AxisY(), mouse.x * deltaTime * EYE_SPEED );
		CQuaternion rotX;
		rotX.SetRotationDeg( camera->GetLeft(), -mouse.y * deltaTime * EYE_SPEED );

		rotY.Multiply( rotX );

		rotY.Multiply( m_lookVec );
	}

	camera->SetTarget( pos + m_lookVec );
}
