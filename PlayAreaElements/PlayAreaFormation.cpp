#include "./stdafx.h"
#include "PlayAreaElementBase.h"
#include "PlayAreaFormation.h"
#include "GameGraphics.h"

CPlayAreaFormation::CPlayAreaFormation()
{
	m_piFormation=NULL;
	m_bAlreadyOverPoint=false;
	m_bUsingAlternative=false;
	m_eConditionType=ePlayerStateCondition_None;
	m_nConditionValue=0;
	g_FrameManagerSingleton.AddRef();
	g_EntityManagerWrapper.AddRef();
}

CPlayAreaFormation::~CPlayAreaFormation()
{
	g_FrameManagerSingleton.Release();
	g_EntityManagerWrapper.Release();
}

bool CPlayAreaFormation::Init(std::string sClass,std::string sName,ISystem *piSystem)
{
	bool bOk=CPlayAreaElementBase::Init(sClass,sName,piSystem);
	return bOk;
}

void CPlayAreaFormation::Destroy()
{
	Deactivate();
	m_FormationType.Detach();
	m_BonusType.Detach();
	CPlayAreaElementBase::Destroy();
}

bool CPlayAreaFormation::ProcessFrame(CVector vPlayPosition,SPlayAreaInfo *pAreaInfo,unsigned int dwCurrentTime,double dInterval)
{
	bool bOverPoint=Util_IsInPlayArea(m_vPosition,pAreaInfo);
	if(!m_bActive)
	{ 
		if(!m_bAlreadyOverPoint)
		{
			if(bOverPoint){Activate(dwCurrentTime);}
			if(m_piFormation){m_piFormation->ProcessFrame(dwCurrentTime,0.0);}
		}
	}
	else
	{
		if(m_piFormation){m_piFormation->ProcessFrame(dwCurrentTime,dInterval);}
	}
	m_bAlreadyOverPoint=bOverPoint;
	return m_bActive;
}

void CPlayAreaFormation::Activate(unsigned int dwCurrentTime)
{
	CPlayAreaElementBase::Activate(dwCurrentTime);
	m_bUsingAlternative=false;
	if(m_eConditionType==ePlayerStateCondition_MaxPrimaryWeaponLevel)
	{
		IEntity *piPlayerEntity=NULL;
		IWeapon *piWeapon=NULL;
		if(g_EntityManagerWrapper.m_piInterface){piPlayerEntity=g_EntityManagerWrapper.m_piInterface->FindEntity("Player");}
		if(piPlayerEntity){piWeapon=piPlayerEntity->GetWeapon(0);}
		if(piWeapon){m_bUsingAlternative=piWeapon->GetCurrentLevel()<=m_nConditionValue;}
	}

	if(m_bUsingAlternative && m_AlternativeFormationType.m_piFormationType)
	{
		m_piFormation=m_AlternativeFormationType.m_piFormationType->CreateInstance(m_vPosition,dwCurrentTime);
		SUBSCRIBE_TO_CAST(m_piFormation,IFormationEvents);
	}
	else if(m_FormationType.m_piFormationType)
	{
		m_piFormation=m_FormationType.m_piFormationType->CreateInstance(m_vPosition,dwCurrentTime);
		SUBSCRIBE_TO_CAST(m_piFormation,IFormationEvents);
	}

}

void CPlayAreaFormation::Deactivate()
{
	UNSUBSCRIBE_FROM_CAST(m_piFormation,IFormationEvents);
	REL(m_piFormation);
	CPlayAreaElementBase::Deactivate();
}

void CPlayAreaFormation::Reset()
{
	CPlayAreaElementBase::Reset();
	m_bAlreadyOverPoint=false;
}

void CPlayAreaFormation::OnFormationKilled(ISystemObject *piFormation,IEntity *piLastEntity)
{
	if(m_bUsingAlternative && m_AlternativeBonusType.m_piEntityType)
	{
		m_AlternativeBonusType.m_piEntityType->CreateInstance(piLastEntity,g_FrameManagerSingleton.m_piInterface->GetCurrentTime());
	}
	else if(m_BonusType.m_piEntityType)
	{
		m_BonusType.m_piEntityType->CreateInstance(piLastEntity,g_FrameManagerSingleton.m_piInterface->GetCurrentTime());
	}

	Deactivate();
}

void CPlayAreaFormation::OnFormationRemoved(ISystemObject *piFormation){Deactivate();}

void CPlayAreaFormation::DesignRender( IGenericRender *piRender ,bool bSelected)
{
	if(m_FormationType.m_piFormationType){m_FormationType.m_piFormationType->DesignRender(piRender,m_vPosition,Origin,bSelected);}
}

void CPlayAreaFormation::DesignGetBBox( CVector *pvMins,CVector *pvMaxs )
{
	if(m_FormationType.m_piFormationType){m_FormationType.m_piFormationType->DesignGetBBox(pvMins,pvMaxs);}
}

CTraceInfo CPlayAreaFormation::DesignGetTrace( const CVector &p1,const CVector &p2 )
{
	CTraceInfo info;
	info.m_dTraceFraction=1.0;
	info.m_vTracePos=p2;
	return info;
}

void CPlayAreaFormation::SetPosition(CVector &vPosition){m_vPosition=vPosition;}
void CPlayAreaFormation::SetFormationType(IFormationType *piFormationType){m_FormationType.Attach(piFormationType);}
void CPlayAreaFormation::SetBonusType(IEntityType *piBonusType){m_BonusType.Attach(piBonusType);}
void CPlayAreaFormation::SetAlternativeFormationType(IFormationType *piFormationType){m_AlternativeFormationType.Attach(piFormationType);}
void CPlayAreaFormation::SetAlternativeBonusType(IEntityType *piBonusType){m_AlternativeBonusType.Attach(piBonusType);}
void CPlayAreaFormation::SetCondition(EPlayerStateCondition eConditionType,unsigned int nValue){m_eConditionType=eConditionType;m_nConditionValue=nValue;}
CVector CPlayAreaFormation::GetPosition(){return m_vPosition;}
void	CPlayAreaFormation::GetFormationType(IFormationType **ppiFormationType){if(ppiFormationType){*ppiFormationType=ADD(m_FormationType.m_piFormationType);}}
void	CPlayAreaFormation::GetBonusType(IEntityType **ppiBonusType){if(ppiBonusType){*ppiBonusType=ADD(m_BonusType.m_piEntityType);}}
void	CPlayAreaFormation::GetAlternativeFormationType(IFormationType **ppiFormationType){if(ppiFormationType){*ppiFormationType=ADD(m_AlternativeFormationType.m_piFormationType);}}
void	CPlayAreaFormation::GetAlternativeBonusType(IEntityType **ppiBonusType){if(ppiBonusType){*ppiBonusType=ADD(m_AlternativeBonusType.m_piEntityType);}}
void    CPlayAreaFormation::GetCondition(EPlayerStateCondition *peConditionType,unsigned int *pnValue){if(peConditionType){*peConditionType=m_eConditionType;}if(pnValue){*pnValue=m_nConditionValue;}}
