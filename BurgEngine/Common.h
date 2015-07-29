#ifndef __COMMON_H
#define __COMMON_H

//#include <debugapi.h>
#include <Windows.h>
#include <memory.h>
#include <string>
#include <vector>

//===========================================================
//                     TYPEDEFS
//===========================================================

typedef char				s8;
typedef short				s16;
typedef int					s32;
typedef __int64				s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned int		uint;
typedef unsigned __int64	u64;

typedef float				f32;
typedef double				f64;

//===========================================================
//                HELPER FUNCTIONS
//===========================================================



template <typename T>
inline void SafeDelete(T*& ptr)
{
	delete ptr;
	ptr = nullptr;
}

// ----------------------------------------------------------

template <typename T>
inline void SafeDeleteArray(T*& ptr)
{
	delete[] ptr;
	ptr = nullptr;
}

// ----------------------------------------------------------

#define NONCOPYABLE(type)\
	type(const type&) = delete; \
	type& operator=(const type&) = delete;

// ----------------------------------------------------------

#if defined(_DEBUG)
#define LOG(format, ...)\
{\
	{\
		char buffer[1024]; \
		sprintf_s(buffer, 1024, (#format), __VA_ARGS__); \
		std::string message; \
		message += (buffer); \
		message += "\n"; \
		OutputDebugStringA(message.c_str()); \
	}\
}
#else
	#define LOG(format, ...)
#endif

// ----------------------------------------------------------

#if defined(_DEBUG)
#define ASSERT(condition, format, ...)\
{\
	if (!(condition))\
	{\
		LOG(format, __VA_ARGS__)\
		DebugBreak(); \
	}\
}
#else
	#define ASSERT(condition, format, ...)
#endif



#endif // #define __COMMON_H