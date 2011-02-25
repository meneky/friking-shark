#pragma once

#include "IATestUtils.h"
#include "IAEntityBase.h"
#include "TestScenarios.h"

DECLARE_CUSTOM_WRAPPER1(CViewportWrapper,IGenericViewport,m_piViewport)
DECLARE_CUSTOM_WRAPPER1(CButtonWrapper,IGameGUIButton,m_piButton)

class CIATestMainWindow: virtual public CGameWindowBase, virtual public IGameGUIButtonEvents
{
	CConfigFile m_GUIConfigFile;
	CViewportWrapper   m_Viewport;
	CGenericCameraWrapper m_Camera;

	void ProcessInput(double dTimeFraction,double dRealTimeFraction);
	void ProcessKey(unsigned short nKey,double dTimeFraction,double dRealTimeFraction);

	map<string,CIAEntityBase *> m_mEntities;
	map<string,CButtonWrapper *> m_mEntityButtons;
	CIAEntityBase *m_pSelectedEntity;

	IGameGUILabel *m_piSTFps;
	IGameGUILabel *m_piSTTime;
	IGameGUILabel *m_piSTEntityPos;
	IGameGUILabel *m_piSTEntityVel;

	map<ITestScenario *,CButtonWrapper *> m_mScenarios;
	ITestScenario *m_piCurrentScenario;

	bool				 m_bPauseOnNextFrame;
	unsigned int				 m_dwNextAcceptedPauseKeyTime;
	CFrameManagerWrapper m_FrameManager;

	BEGIN_CHILD_MAP()
		CHILD_MAP_ENTRY("FPS",m_piSTFps)
		CHILD_MAP_ENTRY("Time",m_piSTTime)
		CHILD_MAP_ENTRY("EntityPos",m_piSTEntityPos)
		CHILD_MAP_ENTRY("EntityVel",m_piSTEntityVel)
	END_CHILD_MAP()

	void ProcessPhysics(double dTimeFraction);
	void ProcessPhysics(CIAEntityBase *pEntity,double dTimeFraction);

	void ProcessIA(double dTimeFraction);

	double m_pdModelMatrix[16];
	double m_pdProjectionMatrix[16];
	int	   m_pnViewport[4];


	CVector	UnProject(double x, double y);

	void AddScenario(ITestScenario *piScenario);
	void ActivateScenario(ITestScenario *piScenario);
	void RemoveAllScenarios();
	void UpdateScenarioButtons();

public:

	void AddEntity(std::string sName,CIAEntityBase *pEntity,bool bAddButton);
	void AddEntityButton(std::string sName,bool bSelect);
	void RemoveAllEntities();
	
	// Sobrecarga para cambiar el valor por defecto del sistema de referencia.

	BEGIN_PROP_MAP(CIATestMainWindow)
		PROP_CLASS_CHAIN(CGameWindowBase);
		PROP_VALUE_FLAGS(m_eReferenceSystem,"ReferenceSystem",eGameGUIReferenceSystem_Relative,MRPF_NORMAL|MRPF_OPTIONAL)
	END_PROP_MAP()

    bool Unserialize(ISystemPersistencyNode *piNode);
	void DestroyWindow();

	void OnDraw(IGenericRender *piRender);
	void OnButtonClicked(IGameGUIButton *piControl);
	
	void OnMouseDown(int nButton,double x,double y);

	void UpdateRealRect();


	CIATestMainWindow(void);
	~CIATestMainWindow(void);
};