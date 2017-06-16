//////////////////////////////////////////////////////////////////////////////
// Filename    : ShopTypes.h
// Written By  : �輺��
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SHOP_TYPES_H__
#define __SHOP_TYPES_H__

#include "SystemTypes.h"
#include "ItemTypes.h"

//////////////////////////////////////////////////////////////////////////////
// ������ ����â ����
//////////////////////////////////////////////////////////////////////////////
typedef BYTE ShopRackType_t;
const int szShopRackType = sizeof(ShopRackType_t);

enum SHOP_RACK_TYPE
{
	SHOP_RACK_NORMAL,
	SHOP_RACK_SPECIAL,
	SHOP_RACK_MYSTERIOUS,
	SHOP_RACK_TYPE_MAX
};


//////////////////////////////////////////////////////////////////////////////
// ���� ����â�� ũ�� �� �ε���
//////////////////////////////////////////////////////////////////////////////
const int SHOP_RACK_INDEX_MAX = 5*4; // width*height = 20


//////////////////////////////////////////////////////////////////////////////
// ���� ��Ÿ�� ���� üũ�� ���̴� �ڷ���
//////////////////////////////////////////////////////////////////////////////
typedef long ShopVersion_t;
const int szShopVersion = sizeof(ShopVersion_t);


//////////////////////////////////////////////////////////////////////////////
// ���� ��ǰ �ڵ� ������ ���̴� DB table�� ���̴� ID
//////////////////////////////////////////////////////////////////////////////
typedef uint ShopTemplateID_t;
const int szShopTemplateID = sizeof(ShopTemplateID_t);


//////////////////////////////////////////////////////////////////////////////
// ���� ���� ������ �����ϴ� �� ���̴� �ü� ������ Ÿ��
//////////////////////////////////////////////////////////////////////////////
typedef short MarketCond_t;
const int szMarketCond = sizeof(MarketCond_t);


#endif