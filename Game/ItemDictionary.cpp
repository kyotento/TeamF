#include "stdafx.h"
#include "ItemDictionary.h"

ItemDictionary::ItemDictionary(){}

ItemDictionary::~ItemDictionary(){}

ItemDictionary& ItemDictionary::Instance(){
	static ItemDictionary st_instance;
	return st_instance;
}
