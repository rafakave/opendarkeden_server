//////////////////////////////////////////////////////////////////////////////
// Filename    : DivineGuidance.cpp
// Written by  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "DivineGuidance.h"
#include "EffectDivineGuidance.h"
#include "Gpackets/GCSkillToObjectOK1.h"
#include "Gpackets/GCSkillToObjectOK2.h"
#include "Gpackets/GCSkillToObjectOK5.h"
#include "Gpackets/GCSkillToSelfOK1.h"
#include "Gpackets/GCSkillToSelfOK2.h"
#include "Gpackets/GCAddEffect.h"

//////////////////////////////////////////////////////////////////////////////
// �����̾� ������Ʈ �ڵ鷯
//////////////////////////////////////////////////////////////////////////////
void DivineGuidance::execute(Slayer* pSlayer, ObjectID_t TargetObjectID, SkillSlot* pSkillSlot, CEffectID_t CEffectID)
	throw(Error)
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << "begin " << endl;

	Assert(pSlayer != NULL);
	Assert(pSkillSlot != NULL);

	try 
	{
		Player* pPlayer = pSlayer->getPlayer();
		Zone* pZone = pSlayer->getZone();
		Assert(pPlayer != NULL);
		Assert(pZone != NULL);

		Creature* pTargetCreature = pZone->getCreature(TargetObjectID);
		//Assert(pTargetCreature != NULL);

		// NoSuch����. by sigi. 2002.5.2
		if (pTargetCreature==NULL
			|| pTargetCreature->isFlag(Effect::EFFECT_CLASS_NO_DAMAGE) // by sigi. 2002.10.30
		)
		{
			executeSkillFailException(pSlayer, getSkillType());
			//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " end " << endl;
			return;
		}

		// ����Ʈ�� ȿ���� ���ӽð��� ����Ѵ�.
		SkillInput input(pSlayer, pSkillSlot);
		SkillOutput output;
		if ( pTargetCreature->isPC() )
		{
			input.TargetType = SkillInput::TARGET_PC;
		}
		else
		{
			input.TargetType = SkillInput::TARGET_MONSTER;
		}
		computeOutput(input, output);

		GCSkillToObjectOK1 _GCSkillToObjectOK1;
		GCSkillToObjectOK2 _GCSkillToObjectOK2;
		GCSkillToObjectOK5 _GCSkillToObjectOK5;

		SkillType_t       SkillType  = pSkillSlot->getSkillType();
		SkillInfo*        pSkillInfo = g_pSkillInfoManager->getSkillInfo(SkillType);
		SkillDomainType_t DomainType = pSkillInfo->getDomainType();

		bool bIncreaseDomainExp = pSlayer->isRealWearingEx(Slayer::WEAR_RIGHTHAND);

		int  RequiredMP  = (int)pSkillInfo->getConsumeMP();
		bool bManaCheck  = hasEnoughMana(pSlayer, RequiredMP);
		bool bTimeCheck  = verifyRunTime(pSkillSlot);
		bool bRangeCheck = verifyDistance(pSlayer, pTargetCreature, pSkillInfo->getRange());
		bool bHitRoll    = (rand()%100 < output.Range);//HitRoll::isSuccessMagic(pSlayer, pSkillInfo, pSkillSlot);
		bool bEffected   = pTargetCreature->isFlag(Effect::EFFECT_CLASS_DIVINE_GUIDANCE);
		bool bPK         = verifyPK(pSlayer, pTargetCreature);

		if (bManaCheck && bTimeCheck && bRangeCheck && bHitRoll && !bEffected && bPK)
		{
			// ������ ���δ�.
			decreaseMana(pSlayer, RequiredMP, _GCSkillToObjectOK1);

			if (!pTargetCreature->isSlayer())
			{
				// ����ġ�� �÷��ش�.
				//SkillGrade Grade = g_pSkillInfoManager->getGradeByDomainLevel(pSlayer->getSkillDomainLevel(DomainType));
				//Exp_t      ExpUp = 10* (Grade + 1);
				//shareAttrExp(pSlayer, ExpUp, 1, 1, 8, _GCSkillToObjectOK1);
				//
				if ( bIncreaseDomainExp )
				{
					increaseDomainExp(pSlayer, DomainType, pSkillInfo->getPoint(), _GCSkillToObjectOK1);
				}

				increaseSkillExp(pSlayer, DomainType,  pSkillSlot, pSkillInfo, _GCSkillToObjectOK1);
			}
		
			// ����Ʈ�� �����ؼ� ���δ�.
			EffectDivineGuidance* pEffect = new EffectDivineGuidance(pTargetCreature);
			Assert(pEffect != NULL);
			pEffect->setDeadline(output.Duration);
			pEffect->setTick(output.Tick);
			pEffect->setPoint(output.Damage); 
			pEffect->setUserObjectID( pSlayer->getObjectID() );
			pEffect->setNextTime(0);

			pTargetCreature->addEffect(pEffect);
			pTargetCreature->setFlag(Effect::EFFECT_CLASS_DIVINE_GUIDANCE);

			// ��Ŷ�� �غ��ؼ� ������.
			_GCSkillToObjectOK1.setSkillType(SkillType);
			_GCSkillToObjectOK1.setCEffectID(CEffectID);
			_GCSkillToObjectOK1.setTargetObjectID(TargetObjectID);
			_GCSkillToObjectOK1.setDuration(output.Duration);

			_GCSkillToObjectOK2.setObjectID(pSlayer->getObjectID());
			_GCSkillToObjectOK2.setSkillType(SkillType);
			_GCSkillToObjectOK2.setDuration (output.Duration);

			_GCSkillToObjectOK5.setObjectID(pSlayer->getObjectID());
			_GCSkillToObjectOK5.setSkillType(SkillType);
			_GCSkillToObjectOK5.setTargetObjectID (TargetObjectID);
			_GCSkillToObjectOK5.setDuration (output.Duration);

			// Send Packet
			pPlayer->sendPacket(&_GCSkillToObjectOK1);

			if (pSlayer != pTargetCreature && pTargetCreature->isPC()) 
			{
				Player* pTargetPlayer = pTargetCreature->getPlayer();
				Assert(pTargetPlayer != NULL);
				pTargetPlayer->sendPacket(&_GCSkillToObjectOK2);
			}
			else if ( pTargetCreature->isMonster() )
			{
				Monster* pMonster = dynamic_cast<Monster*>(pTargetCreature);
				if ( pMonster != NULL ) pMonster->addEnemy(pSlayer);
			}

			list<Creature*> cList;
			cList.push_back(pSlayer);
			cList.push_back(pTargetCreature);
		
			pZone->broadcastPacket(pSlayer->getX(), pSlayer->getY(),  &_GCSkillToObjectOK5 , cList);

//			// ����Ʈ�� �پ��ٰ� �˷��ش�.
//			GCAddEffect gcAddEffect;
//			gcAddEffect.setObjectID(pTargetCreature->getObjectID());
//			gcAddEffect.setEffectID(Effect::EFFECT_CLASS_DIVINE_GUIDANCE);
//			gcAddEffect.setDuration(output.Duration);
//			pZone->broadcastPacket(pTargetCreature->getX(), pTargetCreature->getY(), &gcAddEffect, pTargetCreature);
//
			pSkillSlot->setRunTime(output.Delay);
		} 
		else 
		{
			executeSkillFailNormal(pSlayer, getSkillType(), pTargetCreature);
		}
	} 
	catch (Throwable & t) 
	{
		executeSkillFailException(pSlayer, getSkillType());
	}

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " end " << endl;

	__END_CATCH
}

DivineGuidance g_DivineGuidance;