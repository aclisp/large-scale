/* -*- mode: c -*- */

/* Author:   Huanghao@cellcom
 * Date:     2005/10/20 14:24:27
 * Revision: 0.0.1
 * Function:
 * Usage:
 */

#ifndef __RAD_IPC_H__
#define __RAD_IPC_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define RADIPC_CREAT 01000
#define RADIPC_EXCL  02000

typedef int          sem_key_t;
typedef unsigned int sem_t;
#define NULL_SEM  ((sem_t)-1)
#define EXIST_SEM ((sem_t)-2)
#define RADIPC_WAIT_INFINITE 0xFFFFFFFF

sem_t rad_semget(sem_key_t key, int init, int flags);
int   rad_semop (sem_t id, int op, unsigned int msec);
void  rad_semdel(sem_t id, int rmid);

typedef int          shm_key_t;
typedef unsigned int shm_t;
#define NULL_SHM  ((shm_t)-1)
#define EXIST_SHM ((shm_t)-2)
#define RADIPC_SHM_RDONLY 010000

shm_t rad_shmget(shm_key_t key, size_t size, int flags);
void *rad_shmat (shm_t id, int flags);
void  rad_shmdt (void *paddr);
void  rad_shmdel(shm_t id, int rmid);

typedef struct _msq_key_t {
    sem_key_t semkey;
    shm_key_t shmkey;
} msq_key_t;

typedef struct _msq_t {
    sem_t sem;
    shm_t shm;
    void *ptr;
} msq_t;

#define EXIST_MSQ ((msq_t*)0xFFFFFFFF)

msq_t* rad_msgget(const msq_key_t* key, int flags);
int    rad_msgsnd(const msq_t* msqid,
                  const void *msg_ptr, size_t msg_size, long msg_type);
int    rad_msgrcv(const msq_t* msqid,
                  void *msg_ptr, size_t msg_size, long msg_type);
void   rad_msgdel(msq_t** pmsqid, int rmid);

void   rad_msgdmp(const msq_t* msqid);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __RAD_IPC_H__ */
