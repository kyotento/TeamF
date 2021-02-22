#pragma once
#include "MobGenerator.h"

class Sun;

class BossGenerator : public MobGenerator
{
public:
	BossGenerator() {
		std::ifstream file("Save/Time/boss.time", std::ios::binary);
		if (!file) {
			return;
		}
		file.read(reinterpret_cast<char*>(&m_runCnt), sizeof(m_runCnt));
	}
	~BossGenerator() {
		std::filesystem::path path("./Save/Time/boss.time");
		//ディレクトリが無い場合に作成する。
		std::filesystem::create_directories(path.parent_path());
		//セーブ
		std::ofstream file(path.string(), std::ios::binary);
		file.write(reinterpret_cast<const char*>(&m_runCnt), sizeof(m_runCnt));
	}

	void Init() override;
	void NewGOMob(const CVector3& pos) override;
	void PreUpdate()override;
private:
	Sun* m_sun = nullptr;
	int m_runCnt = 1;//試行回数
	float m_bossTimerSec = 0.0f;
};
