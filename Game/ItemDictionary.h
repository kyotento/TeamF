#pragma once

class ItemDictionary{
public:

	static ItemDictionary& Instance();

	Item* GetItem( EnCube enCube ){
		return m_itemList[enCube];
	}

private:
	ItemDictionary();
	~ItemDictionary();

	std::unordered_map<int, Item*> m_itemList;   //�v���C���[�̍��W�̃��X�g
};

class ItemData{
private:
	ItemData();
	~ItemData(){}
public:
	static ItemData& GetInstance(){
		static ItemData instance;
		return instance;
	}
	Item* GetItem( EnCube enCube ){
		return m_itemList[enCube];
	}
private:
	std::unordered_map<int, Item*> m_itemList;   //�v���C���[�̍��W�̃��X�g
};