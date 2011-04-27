#include "./stdafx.h"
#include "resource.h"
#include "GameRunTimeLib.h"
#include "GameGUILib.h"
#include "GUISystems.h"
#include "MainWindow.h"

extern CSystemModuleHelper *g_pSystemModuleHelper;

CMainWindow::CMainWindow(void)
{
	m_eStage=eInterfaceStage_MainMenu;
	m_dBackgroundAlpha=0;
	m_bVisible=true;
}

CMainWindow::~CMainWindow(void)
{
}

bool CMainWindow::InitWindow(IGameWindow *piParent,bool bPopup)
{
	bool bResult=CGameWindowBase::InitWindow(piParent,bPopup);
	if(bResult)
	{
		CViewportWrapper viewport;
		viewport.Attach("GameGUI","Viewport");
		if(viewport.m_piViewport){viewport.m_piViewport->SetCaption("Friking Shark");}

		SGameRect sRect;
		sRect.x=0;
		sRect.y=0;
		sRect.w=1;
		sRect.h=1;
		m_eReferenceSystem=eGameGUIReferenceSystem_Relative;
		SetRect(&sRect);

		bResult=m_GUIConfigFile.Open("Scripts/GameGUI.cfg");

		if(bResult)
		{
			m_MainMenuDialog.Create("GameGUI","CMainMenu","MainMenu");
			m_GameMenuDialog.Create("GameGUI","CGameMenu","GameMenu");
			m_ConfirmationDialog.Create("GameGUI","CConfirmationDialog","ConfirmationDialog");
			m_GameOverDialog.Create("GameGUI","CGameSimpleDialog","GameOverDialog");
			m_CongratulationsDialog.Create("GameGUI","CGameSimpleDialog","CongratulationsDialog");

			m_MainMenuDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\MainMenu"));
			m_GameMenuDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\GameMenu"));
			m_ConfirmationDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\ConfirmationDialog"));
			m_GameOverDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\GameOverDialog"));
			m_CongratulationsDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\CongratulationsDialog"));
		}

		if(bResult)
		{
			m_piGameInterface->Show(false);
			m_piSTBackground->Show(true);
		}
		m_piGUIManager->SetFocus(this);
	}

	return bResult;
}

void CMainWindow::OnKeyDown(int nKey,bool *pbProcessed)
{
	if(nKey==GK_ESCAPE)
	{
		*pbProcessed=true;

		if(m_eStage==eInterfaceStage_MainMenu)
		{
			int result=m_MainMenuDialog.m_piDialog->Execute(this);
			if(result==eMainMenuAction_Exit)
			{
				m_piGUIManager->ExitGUILoop();
			}
			else if(result==eMainMenuAction_NewGame)
			{
				m_eStage=eInterfaceStage_Playing;

				m_piGUIManager->ShowMouseCursor(false);
				m_piSTBackground->Show(false);
				m_piGameInterface->Show(true);
			}
			m_piGUIManager->SetFocus(this);
		}
		else if(m_eStage==eInterfaceStage_Playing)
		{
			m_piGUIManager->ShowMouseCursor(true);
			m_piGameInterface->Freeze(true);
			int result=m_GameMenuDialog.m_piDialog->Execute(this);
			m_piGameInterface->Freeze(false);
			if(result==eGameMenuAction_EndGame)
			{
				m_piGameInterface->Show(false);
				m_piSTBackground->Show(true);
				m_eStage=eInterfaceStage_MainMenu;
			}
			else
			{
				m_piGUIManager->ShowMouseCursor(false);
			}
			m_piGUIManager->SetFocus(this);
		}
	}
}

void CMainWindow::OnScenarioFinished(eScenarioFinishedReason eReason)
{
	m_piGUIManager->ShowMouseCursor(true);
	m_piGameInterface->Freeze(true);
	if(eReason==eScenarioFinishedReason_Completed)
	{
		m_CongratulationsDialog.m_piDialog->Execute(this);
	}
	else
	{
		m_GameOverDialog.m_piDialog->Execute(this);
	}
	m_piGameInterface->Freeze(false);
	m_piGameInterface->Show(false);
	m_piSTBackground->Show(true);
	m_eStage=eInterfaceStage_MainMenu;
}

void CMainWindow::OnWantFocus(bool *pbWant){*pbWant=true;}