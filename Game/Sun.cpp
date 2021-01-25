#include "stdafx.h"
#include "Sun.h"
#include "BlockFactory.h"

void Sun::Update() {
	//���Ԍo��
	CQuaternion rot(CVector3::AxisX(), 0.05f * GetDeltaTimeSec());
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