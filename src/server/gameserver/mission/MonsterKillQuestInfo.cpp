//////////////////////////////////////////////////////////////////////////////
// Filename    : MonsterKillQuestInfo.cpp
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "MonsterKillQuestInfo.h"
#include "MonsterKillQuestStatus.h"
#include "DB.h"
#include "Assert.h"
#include "StringStream.h"

MonsterKillQuestInfo::MonsterKillQuestInfo(QuestID_t questID, Race_t race, QuestGrade_t maxGrade, QuestGrade_t minGrade, DWORD timeLimitSec, RewardClass_t rClass,
										   SpriteType_t sType, bool isChief, int killCount)
: QuestInfo( questID, race, maxGrade, minGrade, timeLimitSec, rClass, QUEST_CLASS_MONSTER_KILL )
{
	m_TargetMonsterSpriteType = sType;
	m_IsChief = isChief;
	m_GoalNum = killCount;
}

MonsterKillQuestInfo::~MonsterKillQuestInfo()
{
}

string MonsterKillQuestInfo::toString () const throw ()
{
	StringStream msg;

	msg << "MonsterKillQuestInfo(";
	msg << "Target Monster Sprite Type : ";
	msg << m_TargetMonsterSpriteType;
	msg << ", Chief Monster Quest : ";
	msg << ((m_IsChief) ? "yes" : "no" );
	msg << ", Goal Monster Num : ";
	msg << m_GoalNum << ") : ";
	msg << "\n";
	msg << QuestInfo::toString().c_str();

	return msg.toString();
}

MonsterKillQuestStatus* MonsterKillQuestInfo::makeQuestStatus( PlayerCreature* pPC ) const throw(Error)
{
	__BEGIN_TRY

	MonsterKillQuestStatus* newQS = new MonsterKillQuestStatus(
			m_QuestID, VSDateTime::currentDateTime().addSecs( m_TimeLimitSec ),
			m_TargetMonsterSpriteType, m_IsChief, m_GoalNum );

	Assert( newQS != NULL );

	return newQS;

	__END_CATCH
}

