#ifndef __ZC_BASE_COMMON_H__
#define __ZC_BASE_COMMON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if defined(WIN32) || defined(WIN64)
#	if defined(ZC_BASE_LIBRARY_EXPORTS)
#		define	ZC_BASE_API __declspec(dllexport) 
#	else
#		define	ZC_BASE_API __declspec(dllimport) 
#	endif
#else
#	define	ZC_BASE_API 
#endif


#endif