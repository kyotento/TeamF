#pragma once

class Player;
class World;
/// <summary>
/// �]���r�𐶐����Ă����N���X�B
/// �]���r�̓u���b�N�̂���ꏊ�ɂ�����������܂���B
/// </summary>
class ZombieGenerator : public IGameObject
{
public:
	bool Start() override;
	void Update() override;
	/// <summary>
	/// ���[���h���Z�b�g����B
	/// </summary>
	/// <param name="world">���[���h�B</param>
	void SetWorld(World* world)
	{
		m_world = world;
	}
private:
	Player* GetPlayer()
	{
		if (m_player == nullptr)
		{
			m_player = FindGO<Player>(L"player");
		}
		return m_player;
	}
	/// <summary>
	/// �]���r�𐶐�����B
	/// </summary>
	void GenerateZombie();
	/// <summary>
	/// �]���r�������ł����Ԃ����邩�ǂ����������B
	/// </summary>
	/// <param name="pos">�]���r�𐶐��\��̍��W�B</param>
	void SearchSpaceZombieGenerate(CVector3& pos);
	/// <summary>
	/// �]���r������͈͓̔��ɂǂꂾ�����݂��Ă��邩���ׂ�B
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W�B</param>
	void SearchNumberZombieRange(CVector3& playerPos);
private:
	Player* m_player = nullptr;			//�v���C���[�B
	World* m_world;				//���[���h�B
	float m_timer = 0.0f;			//�^�C�}�[�B
	int m_numberZombieRange = 0;		//�͈͓��ɑ��݂��Ă���]���r�B
};

