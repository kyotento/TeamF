#pragma once

class PlayerArmor : public IGameObject
{
public:
	PlayerArmor();
	~PlayerArmor();

	bool Start()override;
	void Update()override;

	/// <summary>
	/// �e���f���̏������B
	/// </summary>
	void InitModel();

	/// <summary>
	/// �����擾�B
	/// </summary>
	void GetBoneInfor();

	/// <summary>
	/// �A�[�}�[�̍��W�Ɖ�]�ݒ�B
	/// </summary>
	void SetArmorPos();

	/// <summary>
	/// �v���C���[�̃X�L�����f����ݒ肷��B
	/// </summary>
	/// <param name="skin">SkinModel</param>
	void SetPlayerSkinModel(GameObj::CSkinModelRender* skin) {
		m_playerSkinModel = skin;
	}

private:

	static const int m_armorNum = 8;
	
	float m_scale = 1.5f;											//�X�P�[���B
	CVector3 m_position[m_armorNum] = { CVector3::Zero() };			//���W�B
	CQuaternion m_rot[m_armorNum] = { CQuaternion::Identity() };	//��]�B

	GameObj::CSkinModelRender* m_playerSkinModel = nullptr;				//�v���C���[��SkinModel�B

	GameObj::CSkinModelRender* m_skinModelArmor[m_armorNum] = { nullptr };		//�h��̃��f���B

	Bone* m_bone[m_armorNum] = { nullptr };				//���B

	/// <summary>
	/// �A�[�}�[�ԍ��ڍׁB
	/// </summary>
	//0		�w�����b�g�B
	//1		���̃`�F�X�g�v���[�g�B
	//2		�E��`�F�X�g�v���[�g�B
	//3		����`�F�X�g�v���[�g�B
	//4		�E���M���X�B
	//5		�����M���X�B
	//6		�E�u�[�c�B
	//7		���u�[�c�B
};

