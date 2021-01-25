#include "stdafx.h"
#include "Sun.h"
#include "BlockFactory.h"

void Sun::Update() {
	//時間経過
	CQuaternion rot(CVector3::AxisX(), 0.05f * GetDeltaTimeSec());
	rot.Multiply(m_direction);

	//ライト方向
	m_light.SetDirection(m_direction);
	//ライト色
	CVector3 color = CVector3::One() * -m_light.GetDirection().y;
	color.Max(0.0f);
	m_light.SetColor(color);

	//スカイライトの影響力を設定
	BlockFactory::FindBlockModel(
		[&](GameObj::InstancingModel* model) {
			model->GetModelRender().GetSkinModel().Set_t(max(0.0f, -m_light.GetDirection().y));
		}
	);
}