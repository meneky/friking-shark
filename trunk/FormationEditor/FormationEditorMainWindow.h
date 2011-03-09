#pragma once

#include "FormationEditor.h"
#include "FormationEditorObjectSelector.h"

DECLARE_CUSTOM_WRAPPER1(CFormationEditorObjectSelectorWrapper,IFormationEditorObjectSelector,m_piObjectSelector)

#define KEY_FORWARD		1
#define KEY_BACK		2
#define KEY_RIGHT		3
#define KEY_LEFT		4
#define KEY_UP			5
#define KEY_DOWN		6
#define KEY_PAUSE		7
#define KEY_PROCESS_ONE_FRAME		8

struct SEntityControls
{
	CButtonWrapper					m_BTListRow;
	CObjectLabelWrapper				m_STEntity;
	unsigned long					m_nElementId;
	IEntityType						*m_piEntityType;
	IDesignObject					*m_piDesignObject;
	ISystemObject					*m_piObject;
	
	SEntityControls(){m_piEntityType=NULL;m_piDesignObject=NULL;m_piObject=NULL;m_nElementId=0;}
	~SEntityControls(){REL(m_piEntityType);REL(m_piDesignObject);REL(m_piObject);}
};

class CFormationEditorMainWindow: public CGameWindowBase, public IGameGUIButtonEvents
{
public:
	CConfigFile				m_GUIConfigFile;
	CViewportWrapper		m_Viewport;
	CRenderWrapper			m_Render;
	CGenericCameraWrapper	m_Camera;
	CFrameManagerWrapper	m_FrameManager;
	
	ISystem					*m_piGameSystem;
	CGameControllerWrapper   m_GameControllerWrapper;
	CPlayAreaManagerWrapper  m_PlayAreaManagerWrapper;
	CGameRenderWrapper		 m_GameRenderWrapper;
	CSoundManagerWrapper	 m_SoundManagerWrapper;
	CWorldManagerWrapper	 m_WorldManagerWrapper;
	
	CFormationTypeWrapper    m_FormationType;
	CFormationWrapper		 m_Formation;
	unsigned int			 m_nFormationId;
	
	std::vector<SEntityControls *>		m_vEntityControls;

	unsigned long		m_dwNexControlKey;
	bool				m_bBlend;
	bool				m_bSolid;
	bool				m_bTextures;
	bool				m_bColors;
	bool				m_bShowFilePanel;
	bool				m_bShowEntitiesPanel;
	bool				m_bShowEntityPanel;
	bool				m_bShowOptionsPanel;
	bool				m_bShowPlayAreaPanel;
	bool				m_bRenderPlayArea;

	int					m_nSelectedRoutePoint;

	bool				m_bMovingRoutePoint;
	CVector				m_vObjectOriginalPosition;
	CVector				m_vCursorOriginalPosition;

	int					m_nSelectedEntity;


	IGameGUILabel *m_piSTFps;
	IGameGUILabel *m_piSTVolume;
	IGameGUILabel *m_piBTDecreaseVolume;
	IGameGUILabel *m_piBTIncreaseVolume;

	IGameWindow	  *m_piGREntityPanel;
	IGameWindow	  *m_piGRPlayAreaPanel;
	IGameWindow	  *m_piGREntitiesPanel;
	IGameWindow	  *m_piGROptionsPanel;

	IGameGUIButton *m_piBTShowOptionsPanel;
	IGameGUIButton *m_piBTShowEntitiesPanel;
	IGameGUIButton *m_piBTShowFilePanel;
	IGameGUIButton *m_piBTShowPlayAreaProperties;

	// Entity

	IGameGUIButton *m_piBTNewEntity;
	IGameGUILabel  *m_piSTEntityName;
	IFormationEditorObjectLabel *m_piSTEntityObjectLabel;
	IGameGUIButton *m_piBTEntitySample;
	IGameGUIButton *m_piBTEntityRemove;
	IGameGUILabel  *m_piSTEntityCount;
	IGameGUIButton *m_piBTEntityDecreaseCount;
	IGameGUIButton *m_piBTEntityIncreaseCount;
	IGameGUILabel  *m_piSTEntityDelay;
	IGameGUIButton *m_piBTEntityDecreaseDelay;
	IGameGUIButton *m_piBTEntityIncreaseDelay;
	IGameGUILabel  *m_piSTEntityInterval;
	IGameGUIButton *m_piBTEntityDecreaseInterval;
	IGameGUIButton *m_piBTEntityIncreaseInterval;
	IGameGUIButton *m_piBTEntityClearRoute;

	// Formation

	IGameGUIButton *m_piBTNewFormation;
	IGameGUILabel  *m_piSTFormationName;
	IGameGUIButton *m_piBTFormationSample;
	IGameGUIButton *m_piBTFormationBonusSample;
	IGameGUIButton *m_piBTFormationRemove;

	// Entity Layers

	IGameWindow *m_piGREntityLayerList;

	// Options

	IGameGUIButton *m_piBTOptionsTextures;
	IGameGUIButton *m_piBTOptionsSolid;
	IGameGUIButton *m_piBTOptionsBlend;


	// File
	IGameWindow	   *m_piGRFormation;
	IGameGUIButton *m_piBTFormationNew;
	IGameGUIButton *m_piBTFormationOpen;
	IGameGUIButton *m_piBTFormationSave;
	IGameGUIButton *m_piBTFormationSaveAs;
	IGameGUIButton *m_piBTFormationCompile;
	IGameGUIButton *m_piBTFormationExit;

	// Play Area

	IGameGUILabel  *m_piSTPlayAreaCameraDistance;
	IGameGUIButton *m_piBTPlayAreaDecreaseCameraDistance;
	IGameGUIButton *m_piBTPlayAreaIncreaseCameraDistance;
	IGameGUILabel  *m_piSTPlayAreaCameraSpeed;
	IGameGUIButton *m_piBTPlayAreaDecreaseCameraSpeed;
	IGameGUIButton *m_piBTPlayAreaIncreaseCameraSpeed;
	IGameGUILabel  *m_piSTPlayAreaCameraViewAngle;
	IGameGUIButton *m_piBTPlayAreaDecreaseCameraViewAngle;
	IGameGUIButton *m_piBTPlayAreaIncreaseCameraViewAngle;
	IGameGUILabel  *m_piSTPlayAreaCameraAspectRatio;
	IGameGUIButton *m_piBTPlayAreaDecreaseCameraAspectRatio;
	IGameGUIButton *m_piBTPlayAreaIncreaseCameraAspectRatio;
	IGameGUILabel  *m_piSTPlayAreaAirPlane;
	IGameGUIButton *m_piBTPlayAreaDecreaseAirPlane;
	IGameGUIButton *m_piBTPlayAreaIncreaseAirPlane;
	IGameGUILabel  *m_piSTPlayAreaScroll;
	IGameGUIButton *m_piBTPlayAreaDecreaseScroll;
	IGameGUIButton *m_piBTPlayAreaIncreaseScroll;
	IGameGUIButton *m_piBTPlayAreaEnable;

	CVector WorldToFormation(CVector vWorldPoint);
	CVector FormationToWorld(CVector vFormationPoint);
	
	
	void ProcessInput(double dTimeFraction,double dRealTimeFraction);
	void ProcessKey(unsigned short nKey,double dTimeFraction,double dRealTimeFraction);

	void ProcessFileNew();
	void ProcessFileOpen();

	void ProcessFileSave();
	void ProcessFileSaveAs();
	void ProcessFileExit();

	void Reset();

	BEGIN_CHILD_MAP()
		CHILD_MAP_ENTRY("FPS",m_piSTFps);
		CHILD_MAP_ENTRY("Volume",m_piSTVolume);
		CHILD_MAP_ENTRY_EX("DecreaseVolume",m_piBTDecreaseVolume,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("IncreaseVolume",m_piBTIncreaseVolume,IGameGUIButtonEvents);

		CHILD_MAP_ENTRY_EX("PlayAreaProperties",m_piBTShowPlayAreaProperties,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("Options",m_piBTShowOptionsPanel,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("Entities",m_piBTShowEntitiesPanel,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("Formation",m_piBTShowFilePanel,IGameGUIButtonEvents);

		CHILD_MAP_ENTRY("EntitiesPanel",m_piGREntitiesPanel);
		CHILD_MAP_ENTRY("EntityLayerList",m_piGREntityLayerList);
		CHILD_MAP_ENTRY_EX("NewEntity",m_piBTNewEntity,IGameGUIButtonEvents);
	
		CHILD_MAP_ENTRY("EntityPanel",m_piGREntityPanel);
		CHILD_MAP_ENTRY("EntityName",m_piSTEntityName);
		CHILD_MAP_ENTRY("EntityObjectLabel",m_piSTEntityObjectLabel);
		CHILD_MAP_ENTRY("EntityCount",m_piSTEntityCount);
		CHILD_MAP_ENTRY("EntityDelay",m_piSTEntityDelay);
		CHILD_MAP_ENTRY("EntityInterval",m_piSTEntityInterval);
		CHILD_MAP_ENTRY_EX("EntityDecreaseCount",m_piBTEntityDecreaseCount,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityIncreaseCount",m_piBTEntityIncreaseCount,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityDecreaseDelay",m_piBTEntityDecreaseDelay,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityIncreaseDelay",m_piBTEntityIncreaseDelay,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityDecreaseInterval",m_piBTEntityDecreaseInterval,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityIncreaseInterval",m_piBTEntityIncreaseInterval,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntitySample",m_piBTEntitySample,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityRemove",m_piBTEntityRemove,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("EntityClearRoute",m_piBTEntityClearRoute,IGameGUIButtonEvents);

		CHILD_MAP_ENTRY("OptionsPanel",m_piGROptionsPanel);
		CHILD_MAP_ENTRY_EX("OptionShowTextures",m_piBTOptionsTextures,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("OptionSolid",m_piBTOptionsSolid,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("OptionBlend",m_piBTOptionsBlend,IGameGUIButtonEvents);

		CHILD_MAP_ENTRY("FormationPanel",m_piGRFormation);
		CHILD_MAP_ENTRY_EX("FormationNew",m_piBTFormationNew,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("FormationOpen",m_piBTFormationOpen,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("FormationSave",m_piBTFormationSave,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("FormationSaveAs",m_piBTFormationSaveAs,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("FormationExit",m_piBTFormationExit,IGameGUIButtonEvents);

		CHILD_MAP_ENTRY("PlayAreaPanel",m_piGRPlayAreaPanel);
		CHILD_MAP_ENTRY("PlayAreaCameraDistance",m_piSTPlayAreaCameraDistance);
		CHILD_MAP_ENTRY("PlayAreaCameraSpeed",m_piSTPlayAreaCameraSpeed);
		CHILD_MAP_ENTRY("PlayAreaCameraViewAngle",m_piSTPlayAreaCameraViewAngle);
		CHILD_MAP_ENTRY("PlayAreaCameraAspectRatio",m_piSTPlayAreaCameraAspectRatio);
		CHILD_MAP_ENTRY("PlayAreaAirPlane",m_piSTPlayAreaAirPlane);
		CHILD_MAP_ENTRY("PlayAreaScroll",m_piSTPlayAreaScroll);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseCameraDistance",m_piBTPlayAreaDecreaseCameraDistance,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseCameraDistance",m_piBTPlayAreaIncreaseCameraDistance,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseCameraSpeed",m_piBTPlayAreaDecreaseCameraSpeed,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseCameraSpeed",m_piBTPlayAreaIncreaseCameraSpeed,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseCameraViewAngle",m_piBTPlayAreaDecreaseCameraViewAngle,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseCameraViewAngle",m_piBTPlayAreaIncreaseCameraViewAngle,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseCameraAspectRatio",m_piBTPlayAreaDecreaseCameraAspectRatio,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseCameraAspectRatio",m_piBTPlayAreaIncreaseCameraAspectRatio,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseAirPlane",m_piBTPlayAreaDecreaseAirPlane,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseAirPlane",m_piBTPlayAreaIncreaseAirPlane,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaDecreaseScroll",m_piBTPlayAreaDecreaseScroll,IGameGUIButtonEvents);
		CHILD_MAP_ENTRY_EX("PlayAreaIncreaseScroll",m_piBTPlayAreaIncreaseScroll,IGameGUIButtonEvents);
	END_CHILD_MAP()

public:

	CFormationEditorObjectSelectorWrapper m_ObjectSelector;

	bool m_bSimulationStarted;
	bool m_bInspectionMode;

	
	bool InitWindow(IGameWindow *piParent,bool bPopup);
	void DestroyWindow();

	void OnWantFocus(bool *pbWant);
	void OnDraw(IGenericRender *piRender);
	void OnButtonClicked(IGameGUIButton *piControl);
	void OnKeyDown(int nKey,bool *pbProcessed);
	void OnMouseDown(int nButton,double x,double y);
	void OnMouseMove(double x,double y);
	void OnMouseUp(int nButton,double x,double y);

	void UpdateLayerPanel();
	void UpdateCaption();

	void CenterCamera();

	void SetupRenderOptions(IGenericRender *piRender,IGenericCamera *piCamera);

	bool GetAirPlaneCoordinatesFromCursorPos(double x,double y,CVector *pAirPlanePos);
	double GetAirPlaneAbsoluteHeight();

	void StopGameSimulation();
	void StartGameSimulation();
	void UpdateEntityControls();

	CFormationEditorMainWindow(void);
	~CFormationEditorMainWindow(void);
	
	void RenderRoute( IGenericRender * piRender,int nEntity, int nSelectedEntity, int nSelectedRoutePoint );
};


