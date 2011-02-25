#ifndef _VECTOR_LIB_UTILITIES_
#define _VECTOR_LIB_UTILITIES_

#include <vector>

template<typename T>
T* BufferFromVector(std::vector< T > *pContainer)
{
	T* pBuffer=new T[pContainer->size()];
	T* pBufferCursor=pBuffer;
	typename std::vector<T>::iterator i;
	for(i=pContainer->begin();i!=pContainer->end();i++,pBufferCursor++)
	{
		*pBufferCursor=*i;
	}
	return pBuffer;
}

void SkipCommentsStringsAndSpaces(char *pBuffer,unsigned int *pOffset,unsigned int bufLen);
void ReplaceExtension(char *pFileName,const char *pExt);
void GetExtension(const char *pFileName,char *pExt);
void GetFileFolder(const char *pFilePath,char *pFolder);
void GetFileName(const char *pFilePath,char *pFileName);

#ifdef WIN32
#define strcasecmp _stricmp
#endif

#endif