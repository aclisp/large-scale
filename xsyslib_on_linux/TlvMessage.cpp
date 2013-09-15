#include "TlvMessage.h"


TlvMessage::TlvMessage()
{
    buffsize_ = 100;//sizeof(buffer_);
    TlvMsgLayout *mptr = (TlvMsgLayout*) &buffer_[0];
    mptr->header.protocol = 0x01;
    mptr->header.signal   = 0x01;
    mptr->header.length   = 0;
    AddFence();
}

TlvParmLayout *TlvMessage::AddParm(TlvParmId id, TlvParmLength length)
{
    int           size;
    TlvMsgLayout  *mptr = (TlvMsgLayout*) &buffer_[0];
    TlvParmLayout *pptr;
    bool          moved;
    int           offset;

    // Check if the fence pattern is trampled. Ensure that the new parameter
    // will fit into the buffer. This includes space for its header, padding 
    // (for alignment), and fence pattern.
    //
    if(!CheckFence())
        return NULL;
    size = sizeof(TlvParmHeader) + length + Align64(length);
    if(!AllocBytes(size + 2, moved))
        return NULL;
    if(moved) // set by AllocBytes if buffer_ moved
        mptr = (TlvMsgLayout*) &buffer_[0];

    // The new paramter starts just after the end of the message. Set its 
    // header and update the message length.
    //
    offset = sizeof(TlvMsgHeader) + mptr->header.length;
    pptr = (TlvParmLayout*) &buffer_[offset];
    pptr->header.id = id;
    pptr->header.length = length + Align64(length);
    mptr->header.length += size;

    // Initialize the parameter using its template.
    // Add the fence.
    //
    //ProtocolRegistry *preg = SingletonObject<ProtocolRegistry>::Instance();
    //Protocol *prot = preg->Protocol(mptr->header.protocol);
    //Parameter *parm = prot->Parameter(id);
    //memcpy(pptr, parm->Template(), parm->Size());
    *FencePtr() = TlvParmFencePattern;
    return pptr;
}

bool TlvMessage::AllocBytes(unsigned short count, bool &moved)
{
    TlvMsgLayout  *mptr = (TlvMsgLayout*) &buffer_[0];
    //unsigned char *buff;
    unsigned short oldlen;
    unsigned short newlen;

    // If the buffer can't hold COUNT more bytes, extend its size.
    //
    moved = false;
    oldlen = mptr->header.length;
    newlen = sizeof(TlvMsgHeader) + oldlen + count;
    if(newlen > buffsize_)
    {
        //buff = (unsigned char*) realloc(buffer_, newlen);
        //if(buff == NULL)
        //    return false;
        //buffer_ = buff;
        //buffsize_ = newlen;
        //moved = true;
        return false;
    }
    return true;
}

unsigned short *TlvMessage::FencePtr(void)
{
    // Return a pointer to the parameter fence.
    // 
    TlvMsgLayout *msg = (TlvMsgLayout*) &buffer_[0];
    int offset = sizeof(TlvMsgHeader) + msg->header.length;
    return (unsigned short*) &buffer_[offset];
}

void TlvMessage::AddFence(void)
{
    // Add the fence to the end of the message.
    //
    bool moved;
    if (!AllocBytes(2, moved))
        return;
    *FencePtr() = TlvParmFencePattern;
}

bool TlvMessage::CheckFence(void)
{
    // If the fence has been trampled, throw an exception after marking the
    // location where trampling began.
    //
    if(*FencePtr() != TlvParmFencePattern)
    {
        *FencePtr() = TlvParmDeathPattern;
        //throw MessageTrampledException();
        return false;
    }
    return true;
}

TlvParmLayout *TlvMessage::FirstParm(ParmIterator &pit)
{
    TlvMsgLayout *mptr = (TlvMsgLayout*) &buffer_[0];
    if(mptr->header.length == 0)
        return NULL;

    TlvParmLayout *pptr = &mptr->TlvParmArea.firstParm;
    pit = sizeof(TlvMsgHeader) + sizeof(TlvParmHeader) + pptr->header.length;
    return pptr;
}

TlvParmLayout *TlvMessage::NextParm(ParmIterator &pit)
{
    TlvMsgLayout *mptr = (TlvMsgLayout*) &buffer_[0];
    if(pit >= int(sizeof(TlvMsgHeader) + mptr->header.length))
        return NULL;
    
    TlvParmLayout *pptr = (TlvParmLayout *) &buffer_[pit];
    pit += sizeof(TlvParmHeader) + pptr->header.length;
    return pptr;
}

TlvParmLayout *TlvMessage::FindParm(TlvParmId id)
{
    TlvParmLayout *pptr;
    ParmIterator pit;

    for(pptr = FirstParm(pit); pptr != NULL; pptr = NextParm(pit))
    {
        if(pptr->header.id == id)
            return pptr;
    }

    return NULL;
}
