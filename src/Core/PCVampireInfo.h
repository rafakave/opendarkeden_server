//////////////////////////////////////////////////////////////////////////////
// Filename    : PCVampireInfo.h
// Written By  : Reiot
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __PC_VAMPIRE_INFO_H__
#define __PC_VAMPIRE_INFO_H__

#include "PCInfo.h"

//////////////////////////////////////////////////////////////////////////////
// Vampire ������ ��� �ִ� ��ü.
// GCPCList ��Ŷ�� ��ܼ� Ŭ���̾�Ʈ���� ���۵ȴ�.
// �������̳� �ɷ��ִ� ���� ���� ������ ������� �ʴ�.
//////////////////////////////////////////////////////////////////////////////

class PCVampireInfo : public PCInfo 
{
public:
	// ���� bitset�� ������� �ʴ´�.
	enum VampireBits
	{
		VAMPIRE_BIT_COAT1,
		VAMPIRE_BIT_COAT2,
		VAMPIRE_BIT_COAT3,
		VAMPIRE_BIT_MAX
	};

	enum VampireColors
	{
		VAMPIRE_COLOR_COAT,
		VAMPIRE_COLOR_MAX
	};

public:
	PCType getPCType () const throw () { return PC_VAMPIRE; }

	void read (SocketInputStream & iStream) throw (ProtocolException, Error);

	void write (SocketOutputStream & oStream) const throw (ProtocolException, Error);

	uint getSize () const throw ()
	{
		return szBYTE					// �̸�����
			+ m_Name.size() 			// �����̾� �̸�
			+ szSlot					// ����
			+ szAlignment				// ����
			+ szSex						// ����
			+ szColor* 2				// ����
			+ szBYTE + szColor		// �� Ÿ��, ����
			+ szAttr* 3 				// �ɷ�ġ
			+ szHP* 2					// HP
			+ szRank					// ���
			+ szExp						// ����ġ
			+ szLevel					// ����ġ
			+ szFame
			//+ szGold					// ��
			+ szBonus;					// ���ʽ� ����Ʈ
			//+ szZoneID;					// ZoneID
	}

	// get max size of object
	static uint getMaxSize () throw ()
	{
		return szBYTE + 20				// �����̾� �̸�
			+ szSlot					// ����
			+ szAlignment				// ����
			+ szSex						// ����
			+ szColor* 2				// ����
			+ szBYTE + szColor		// �� Ÿ��, ����
			+ szAttr* 3	 			// �ɷ�ġ
			+ szHP* 2					// HP
			+ szRank					// ���
			+ szExp						// ����ġ
			+ szFame
			+ szLevel
			//+ szGold					// ��
			+ szBonus;					// ���ʽ� ����Ʈ
			//+ szZoneID;					// ZoneID
	}

	string toString () const throw ();

public:
    string getName () const throw () { return m_Name; }
    void setName (string name) throw () { m_Name = (name.size() > 20) ? name.substr(0,20) : name; }

	Slot getSlot () const throw () { return m_Slot; }
	void setSlot (Slot slot) throw () { m_Slot = slot; }
	void setSlot (string slot) throw (InvalidProtocolException)
	{
		if (slot == Slot2String[SLOT1])
			m_Slot = SLOT1;
		else if (slot == Slot2String[SLOT2])
			m_Slot = SLOT2;
		else if (slot == Slot2String[SLOT3])
			m_Slot = SLOT3;
		else
			throw InvalidProtocolException("invalid slot value");
	}

	Alignment_t getAlignment() const throw() { return m_Alignment; }
	void setAlignment(Alignment_t Alignment) throw() { m_Alignment = Alignment; }

	Fame_t getFame () const throw () { return m_Fame; }
	void setFame (Fame_t f) throw () { m_Fame = f; }

	Sex getSex () const throw () { return m_Sex; }
	void setSex (Sex sex) throw () { m_Sex = sex; }
	void setSex (string sex) throw (InvalidProtocolException)
	{
		if (sex == Sex2String[MALE]) 
			m_Sex = MALE;
		else if (sex == Sex2String[FEMALE]) 
			m_Sex = FEMALE;
		else
			throw InvalidProtocolException("invalid sex value");
	}

	Color_t getBatColor () const throw () { return m_BatColor; }
	void setBatColor (Color_t batColor) throw () { m_BatColor = batColor; }

	Color_t getSkinColor () const throw () { return m_SkinColor; }
	void setSkinColor (Color_t skinColor) throw () { m_SkinColor = skinColor; }

	ItemType_t getCoatType() const throw() { return m_CoatType; }
	void setCoatType(ItemType_t CoatType) throw() { m_CoatType = CoatType; }
	
	Color_t getCoatColor() const throw() { return m_CoatColor; }
	void setCoatColor(Color_t CoatColor) throw() { m_CoatColor = CoatColor; }

    Attr_t getSTR () const throw (Error) { if (m_STR > maxVampireAttr) throw Error("STR out of range"); return m_STR; }
	void setSTR (Attr_t str) throw (Error) { if (str > maxVampireAttr) throw Error("STR out of range"); m_STR = str; }

	Attr_t getDEX () const throw (Error) { if (m_DEX > maxVampireAttr) throw Error("DEX out of range"); return m_DEX; }
	void setDEX (Attr_t dex) throw (Error) { if (dex > maxVampireAttr) throw Error("DEX out of range"); m_DEX = dex; }

	Attr_t getINT () const throw (Error) { if (m_INT > maxVampireAttr) throw Error("INT out of range"); return m_INT; }
	void setINT (Attr_t inte) throw (Error) { if (inte > maxVampireAttr) throw Error("INT out of range"); m_INT = inte; }

	HP_t getHP (AttrType attrType = ATTR_CURRENT) const throw () { return m_HP[attrType]; }
	void setHP (HP_t hp, AttrType attrType = ATTR_CURRENT) throw () { m_HP[attrType] = hp; }
	void setHP (HP_t curHP, HP_t maxHP) throw () { m_HP[ATTR_CURRENT] = curHP; m_HP[ATTR_MAX] = maxHP; }

	Level_t getLevel () const throw () { return m_Level; }
	void setLevel (Level_t exp) throw () { m_Level = exp; }

	Exp_t getExp () const throw () { return m_Exp; }
	void setExp (Exp_t exp) throw () { m_Exp = exp; }

	Rank_t getRank () const throw () { return m_Rank; }
	void setRank (Rank_t rank) throw () { m_Rank = rank; }

	// get/set gold
	//Gold_t getGold () const throw () { return m_Gold; }
	//void setGold (Gold_t gold) throw () { m_Gold = gold; }

	// get/set zoneID
	//ZoneID_t getZoneID () const throw () { return m_ZoneID; }
	//void setZoneID (ZoneID_t zoneID) throw () { m_ZoneID = zoneID; }

	Bonus_t getBonus() const throw () { return m_Bonus; }
	void setBonus(Bonus_t Bonus) throw () { m_Bonus = Bonus; }

	void setShapeInfo(DWORD flag, Color_t color[VAMPIRE_COLOR_MAX]) throw();

private:
	// PC name
	string m_Name;

	// PC ����
	Slot m_Slot;

	// Alignment
	Alignment_t m_Alignment;

	// PC sex
	Sex m_Sex;

	// hair color, skin color,
	Color_t m_BatColor;
	Color_t m_SkinColor;

	// wear Type/ color
	ItemType_t m_CoatType;
	Color_t m_CoatColor;

	// ���
	Rank_t m_Rank;

	// Exp
	Level_t m_Level;

	// Attributes
	Attr_t m_STR;
	Attr_t m_DEX;
	Attr_t m_INT;

	// HP
	// HP[0] = current hp, hp[1] == max hp
	HP_t m_HP[2];

	// Exp
	Exp_t m_Exp;

	// Gold
	//Gold_t m_Gold;
		
	// Fame 
	Fame_t m_Fame;

	// ZoneID
	//ZoneID_t m_ZoneID;

	// Bonus Point
	Bonus_t m_Bonus;

	// �ɷ� �ִ� ����
};

#endif