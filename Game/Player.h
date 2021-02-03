#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"
#include "Entity.h"
#include "Inventory.h"
#include "MCCharaCon.h"

class World;
class GameCamera;
class Item;
class GameMode;
class ItemDisplay;
class PlayerParameter;
class PlayerDeath;
class Game;
namespace GUI{
	class RootNode;
}

class Player : public Entity
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;

	void Test();//todo�@�����[�X�̂Ƃ��ɏ����܂��B

	CFont font;
	void HUDRender( int HUDNum ) override{
		std::wstringstream str;
		CVector3 pos = GetPos() / Block::WIDTH;
		str << pos.x << " , " << pos.y << " , " << pos.z << "\n";
		font.Draw( str.str().c_str(), { 0.9f , 0.1f }, CVector4::White(), 0.5f, {0.5f, 0.5f} );
	}

	/// <summary>
	/// �A�j���[�V�����B
	/// </summary>
	enum enAnimationClip{
		enAnimationClip_Idle,			//�ҋ@��ԁB
		enAnimationClip_move,			//�ړ��B
		enAnimationClip_excavate,		//�����@��B
		enAnimationClip_Num,			//�A�j���[�V�����̐��B
	};

	AnimationClip m_animationClip[enAnimationClip_Num];			//�A�j���[�V�����̐����i�[�B

	/// <summary>
	/// �v���C���[�̏�ԁB
	/// </summary>
	enum enPlayerState {
		enPlayerState_idle,				//�ҋ@�B	
		enPlayerState_move,				//�ړ��B
		enPlayerState_run,				//�����Ă���Ƃ��B
		enPlayerState_excavate,			//�����@��B
		enPlayerState_KnockBack,			//�m�b�N�o�b�N�B
		enPlayerState_death,			//���񂾂Ƃ��B
		enPlayerState_num,				//��Ԃ̐��B
	};

	/// <summary>
	/// �v���C���[�̏�Ԃ��擾�B
	/// </summary>
	/// <returns>�v���C���[�̏��</returns>
	enPlayerState GetplayerState()const {
		return m_playerState;
	}

	/// <summary>
	/// �v���C���[�����Ⴊ��ł��邩�擾�B
	/// </summary>
	/// <returns></returns>
	bool GetIsSneaking()const {
		return m_characon.IsShiftMove();
	}

	/// <summary>
	/// �v���C���[�̉E�������擾�B
	/// </summary>
	/// <returns>�v���C���[�̉E����</returns>
	const CVector3& GetRight()
	{
		return m_right;
	}

	/// <summary>
	/// �v���C���[�̐��ʕ������擾�B
	/// </summary>
	/// <returns>�v���C���[�̐��ʕ���</returns>
	const CVector3& GetFront()
	{
		return m_front;
	}
	
	//! @brief ���W���擾�B
	CVector3 GetPos() const override{
		return m_position;
	}

	//! @brief ���f�����W���擾�B
	const CVector3& GetModelPos() const {
		if (!m_skinModelRender) {
			return CVector3::Zero();
		}
		return m_skinModelRender->GetPos();
	}
	/// <summary>
	/// �E��̍��W���擾�B
	/// </summary>
	/// <returns>�E��̃{�[�����W�B</returns>
	const CVector3& GetRightHandPos()  const
	{
		if (!m_rightHandBone)
		{
			return CVector3::Zero();
		}
		return m_rightHandBone->GetPosition();
	}
	//! @brief ��]���擾�B
	CQuaternion GetRot() const {
		return m_rotation;
	}

	//! @brief ���W��ݒ�B
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
	}

	/// <summary>
	/// HP���擾����B
	/// </summary>
	/// <returns>HP</returns>
	const float& GetHP()
	{
		return m_hp;
	}

	/// <summary>
	/// �X�^�~�i���擾����B
	/// </summary>
	/// <returns>�X�^�~�i</returns>
	const float& GetStamina()
	{
		return m_stamina;
	}

	/// <summary>
	/// �h��͂��擾�B
	/// </summary>
	/// <returns>�h���</returns>
	const int& GetDefPow()
	{
		return m_defensePower;
	}

	/// <summary>
	/// �o���l���擾����B
	/// </summary>
	/// <returns>�o���l</returns>
	const float& GetExp()
	{
		return m_exp;
	}

	/// <summary>
	/// �I������A�C�e�����w��B
	/// </summary>
	/// <param name="selItem">�I�������A�C�e���ԍ�</param>
	void SetSelectItemNum(int selItem)
	{
		m_selItemNum = selItem;
	}

	/// <summary>
	/// �I������Ă���A�C�e���ԍ����擾�B
	/// </summary>
	/// <returns>�I�𒆂̃A�C�e���ԍ�</returns>
	const int& GetSelectItemNum()
	{
		return m_selItemNum;
	}

	/// <summary>
	/// �v���C���[��Y���̉�]���擾�B
	/// </summary>
	/// <returns>�v���C���[��Y���̉�]</returns>
	float GetRadianY()
	{
		return m_radianY;
	}

	/// <summary>
	/// �v���C���[��XZ���̉�]���擾�B
	/// </summary>
	/// <returns>�v���C���[��XZ���̉�]</returns>
	float GetRadianXZ()
	{
		return m_radianXZ;
	}

	World* GetWorld(){
		return m_world;
	}

	//�C���x���g���̒���
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;

	Inventory& GetInventory(){
		return m_inventory;
	}

	/// <summary>
	/// GUI���J������B
	/// </summary>
	/// <param name="gui">�J������GUI</param>
	void OpenGUI( std::unique_ptr<GUI::RootNode>&& gui );

	/// <summary>
	/// GUI���������B
	/// </summary>
	void CloseGUI();

	/// <summary>
	/// ��_���[�W
	/// </summary>
	/// <param name="AttackePow">�U����</param>
	void TakenDamage(int AttackePow, CVector3 knockBackDirection = CVector3::Zero(),bool isAttacked = false);

	/// <summary>
	/// �Q�[���̃C���X�^���X��ݒ肷��B
	/// </summary>
	/// <param name="game"></param>
	void SetGameIns(Game* game)
	{
		m_game = game;
	}
	/// <summary>
	/// �v���C���[����ł�H�H
	/// </summary>
	/// <returns>�t���O</returns>
	bool GetIsPlayerDead()
	{
		return m_deathFlag;
	}
	/// <summary>
	/// �u���b�N�j��������H
	/// </summary>
	/// <returns>�t���O</returns>
	bool GetIsBlockDestruction()
	{
		return m_isBlockDestruction;
	}
	/// <summary>
	/// �v���C���[������ł邩�ǂ����擾�B
	/// </summary>
	/// <returns>�t���O</returns>
	bool GetIsDeath()
	{
		return m_playerState == enPlayerState_death;
	}
	/// <summary>
	/// �H�ׂĂ邩�B
	/// </summary>
	/// <returns>�t���O</returns>
	bool GetIsEating()
	{
		return m_eatingFlag;
	}
private:
	/// <summary>
	/// �L�[�{�[�h�̓��͏��Ǘ��B
	/// </summary>
	void KeyBoardInput();

	/// <summary>
	/// �ړ������B
	/// </summary>
	void Move();

	/// <summary>
	/// ���鏈���B
	/// </summary>
	void Dash();

	/// <summary>
	/// �W�����v�����B
	/// </summary>
	void Jump();

	/// <summary>
	/// ��]�����B
	/// </summary>
	void Turn();

	/// <summary>
	/// ���Ⴊ�݂̏����B
	/// </summary>
	void Shift();

	/// <summary>
	/// ���̉�]�����B
	/// </summary>
	void Headbang();

	/// <summary>
	/// �U�������B
	/// </summary>
	void Attack();
	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void KnockBack();

	/// <summary>
	/// �C���x���g�����J���B
	/// </summary>
	void OpenInventory();

	/// <summary>
	/// �ړ����@�̐؂�ւ�(�N���G�C�e�B�u)�B
	/// </summary>
	void ChangeMovemontC();

	/// <summary>
	/// �v���C���[�̏�ԊǗ��B
	/// </summary>
	void StateManagement();

	/// <summary>
	/// �v���C���[�̐ݒu�Ɣj��B
	/// </summary>
	/// <param name="ray">���������I�u�W�F�N�g�̔���</param>
	/// <param name="frontRotAdd">�v���C���[�̉�]</param>
	void InstallAndDestruct(btCollisionWorld::ClosestRayResultCallback ray , CVector3 frontRotAdd);
	/// <summary>
	/// �u���b�N��j�󂷂邩�ǂ������f����B
	/// </summary>
	void DecideCanDestroyBlock();

	/// <summary>
	/// �v���C���[�̑O���Ƀ��C���΂��B
	/// </summary>
	void FlyTheRay();
	/// <summary>
	/// ���S�����B
	/// </summary>
	void Death();

	/// <summary>
	/// ���X�|�[���B
	/// </summary>
	void Respawn();

	/// <summary>
	/// ���f���̕`������邩�B
	/// </summary>
	void IsDraw();

	void Stamina();

	void Shoulder();
	/// <summary>
	/// �X�y�[�X���_�u���N���b�N�������ǂ����B
	/// </summary>
	/// <returns>doubleClickFlag</returns>
	bool SpaceDoubleClick();

	bool m_isJump = false;					//���ł��邩�B
	bool m_doubleCilckFlag = false;			//�_�u���N���b�N�t���O�B
	bool m_runFlag = false;					//�����Ă��邩�B
	bool m_flyingMode = true;				//�N���G�C�e�B�u�̍ہA��s���[�h���ǂ����B
	bool m_doubleClickFlagC = false;		//�_�u���N���b�N�t���O(�N���G�C�e�B�u)�B
	bool m_flyingflag = false;				//��ׂ��Ԃ��B
	bool m_attackFlag = false;				//�G�l�~�[�ɍU���������B
	bool m_deathFlag = false;				//���񂾂��ǂ����B
	bool m_eatingFlag = false;				//�H�ׂĂ邩�ǂ����B

	float m_degreeY = 0.0f;									//Y���̉�]�B
	float m_degreeXZ = 0.0f;								//XZ���̉�]�B
	float m_radianY = 0.0f;									//Y���̉�](���W�A��)�B
	float m_radianXZ = 0.0f;								//XZ���̉�](���W�A��)�B
	float m_fallSpeed = 0.5f;								//�v���C���[�̗������x�B
	float m_jmpInitialVelocity = 3.f;						//�W�����v�̏����x�B
	float m_runSpeedDouble = 1.f;							//����Ƃ��̈ړ����x�̔{���B(����Ƃ���2.f�ɂȂ�\��)�B
	float m_doubleClickTimer = 0.0f;						//�_�u���N���b�N�̔��莞�ԁB
	float m_doubleClickTimerC = 0.0f;						//�_�u���N���b�N�̔��莞��(�N���G�C�e�B�u)�B
	float m_deathAddRot = 0.f;								//���S���̉�]�����B
	float m_eatingTimer = 0.0f;
	const float m_gravity = 0.65f;							//�d�́B
	const float m_creativeSpeedMag = 3.f;					//�N���G�C�e�B�u�̔�s���̈ړ����x�̔{���B	
	const int installableBlockNum = 4;						//�u���b�N�ݒu�\����(�u���b�N����)�B

	int upDownY = 0;
	int FallDamage();		//�����_���[�W�B

	float m_hp = 20.f;				//�̗́B
	float m_stamina = 20.000f;		//�X�^�~�i�B
	int m_attackPower = 5;			//�U���́B
	int m_defensePower = 15;		//�h��́B
	float m_exp = 5.50f;			//�o���l�B
	int m_selItemNum = 0;			//�v���C���[���I�������A�C�e���ԍ��i�C���x���g���ԍ��j�B

	CVector3 m_position = CVector3::One() * 15.0f* Block::WIDTH;				//�v���C���[�̍��W�B
	CVector3 m_respawnPos = m_position;											//���X�|�[�����W�B
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//�E�����B
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//���ʁB	

	CQuaternion m_rotation = CQuaternion::Identity();			//�N�H�[�^�j�I���B
	CQuaternion m_headBoneRot = CQuaternion::Identity();		//���̍��̉�]�B
	CQuaternion m_shoulderBoneRot = CQuaternion::Identity();		//���̍��̉�]�B

	Inventory m_inventory; //�A�C�e����ۊǂ���C���x���g���B
	std::unique_ptr<GUI::RootNode> m_openedGUI; //���݊J���Ă���GUI

	enPlayerState m_playerState = enPlayerState_num;			//�v���C���[�̏�ԁB

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//���f���B
	CRayTracingModelRender m_raytraceModel;						//���C�g�����f���B
	MCCharaCon m_characon;										//�L�����R���B
	std::unique_ptr<SuicideObj::CCollisionObj> m_damageCollision;		//�U����e����p�R���W�����B

	Bone* m_headBone;												//���̍��B
	Bone* m_rightHandBone;											//�E��̍��B
	Bone* m_shoulderBone;											//���̍��B
	
	GameCamera* m_gameCamera = nullptr;							//�Q�[���J�����B
	GameMode* m_gameMode = nullptr;								//�Q�[�����[�h�B
	PlayerParameter* m_playerParameter = nullptr;				//�v���C���[�̃p�����[�^�[�B
	PlayerDeath* m_playerDeath = nullptr;						//�v���C���[�̎��S���̉摜�����B
	Game* m_game = nullptr;										//Game�N���X�B
	float m_timerBlockDestruction = 0.0f;						//�}�E�X�������Ńu���b�N�j�󂷂鎞�̃^�C�}�[�A��莞�Ԍo�߂Ńu���b�N�j������s����B
	bool m_isBlockDestruction = false;							//�u���b�N�j����������ǂ����A���삪�g���B
	float m_knockBackTimer = 0.0f;								//�m�b�N�o�b�N�̃^�C�}�[
	CVector3 m_knockBackDirection = CVector3::Zero();		//�m�b�N�o�b�N�̕����B
	float m_knockBack = 1.f;	//�m�b�N�o�b�N���x�B
	float m_knoceBackY = 1.f;	//�m�b�N�o�b�NY���W�B
};

