#include "stdafx.h"
#include "Sun.h"
#include "BlockFactory.h"

namespace {
	constexpr int _20minSec = 1200;//20��(���)��1200�b
}
void Sun::Update() {
	//���Ԍo��
	CQuaternion rot(CVector3::AxisX(), CMath::PI2 / _20minSec * GetDeltaTimeSec() * m_timeScale);
	rot.Multiply(m_direction);

	//���C�g����
	m_light.SetDirection(m_direction);
	//���C�g�F
	CVector3 color = CVector3::One() * -m_light.GetDirection().y;
	color.Max(0.0f);
	m_light.SetColor(color);

	//�X�J�C���C�g�̉e���͂�ݒ�
	BlockFactory::FindBlockModel(
		[&](GameObj::InstancingModel* model) {
			model->GetModelRender().GetSkinModel().Set_t(max(0.0f, -m_light.GetDirection().y));
		}
	);
}