//  Friking Shark, a Flying Shark arcade remake.
//  Copyright (C) 2011  Javier Martin Garcia (javiermartingarcia@gmail.com)
//	
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//  
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  


#include "./stdafx.h"
#include "BombProjectileType.h"
#include "../GameGraphics/GameGraphics.h"

struct SBombDamageData
{
	double dTimeFraction;
	double dDamage;
	double dRadius;
	CVector vCameraPos;
	CPlane  playAreaPlane;
	
	SBombDamageData(){dTimeFraction=0;dDamage=0;dRadius=0;}
};

CBombProjectileType::CBombProjectileType()
{
   m_nTimeToExplode=400;
   m_nCollisionType=PHYSIC_COLLISION_TYPE_NONE;
   m_nMovementType=PHYSIC_MOVE_TYPE_FLY;
   m_dDamagePerSecond=0;
   m_dDamageStartRadius=0;
   m_dDamageEndRadius=0;
   m_nDamageStartTime=0;
   m_nDamageEndTime=0;
   g_PlayAreaManagerWrapper.AddRef();
}

CBombProjectileType::~CBombProjectileType()
{
	g_PlayAreaManagerWrapper.Release();
}

IEntity *CBombProjectileType::CreateInstance(IEntity *piParent,unsigned int dwCurrentTime)
{
  CBombProjectile *piEntity=new CBombProjectile(this,piParent);
  InitializeEntity(piEntity,dwCurrentTime);
  piEntity->SetAlignment(piParent->GetAlignment());
  piEntity->SetState(eBombState_Normal);
  return piEntity;
}

CBombProjectile::CBombProjectile(CBombProjectileType *pType,IEntity *piParent)
{
  m_nCurrentTime=0;
  m_sClassName="CBombProjectile";
  m_sName="BombProjectile";
  m_pType=pType;
  m_piParent=piParent;
}

CBombProjectile::~CBombProjectile()
{
}

void CBombProjectile::ApplyDamageOperation(IEntity *piEntity,void *pParam1,void *pParam2)
{
	CBombProjectile *pThis=(CBombProjectile *)pParam1;
	SBombDamageData *pDamageData=((SBombDamageData*)pParam2);
	
	bool bProjectile=(*piEntity->GetEntityClass())=="CBulletProjectile";
	
	if(pThis->m_piParent==NULL){return;}
	if(piEntity->IsRemoved()){return;}
	if(piEntity->GetAlignment()==pThis->m_dwAlignment){return;}
	if(piEntity->GetAlignment()==ENTITY_ALIGNMENT_NEUTRAL ){return;}
	if(piEntity->GetDamageType()==DAMAGE_TYPE_NONE && !bProjectile){return;}
	if(piEntity->GetHealth()<=0.0){return;}
	
	SPhysicInfo *pPhysicInfo=piEntity->GetPhysicInfo();
	
	CVector vProjection;
	
	bool bHit=false;
	
	if(pDamageData->playAreaPlane.Cut(pDamageData->vCameraPos,pPhysicInfo->vPosition,&vProjection))
	{
		CVector vDist=vProjection-pThis->m_PhysicInfo.vPosition;
		double dDist=vDist;
		bHit=(dDist<pDamageData->dRadius);
	}
	if(!bHit)
	{
		for(unsigned int b=0;pPhysicInfo->pvBBoxes && b<pPhysicInfo->pvBBoxes->size();b++)
		{
			CVector pVolumePoints[8];
			CalcBBoxVolume(pPhysicInfo->vPosition,pPhysicInfo->vAngles,(*pPhysicInfo->pvBBoxes)[b].vMins,(*pPhysicInfo->pvBBoxes)[b].vMaxs,pVolumePoints);
			
			for(unsigned int x=0;!bHit && x<8;x++)
			{
				if(pDamageData->playAreaPlane.Cut(pDamageData->vCameraPos,pVolumePoints[x],&vProjection))
				{
					CVector vDist=vProjection-pThis->m_PhysicInfo.vPosition;
					double dDist=vDist;
					bHit=(dDist<pDamageData->dRadius);
				}
			}
		}
	}
	if(bHit)
	{
		if(bProjectile)
		{
			piEntity->Remove();
		}
		else
		{
			piEntity->OnDamage(pDamageData->dDamage,pThis);
		}
	}
}

void CBombProjectile::ProcessFrame(unsigned int dwCurrentTime,double dTimeFraction)
{
	CEntityBase::ProcessFrame(dwCurrentTime,dTimeFraction);
	m_nCurrentTime=dwCurrentTime;
	if(GetState()==eBombState_Normal)
	{
		if(dwCurrentTime>(m_dwCreationTime+m_pType->m_nTimeToExplode))
		{
			if(m_pTypeBase->GetStateAnimations(eBombState_Hit))
			{
				m_PhysicInfo.vVelocity=Origin;
				m_PhysicInfo.dwMoveType=PHYSIC_MOVE_TYPE_NONE;
				
				SetState(eBombState_Hit);
			}
			else
			{
				Remove();
			}
		}
	}
	else if(GetState()==eBombState_Hit)
	{
		if(dwCurrentTime>(m_dwCreationTime+m_pType->m_nTimeToExplode+m_pType->m_nDamageEndTime))
		{
			Remove();
		}
		else if(dwCurrentTime>(m_dwCreationTime+m_pType->m_nTimeToExplode+m_pType->m_nDamageStartTime))
		{
			IGenericCamera *piCamera=g_PlayAreaManagerWrapper.m_piInterface->GetCamera();
			CVector vCameraPos=piCamera?piCamera->GetPosition():Origin;
			REL(piCamera);
			
			double dElapsedTime=((double)(dwCurrentTime-(m_dwCreationTime+m_pType->m_nTimeToExplode+m_pType->m_nDamageStartTime)));
			double dTotalDuration=((double)(m_pType->m_nDamageEndTime-m_pType->m_nDamageStartTime));
			double dElapsedFraction=dElapsedTime/dTotalDuration;
			
			SBombDamageData data;
			data.dTimeFraction=dTimeFraction;
			data.dDamage=m_pType->m_dDamagePerSecond*dTimeFraction;
			data.dRadius=(m_pType->m_dDamageEndRadius-m_pType->m_dDamageStartRadius)*dElapsedFraction+m_pType->m_dDamageStartRadius;
			data.vCameraPos=vCameraPos;
			data.playAreaPlane=CPlane(AxisPosY,m_PhysicInfo.vPosition);
			GetEntityManager()->PerformUnaryOperation(ApplyDamageOperation,this,&data);
		}
	}
}







	
