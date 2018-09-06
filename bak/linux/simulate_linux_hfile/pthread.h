
#pragma once

typedef unsigned pthread_t;
typedef struct thread_attr {
    unsigned dwStackSize ;
    unsigned dwCreatingFlag ;
} pthread_attr_t ;
typedef unsigned pthread_mutex_t;
typedef unsigned pthread_cond_t;
const pthread_mutex_t PTHREAD_MUTEX_INITIALIZER = 0;
const pthread_cond_t PTHREAD_COND_INITIALIZER = 0;

typedef void* (*start_rtn)(void*);

inline void pthread_exit(void *a){};
inline int pthread_join(pthread_t thread, void **value_ptr){return 0;};
inline int pthread_cancel(pthread_t thread){return 0;};
inline int pthread_create(pthread_t *tidp, const pthread_attr_t *attr, start_rtn fun, void *arg){return 0;};
inline int pthread_kill(pthread_t thread, int sig){return 0;};

inline int pthread_cond_signal(pthread_cond_t *cond);

inline void pthread_mutex_lock(pthread_mutex_t *m_mutex);
inline void pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *m_mutex);
inline void pthread_mutex_unlock(pthread_mutex_t *m_mutex);
inline void pthread_cond_init(pthread_cond_t *cond, void *p);
inline void pthread_mutex_init(pthread_mutex_t *m_mutex, void *p);