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


#pragma once
#include "GameDialogBase.h"

class CGameDialogBase :	virtual public CGameWindowBase, virtual public IGameDialog
{
	bool m_bExecuting;
	int	 m_nDialogResult;

protected:

	virtual void EndDialog(int nResult);

	virtual void OnInitDialog();
	virtual void OnEndDialog();

public:

	// IGameDialog

	int	Execute(IGameWindow *piParent);

	void OnKeyDown(int nKey,bool *pbProcessed);
	void OnWantFocus(bool *pbWant);

	CGameDialogBase(void);
	~CGameDialogBase(void);
};
