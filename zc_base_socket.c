#if defined(WIN32) || defined(WIN64) //WIN下面头文件
#include <sdkddkver.h>
#if  _WIN32_WINNT < 0x0600
#error Windows version lower vista
#endif
#else //非WIN平台的头文件
#include <poll.h>
#endif
#include "zc_base_socket.h"



int zc_base_select(zc_socket_t sock,int mask, const long ms, int *retmask)
{
	int ret = 0;

	fd_set readfds,writefds,exceptfds;
	//测试端口 是否可读 是否可写
	struct timeval timeout = {0};

	if ( ms != -1)
	{
		timeout.tv_sec	= ms / 1000;
		timeout.tv_usec	= (ms % 1000) * 1000;
	}

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);

	if (mask & ZC_BASE_POLL_READABLE) 
	{
		FD_SET(sock, &readfds);
	}
	if (mask & ZC_BASE_POLL_WRITABLE) 
	{
		FD_SET(sock, &writefds);
	}

	ret = select(sock + 1, &readfds, &writefds, &exceptfds, &timeout);
	if ( ret < 0)// 出错 
	{
#if defined(WIN32) || defined(WIN64)
		int err = WSAGetLastError();
		if ( err != WSAEINTR )//被中断
#else
		int err = errno;
		if ( err != EINTR )//被中断
#endif
		{
			return ZC_BASE_POLL_ERR;
		}
		return ZC_BASE_POLL_SUC;
	}
	if (ret == 0)//超时  不可读 不可写
	{
		return ZC_BASE_POLL_SUC;
	}
	//判断是否可读或可写
	if (mask & ZC_BASE_POLL_READABLE)
	{
		if ( FD_ISSET(sock,&readfds)  )//有可读
		{	
			*retmask |= ZC_BASE_POLL_READABLE;
		}
	}
	if (mask & ZC_BASE_POLL_WRITABLE)
	{
		if( FD_ISSET(sock, &writefds) )//可写
		{
			*retmask |= ZC_BASE_POLL_WRITABLE;
		}
	}
	return ret;
}


int zc_base_poll(zc_socket_t sock,int mask, const long ms, int *retmask)
{
	int  retval = 0;
	struct pollfd pfd;

	memset(&pfd, 0, sizeof(pfd));
	pfd.fd = sock;

	if (mask & ZC_BASE_POLL_READABLE) 
	{
		pfd.events |= POLLIN;
	}

	if (mask & ZC_BASE_POLL_WRITABLE) 
	{
		pfd.events |= POLLOUT;
	}
#if defined(WIN32) || defined(WIN64) 
	 retval = WSAPoll(&pfd, 1, ms);
#else
	 retval = poll(&pfd, 1, ms);
#endif
	if ( retval > 0 ) 
	{
		if (pfd.revents & POLLIN)
		{
			*retmask |= ZC_BASE_POLL_READABLE;
		}
		if (pfd.revents & POLLOUT)
		{
			*retmask |= ZC_BASE_POLL_WRITABLE;
		}
		if (pfd.revents & POLLERR)
		{
			*retmask |= ZC_BASE_POLL_READABLE;
			*retmask |= ZC_BASE_POLL_WRITABLE;
		}
		if (pfd.revents & POLLHUP) 
		{
			*retmask |= ZC_BASE_POLL_READABLE;
			*retmask |= ZC_BASE_POLL_WRITABLE;
		}
	} 
	else if(retval == 0 )
	{
		return ZC_BASE_POLL_SUC;
	}
	else
	{

#if defined(WIN32) || defined(WIN64)
		int err = WSAGetLastError();
		if ( err != WSAEINTR )//被中断
#else
		int err = errno;
		if ( err != EINTR )//被中断
#endif
		{
			return ZC_BASE_POLL_ERR;
		}
		return ZC_BASE_POLL_SUC;
	}
	return retval;
}