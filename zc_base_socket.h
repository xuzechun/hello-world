#ifndef __ZC_BASE_SOCKET_H__
#define __ZC_BASE_SOCKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined (WIN32) || defined (WIN64)
#include <winsock2.h>

#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "zc_base_common.h"

#ifndef zc_socklen_t
#	if defined(__hpux) || defined(HPUX)
#		if defined(_XOPEN_SOURCE_EXTENDED)
#			define zc_socklen_t	socklen_t
#		else
#			define zc_socklen_t	int
#		endif
#	elif  defined(WIN32) || defined(WIN64)
#		define zc_socklen_t	int
#	else
#		define zc_socklen_t	socklen_t
#	endif
#else
#error "redefine zc_socklen_t error"
#endif


#if defined(WIN32) || defined(WIN64)
#	ifndef zc_socket_t 
#		define zc_socket_t		SOCKET
#	else
#		error "redefine zc_socket_t error"
#	endif
#else
#	ifndef zc_socket_t 
#		define zc_socket_t		int
#	else
#		error "redefine zc_socket_t error"
#	endif
#endif


#ifndef ZC_BASE_POLL_SUC 
#define ZC_BASE_POLL_SUC 0
#endif

#ifndef ZC_BASE_POLL_ERR 
#define ZC_BASE_POLL_ERR -1
#endif

#ifndef ZC_BASE_POLL_NONE 
#define ZC_BASE_POLL_NONE 0
#endif
#ifndef ZC_BASE_POLL_READABLE 
#define ZC_BASE_POLL_READABLE 1
#endif
#ifndef ZC_BASE_POLL_WRITABLE 
#define ZC_BASE_POLL_WRITABLE 2
#endif

#ifdef	__cplusplus
extern "C" {
#endif
/*
 *
 */
ZC_BASE_API int zc_base_select(zc_socket_t sock,int mask, const long ms, int *retmask);

ZC_BASE_API int zc_base_poll(zc_socket_t sock,int mask, const long ms, int *retmask);
#ifdef	__cplusplus
}:
#endif

#endif