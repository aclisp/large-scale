#include "ObjectPool.h"
#include "PooledObject.h"
#include "Xsyslib.h"
#include "Trace.h"
#include <string.h>
#include <stdlib.h>

using namespace std;

const int BlocksPerSegment = 1024;
const int BytesPerLongLog2 = 2; // assuming that compiler aligns objects on 4-byte boundaries
const int NullificationPattern = 0xdfdfdfdf;

ObjectPool::ObjectPool (size_t size)
{
    int i;
    blockSize_ = ((size + 3) >> BytesPerLongLog2) << BytesPerLongLog2;
    currSegments_ = 0;
    nextSegments_ = 0;
    for(i = 0; i < MaxSegments; ++i) 
        blocks_[i] = NULL;
    freeq_ = NULL;
    nullify_ = NullifyAll;

    highMark_       = 0;
    largestObjSize_ = 0;
    numBlocksInuse_ = 0;
    numAllocFailed_ = 0;
}

ObjectPool::~ObjectPool (void)
{
}

bool ObjectPool::SetSegments (int count)
{
    if (count >  MaxSegments  ) 
        count = MaxSegments;
    if (count >= currSegments_) 
    {
        nextSegments_ = count;
        return true;
    }
    return false;
}

void ObjectPool::Create (void)
{
    size_t i;
    PooledObject *item;
    size_t size = (blockSize_ * BlocksPerSegment) >> BytesPerLongLog2;
    while(currSegments_ < nextSegments_)
    {
        void *p = Alloc(sizeof(int)*size);
        if(p == NULL)
            Trace::ErrorQuit_s(0, 1, "ObjectPool::Create: not enough memory");
        blocks_[currSegments_] = new(p) int[size];
        currSegments_ ++;
        for(i = 0; i < size; 
            i = i + (blockSize_ >> BytesPerLongLog2))
        {
            item = (PooledObject *)
                &blocks_[currSegments_ - 1][i];
            item->link_ = NULL;
            EnqBlock (*item); // numBlocksInuse_ --;
            numBlocksInuse_ ++;
        }
    }
}

void ObjectPool::EnqBlock (PooledObject &obj)
{
    PooledObject *tail;
    //
    // Requeuing a block already on the free queue creates a mess 
    //
    if(obj.link_ != NULL) 
        return;
    if(nullify_ != NullifyNone) 
        Nullify(obj);
    tail = freeq_;
    freeq_ = &obj; // OBJ is now tail of free queue
    if(tail == NULL)
    {
        obj.link_ = &obj; // queue was empty: OBJ points to itself
    }
    else
    {
        obj.link_ = tail->link_; // OBJ points to head (after tail)
        tail->link_ = &obj; // old tail points to OBJ
    }
    numBlocksInuse_ --;
}

void ObjectPool::Nullify (PooledObject &obj)
{
    if (nullify_ == NullifyNone) 
        return;
    if (nullify_ == NullifyAll) 
        memset(&obj, NullificationPattern, blockSize_);
    else // nullify vptr only
        memset(&obj, NullificationPattern, 4);
}

PooledObject *ObjectPool::DeqBlock (size_t size)
{
    // To support efficient enqueue and dequeue operations, freeq_ points 
    // to the tail item, which then points to the head item. The queue is 
    // therefore circular, with the head item second.
    //
    PooledObject *tail;
    PooledObject *head;

    if(size > blockSize_) 
        return NULL; 
    
    if(size > largestObjSize_)
        largestObjSize_ = size;

    tail = freeq_;
    if(tail == NULL) // no blocks left in pool
    {
        numAllocFailed_ ++;
        return NULL;
    }

    head = tail->link_;
    if(head == tail)
        freeq_ = NULL; // queue contained one block, so is now empty.
    else
        tail->link_ = head->link_; // queue contained >= 2 blocks

    numBlocksInuse_ ++;
    if(numBlocksInuse_ > highMark_)
        highMark_ = numBlocksInuse_;
    return head;
}

void ObjectPool::Display(std::ostream &stream, int indent, bool verbose)
{
    Object::Display (stream, indent, verbose);
    indent = indent + 2;
    stream << Spaces (indent) << "blockSize    : " << blockSize_ << endl;
    stream << Spaces (indent) << "largestObjSz : " << largestObjSize_ << endl;
    stream << Spaces (indent) << "currSegments : " << currSegments_ << endl;
    stream << Spaces (indent) << "nextSegments : " << nextSegments_ << endl;
    stream << Spaces (indent) << "Nullify      : " << nullify_ << endl;
    stream << Spaces (indent) << "totalBlocks  : " << GetTotalBlocksNum() << endl;
    stream << Spaces (indent) << "availBlocks  : " << GetAvailBlocksNum() << endl;
    stream << Spaces (indent) << "inuseBlocks  : " << numBlocksInuse_ << endl;
    stream << Spaces (indent) << "failedAlloc  : " << GetAllocFailed() << endl;
    stream << Spaces (indent) << "highMark     : " << GetHighMark() << endl;
}

int ObjectPool::GetTotalBlocksNum(void) const
{
    return BlocksPerSegment * currSegments_;
}

int ObjectPool::GetAvailBlocksNum(void) const
{
    if(freeq_ == NULL)
        return 0;

    int numAvailBlocks = 1;
    for(PooledObject *p = freeq_->link_; p != freeq_; p = p->link_)
        numAvailBlocks ++;

    return numAvailBlocks;
}
