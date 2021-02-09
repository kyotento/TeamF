#pragma once

class Player;
class ItemDisplay;
class PlayerParameter : public IGameObject
{
public:
	PlayerParameter();
	~PlayerParameter();

	bool Start() override;
	void Update() override;
	void PostRender() override;

	/// <summary>
	/// �v���C���[�̃C���X�^���X��ݒ肷��B
	/// </summary>
	/// <param name="pl">�v���C���[�̃C���X�^���X</param>
	void SetPlayerIns(Player* pl)
	{
		m_player = pl;
	}

	/// <summary>
	/// �I������Ă���A�C�e���ԍ����擾����B
	/// </summary>
	/// <returns>�I������Ă���ԍ�</returns>
	int GetSelectItem()
	{
		return m_selectNum;
	}
private:

	/// <summary>
	/// �p�����[�^�摜�̊�Ղ𐶐��B
	/// </summary>
	void SetParamFound();

	/// <summary>
	/// �̗͂�ύX����B
	/// </summary>
	void ChangeHP();

	/// <summary>
	/// �X�^�~�i��ύX����B
	/// </summary>
	void ChangeStamina();

	/// <summary>
	/// �h���ύX����B
	/// </summary>
	void ChangeArmor();

	/// <summary>
	/// �o���l�Q�[�W��ύX����B
	/// </summary>
	void ChangeExp();

	/// <summary>
	/// �莝���̃A�C�e�����w�肷��B
	/// </summary>
	void SelectItem();

	/// <summary>
	//todo  1~9�{�^���ɂ��A�C�e���Z���N�g(���艟���̋ɂ�)�B
	/// </summary>
	void KariItemS();

	/// <summary>
	/// �E��̐����B
	/// </summary>
	void InitRightHand();

	/// <summary>
	/// �A�C�e�����C���x���g���[�Ő؂�ւ�������̏����B
	/// </summary>
	void StopMoveToChange();

	void PlayerLevelUp();
	static const int m_paramNum = 10;				//�p�����[�^�[�̉摜�̐��B
	int m_oldHP = 0;								//�v���C���[��1�t���[���O��HP���i�[�B
	int m_oldStamina = 0;							//�v���C���[��1�t���[���O�̃X�^�~�i���i�[�B
	int m_oldArmor = 0;								//�v���C���[��1�t���[���O�̖h��͂��i�[�B
	int m_selectNum = 1;							//���Ԗڂ̃A�C�e�����Z���N�g���Ă��邩 1~9(�v���C���[�i�[�p)�B
	int m_selectNumOld = 0;							//�P�t���[���O�́��B
	int m_idOld = 0;								//1�t���[���O��ID

	const float m_scale = 3.f;						//HP�ƃX�^�~�i�̃X�P�[���B
	const float m_selectPosX = 0.260f;				//�A�C�e���Z���N�g0�Ԗڂ�X���W�B
	const float m_expScale = 1.5f;					//�o���l�n�摜�̃X�P�[���B

	bool m_isItemChangeFlag = false;				//�����Ă���̐؂�ւ������ǂ����B

	bool m_isStopMoveToChangeFlag = false;			//�C���x���g���[�J���Ă�Ƃ��Ɏ��������؂�ւ�������B
	CVector2 m_hpPosition[m_paramNum] = { CVector2::Zero() };		//HP�摜�̍��W�B
	CVector2 m_staminaPosition[m_paramNum] = { CVector2::Zero() };	//�X�^�~�i�摜�̍��W�B
	CVector2 m_armorPos[m_paramNum] = { CVector2::Zero() };			//�h��͉摜�̍��W�B
	CVector2 m_sItemPos = CVector2::Zero();							//�A�C�e���Z���N�g���W�B

	GameObj::CSpriteRender m_spriteRenderHP[m_paramNum];			//HP�摜�B
	GameObj::CSpriteRender m_spriteRenderStamina[m_paramNum];		//�X�^�~�i�摜�B
	GameObj::CSpriteRender m_spriteRenderArmor[m_paramNum];			//�h��͉摜�B
	GameObj::CSpriteRender m_spriteRenderOnHand;					//�莝���A�C�e���摜��ՁB
	GameObj::CSpriteRender m_spriteRenderExp;						//�o���l�B
	GameObj::CSpriteRender m_spriteRenderExpGauge;					//�o���l(�Q�[�W�̑���)�B
	GameObj::CSpriteRender m_spriteRenderSelectItem;				//�x�̃A�C�e����I�����Ă��邩�B

	Player* m_player = nullptr;			//�v���C���[�B
	CFont m_font;						//�t�H���g�B

	ItemDisplay* m_rightHandDisplay = nullptr;					//�E��\���B
};

