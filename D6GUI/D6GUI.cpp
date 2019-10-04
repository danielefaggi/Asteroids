/*

 D6GUI - A little and quick GUI management library

 (c) 2007, 2019 Daniele Faggi

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

#include <windows.h>
#include <windowsx.h>
#include <memory.h>
#include <tchar.h>
#include "D6GUI.h"

namespace d6gui {

	BOOL D6Window::Create(HINSTANCE hInstance, Rect size, LPCTSTR szWindowTitle, LPCTSTR idIcon, LPCTSTR idIconSmall) {

		m_hInstance = hInstance;

		m_D6WinInstance = this;

		ATOM pip = this->RegisterClass(idIcon, idIconSmall);		

		int w = GetSystemMetrics(SM_CXSCREEN);
		int h = GetSystemMetrics(SM_CYSCREEN);

		RECT rect;

		rect.left = size.getLeft();
		rect.top = size.getTop();
		rect.right = size.getRight();
		rect.bottom = size.getBottom();

		AdjustWindowRect(&rect, (WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME) | WS_DLGFRAME, TRUE);

		HWND hWnd;

		hWnd = CreateWindow(_T("D6Window"),
			szWindowTitle,
			(WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX) | WS_DLGFRAME,
			(w - rect.right) / 2, (h - rect.bottom) / 2,
			rect.right, rect.bottom, NULL, NULL,
			m_hInstance, NULL);

		if (!hWnd)
		{
			DWORD error = GetLastError();
			return FALSE;
		}

		m_hWnd = hWnd;

		ShowWindow();
		Refresh();

		return TRUE;

	}


	ATOM D6Window::RegisterClass(LPCTSTR idIcon, LPCTSTR idIconSmall) {

		WNDCLASSEX wcex;

		memset(&wcex, 0, sizeof(WNDCLASSEX));
		wcex.cbSize = sizeof(WNDCLASSEX);


		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = (WNDPROC)this->WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = m_hInstance;
		wcex.hIcon = LoadIcon(wcex.hInstance, idIcon);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL; // (LPCSTR)IDC_INVADERS;
		wcex.lpszClassName = _T("D6Window");
		wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)idIconSmall);

		ATOM result = RegisterClassEx(&wcex);

		return result;
	}

	void D6Window::ShowWindow() {

		::ShowWindow(m_hWnd, SW_SHOW);
		UpdateWindow(m_hWnd);

	}

	void D6Window::HideWindow() {

		::ShowWindow(m_hWnd, SW_HIDE);
		UpdateWindow(m_hWnd);

	}

	void D6Window::Refresh() {

		UpdateWindow(m_hWnd);

	}

	// Message pump
	LONG D6Window::Run() {

		MSG msg;

		while (TRUE)
		{
			// Timer handling
			if (m_timeout > 0) {
				
				ULONGLONG ticks = GetTickCount64();
				int delta = (int) (ticks - m_prevTicks);
				m_cumTime += delta;
				m_prevTicks = ticks;
				
				if (m_cumTime > m_timeout) {
					OnTimer(m_cumTime);
					m_cumTime = 0;
				}
			}

			// Message processing
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			// Quit message pump
			if (msg.message == WM_QUIT) break;

		}

		return msg.wParam;
	}

	D6Window* D6Window::m_D6WinInstance;

	LRESULT CALLBACK D6Window::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		return m_D6WinInstance->RealWndProc(hWnd, message, wParam, lParam);
	}

	LRESULT CALLBACK D6Window::RealWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

		int wmId, wmEvent;
		PAINTSTRUCT ps;
		HDC hdc;

		int nVirtKey;
		long lKeyData;

		switch (message)
		{
		case WM_COMMAND:
			wmId = LOWORD(wParam);
			wmEvent = HIWORD(wParam);
			// Parse the menu selections:
			//switch (wmId)
			//{
			//}
			break;

		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			DWORD error;
			if (hdc == NULL)
			{
				error = GetLastError();
			}
			RECT rt;
			GetClientRect(hWnd, &rt);
		    OnDraw(hdc);
			EndPaint(hWnd, &ps);
			break;

		case WM_CLOSE:
			if (IDOK == MessageBox(m_hWnd, _T("Surrender, are you sure ?"), _T("Question"), MB_OKCANCEL | MB_ICONQUESTION)) {
				DestroyWindow(hWnd);
			}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			nVirtKey = (int)wParam;    // virtual-key code 
			lKeyData = lParam;          // key data 
			OnKeyDown(nVirtKey);
			break;

		case WM_KEYUP:
			nVirtKey = (int)wParam;    // virtual-key code 
			lKeyData = lParam;          // key data 
			OnKeyUp(nVirtKey);
			break;

		case WM_MOUSEMOVE:
			if (wParam & MK_LBUTTON) {
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				OnMouseLButtonDown(xPos, yPos);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;

	}

}