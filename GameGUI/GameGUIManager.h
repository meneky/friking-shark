#pragma once

DECLARE_CUSTOM_WRAPPER1(CRenderWrapper,IGenericRender,m_piRender);
DECLARE_CUSTOM_WRAPPER1(CViewportWrapper,IGenericViewport,m_piViewport);

class CGameGUIManager :	virtual public CSystemObjectBase,virtual public IGameGUIManager,virtual public IGenericViewportCallBack
{
	IGameWindow *m_piMainWindow;
	IGameWindow *m_piFocusedWindow;
	IGameWindow *m_piMouseCaptureWindow;

	bool		m_bShowMouseCursor;
	bool		m_bScreenSettingsChanged;
	DEVMODE m_OldScreenDeviceSettings;
	SGameScreenProperties m_sScreenProperties;

	CRenderWrapper m_Render;
	CViewportWrapper m_Viewport;

	std::vector<IGameWindow*> m_vPopups;

	void RenderWindow(IGenericRender *piRender,IGameWindow *piWindow);
	void ProcessMouseActivation(IGameWindow *piWindow);

	IGameWindow *GetWindowFromPos(SGamePos *pPosition,bool bOnlyActive);
	IGameWindow *GetWindowFromPos(IGameWindow *piWindow,SGamePos *pPosition,bool bOnlyActive);

	void OnKeyDown(WORD wKeyState);
	void OnKeyUp(WORD wKeyState);

	// IGenericViewportCallBack

	void		OnRender();
	LRESULT OnProcessMessage(LRESULT dwPreviousResult,HWND hWnd,UINT  uMsg, WPARAM  wParam,LPARAM  lParam);

	void OnLButtonDown(WORD wKeyState,unsigned x,unsigned y);
	void OnLButtonUp(WORD wKeyState,unsigned x,unsigned y);
	void OnRButtonDown(WORD wKeyState,unsigned x,unsigned y);
	void OnRButtonUp(WORD wKeyState,unsigned x,unsigned y);
	void OnMouseMove(unsigned x,unsigned y);
	void OnSize(unsigned cx,unsigned cy);

	void UpdateScreenPlacement();

	bool Unserialize(ISystemPersistencyNode *piNode);

public:

	BEGIN_PROP_MAP(CGameGUIManager)
		PROP_FLAGS(m_sScreenProperties,"Screen",MRPF_NORMAL|MRPF_OPTIONAL);
	END_PROP_MAP()

	// IGameGUIManager overloaded.

  bool Init(std::string sClass,std::string sName,ISystem *piSystem);
	void Destroy();

	void EnterGUILoop(); 
	void ExitGUILoop(); 

	void		GetWindowSize(SGameSize *pSize);
	void		GetMousePosition(IGameWindow *piWindow,SGamePos *pPos);
	void		SetMousePosition(IGameWindow *piWindow,SGamePos &pos);

	bool		IsKeyDown(int nKey);
	bool		IsMouseDown(int nMouseButton);

	void		SetFocus(IGameWindow *piWindow);
	IGameWindow *GetFocus();

	void		SetMouseCapture(IGameWindow *piWindow);
	void		ReleaseMouseCapture();
	IGameWindow *GetMouseCapture();

	void 		ShowMouseCursor(bool bShow);
	bool 		IsMouseCursorVisible();

	void		RedrawAll();

	IGameWindow *GetMainWindow();

	void		GetScreenProperties(SGameScreenProperties *pProperties);
	void    SetScreenProperties(SGameScreenProperties *pProperties);

	// Popup management.

	void		 AddPopup(IGameWindow *);
	void		 RemovePopup(IGameWindow *);
	void		 EnumeratePopups(IGameWindowEnumerationCallback *piCallback);
	void		 GetPopups(std::vector<IGameWindow *> *pvPopups);

	CGameGUIManager(void);
	~CGameGUIManager(void);
};