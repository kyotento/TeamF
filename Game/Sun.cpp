#include "stdafx.h"
#include "Sun.h"
#include "BlockFactory.h"

namespace {
	constexpr int _20minSec = 1200;//20分(一日)は1200秒
}
void Sun::Update() {
	//時間経過
	CQuaternion rot(CVector3::AxisX(), CMath::PI2 / _20minSec * GetDeltaTimeSec() * m_timeScale);
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