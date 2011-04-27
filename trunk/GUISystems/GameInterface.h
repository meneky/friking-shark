#pragma once

#define MAX_LIVES_TO_DISPLAY 6
#define MAX_BOMBS_TO_DISPLAY 6

class CGameInterface: virtual public CGameWindowBase, virtual public IGameInterfaceWindow,virtual public IPlayAreaElementEnumerationCallback,virtual public IEntityEvents
{
	ISystemManager			*m_piSystemManager;
	ISystem					*m_piGameSystem;

	CGameControllerWrapper   m_GameControllerWrapper;
	CPlayAreaManagerWrapper  m_PlayAreaManagerWrapper;
	CEntityManagerWrapper	 m_EntityManagerWrapper;
	CFrameManagerWrapper	 m_FrameManagerWrapper;
	CWorldManagerWrapper	 m_WorldManagerWrapper;
	IPlayer					*m_piPlayer;
	IEntity					*m_piPlayerEntity;

	bool m_bPlayerKilledOnPreviousFrame;

	IGameGUILabel *m_piSTFrameRate;
	IGameGUILabel *m_piSTGameTime;
	IGameGUILabel *m_piSTObjectCount;
	IGameGUILabel *m_piSTEntityCount;

	bool		m_bCompleted;
	bool		m_bFrozen;
	bool		m_bResumeAfterFreeze;

	unsigned int		m_dwNextAcceptedControlKeyTime;
	unsigned int		m_dwNextAcceptedPauseKeyTime;
	unsigned int		m_dwMovementType;

	bool m_bGameStarted;
	CVector m_vLastCheckPointPosition;
	bool m_bShowPerformanceIndicators;

	SGamePos m_InspectionMovementStartPoint;

	IGameGUILabel   *m_piSTPoints;
	IGameWindow		*m_piSTLives[MAX_LIVES_TO_DISPLAY];
	IGameWindow		*m_piSTBombs[MAX_BOMBS_TO_DISPLAY];
	
	BEGIN_CHILD_MAP()
		CHILD_MAP_ENTRY_FLAGS("FrameRateLabel",m_piSTFrameRate,CMEF_OPTIONAL);
		CHILD_MAP_ENTRY_FLAGS("GameTimeLabel",m_piSTGameTime,CMEF_OPTIONAL);
		CHILD_MAP_ENTRY_FLAGS("EntityCountLabel",m_piSTEntityCount,CMEF_OPTIONAL);
		CHILD_MAP_ENTRY_FLAGS("ObjectCountLabel",m_piSTObjectCount,CMEF_OPTIONAL);
		CHILD_MAP_ENTRY("PlayerPoints",m_piSTPoints);
		CHILD_MAP_ENTRY("PlayerLive0",m_piSTLives[0]);
		CHILD_MAP_ENTRY("PlayerLive1",m_piSTLives[1]);
		CHILD_MAP_ENTRY("PlayerLive2",m_piSTLives[2]);
		CHILD_MAP_ENTRY("PlayerLive3",m_piSTLives[3]);
		CHILD_MAP_ENTRY("PlayerLive4",m_piSTLives[4]);
		CHILD_MAP_ENTRY("PlayerLive5",m_piSTLives[5]);
		CHILD_MAP_ENTRY("PlayerBomb0",m_piSTBombs[0]);
		CHILD_MAP_ENTRY("PlayerBomb1",m_piSTBombs[1]);
		CHILD_MAP_ENTRY("PlayerBomb2",m_piSTBombs[2]);
		CHILD_MAP_ENTRY("PlayerBomb3",m_piSTBombs[3]);
		CHILD_MAP_ENTRY("PlayerBomb4",m_piSTBombs[4]);
		CHILD_MAP_ENTRY("PlayerBomb5",m_piSTBombs[5]);
	END_CHILD_MAP()


	void StartGame();
	void StopGame();
	void ResetGame(bool bGoToLastCheckPoint);

	bool LoadScenario(std::string sFileName);
	void CloseScenario();

	void UpdateGUI();
	void UpdatePlayCameraPosition();
	void ProcessInput();
	void ProcessKey(unsigned short nKey);

	// IPlayAreaElementEnumerationCallback

	void ProcessEnumeratedPlayAreaElement(IPlayAreaElement *piElement,bool *pbStopEnumerating);

	// IGameInterfaceWindow

	void	Freeze(bool bFreeze);
	bool	IsFrozen();

	// IEntityEvents

	void OnKilled(IEntity *piEntity);

	static void RenderEntity(IEntity *piEntity,void *pParam1,void *pParam2);

public:

	bool InitWindow(IGameWindow *piParent,bool bPopup);
	void DestroyWindow();
	void OnDraw(IGenericRender *piRender);
	
	CGameInterface(void);
	~CGameInterface(void);
};
