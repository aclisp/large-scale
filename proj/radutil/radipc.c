#include "radutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(RAD_OS_WIN32)
  #include <windows.h>
#else
  #include <sys/ipc.h>
  #include <sys/sem.h>
  #include <sys/shm.h>
  #include <sys/time.h>
#endif

#if defined(RAD_OS_WIN32)

sem_t rad_semget(sem_key_t key, int init, int flags)
{
    DWORD errnum;
    HANDLE hsem;
    char keystr[9];
    char buf[256];

    sprintf(keystr, "%08X", key);

    if(!(flags & RADIPC_CREAT)) { /* Open semantics */
        hsem = OpenSemaphore(SYNCHRONIZE|SEMAPHORE_MODIFY_STATE,
                             FALSE, keystr);
        errnum = GetLastError();

        if(hsem == NULL) {
            strerror_r(errnum, buf, sizeof(buf));
            radlog(L_ERROR|L_CONS, "[rad_semget] OpenSemaphore(%s): "
                   "%d %s", keystr, errnum, buf);
            return NULL_SEM;
        }

        return (sem_t)hsem;
    }

    hsem = CreateSemaphore(NULL, init, 0x7FFFFFFF, keystr);
    errnum = GetLastError();

    if(hsem == NULL) {
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_semget] CreateSemaphore(%s, %d, %d): "
               "%d %s", keystr, init, flags, errnum, buf);
        return NULL_SEM;
    }

    if(errnum == ERROR_ALREADY_EXISTS &&
       (flags & RADIPC_EXCL)) {
        CloseHandle(hsem);
        return EXIST_SEM;
    }

    return (sem_t)hsem;
}

int   rad_semop (sem_t id, int op, unsigned int msec)
{
    DWORD errnum, retval;
    char buf[256];
    HANDLE hsem = (HANDLE)id;

    rad_assert(op != 0);
    if(op == 0)
        return 0;

    if(op > 0) {

        if(!ReleaseSemaphore(hsem, op, NULL)) {
            errnum = GetLastError();
            strerror_r(errnum, buf, sizeof(buf));
            radlog(L_ERROR|L_CONS, "[rad_semop] ReleaseSemaphore(%d): "
                   "%d %s", op, errnum, buf);
            return -1;
        }

    } else {

        op = 0 - op;
        msec = (msec == RADIPC_WAIT_INFINITE ? INFINITE : msec);

        /* It is not possible to reduce the semaphore object count
        using ReleaseSemaphore, because lReleaseCount cannot be a
        negative number. To temporarily restrict or reduce access,
        create a loop in which you call the WaitForSingleObject
        function with a time-out interval of zero until the semaphore
        count has been reduced sufficiently. (Note that other threads
        can reduce the count while this loop is being executed.) To
        restore access, call ReleaseSemaphore with the release count
        equal to the number of times WaitForSingleObject was called in
        the loop. */

        while(op-- > 1) {
            if(WaitForSingleObject(hsem, 0) != WAIT_OBJECT_0) {
                radlog(L_ERROR|L_CONS, "[rad_semop] WaitForSingleObject: "
                       "Can not reduce the semaphore object count");
                return -1;
            }
        }

        retval = WaitForSingleObject(hsem, msec);
        switch(retval) {
        case WAIT_OBJECT_0:
            break;
        case WAIT_TIMEOUT:
            return 1;
        default:
            errnum = GetLastError();
            strerror_r(errnum, buf, sizeof(buf));
            radlog(L_ERROR|L_CONS, "[rad_semop] WaitForSingleObject: "
                   "%d %s", errnum, buf);
            return -1;
        }
    }

    return 0;
}

void  rad_semdel(sem_t id, int rmid)
{
    DWORD errnum;
    char buf[256];
    HANDLE hsem = (HANDLE)id;

    if(!CloseHandle(hsem)) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_semdel] CloseHandle: %d %s", errnum, buf);
    }
}

shm_t rad_shmget(shm_key_t key, size_t size, int flags)
{
    DWORD errnum;
    HANDLE hshm;
    char keystr[9];
    char buf[256];

    sprintf(keystr, "%08X", key);

    if(!(flags & RADIPC_CREAT)) { /* Open semantics */
        hshm = OpenFileMapping(FILE_MAP_WRITE|FILE_MAP_READ,
                               FALSE, keystr);
        errnum = GetLastError();

        if(hshm == NULL) {
            strerror_r(errnum, buf, sizeof(buf));
            radlog(L_ERROR|L_CONS, "[rad_shmget] OpenFileMapping(%s): "
                   "%d %s", keystr, errnum, buf);
            return NULL_SHM;
        }

        return (shm_t)hshm;
    }

    hshm = CreateFileMapping(INVALID_HANDLE_VALUE,
                             NULL,
                             PAGE_READWRITE,
                             0,
                             size,
                             keystr);
    errnum = GetLastError();

    if(hshm == NULL) {
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_shmget] CreateFileMapping(%s, %u, %d): "
               "%d %s", keystr, size, flags, errnum, buf);
        return NULL_SHM;
    }

    if(errnum == ERROR_ALREADY_EXISTS &&
       (flags & RADIPC_EXCL)) {
        CloseHandle(hshm);
        return EXIST_SHM;
    }

    return (shm_t)hshm;
}

void *rad_shmat (shm_t id, int flags)
{
    DWORD errnum;
    char buf[256];
    HANDLE hshm = (HANDLE)id;
    LPVOID paddr;

    paddr = MapViewOfFile(hshm,
        flags & RADIPC_SHM_RDONLY ? FILE_MAP_READ : FILE_MAP_ALL_ACCESS,
                          0,
                          0,
                          0);
    if(paddr == NULL) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_shmat] MapViewOfFile: %d %s", errnum, buf);
    }

    return paddr;
}

void  rad_shmdt (void *paddr)
{
    DWORD errnum;
    char buf[256];

    if(!UnmapViewOfFile(paddr)) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_shmdt] UnmapViewOfFile: "
               "%d %s", errnum, buf);
    }
}

void  rad_shmdel(shm_t id, int rmid)
{
    DWORD errnum;
    char buf[256];
    HANDLE hshm = (HANDLE)id;

    if(!CloseHandle(hshm)) {
        errnum = GetLastError();
        strerror_r(errnum, buf, sizeof(buf));
        radlog(L_ERROR|L_CONS, "[rad_shmdel] CloseHandle: "
               "%d %s", errnum, buf);
    }
}

#else /* !RAD_OS_WIN32 */

sem_t rad_semget(sem_key_t key, int init, int flags)
{
    int errnum;
    int sem;
    int fresh;
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } arg;

    if(flags & RADIPC_CREAT) {
        flags &= ~RADIPC_CREAT;
        flags |= IPC_CREAT;
    }

    if(flags & RADIPC_EXCL) {
        flags &= ~RADIPC_EXCL;
        flags |= IPC_EXCL;
    }

    if(!(flags & IPC_CREAT)) { /* Open semantics */
        sem = semget(key, 0, flags);
        if(sem == -1) {
            radlog(L_ERROR|L_CONS, "[rad_semget] semget(%08X): "
                   "%d %s", key, errno, strerror(errno));
            return NULL_SEM;
        }
        return (sem_t)sem;
    }

    /* Whether it is already exist */
    sem = semget(key, 1, flags|IPC_CREAT|IPC_EXCL);
    errnum = errno;
    if(sem == -1 && errnum != EEXIST) {
        radlog(L_ERROR|L_CONS, "[rad_semget] semget(%08X, %d, %d): "
               "%d %s", key, init, flags, errnum, strerror(errnum));
        return NULL_SEM;
    }

    if(sem == -1 && errnum == EEXIST) {
        fresh = 0;
        sem = semget(key, 1, flags);
        errnum = errno;
    } else {
        fresh = 1;
        sem = sem;
        errnum = 0;
    }

    if(sem == -1 && errnum != EEXIST) {
        radlog(L_ERROR|L_CONS, "[rad_semget] semget(%08X, %d, %d): "
               "%d %s", key, init, flags, errnum, strerror(errnum));
        return NULL_SEM;
    }

    if(sem == -1 && errnum == EEXIST) {
        return EXIST_SEM;
    }

    if(fresh) {
        arg.val = init;
        if(semctl(sem, 0, SETVAL, arg) == -1) {
            radlog(L_ERROR|L_CONS, "[rad_semget] semctl(%08X, %d, %d): "
                   "%d %s", key, init, flags, errno, strerror(errno));
            rad_semdel(sem, fresh);
            return NULL_SEM;
        }
    }

    return (sem_t)sem;
}

int   rad_semop (sem_t id, int op, unsigned int msec)
{
    struct sembuf sem_b;
    struct timeval tv, tv0;
    unsigned int elapse = 0;
    int sem = (int)id;

    rad_assert(op != 0);
    if(op == 0)
        return 0;

    sem_b.sem_num = 0;
    sem_b.sem_op = op;
    sem_b.sem_flg = SEM_UNDO|IPC_NOWAIT;

    if(msec == RADIPC_WAIT_INFINITE) {
        sem_b.sem_flg &= ~IPC_NOWAIT;

        if(semop(sem, &sem_b, 1) == -1) {
            radlog(L_ERROR|L_CONS, "[rad_semop] semop(%d): "
                   "%d %s", op, errno, strerror(errno));
            return -1;
        }

        return 0;
    }

    if(msec == 0) {
        if(semop(sem, &sem_b, 1) == 0) {
            return 0;
        } else if(errno == EAGAIN) {
            return 1;
        } else {
            radlog(L_ERROR|L_CONS, "[rad_semop] semop(%d): "
                   "%d %s", op, errno, strerror(errno));
            return -1;
        }
    }

    if(msec > 0xFFFFFFFF/1000 /* 4294967 */) {
        radlog(L_ERROR|L_CONS, "[rad_semop] msec is overflow");
        return -1;
    }

    msec *= 1000;
    gettimeofday(&tv0, NULL);

    do {
        if(semop(sem, &sem_b, 1) == 0) {
            return 0;
        }

        if(errno == EAGAIN) {
            Sleep(1);
            gettimeofday(&tv, NULL);
            elapse = (tv.tv_sec-tv0.tv_sec)*1000*1000 +
                (tv.tv_usec-tv0.tv_usec);
        } else {
            radlog(L_ERROR|L_CONS, "[rad_semop] semop(%d): "
                   "%d %s", op, errno, strerror(errno));
            return -1;
        }
    } while(elapse < msec);

    return 1;
}

void  rad_semdel(sem_t id, int rmid)
{
    int sem = (int)id;

    if(!rmid)
        return;

    if(semctl(sem, 0, IPC_RMID) == -1) {
        radlog(L_ERROR|L_CONS, "[rad_semdel] semctl: %d %s",
               errno, strerror(errno));
    }
}

shm_t rad_shmget(shm_key_t key, size_t size, int flags)
{
    int errnum;
    int shm;

    if(flags & RADIPC_CREAT) {
        flags &= ~RADIPC_CREAT;
        flags |= IPC_CREAT;
    }

    if(flags & RADIPC_EXCL) {
        flags &= ~RADIPC_EXCL;
        flags |= IPC_EXCL;
    }

    if(!(flags & IPC_CREAT)) { /* Open semantics */
        shm = shmget(key, 0, flags);
        if(shm == -1) {
            radlog(L_ERROR|L_CONS, "[rad_shmget] shmget(%08X): "
                   "%d %s", key, errno, strerror(errno));
            return NULL_SHM;
        }
        return (shm_t)shm;
    }

    shm = shmget(key, size, flags);
    errnum = errno;
    if(shm == -1 && errnum != EEXIST) {
        radlog(L_ERROR|L_CONS, "[rad_shmget] shmget(%08X, %u, %d): "
               "%d %s", key, size, flags, errnum, strerror(errnum));
        return NULL_SHM;
    }

    if(shm == -1 && errnum == EEXIST) {
        return EXIST_SHM;
    }

    return (shm_t)shm;
}

void *rad_shmat (shm_t id, int flags)
{
    void *paddr;
    int shm = (int)id;

    paddr = shmat(shm, NULL,
                  flags & RADIPC_SHM_RDONLY ? SHM_RDONLY : 0);
    if((int)paddr == -1) {
        radlog(L_ERROR|L_CONS, "[rad_shmat] shmat: %d %s",
               errno, strerror(errno));
        paddr = NULL;
    }

    return paddr;
}

void  rad_shmdt (void *paddr)
{
    if(shmdt(paddr) == -1) {
        radlog(L_ERROR|L_CONS, "[rad_shmdt] shmdt: %d %s",
               errno, strerror(errno));
    }
}

void  rad_shmdel(shm_t id, int rmid)
{
    int shm = (int)id;

    if(!rmid)
        return;

    if(shmctl(shm, IPC_RMID, NULL) == -1) {
        radlog(L_ERROR|L_CONS, "[rad_shmdel] shmctl: %d %s",
               errno, strerror(errno));
    }
}

#endif /* !RAD_OS_WIN32 */

#define QUEUE_DATABUF_SIZE 8192

typedef struct _QUEUE_DATA {
    size_t datalen;
    char databuf[QUEUE_DATABUF_SIZE];
} QUEUE_DATA;

typedef struct _QUEUE {
    lrad_shift_entry entry;
    QUEUE_DATA data[0];
} QUEUE;

typedef struct _QUEUE_NODE {
    long type;
} QUEUE_NODE;

typedef struct _QUEUE_MAP {
    size_t q_maxsz;
    size_t qm_maxsz;
    QUEUE_NODE qs[0];
} QUEUE_MAP;

#define QUEUE_MAP_MAXSIZE 5
#define QUEUE_MAXSIZE 100

#define QUEUE(qm, i, qm_maxsz, q_maxsz)                         \
    ((QUEUE*)(((char*)qm) + vsizeof(QUEUE_MAP, qs, qm_maxsz) +  \
              i * vsizeof(QUEUE, data, q_maxsz)))

msq_t* rad_msgget(const msq_key_t* key, int flags)
{
    int qm_maxsz = 0, q_maxsz = 0;
    size_t shmsz = 0;
    char* p;
    msq_t* msq;
    QUEUE_MAP* qm;
    sem_t sem;
    int fresh;

    if(!(flags & RADIPC_CREAT)) { /* Open semantics */
        msq = (msq_t*)rad_malloc(sizeof(msq_t));
        msq->sem = rad_semget(key->semkey, 0, flags);
        if(msq->sem == NULL_SEM) {
            rad_free(msq);
            return NULL;
        }
        msq->shm = rad_shmget(key->shmkey, 0, flags);
        if(msq->shm == NULL_SHM) {
            rad_semdel(msq->sem, 0);
            rad_free(msq);
            return NULL;
        }
        msq->ptr = rad_shmat(msq->shm, 0);
        if(!msq->ptr) {
            rad_shmdel(msq->shm, 0);
            rad_semdel(msq->sem, 0);
            rad_free(msq);
            return NULL;
        }
        return msq;
    }

    /* Whether it is already exist */
    sem = rad_semget(key->semkey, 1, flags|RADIPC_CREAT|RADIPC_EXCL);
    if(sem == NULL_SEM) {
        return NULL;
    }

    /* Init msq_t */
    msq = (msq_t*)rad_malloc(sizeof(msq_t));
    if(sem == EXIST_SEM) {
        fresh = 0;
        msq->sem = rad_semget(key->semkey, 1, flags);
    } else {
        fresh = 1;
        msq->sem = sem;
    }
    if(msq->sem == NULL_SEM || msq->sem == EXIST_SEM) {
        rad_free(msq);
        return msq->sem == NULL_SEM ? NULL : EXIST_MSQ;
    }

    if(fresh) {
        /* Get maxsize from env */
        p = getenv("QUEUE_MAP_MAXSIZE");
        if(p)
            qm_maxsz = atoi(p);
        p = getenv("QUEUE_MAXSIZE");
        if(p)
            q_maxsz = atoi(p);

        /* Get good qm_maxsz and q_maxsz */
        if(qm_maxsz <= 0)
            qm_maxsz = QUEUE_MAP_MAXSIZE;
        if(q_maxsz <= 0)
            q_maxsz = QUEUE_MAXSIZE;

        DEBUG("[rad_msgget] queue_map_maxsize(=%d)", qm_maxsz);
        DEBUG("[rad_msgget] queue_maxsize(=%d)", q_maxsz);

        /* Calculate total shm size */
        shmsz = vsizeof(QUEUE_MAP, qs, qm_maxsz) +
            qm_maxsz * vsizeof(QUEUE, data, q_maxsz);

        DEBUG("[rad_msgget] total_shm_size(=%u) == vsizeof(QUEUE_MAP)(=%u) "
              "+ %d * vsizeof(QUEUE)(=%u)",
              shmsz, vsizeof(QUEUE_MAP, qs, qm_maxsz),
              qm_maxsz, vsizeof(QUEUE, data, q_maxsz));
    } else {
        shmsz = 1;
    }

    msq->shm = rad_shmget(key->shmkey, shmsz, flags);
    if(msq->shm == NULL_SHM || msq->shm == EXIST_SHM) {
        rad_semdel(msq->sem, fresh);
        rad_free(msq);
        return NULL;
    }
    msq->ptr = rad_shmat(msq->shm, 0);
    if(!msq->ptr) {
        rad_shmdel(msq->shm, fresh);
        rad_semdel(msq->sem, fresh);
        rad_free(msq);
        return NULL;
    }

    /* Init QUEUE_MAP */
    if(fresh) {
        qm = (QUEUE_MAP*)msq->ptr;
        memset(qm, 0, shmsz);
        qm->qm_maxsz = qm_maxsz;
        qm->q_maxsz = q_maxsz;
    }

    return msq;
}

int    rad_msgsnd(const msq_t* msqid,
                  const void *msg_ptr, size_t msg_size, long msg_type)
{
    QUEUE* q;
    QUEUE_MAP* qm;
    size_t i, len, n;

    rad_assert(msg_type > 0);
    rad_assert(msg_size != 0 && msg_size <= QUEUE_DATABUF_SIZE);
    if(msg_type <= 0 ) {
        radlog(L_ERROR|L_CONS, "[rad_msgsnd] msg_type(=%ld) < 0", msg_type);
        return -1;
    }
    if(msg_size == 0 || msg_size > QUEUE_DATABUF_SIZE) {
        radlog(L_ERROR|L_CONS, "[rad_msgsnd] msg_size(=%u) > %u or == 0",
               msg_size, QUEUE_DATABUF_SIZE);
        return -1;
    }

    qm = (QUEUE_MAP*)msqid->ptr;
    if(rad_semop(msqid->sem, -1, RADIPC_WAIT_INFINITE) != 0)
        return -1;

    for(i=0; i<qm->qm_maxsz; ++i)
        if(qm->qs[i].type == msg_type)
            break;

    if(i == qm->qm_maxsz)
        for(i=0; i<qm->qm_maxsz; ++i)
            if(qm->qs[i].type == 0) {
                qm->qs[i].type = msg_type;
                break;
            }

    if(i == qm->qm_maxsz) {
        radlog(L_ERROR|L_CONS, "[rad_msgsnd] queue_map full, "
               "msg_type(=%ld) can not be sent", msg_type);
        rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
        return 2;
    }

    q = QUEUE(qm, i, qm->qm_maxsz, qm->q_maxsz);
    len = LRAD_SHIFT_LENGTH(q->entry.fr, q->entry.to, qm->q_maxsz);
    if(len == qm->q_maxsz) {
        radlog(L_ERROR|L_CONS, "[rad_msgsnd] queue(type=%ld) full", msg_type);
        rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
        return 1;
    }

    n = lrad_shift_get_push_back_index(&q->entry.fr, &q->entry.to, qm->q_maxsz);
    q->data[n].datalen = msg_size;
    memcpy(q->data[n].databuf, msg_ptr, msg_size);

    rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
    return 0;
}

int    rad_msgrcv(const msq_t* msqid,
                  void *msg_ptr, size_t msg_size, long msg_type)
{
    QUEUE* q;
    QUEUE_MAP* qm;
    size_t i, n, len;
    unsigned int fr, to;

    qm = (QUEUE_MAP*)msqid->ptr;
    if(rad_semop(msqid->sem, -1, RADIPC_WAIT_INFINITE) != 0)
        return -1;

    if(msg_type > 0) {
        for(i=0; i<qm->qm_maxsz; ++i)
            if(qm->qs[i].type == msg_type)
                break;
    } else if(msg_type < 0) {
        for(i=0; i<qm->qm_maxsz; ++i)
            if(qm->qs[i].type > 0 && qm->qs[i].type <= 0-msg_type)
                break;
    } else {
        for(i=0; i<qm->qm_maxsz; ++i)
            if(qm->qs[i].type != 0)
                break;
    }

    if(i == qm->qm_maxsz) { /* empty */
        rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
        return 0;
    }

    q = QUEUE(qm, i, qm->qm_maxsz, qm->q_maxsz);
    fr = q->entry.fr, to = q->entry.to;
    n = lrad_shift_get_pop_front_index(&q->entry.fr, &q->entry.to, qm->q_maxsz);
    rad_assert(n != LRAD_SHIFT_TOINFI); /* will never be empty */
    if(n == LRAD_SHIFT_TOINFI) { /* empty */
        rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
        return 0;
    }

    if(msg_size < q->data[n].datalen) {
        radlog(L_ERROR|L_CONS, "[rad_msgrcv] msg_size(=%u) < %u",
               msg_size, q->data[n].datalen);
        q->entry.fr = fr, q->entry.to = to; /* roll back */
        rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
        return -1;
    }
    memcpy(msg_ptr, q->data[n].databuf, q->data[n].datalen);

    len = LRAD_SHIFT_LENGTH(q->entry.fr, q->entry.to, qm->q_maxsz);
    if(len == 0) /* empty afterward */
        qm->qs[i].type = 0;

    rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
    return q->data[n].datalen;
}

void   rad_msgdel(msq_t** pmsqid, int rmid)
{
    msq_t* msq = *pmsqid;

    if(!msq)
        return;

    rad_shmdt(msq->ptr);
    rad_shmdel(msq->shm, rmid);
    rad_semdel(msq->sem, rmid);
    rad_free(msq);

    *pmsqid = NULL;
}

void   rad_msgdmp(const msq_t* msqid)
{
    QUEUE* q;
    QUEUE_MAP* qm;
    unsigned int i, len;

    qm = (QUEUE_MAP*)msqid->ptr;
    if(rad_semop(msqid->sem, -1, RADIPC_WAIT_INFINITE) != 0)
        return;

    for(i=0; i<qm->qm_maxsz; ++i) {
        q = QUEUE(qm, i, qm->qm_maxsz, qm->q_maxsz);
        len = LRAD_SHIFT_LENGTH(q->entry.fr, q->entry.to, qm->q_maxsz);
        printf("QUEUE[%u] type=%ld cnt/max=%u/%u\n",
               i, qm->qs[i].type, len, qm->q_maxsz);
    }

    rad_verify(rad_semop(msqid->sem, 1, RADIPC_WAIT_INFINITE) == 0);
}
