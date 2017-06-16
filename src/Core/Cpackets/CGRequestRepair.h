//-----------------------------------------------------------------------------
// 
// Filename    : CGRequestRepair.h 
// Written By  : �輺��
// Description :
// 
//-----------------------------------------------------------------------------

#ifndef __CG_REQUEST_REPAIR_H__
#define __CG_REQUEST_REPAIR_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//--------------------------------------------------------------------------------
//
// class CGRequestRepair;
//
//--------------------------------------------------------------------------------

class CGRequestRepair : public Packet {

public:
	
	// �Է½�Ʈ��(����)���κ��� ����Ÿ�� �о ��Ŷ�� �ʱ�ȭ�Ѵ�.
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
		    
	// ��½�Ʈ��(����)���� ��Ŷ�� ���̳ʸ� �̹����� ������.
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);

	// execute packet's handler
	void execute(Player* pPlayer) throw(ProtocolException, Error);

	// get packet id
	PacketID_t getPacketID() const throw() { return PACKET_CG_REQUEST_REPAIR; }
	
	// get packet's body size
	PacketSize_t getPacketSize() const throw() { return szObjectID; }

	// get packet name
	string getPacketName() const throw() { return "CGRequestRepair"; }
	
	// get packet's debug string
	string toString() const throw();
	
public:

	// get/set ObjectID
	ObjectID_t getObjectID() throw() { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID) throw() { m_ObjectID = ObjectID; }

private :
	
	// Item Object ID
	ObjectID_t m_ObjectID;

};


//-----------------------------------------------------------------------------
//
// class CGRequestRepairFactory;
//
// Factory for CGRequestRepair
//
//-----------------------------------------------------------------------------

class CGRequestRepairFactory : public PacketFactory {

public:
	
	// create packet
	Packet* createPacket() throw() { return new CGRequestRepair(); }

	// get packet name
	string getPacketName() const throw() { return "CGRequestRepair"; }
	
	// get packet id
	PacketID_t getPacketID() const throw() { return Packet::PACKET_CG_REQUEST_REPAIR; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize() const throw() { return szObjectID; }

};


//--------------------------------------------------------------------------------
//
// class CGRequestRepairHandler;
//
//--------------------------------------------------------------------------------

class CGRequestRepairHandler {
	
public:

	// execute packet's handler
	static void execute(CGRequestRepair* pPacket, Player* player) throw(ProtocolException, Error);
	static void executeNormal(CGRequestRepair* pPacket, Player* player) throw(ProtocolException, Error);
	static void executeMotorcycle(CGRequestRepair* pPacket, Player* player) throw(ProtocolException, Error);
	static void executeAll(CGRequestRepair* pPacket, Player* player) throw(ProtocolException, Error);
};

#endif