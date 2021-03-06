//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCShowWaitGuildInfo.h 
// Written By  : 
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_WAIT_SHOW_GUILD_INFO_H__
#define __GC_WAIT_SHOW_GUILD_INFO_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"
#include <list>


//////////////////////////////////////////////////////////////////////
//
// class GCShowWaitGuildInfo;
//
// Ŭ���̾�Ʈ�� ���� ���� â� ���쵵�� �Ѵ�.
//
//////////////////////////////////////////////////////////////////////

class GCShowWaitGuildInfo : public Packet {

public :
	
    // �Է½�Ʈ��(����)��κ��� ����Ÿ�� �о ��Ŷ� �ʱ�ȭ�Ѵ�.
    void read(SocketInputStream & iStream) throw(ProtocolException, Error);
		    
    // ���½�Ʈ��(����)��� ��Ŷ�� ���̳ʸ� �̹����� ������.
    void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_GC_SHOW_WAIT_GUILD_INFO; }
	
	// get packet's body size
	PacketSize_t getPacketSize() const throw();

	// get packet name
	string getPacketName() const throw() { return "GCShowWaitGuildInfo"; }
	
	// get packet's debug string
	string toString() const throw();

	// get/set Guild ID
	GuildID_t getGuildID() const throw() { return m_GuildID; }
	void setGuildID( GuildID_t GuildID ) throw() { m_GuildID = GuildID; }

	// get/set Guild Name
	const string& getGuildName() const throw() { return m_GuildName; }
	void setGuildName( const string& GuildName ) throw() { m_GuildName = GuildName; }

	// get/set Guild State
	GuildState_t getGuildState() const throw() { return m_GuildState; }
	void setGuildState( GuildState_t GuildState ) throw() { m_GuildState = GuildState; }

	// get/set Guild Master
	const string& getGuildMaster() const throw() { return m_GuildMaster; }
	void setGuildMaster( const string& GuildMaster ) throw() { m_GuildMaster = GuildMaster; }

	// get/set Guild Member Count
	BYTE getGuildMemberCount() const throw() { return m_GuildMemberCount; }
	void setGuildMemberCount( BYTE GuildMemberCount ) throw() { m_GuildMemberCount = GuildMemberCount; }

	// get/set Guild Intro
	const string& getGuildIntro() const throw() { return m_GuildIntro; }
	void setGuildIntro( const string& GuildIntro ) throw() { m_GuildIntro = GuildIntro; }

	// get/set Join Fee
	Gold_t getJoinFee() const throw() { return m_JoinFee; }
	void setJoinFee( Gold_t JoinFee ) throw() { m_JoinFee = JoinFee; }

	// Starting Member List
	void addMember( const string& member ) throw() { m_MemberList.push_front( member ); }
	string popMember() throw() {
		if ( !m_MemberList.empty() )
		{
			string member = m_MemberList.front();
			m_MemberList.pop_front();
			return member;
		}
		return "";
	}
	BYTE getMemberNum() const throw() { return m_MemberList.size(); }
	

private :
	
	// Guild ID
	GuildID_t m_GuildID;

	// Guild Name
	string m_GuildName;

	// Guild State
	GuildState_t m_GuildState;

	// Guild Master
	string m_GuildMaster;

	// Guild Member Count
	BYTE m_GuildMemberCount;

	// Guild Intro
	string m_GuildIntro;

	// Guild Join Fee
	Gold_t m_JoinFee;
	
	// Starting Lember List
	list<string> m_MemberList;
	
};


//////////////////////////////////////////////////////////////////////
//
// class GCShowWaitGuildInfoFactory;
//
// Factory for GCShowWaitGuildInfo
//
//////////////////////////////////////////////////////////////////////

class GCShowWaitGuildInfoFactory : public PacketFactory {

public :
	
	// create packet
	Packet* createPacket() throw() { return new GCShowWaitGuildInfo(); }

	// get packet name
	string getPacketName() const throw() { return "GCShowWaitGuildInfo"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_SHOW_WAIT_GUILD_INFO; }

	// get packet's max body size
	// *OPTIMIZATION HINT*
	// const static GCSystemMessagePacketMaxSize �� ���, �����϶�.
	PacketSize_t getPacketMaxSize() const throw()
	{
		return szGuildID +		// Guild ID
			   szBYTE +			// Guild Name length
			   30 +				// Guild Name
			   szGuildState +	// Guild State
			   szBYTE +			// Guild Master length
			   20 +				// Guild Master
			   szBYTE +			// Guild Member Count
			   szBYTE +			// Guild Intro length
			   256 +			// Guild Intro
			   szGold +			// Guild Join Fee
			   szBYTE +			// Member Num
			   ( szBYTE + 20 ) * 5;		// Member List Mex Length
	}

};


//////////////////////////////////////////////////////////////////////
//
// class GCShowWaitGuildInfo;
//
//////////////////////////////////////////////////////////////////////

class GCShowWaitGuildInfoHandler {
	
public :
	
	// execute packet's handler
	static void execute(GCShowWaitGuildInfo* pPacket, Player* pPlayer) throw(ProtocolException, Error);

};

#endif
