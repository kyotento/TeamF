#include "stdafx.h"
#include "WasdCamera.h"

void WasdCamera::Update(){
	GameObj::ICamera* camera = GetMainCamera();

	const float speed = 10;
	CVector3 front = camera->GetFront() * GetRealDeltaTimeSec() * speed;
	CVector3 left = camera->GetLeft() * GetRealDeltaTimeSec() * speed;
	CVector3 pos = camera->GetPos();

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

	camera->SetPos( pos );

	CVector2 mouse = MouseCursor().GetMouseMove();
	CQuaternion rotY;
	rotY.SetRotationDeg( CVector3::AxisY(), mouse.x );
	CQuaternion rotX;
	rotX.SetRotationDeg( left, -mouse.y );

	rotY.Multiply( rotX );

	rotY.Multiply( m_lookVec );

	camera->SetTarget( pos + m_lookVec );
}
