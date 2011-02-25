#ifndef _PLATFORM_DEPENDENT_
#define _PLATFORM_DEPENDENT_

#ifdef WIN32

	#define _WIN32_WINNT 0x0501
	#define WINVER 0x0501
	#define WIN32_LEAN_AND_MEAN

	#include <windows.h>
	// warning C4250: 'xxx' : inherits 'CSystemObjectBase::AddReference' via dominance
	#pragma warning ( disable : 4250 )

	#define PATH_SEPARATOR "\\"
	#define PATH_SEPARATOR_CHAR '\\'
	#define strcasecmp _stricmp

	#define RTASSERT(x) _ASSERTE((x))
	#define __UNUSED_ATTRIB__
#else
	#include <libgen.h>

	#define MAX_PATH 260
	#define PATH_SEPARATOR "/"
	#define PATH_SEPARATOR_CHAR '/'

	#ifdef NDEBUG
		#define RTASSERT(x)
	#else
		#include <assert.h>
		#define RTASSERT(x) assert(x)
	#endif

	#define __UNUSED_ATTRIB__ __attribute__((unused))
#endif

void ReplaceExtension(char *pFileName,const char *pExt);
void GetExtension(const char *pFileName,char *pExt);
void GetFileFolder(const char *pFilePath,char *pFolder);
void GetFileName(const char *pFilePath,char *pFileName);

unsigned int GetTimeStamp();

void RTTRACE(const char *format, ...);

#endif