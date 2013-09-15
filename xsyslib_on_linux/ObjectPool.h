#ifndef __XSYSLIB_OBJECTPOOL_H__
#define __XSYSLIB_OBJECTPOOL_H__

#include "Object.h"

class PooledObject;

const int MaxSegments = 256; // supports up to 256K blocks in pool

enum Nullification
{
    NullifyNone, // do not nullify deleted object
    NullifyVptr, // nullify vptr only
    NullifyAll   // nullify entire contents
};

class ObjectPool : public Object
{
public:
    virtual ~ObjectPool(void);
    bool SetSegments(int count);
    void Create(void);
    PooledObject *DeqBlock(size_t size);
    void EnqBlock(PooledObject &obj);
    void SetNullification(Nullification n) { nullify_ = n; };
    void Display(std::ostream &stream, int indent, bool verbose);
    
    // Collect statistics
    //
    int GetTotalBlocksNum(void) const;
    int GetAvailBlocksNum(void) const;
    int GetAllocFailed(void) const // because the pool was empty
    { return numAllocFailed_; }
    int GetHighMark(void) const // the max number of blocks that were simultaneously in use
    { return highMark_; }
    size_t GetLargestObjectSize(void) const // the largest object that was placed in a block
    { return largestObjSize_; }

protected:
    ObjectPool(size_t size);
    void Nullify(PooledObject &obj);

private:
    ObjectPool(const ObjectPool& );
    ObjectPool& operator=(const ObjectPool& );

    size_t blockSize_; // size of blocks in bytes
    int    currSegments_; // current number of segments in pool
    int    nextSegments_; // segments to be reached by Create
    int    *blocks_[MaxSegments]; // the actual blocks
    PooledObject  *freeq_; // queue of available blocks
    Nullification nullify_; // how to nullify deleted objects

    // statistics
    //
    int highMark_;
    size_t largestObjSize_;
    int numBlocksInuse_;
    int numAllocFailed_;
};

#endif // __XSYSLIB_OBJECTPOOL_H__
