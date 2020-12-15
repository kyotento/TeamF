#pragma once

/// <summary>
/// �A�C�e���f�B�X�v���C�N���X
/// </summary>
/// �A�C�e����\������N���X�ł���˂���
/// 
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
	/// <summary>
	/// ���ɍ��킹�ď㉺������悤
	/// <summary>
	void SetHeadRot(const CQuaternion& h_rot)
	{
		m_headRot = h_rot;
	}
private:

	enum EnType
	{
		enHand,
		enBlock,
		enTool
	};

	/// <summary>
	/// ���f���̐����B
	/// </summary>
	void InitModel();
	/// <summary>
	/// �Ǐ]
	/// </summary>
	void Follow();
	/// ���f���̐؂�ւ�
	/// </summary>
	void Switching();
	/// <summary>
	/// �A�C�e�����΂ߏ�ɕ\�����邽�߂ɁB
	/// </summary>
	void Rotation();
	/// <summary>
	/// �J�����̃��[�h�ɍ��킹�ĕ\�������邽�߂̊֐��B
	/// </summary>
	void CameraModeChangeToDisplay();
	/// <summary>
	/// �؂�ւ����Ƃ��ɍēx�����B
	/// </summary>
	void BuildAgain();
	/// <summary>
	/// ��̉�]����
	/// </summary>
	void HandRotation();
	/// <summary>
	/// �u���b�N�n�̉�]����
	/// </summary>
	void BlockRotation();
	/// <summary>
	/// �c�[���n�̉�]����
	/// </summary>
	void ToolRotation();
private:
	GameObj::CSkinModelRender* m_skinModelRender = nullptr;	//���f���B
	CVector3 m_position = CVector3::Zero();					//���f����\��������W�B
	CVector3 m_forward = CVector3::Zero();					//�O���B
	CVector3 m_scale = CVector3::One();						//�傫���B

	float m_radianY = 0.0f;									//Y���̉�](���W�A��)
	float m_radianXZ = 0.0f;								//XZ���̉�](���W�A��)

	int m_cameraDisplayMode = 0;							//�J�����̃��[�h�B

	CQuaternion m_rotation = CQuaternion::Identity();		//���f������]������B
	CQuaternion m_headRot = CQuaternion::Identity();
	Player* m_player = nullptr;								//�v���C���[�̃C���X�^���X�B
	GameCamera* m_gameCamera = nullptr;						//�Q�[���J�����C�F�A�B

	EnType type;											//�^�C�v
};

