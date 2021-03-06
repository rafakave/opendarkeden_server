//////////////////////////////////////////////////////////////////////////////
// Filename    : Item.cpp
// Written by  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "Item.h"
#include "DB.h"
#include "ItemInfoManager.h"
#include "Assert.h"

#include "PCItemInfo.h"
#include "PlayerCreature.h"

#include <cstdio>

//////////////////////////////////////////////////////////////////////////////
// class Item member methods
//////////////////////////////////////////////////////////////////////////////

Item::Item()
	throw()
{
	m_ItemID = 0;
	m_CreateType = CREATE_TYPE_NORMAL;
	m_bUnique = false;
	m_bQuest = false;
	m_bTrace = false;
	m_bTimeLimit = false;
}

Item::~Item()
{
}

bool Item::destroy() 
	throw(Error)
{
	__BEGIN_TRY

	Statement* pStmt = NULL;

	BEGIN_DB
	{
		pStmt = g_pDatabaseManager->getConnection("DARKEDEN")->createStatement();

		pStmt->executeQuery("DELETE FROM %s WHERE ItemID = %lu", getObjectTableName().c_str(), m_ItemID);

		// DB에서 지우는건데..
		// DB에 이미 아이템이 없는 경우
		if (pStmt->getAffectedRowCount()==0)
		{
			SAFE_DELETE(pStmt);
			return false;
		}

		SAFE_DELETE(pStmt);
	}
	END_DB(pStmt)

	return true;

	__END_CATCH
}

// 아이템을 버린다. 기본은 가비지로, 특별한 경우엔 Timeover 나 그외 안 쓰는 storage로..
void Item::waste( Storage storage ) const
	throw(Error)
{
	Assert( (uint)storage >= (uint)STORAGE_GARBAGE );

	char query[50];

	sprintf( query, "Storage = %u", (uint)storage );

	tinysave( query );
}

const list<OptionType_t>& Item::getDefaultOptions(void) const 
	throw()
{
	__BEGIN_TRY

	try {

		return g_pItemInfoManager->getItemInfo(getItemClass(), getItemType())->getDefaultOptions();

	} catch (NoSuchElementException&){
		StringStream msg;
		msg << "그런 아이템 정보가 없습니다.(" << (int)getItemClass() << ", " << (int)getItemType() << ")";

		filelog("itemError.txt", "%s", msg.toString().c_str());
		//throw Error(msg.toString());
	}

	static list<OptionType_t> optionTypes;
	return optionTypes;

	__END_CATCH
}

void Item::makePCItemInfo(PCItemInfo& result) const
{
	result.setObjectID(getObjectID());
	result.setItemClass(getItemClass());
	result.setItemType(getItemType());
	result.setOptionType(getOptionTypeList());
	result.setSilver(getSilver());
	result.setDurability(getDurability());
	result.setEnchantLevel(getEnchantLevel());
	result.setItemNum(getNum());
	result.setMainColor(0);
	result.setGrade(getGrade());
}

void Item::whenPCTake( PlayerCreature* pPC )
{
//	cout << pPC->getName() << " Take " << toString() << endl; 
}

void Item::whenPCLost( PlayerCreature* pPC )
{
//	cout << pPC->getName() << " Lost " << toString() << endl; 
}
