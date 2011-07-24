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


// stdafx.cpp : source file that includes just the standard includes
// ParticleSystems.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "./stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file
#ifdef WIN32
	#pragma comment (lib, "VectorLib.lib")
	#pragma comment (lib, "GameRuntime.lib")
	#pragma comment (lib, "GameRuntimeLib.lib")
	#pragma comment (lib, "opengl32.lib")
	#pragma comment (lib, "glu32.lib")

	#ifdef _DEBUG
		#pragma comment(lib,"jpgdlib_d.lib")
	#else
		#pragma comment(lib,"jpgdlib.lib")
	#endif
#endif