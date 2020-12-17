//! @file
#pragma once

//エンティティの属性を設定。
enum EnEntity
{
	enEntity_None,			//ノーン(今んとこゾンビしか判定に使ってないので他はこれで)。
	enEntity_Zombie			//ゾンビ。
};

class World;
//! @brief ブロックではない、動物や、アイテム、矢などの基底クラス。
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity(World* world, EnEntity enEntity = enEntity_None);

	virtual ~Entity();

	//! @brief 座標を取得。
	virtual CVector3 GetPos() const = 0;

	//! @brief 座標を設定。
	virtual void SetPos( const CVector3& pos ) = 0;

	EnEntity GetEntityType()
	{
		return m_enEntity;
	}
protected:

	World* m_world = nullptr;
	EnEntity m_enEntity = enEntity_None;
};

