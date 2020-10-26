#pragma once
#include "Block.h"
#include "../physics/character/CCharacterController.h"
#include "Entity.h"

class World;
class GameCamera;
class Item;
class GameMode;
struct Inventory {
	Item* s_item = nullptr;
	int s_number = 0;
};

class Player : public Entity
{
public:
	Player();
	~Player();
	bool Start() override;
	void Update() override;

	CFont font;
	void HUDRender( int HUDNum ) override{
		std::wstringstream str;
		CVector3 pos = GetPos() / Block::WIDTH;
		str << pos.x << " , " << pos.y << " , " << pos.z << "\n";
		font.Draw( str.str().c_str(), { 0.5f , 0.3f }, CVector4::White(), CVector2::One(), {0.5f, 0.5f} );
	}

	/// <summary>
	/// �A�j���[�V�����B
	/// </summary>
	enum enAnimationClip{
		enAnimationClip_Idle,			//�ҋ@��ԁB
		enAnimationClip_move,			//�ړ��B
		enAnimationClip_shift,			//���Ⴊ�݁B
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
		enPlayerState_shift,			//���Ⴊ�݁B
		enPlayerState_excavate,			//�����@��B
		enPlayerState_num,				//��Ԃ̐��B
	};

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

	//! @brief ���W��ݒ�B
	void SetPos( const CVector3& pos ) override{
		m_position = pos;
		m_characon.SetPosition( pos );
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

	//! @brief World ���Z�b�g����B
	//! @param recursive true�Ȃ� World::SetPlayer(this, false) ���Ăяo���B
	void SetWorld( World* world , bool recursive = true);

	//�C���x���g���̒���
	static const int inventryWidth = 9;
	static const int inventryHeight = 1;
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
	/// �ړ����@�̐؂�ւ�(�N���G�C�e�B�u)�B
	/// </summary>
	void ChangeMovemontC();

	/// <summary>
	/// �v���C���[�̏�ԊǗ��B
	/// </summary>
	void StateManagement();

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

	float m_degreeY = 0.0f;									//Y���̉�]�B
	float m_degreeXZ = 0.0f;								//XZ���̉�]�B
	float m_radianY = 0.0f;									//Y���̉�](���W�A��)�B
	float m_radianXZ = 0.0f;								//XZ���̉�](���W�A��)�B
	float m_fallSpeed = 0.5f;								//�v���C���[�̗������x�B
	float m_jmpInitialVelocity = 3.f;						//�W�����v�̏����x�B
	float m_runSpeedDouble = 1.f;							//����Ƃ��̈ړ����x�̔{���B(����Ƃ���2.f�ɂȂ�\��)�B
	float m_doubleClickTimer = 0.0f;						//�_�u���N���b�N�̔��莞�ԁB
	float m_doubleClickTimerC = 0.0f;						//�_�u���N���b�N�̔��莞��(�N���G�C�e�B�u)�B
	const float m_characonRadius = 50.f;					//�L�����R���̔��a�B
	const float m_characonHeight = 160.f;					//�L�����R���̍����B
	const float m_gravity = 0.65f;							//�d�́B
	const float m_creativeSpeedMag = 3.f;					//�N���G�C�e�B�u�̔�s���̈ړ����x�̔{���B	

	CVector3 m_position = CVector3::One() * 15.0f* Block::WIDTH;				//�v���C���[�̍��W�B
	CVector3 m_right = CVector3(1.0f,0.0f,0.0f);				//�E�����B
	CVector3 m_front = CVector3(0.0f, 0.0f, 1.0f);				//���ʁB	

	CQuaternion m_rotation = CQuaternion::Identity();			//�N�H�[�^�j�I���B

	std::vector<Inventory*> m_inventoryList;					//�C���x���g���B

	enPlayerState m_playerState = enPlayerState_num;			//�v���C���[�̏�ԁB

	GameObj::CSkinModelRender* m_skinModelRender = nullptr;		//���f���B
	CCharacterControllerType2 m_characon;							//�L�����R���B
	CFont m_font;												//�t�H���g�B
	GameCamera* m_gameCamera = nullptr;							//�Q�[���J�����B
	
	World* m_world = nullptr;                                   //���[���h�B
	GameMode* m_gameMode = nullptr;								//�Q�[�����[�h�B
};

