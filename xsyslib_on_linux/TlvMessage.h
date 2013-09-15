#ifndef __XSYSLIB_TLVMESSAGE_H__
#define __XSYSLIB_TLVMESSAGE_H__

#include "Message.h"


typedef unsigned short TlvParmId;
typedef unsigned short TlvParmLength;
typedef int ParmIterator;


// Arbitary maximum size for a message.
const TlvParmLength TlvMaxParmLength = 1500;


struct TlvParmHeader
{
    TlvParmId     id;     // type
    TlvParmLength length; // length
};

struct TlvParmLayout
{
    TlvParmHeader header;
    char          value[TlvMaxParmLength - 1]; // value
};

struct TlvMsgHeader
{
    unsigned char  protocol; // identifier
    unsigned char  signal;   // identifier
    unsigned short length;   // excludes header and parameter fence
};

// TlvParmFencePattern: parameter fence
// TlvParmDeathPattern: marks start of trampling
//
const unsigned int   TlvMaxMsgLength = sizeof(TlvParmHeader) + TlvMaxParmLength;
const unsigned short TlvParmFencePattern = 0xaaaa;
const unsigned short TlvParmDeathPattern = 0xdead;

struct TlvMsgLayout
{
    TlvMsgHeader header; // message header
    union
    {
        // firstParm: accesses first parameter bytes:
        // accesses parameters as byte bucket
        // 
        TlvParmLayout firstParm;
        char          bytes[TlvMaxMsgLength - 1];
    } TlvParmArea;
};

class TlvMessage : public Message 
{
public:
    TlvMessage();

    TlvParmLayout *AddParm(TlvParmId id, TlvParmLength length);
    TlvParmLayout *FindParm(TlvParmId id);
    TlvParmLayout *FirstParm(ParmIterator &pit);
    TlvParmLayout *NextParm(ParmIterator &pit);

protected:
    bool AllocBytes(unsigned short count, bool &moved);
    unsigned short *FencePtr(void);
    void AddFence(void);
    bool CheckFence(void);

    inline static unsigned int Align64(TlvParmLength length)
    {
        return ((((length+7) >> 3) << 3) - length);
    };

private:
    unsigned char  buffer_[sizeof(TlvMsgHeader) + TlvMaxMsgLength];  // array to hold message contents
    unsigned short buffsize_; // size of buffer
};



#endif // __XSYSLIB_TLVMESSAGE_H__
