#pragma once

#ifdef WIN32
  #include <windows.h>
  #define RTASSERT(x) _ASSERTE((x))
#else
  #ifdef NDEBUG
	#define RTASSERT(x)
  #else
	#include <assert.h>
	#define RTASSERT(x) assert(x)
  #endif
#endif

#include <set>
#include <list>
#include <deque>
#include <map>
#include <vector>

void RTTRACE(const char *format, ...);

#include "GameRunTime.h"
#include "SystemUnknownBase.h"
#include "EventBase.h"
#include "Persistency.h"
#include "PersistentTypes.h"
#include "ConfigFile.h"
#include "SystemObjectBase.h"
#include "SystemModuleHelpers.h"
#include "SystemObjectWrapper.h"
#include "SystemLoaderHelper.h"

template<typename ITF> 
void GetSystemObjects(std::string sSystem,std::vector<ITF *> *pvObjects)
{
	ISystemManager  *piSystemManager=GetSystemManager();
	if(piSystemManager)
	{
		ISystem *piSystem=piSystemManager->GetSystem(sSystem);
		if(piSystem)
		{
			std::vector<ISystemObject*> vObjects;
			piSystem->GetObjects(&vObjects);
			for(unsigned long x=0;x<vObjects.size();x++)
			{
				ISystemObject *piObject=vObjects[x];
				ITF *piITF=QI(ITF,piObject);
				if(piITF){pvObjects->push_back(piITF);}
				REL(piObject);
			}
		}
		REL(piSystem);
	}
	REL(piSystemManager);
}