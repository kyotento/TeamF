#pragma once

class Player;
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

	static const int m_paramNum = 10;				//�p�����[�^�[�̉摜�̐��B
	int m_oldHP = 0;								//�v���C���[��1�t���[���O��HP���i�[�B
	int m_oldStamina = 0;							//�v���C���[��1�t���[���O�̃X�^�~�i���i�[�B
	int m_oldArmor = 0;								//�v���C���[�̂P�t���[���O�̖h��͂��i�[�B

	const float m_scale = 3.f;						//HP�ƃX�^�~�i�̃X�P�[���B

	CVector2 m_hpPosition[m_paramNum] = { CVector2::Zero() };		//HP�摜�̍��W�B
	CVector2 m_staminaPosition[m_paramNum] = { CVector2::Zero() };	//�X�^�~�i�摜�̍��W�B
	CVector2 m_armorPos[m_paramNum] = { CVector2::Zero() };			//�h��͉摜�̍��W�B

	CSpriteRender m_spriteRenderHP[m_paramNum];				//HP�摜�B
	CSpriteRender m_spriteRenderStamina[m_paramNum];		//�X�^�~�i�摜�B
	CSpriteRender m_spriteRenderArmor[m_paramNum];			//�h��͉摜�B
	CSpriteRender* m_spriteRenderOnHand = nullptr;			//�莝���A�C�e���摜��ՁB
	CSpriteRender* m_spriteRenderExp = nullptr;				//�o���l�B

	Player* m_player = nullptr;			//�v���C���[�B
	CFont m_font;						//�t�H���g�B
};

