#ifndef __COMMON_H__
#define __COMMON_H__

#if defined(_USRDLL_CORE)
#define CORE_DLL     __declspec(dllexport)
#else         /* use a DLL library */
#define CORE_DLL     __declspec(dllimport)
#endif

typedef	char			int8;
typedef	unsigned char	uint8;
typedef	short			int16;
typedef	unsigned short	uint16;
typedef	int				int32;
typedef	unsigned int	uint32;
typedef	unsigned int	uint;
typedef	long			int64;
typedef	unsigned long	uint64;

#define CORE_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CORE_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define CORE_SAFE_FREE(p)             do { if(p) { free(p); (p) = nullptr; } } while(0)

#endif