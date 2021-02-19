#include "stdafx.h"
#include "Sun.h"
#include "BlockFactory.h"

namespace {
	constexpr int _20minSec = 1200;//20分(一日)は1200秒
	constexpr int _1daySec = 86400;//現実時間の一日は86400秒
	constexpr float dayScale = _1daySec / _20minSec;//マイクラの一日を現実の一日に変換する倍率
}

void Sun::Update() {
	//時間経過
	m_seconds += dayScale * GetDeltaTimeSec() * m_timeScale;
	if (m_seconds > _1daySec) {
		m_seconds -= _1daySec;
	}
	CQuaternion rot(CVector3::AxisX(), CMath::PI2 * (m_seconds / _1daySec));
	CVector3 direction = { 0.0f,1.0f,0.0f };
	rot.Multiply(direction);
	m_direction = direction;

	//ライト方向
	m_light.SetDirection(m_direction);
	//ライト色
	CVector3 color = CVector3::One() * -m_light.GetDirection().y;
	color.Max(0.0f);
	m_light.SetColor(color);

	//スカイライトの影響力を設定
	BlockFactory::FindBlockModel(
		[&](GameObj::InstancingModel* model) {
			model->GetModelRender().GetSkinModel().Set_t(GetSkyLightPower());
		}
	);

	//デバッグ操作
	DebugInput();
}

void Sun::PostRender() {
	CFont font;
	std::wstringstream str;
	int h, m, s;
	h = ((int)m_seconds) / 60 / 60;
	m = ((int)m_seconds) / 60 % 60;
	s = ((int)m_seconds) % 60 % 60;
	str << std::setw(2) << h << ":" << std::setw(2) << m << ":" << std::setw(2) << s << "\n";
	font.Draw(str.str().c_str(), { 0.97f , 0.2f }, CVector4::White(), 0.5f, { 1.0f, 0.5f });
}

void Sun::DebugInput() {
	if (GetKeyInput(VK_CONTROL)) {
		if (GetKeyDown(VK_F8)) {
			//30minすすめる
			m_seconds += 30 * 60;
		}
		if (GetKeyDown(VK_F9)) {
			//30min戻る
			m_seconds -= 30 * 60;
		}
	}
	else {
		if (GetKeyDown(VK_F8)) {
			//よる
			m_seconds = 0;
		}
		if (GetKeyDown(VK_F9)) {
			//ひる
			m_seconds = _1daySec / 2;
		}
	}
}