////////////////////////////////////////////////////////////////////////////////
// Filename    : ActionSearchMotorcycle.cpp
// Written By  : 
// Description :
////////////////////////////////////////////////////////////////////////////////

#include "ActionSearchMotorcycle.h"
#include "Creature.h"
#include "NPC.h"
#include "GamePlayer.h"
#include "Slayer.h"
#include "item/Key.h"
#include "Gpackets/GCNPCResponse.h"
#include "Gpackets/GCSearchMotorcycleOK.h"
#include "Gpackets/GCSearchMotorcycleFail.h"
#include "ParkingCenter.h"
#include "DB.h"

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ActionSearchMotorcycle::read (PropertyBuffer & propertyBuffer)
    throw (Error)
{
    __BEGIN_TRY
    __END_CATCH
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void ActionSearchMotorcycle::execute (Creature * pCreature1 , Creature* pCreature2) 
	throw (Error)
{
	__BEGIN_TRY

	Assert(pCreature1 != NULL);
	Assert(pCreature2 != NULL);
	Assert(pCreature1->isNPC());
	Assert(pCreature2->isPC());

	Player* pPlayer = pCreature2->getPlayer();
	Assert(pPlayer != NULL);

	// �ϴ� Ŭ���̾�Ʈ�� ���� ok��Ŷ�� �ϳ� �����ְ�...
	GCNPCResponse answerOKpkt;
	pPlayer->sendPacket(&answerOKpkt);

	// �÷��̾ �����̾����� �˻��Ѵ�.
	if (pCreature2->isSlayer())
	{
		Slayer*    pSlayer     = dynamic_cast<Slayer*>(pCreature2);
		Inventory* pInventory  = pSlayer->getInventory();
		uint       InvenWidth  = pInventory->getWidth();
		uint       InvenHeight = pInventory->getHeight();
		Item*      pItem       = NULL;
		uint       motorZoneID = 0;
		uint       motorX      = 0;
		uint       motorY      = 0;

		// �κ��丮�� �˻��Ѵ�.
		for (uint y=0; y<InvenHeight; y++)
		{
			for (uint x=0; x<InvenWidth; x++)
			{
				// x, y�� �������� �ִٸ�...
				if (pInventory->hasItem(x, y))
				{
					pItem = pInventory->getItem(x, y);
					if (search(pItem, motorZoneID, motorX, motorY))
					{
						GCSearchMotorcycleOK okpkt;
						okpkt.setZoneID(motorZoneID);
						okpkt.setX(motorX);
						okpkt.setY(motorY);
						pPlayer->sendPacket(&okpkt);
						return;
					}
				}
			}
		}
	}
	else // �����̾���...������̸� ã���� ������ ������?
	{
	}

	GCSearchMotorcycleFail failpkt;
	pPlayer->sendPacket(&failpkt);

	__END_CATCH
}

bool ActionSearchMotorcycle::search(Item* pItem, uint& zoneid, uint& x, uint& y) const
	throw()
{
	__BEGIN_TRY

	bool bFound = false;

	// �ܼ��� �Ǵ� �������� Ű�� �ƴ϶�� false�� ����.
	if (pItem->getItemClass() != Item::ITEM_CLASS_KEY) return false;

	// Ű�� �´ٸ� Ű�� Ÿ���� �Ǵ� �������� ������ ID�� ����.
	DWORD targetID = dynamic_cast<Key*>(pItem)->getTarget();

	// �ش��ϴ� ������̰� �����ϴ��� üũ�� �Ѵ�.
	if (g_pParkingCenter->hasMotorcycleBox(targetID))
	{
		MotorcycleBox* pBox = g_pParkingCenter->getMotorcycleBox(targetID);
		Assert(pBox != NULL);

		zoneid = pBox->getZone()->getZoneID();
		x      = pBox->getX();
		y      = pBox->getY();

		bFound = true;
	}

	return bFound;

	__END_CATCH
}

////////////////////////////////////////////////////////////////////////////////
// get debug string
////////////////////////////////////////////////////////////////////////////////
string ActionSearchMotorcycle::toString () const 
	throw ()
{
	__BEGIN_TRY

	StringStream msg;
	msg << "ActionSearchMotorcycle(" << ")";
	return msg.toString();

	__END_CATCH
}