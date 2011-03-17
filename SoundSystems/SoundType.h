#pragma once

#include "SoundSystemManager.h"
#include <deque>


class CSoundType: virtual public CSystemObjectBase,virtual public ISoundType
{
  std::deque<ALuint>   m_dAvailableSources;
  bool            m_bSoundsAcquired;
  ALuint		  m_iSoundBuffer;
  
  bool Unserialize(ISystemPersistencyNode *piNode);
  
  bool LoadFromFile();
  
public:

    ISoundManager  *m_piSoundManager;

    // Propiedades Persistentes

    unsigned int    m_nChannels;
    bool    		m_bLoop;
    std::string  	m_sFileName;
    double  		m_dVolume;

    ALuint AcquireSoundSource();
    void   ReleaseSoundSource(ALuint nSource);

    ISound *CreateInstance();

    bool Init(std::string sClass,std::string sName,ISystem *piSystem);
    void Destroy();
	
	bool        Load(std::string sFileName);
	std::string GetFileName();
	

    BEGIN_PROP_MAP(CSoundType);
        PROP(m_sFileName,"File");
        PROP_VALUE_FLAGS(m_nChannels,"Channels",5,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_bLoop,"Loop",false,MRPF_NORMAL|MRPF_OPTIONAL);
        PROP_VALUE_FLAGS(m_dVolume,"Volume",100,MRPF_NORMAL|MRPF_OPTIONAL);
    END_PROP_MAP();

    CSoundType();
    ~CSoundType();
};

class CSound:virtual public CSystemObjectBase,public ISound
{
protected:

	ALuint 					m_nSource;
    CSoundType             *m_pType;
	
	bool    				m_bLoop;
	double  				m_dVolume;
    
	CVector m_vPosition;
	CVector m_vOrientation;
	CVector m_vVelocity;
	
	void UpdateSource();
public:

    // ISound

    void Play();
    void Stop();
    bool IsPlaying();
	
	void SetLoop(bool bLoop);
	void SetVolume(double nVolume);
	
	void SetPosition(CVector vPosition);
	void SetOrientation(CVector vOrientation);
	void SetVelocity(CVector vVelocity);
	
    CSound(CSoundType *pType);
    virtual ~CSound();
};