//! @file
#pragma once

//�G���e�B�e�B�̑�����ݒ�B
enum EnEntity
{
	enEntity_None,			//�m�[��(����Ƃ��]���r��������Ɏg���ĂȂ��̂ő��͂����)�B
	enEntity_Zombie			//�]���r�B
};

class World;
//! @brief �u���b�N�ł͂Ȃ��A������A�A�C�e���A��Ȃǂ̊��N���X�B
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity(World* world, EnEntity enEntity = enEntity_None);

	virtual ~Entity();

	//! @brief ���W���擾�B
	virtual CVector3 GetPos() const = 0;

	//! @brief ���W��ݒ�B
	virtual void SetPos( const CVector3& pos ) = 0;

	EnEntity GetEntityType()
	{
		return m_enEntity;
	}
protected:

	World* m_world = nullptr;
	EnEntity m_enEntity = enEntity_None;
};

