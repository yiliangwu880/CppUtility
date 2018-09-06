
#pragma once

#define EPOLL_CTL_ADD 1
#define EPOLL_CTL_MOD 2
#define EPOLL_CTL_DEL 3


inline int epoll_create(int size){return 0;};
inline int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event){return 0;};
inline int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout){return 0;};