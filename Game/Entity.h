//! @file
#pragma once

//�G���e�B�e�B�̑�����ݒ�B
enum EnEntity
{
	enEntity_None,			//�m�[��(����Ƃ��]���r��������Ɏg���ĂȂ��̂ő��͂����)�B
	enEntity_Zombie,		//�]���r�B
	enEntity_Cow			//���B			
};

class World;
//! @brief �u���b�N�ł͂Ȃ��A������A�A�C�e���A��Ȃǂ̊��N���X�B
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity(EnEntity enEntity = enEntity_None, bool useBulletColision = false);

	virtual ~Entity();

	//! @brief ���W���擾�B
	virtual CVector3 GetPos() const = 0;

	//! @brief ���W��ݒ�B
	virtual void SetPos( const CVector3& pos ) = 0;

	//! @brief �G���e�B�e�B���L�����擾�B
	bool GetEnableEntity()const {
		return m_enable;
	}
	//! @brief �G���e�B�e�B�L��������ݒ�B
	void SetEnableEntity(bool enable) {
		m_enable = enable;
		SetEnableEntityInner(enable);
		SetEnable(enable);//�Q�[���I�u�W�F�N�g���̂�
	}
	//! @brief �G���e�B�e�B�L��������ݒ肵�����Ƃ̏����B
	virtual void SetEnableEntityInner(bool enable) = 0;

	void SetWorld( World* world ){
		m_world = world;
	}

	EnEntity GetEntityType()
	{
		return m_enEntity;
	}

	bool UseBulletColision()const {
		return m_useBulletColision;
	}

protected:
	World* m_world = nullptr;
	EnEntity m_enEntity = enEntity_None;
	bool m_useBulletColision = false;
	bool m_enable = true;
};

