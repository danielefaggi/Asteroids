/*

Asteroids - A simple asteroids clone game

Copyright (C) 2007, 2019 Daniele Faggi

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "D6GUI/D6GUI.h"
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "resource.h"
#include "BackBuffer.h"
#include "Geometry.h"
#include "VSprite.h"
#include "Sprites.h"
#include "StarObjects.h"
#include "Game.h"

using namespace d6gui;

// These could be objects to be placed inside the
// application object and their allocation/deallocation must
// be made inside such object, however in our case
// due limitations in application class of MinGUI, these are
// allocated inside the init method of such class and are
// never deallocated (however, the OS take care of this at the
// end of the execution)
Game*		g_AsteroidsGame;
BackBuffer* g_WinBackBuffer;

//
// Main Window Implementation
//
class AsteroidsWin : public D6Window
{
public:

	AsteroidsWin(HINSTANCE hInstance, int x=-1,int y=-1, int width=-1, int height=-1, const char* title="") :
	  D6Window(x, y, width, height, title) 
	{

		Rect rect(0, 0, 0, 0);
		rect.setLeft(0);
		rect.setRight(640);
		rect.setTop(0);
		rect.setBottom(480);

		Create(hInstance, rect, title, (LPCSTR)IDI_ICON1, (LPCSTR)IDI_ICON2);

		//SetBackgroundColor(Color(0,0,128)); // Don't care
		Refresh();
	}

    void OnEraseBackground(HDC hdc) {  };

	void OnDraw(HDC hdc)
	{
		if (g_WinBackBuffer == NULL) return;

		// In answer to a refresh request redraw the front buffer
		g_WinBackBuffer->RenderFrontBuffer(hdc);

	}

	void OnTimer(int delta) {

		// Update the game
		g_AsteroidsGame->Step();
		// Clears the backbuffer
		g_WinBackBuffer->Clear();
		// Draw the new frame
		g_AsteroidsGame->Render(g_WinBackBuffer);
		// Exchange the buffers (if a refesh is triggered
		// the same front buffer is redrawn ... however since
		// we are in a single-threaded environment is not
		// really needed) 

		if(g_WinBackBuffer != NULL)
			g_WinBackBuffer->FlipBuffers();

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 640;
		rect.bottom = 480;

		InvalidateRect(m_hWnd, &rect, FALSE);

		// Trigger a refresh
		Refresh();
	}

	void OnKeyDown(int key) {

		// Process the key pressed
		if(key == 37) 
			g_AsteroidsGame->KeyLeftPressed();
		if(key == 39) 
			g_AsteroidsGame->KeyRightPressed();
		if(key == 38) 
			g_AsteroidsGame->KeyUpPressed();
		if(key == 40) 
			g_AsteroidsGame->KeyFirePressed();

	}

	void OnKeyUp(int key) {

		// Process the key released
		if(key == 37) 
			g_AsteroidsGame->KeyLeftReleased();
		if(key == 39) 
			g_AsteroidsGame->KeyRightReleased();
		if(key == 38) 
			g_AsteroidsGame->KeyUpReleased();
		if (key == 40)
			g_AsteroidsGame->KeyFireReleased();

	}

	void OnMouseLButtonDown(int x, int y)
	{
		// Force a test refresh
		Refresh();
	}

private:
};

//
// Application Initialization Implementation
//
void Application::Init(HINSTANCE hInstance)
{

	srand((unsigned)time( NULL ));

	g_WinBackBuffer = NULL;

	//
	// Application initialization
	//

	int width = 600;
	int height = 400;

	// Initialize a new game object
	g_AsteroidsGame = new Game(width, height);
	
	// Initialize the main window
	AsteroidsWin *t = new AsteroidsWin(hInstance, -1, -1, width, height, "Asteroids");

	// Setup the icon
	// However the window sysbox is not updated due a unhandled message
	// inside MinGUI (as opposite of the taskbar icon, that displays correctly)
	HINSTANCE hInst = (HINSTANCE) ::GetClassLong(t->GetHWnd(), GCL_HMODULE);
	HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1));
	HICON hIconsm = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON2));
	if(hIcon != NULL && hIconsm != NULL) {
		::SetClassLong(t->GetHWnd(), GCL_HICON, (LONG) hIcon); 
		::SetClassLong(t->GetHWnd(), GCL_HICONSM, (LONG) hIconsm); 
	}
	
	// Setup appropriate style to the window and adjust the size
	LONG winstyle = (::GetWindowLong(t->GetHWnd(), GWL_STYLE)
					  & ~WS_THICKFRAME
					  & ~WS_MAXIMIZEBOX)
					  |WS_DLGFRAME;

	::SetWindowLong(t->GetHWnd(), GWL_STYLE, winstyle);


	// Adjust the window
	RECT rect;
	rect.top = 0;
	rect.bottom = height;
	rect.left = 0;
	rect.right = width;
	::AdjustWindowRect(&rect, winstyle, FALSE);
	::SetWindowPos(t->GetHWnd(), NULL, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);

	// Update on screen
	::ShowWindow(t->GetHWnd(), SW_HIDE);
	::ShowWindow(t->GetHWnd(), SW_SHOW);

	// Initialze the backbuffer
	g_WinBackBuffer = new BackBuffer(*t, 0, 0, 0);

	// Initialize the timer
	t->StartTimer(50);

	m_mainWin = t;

}	

// Application startup definition
void Application::Run()
{
	((AsteroidsWin*)m_mainWin)->Run();
}

// WinMain
int
#if !defined(_MAC)
#if defined(_M_CEE_PURE)
__clrcall
#else
WINAPI
#endif
#else
CALLBACK
#endif
WinMain(_In_	 HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_	 LPSTR     lpCmdLine,
		_In_	 int       nCmdShow)
{
	Application app;
	app.Init(hInstance);
	app.Run();
}
