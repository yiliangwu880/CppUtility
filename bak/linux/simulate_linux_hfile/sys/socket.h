/*
只为 vs编译通过，不能运行
*/

#pragma once
//#include <WinSock.h>


#define F_GETFL 0
#define F_SETFL 0
#define O_NONBLOCK 0
#define AFINET 0
#define SOL_SOCKET 0
#define SO_REUSEADDR 0
#define SOCK_STREAM 0
#define AF_INET 0

#define INADDR_ANY 0

typedef unsigned short u_short;

typedef struct in_addr {
    union {
        struct { char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { unsigned short s_w1,s_w2; } S_un_w;
        unsigned long S_addr;
    } S_un;
#define s_addr  S_un.S_addr 
#define s_host  S_un.S_un_b.s_b2    // host on imp
#define s_net   S_un.S_un_b.s_b1    // network
#define s_imp   S_un.S_un_w.s_w2    // imp
#define s_impno S_un.S_un_b.s_b4    // imp #
#define s_lh    S_un.S_un_b.s_b3    // logical host
} IN_ADDR, *PIN_ADDR;

struct sockaddr {
    u_short sa_family;              /* address family */
    char    sa_data[14];            /* up to 14 bytes of direct address */
};

struct  hostent {
    char     * h_name;           /* official name of host */
    char     *  * h_aliases;  /* alias list */
    short   h_addrtype;             /* host address type */
    short   h_length;               /* length of address */
    char     *  * h_addr_list; /* list of addresses */
#define h_addr  h_addr_list[0]          /* address, for backward compat */
};



typedef int socklen_t;
inline int fcntl(int, int){return 0;};
inline int fcntl(int, int, int){return 0;};
inline int close(int){return 0;};
inline int send(int sockfd, const void *buf, size_t len, int flags){return 0;};
inline int recv(int sockfd, void *buf, size_t len, int flags){return 0;};
inline int write(int sockfd, const void *buf, size_t len){return 0;};
inline int read(int sockfd, void *buf, size_t len){return 0;};

inline int   bind (
     int s,
   void *addr,
   int namelen){return 0;};

inline char *inet_ntoa( struct in_addr in){return 0;};

inline int   accept (
     int s,
    void *addr,
     int  *addrlen){return 0;};

struct hostent;
inline struct hostent  *gethostbyname(
     const char  * name
     ){return 0;};
inline int   gethostname (
    char  * name,
     int namelen){return 0;};

inline int socket (
     int af,
     int type,
     int protocol){return 0;};

inline int  getsockopt (
     int s,
     int level,
     int optname,
    void  * optval,
     int  *optlen){return 0;};

inline unsigned long   htonl (  unsigned long hostlong){return 0;};

inline unsigned short   htons ( unsigned short hostshort){return 0;};

inline unsigned long   inet_addr ( const char  * cp){return 0;};

inline int   connect (
     int s,
    void *name,
     int namelen){return 0;};
//char  * inet_ntoa ( struct in_addr in){return 0;};

inline int   listen (
     int s,
     int backlog){return 0;};

inline int  setsockopt (
     int s,
     int level,
     int optname,
    const void * optval,
    int optlen){return 0;};

inline int  getpeername (
    int s,
    void *name,
    int  * namelen){return 0;};
inline int  getsockname (
     int s,
    void *name,
     int  * namelen){return 0;};



