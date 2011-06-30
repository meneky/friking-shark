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
#include "resource.h"
#include "GameRunTimeLib.h"
#include "GameGUILib.h"
#include "GUISystems.h"
#include "MainWindow.h"

#define AVAILABLE_LEVELS 4

extern CSystemModuleHelper *g_pSystemModuleHelper;

CMainWindow::CMainWindow(void)
{
	m_eGameMode=eGameMode_Normal;
	m_eStage=eInterfaceStage_MainMenu;
	m_nCurrentLevel=0;
	m_dBackgroundAlpha=0;
	m_bVisible=true;
	m_nPoints=0;
	m_nLivesLeft=0;
	m_nWeaponLevel=0;
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

		bResult=m_GUIConfigFile.Open("Scripts/GameGUI.cfg");

		if(bResult)
		{
			m_MainMenuDialog.Create("GameGUI","CMainMenu","MainMenu");
			m_GameMenuDialog.Create("GameGUI","CGameMenu","GameMenu");
			m_ConfirmationDialog.Create("GameGUI","CConfirmationDialog","ConfirmationDialog");
			m_GameOverDialog.Create("GameGUI","CTimeOutDialog","GameOverDialog");
			m_LevelOptionsDialog.Create("GameGUI","CLevelOptions","LevelOptions");
			m_CongratulationsDialog.Create("GameGUI","CTimeOutDialog","CongratulationsDialog");

			m_MainMenuDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\MainMenu"));
			m_GameMenuDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\GameMenu"));
			m_ConfirmationDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\ConfirmationDialog"));
			m_GameOverDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\GameOverDialog"));
			m_CongratulationsDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\CongratulationsDialog"));
			m_LevelOptionsDialog.m_piSerializable->Unserialize(m_GUIConfigFile.GetNode("GameDialogs\\LevelOptions"));
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

void CMainWindow::OnDraw(IGenericRender *piRender)
{
	CGameWindowBase::OnDraw(piRender);
	
	if(m_eStage==eInterfaceStage_MainMenu 
		&& !m_MainMenuDialog.m_piDialog->IsVisible()
		&& !m_LevelOptionsDialog.m_piDialog->IsVisible())
	{
		bool bProcessed=false;
		OnKeyDown(GK_ESCAPE,&bProcessed);
	}
	
	if(m_eStage==eInterfaceStage_LaunchNextLevel)
	{
		char sFile[200];
		sprintf(sFile,"Level%d.ges",m_nCurrentLevel+1);
		m_eStage=eInterfaceStage_Playing;
		m_piGUIManager->ShowMouseCursor(false);
		m_piSTBackground->Show(false);
		m_piGameInterface->LoadScenario(sFile);
		m_piGameInterface->StartGame(m_eGameMode,m_nPoints,m_nLivesLeft,m_nWeaponLevel);
		m_piGameInterface->Show(true);
	}
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
				m_LevelOptionsDialog.m_piLevelOptions->SelectOptions(this,&m_eGameMode,&m_nCurrentLevel);
				
				char sFile[200];
				sprintf(sFile,"Level%d.ges",m_nCurrentLevel+1);
				
				m_nWeaponLevel=0;
				m_nPoints=0;
				m_nLivesLeft=3;
				
				m_eStage=eInterfaceStage_Playing;
				m_piGUIManager->ShowMouseCursor(false);
				m_piSTBackground->Show(false);
				m_piGameInterface->LoadScenario(sFile);
				m_piGameInterface->StartGame(m_eGameMode,m_nPoints,m_nLivesLeft,m_nWeaponLevel);
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
				m_piGameInterface->StopGame();
				m_piGameInterface->CloseScenario();
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

void CMainWindow::OnScenarioFinished(eScenarioFinishedReason eReason,unsigned int nPoints, unsigned int nLivesLeft, unsigned int nWeaponLevel)
{
	m_piGUIManager->ShowMouseCursor(true);
	m_piGameInterface->Freeze(true);
	m_piGameInterface->Freeze(false);
	m_piGameInterface->Show(false);
	m_piGameInterface->StopGame();
	m_piGameInterface->CloseScenario();
	if(eReason!=eScenarioFinishedReason_Completed)
	{
		m_GameOverDialog.m_piDialog->Execute(this);
		m_piSTBackground->Show(true);
		m_eStage=eInterfaceStage_MainMenu;
	}
	else
	{
		m_nPoints=nPoints;
		m_nLivesLeft=nLivesLeft;
		m_nWeaponLevel=nWeaponLevel;
		
		m_nCurrentLevel++;
		if(m_nCurrentLevel>=AVAILABLE_LEVELS)
		{
			m_nCurrentLevel=0;
		}
		m_eStage=eInterfaceStage_LaunchNextLevel;
	}
	

}

void CMainWindow::OnWantFocus(bool *pbWant){*pbWant=true;}