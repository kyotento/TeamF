//! @file
#pragma once

//エンティティの属性を設定。
enum EnEntity
{
	enEntity_None,			//ノーン(今んとこゾンビしか判定に使ってないので他はこれで)。
	enEntity_Zombie,		//ゾンビ。
	enEntity_Cow			//牛。			
};

class World;
//! @brief ブロックではない、動物や、アイテム、矢などの基底クラス。
//! @author Takayama
class Entity : public IGameObject{
public:
	Entity(EnEntity enEntity = enEntity_None, bool useBulletColision = false);

	virtual ~Entity();

	//! @brief 座標を取得。
	virtual CVector3 GetPos() const = 0;

	//! @brief 座標を設定。
	virtual void SetPos( const CVector3& pos ) = 0;

	//! @brief エンティティが有効か取得。
	bool GetEnableEntity()const {
		return m_enable;
	}
	//! @brief エンティティ有効無効を設定。
	void SetEnableEntity(bool enable) {
		m_enable = enable;
		SetEnableEntityInner(enable);
		SetEnable(enable);//ゲームオブジェクト自体も
	}
	//! @brief エンティティ有効無効を設定したあとの処理。
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

