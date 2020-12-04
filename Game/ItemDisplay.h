#pragma once

class Player;
class GameCamera;
class ItemDisplay : public IGameObject
{
public:
	ItemDisplay();
	~ItemDisplay();
	bool Start()override;
	void Update()override;
	/// <summary>
	/// �v���C���[������W�����炤�B
	/// </summary>
	/// <param name="selItem">�v���C���[�̃|�W�V�����B</param>
	void SetPos(const CVector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// �v���C���[������������炤�����B
	/// </summary>
	/// <param name="selItem">�v���C���[�̉�]�B</param>
	void SetRot(const CQuaternion& rot)
	{
		m_rotation = rot;
	}
private:
	/// <summary>
	/// ���f���̐����B
	/// </summary>
	void InitModel();
	/// <summary>
	/// �Ǐ]
	/// </summary>
	void Follow();
	/// <summary>
	/// ���f���̐؂�ւ�
	/// </summary>
	void Switching();
	/// <summary>
	/// �A�C�e�����΂ߏ�ɕ\�����邽�߂ɁB
	/// </summary>
	void Rotation();
private:
	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//���f���B
	CVector3 m_position = CVector3::Zero();						//���f����\��������W�B
	CVector3 m_forward = CVector3::Zero();						//�O���B

	CQuaternion m_rotation = CQuaternion::Identity();			//���f������]������B
	Player* m_player = nullptr;									//�v���C���[�̃C���X�^���X�B
	GameCamera* m_gameCamera;									//�Q�[���J�����C�F�A�B
};

