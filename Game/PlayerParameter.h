#pragma once

class Player;
class PlayerParameter : public IGameObject
{
public:
	PlayerParameter();
	~PlayerParameter();

	bool Start() override;
	void Update() override;

	/// <summary>
	/// �v���C���[�̃C���X�^���X��ݒ肷��B
	/// </summary>
	/// <param name="pl">�v���C���[�̃C���X�^���X</param>
	void SetPlayerIns(Player* pl)
	{
		m_player = pl;
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

	static const int m_paramNum = 10;				//�p�����[�^�[�̉摜�̐��B
	static const int m_hpStaminaLimit = 20;			//todo Player�N���X�Ɉړ�������\����I HP�ƃX�^�~�i�̏���B
	int m_oldHP = 0;								//�v���C���[��1�t���[���O��HP���i�[�B
	int m_oldStamina = 0;							//�v���C���[��1�t���[���O�̃X�^�~�i���i�[�B

	const float m_scale = 3.f;						//HP�ƃX�^�~�i�̃X�P�[���B

	CVector2 m_hpPosition[m_paramNum] = { CVector2::Zero() };		//HP�摜�̍��W�B
	CVector2 m_staminaPosition[m_paramNum] = { CVector2::Zero() };	//�X�^�~�i�摜�̍��W�B

	CSpriteRender m_spriteRenderHP[m_paramNum];				//HP�摜�B
	CSpriteRender m_spriteRenderStamina[m_paramNum];		//�X�^�~�i�摜�B
	CSpriteRender* m_spriteRenderOnHand = nullptr;			//�莝���A�C�e���摜��ՁB

	Player* m_player = nullptr;			//�v���C���[�B
};

