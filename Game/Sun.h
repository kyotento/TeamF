#pragma once

/// <summary>
/// ���z
/// </summary>
class Sun : public IGameObject {
public:
	Sun() {
		std::ifstream file("Save/Time/time.time", std::ios::binary);
		if (!file) {
			return;
		}
		file.read(reinterpret_cast<char*>(&m_seconds), sizeof(m_seconds));
	}
	~Sun() {
		std::filesystem::path path("./Save/Time/time.time");
		//�f�B���N�g���������ꍇ�ɍ쐬����B
		std::filesystem::create_directories(path.parent_path());
		//�Z�[�u
		std::ofstream file(path.string() , std::ios::binary);
		file.write(reinterpret_cast<const char*>(&m_seconds), sizeof(m_seconds));
	}

private:
	void Update() override;
	void PostRender() override;

	void DebugInput();

	CVector3 m_direction = {0.0f,0.0f,1.0f};
	GameObj::CDirectionLight m_light;
	float m_timeScale = 1.0f;
	float m_seconds = 3600.0f * 12.0f;

public:
	/// <summary>
	/// ���Ԃ̐i�ޑ��x��ݒ�
	/// </summary>
	/// <param name="timeScale"></param>
	void SetTimeScale(float timeScale = 1.0f) {
		m_timeScale = timeScale;
	}
	float GetTimeScale()const {
		return m_timeScale;
	}

	/// <summary>
	/// ���ݎ�����b�Ŏ擾
	/// </summary>
	/// <returns></returns>
	float GetSecond()const {
		return m_seconds;
	}
	void SetSecond(float sec) {
		m_seconds = sec;
	}

	/// <summary>
	/// �X�J�C���C�g�̋������擾
	/// </summary>
	/// <returns></returns>
	float GetSkyLightPower()const {
		return max(0.05f, -m_light.GetDirection().y);
	}
};
	