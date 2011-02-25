#include "./stdafx.h"
#include "ParticleSystems.h"
#include "ParticleModifierType.h"
#include "ParticleSizeModifierType.h"

CParticleSizeModifierType::CParticleSizeModifierType(void)
{
    m_dStartSize=1;
    m_dEndSize=1;
    m_dSizeTransitionStart=0;// de 0 a 1, en tiempo de vida de la particula
    m_dSizeTransitionEnd=1;  // de 0 a 1, en tiempo de vida de la particula
}

CParticleSizeModifierType::~CParticleSizeModifierType(void){}
IParticleModifier *CParticleSizeModifierType::CreateInstance(unsigned int dwCurrentTime){return new CParticleSizeModifier(this);}

CParticleSizeModifier::CParticleSizeModifier(CParticleSizeModifierType *pType)
{
    m_pType=pType;
}

void CParticleSizeModifier::ProcessParticle(IParticle *pParticle,IParticleSystem *pSystem,unsigned int dwCurrentTime,double dInterval)
{
    if(m_sEmitters.size()!=0 && m_sEmitters.find(pParticle->m_piEmiter)==m_sEmitters.end()){return;}
    pParticle->m_dSize=GetTransitionValue(m_pType->m_dSizeTransitionStart,m_pType->m_dStartSize,m_pType->m_dSizeTransitionEnd,m_pType->m_dEndSize,&m_pType->m_dIntermediateSizeTransitions,pParticle->m_dLifeSpent);
}
