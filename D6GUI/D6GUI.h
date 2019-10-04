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


#if !defined(D6_GUI_H)
#define D6_GUI_H

#include <windows.h>
#include <wingdi.h>
#include <string>

namespace d6gui {

	// Common text alignment constants
	const unsigned int CENTER = DT_CENTER;
	const unsigned int LEFT = DT_LEFT;
	const unsigned int RIGHT = DT_RIGHT;

	//
	// Rect definition (sometimes used in other classes)
	//
	class Rect {

	private:
		RECT m_rect;

	public:
		Rect(int left, int top, int right, int bottom) {

			m_rect.left = left;
			m_rect.top = top;
			m_rect.right = right;
			m_rect.bottom = bottom;

		}

		int getLeft() { return m_rect.left; }
		void setLeft(int left) { m_rect.left = left; }

		int  getTop() { return m_rect.top; }
		void setTop(int top) { m_rect.top = top; }

		int  getRight() { return m_rect.right; }
		void setRight(int right) { m_rect.right = right; }

		int  getBottom() { return m_rect.bottom; }
		void setBottom(int bottom) { m_rect.bottom = bottom; }

	};

	//
	// D6Window - A simple abstract window
	//
	class D6Window {

		// Fields
	private:
		int         m_timeout = 0;		// Timer management (limit)
		ULONGLONG	m_prevTicks = 0;	// Timer management (previous system ticks)
		int			m_cumTime = 0;		// Timer management (time elapsed so far)

		static D6Window*	m_D6WinInstance;	// Sef Window instance (useful to be used by WndProc)

	protected:
		HWND		m_hWnd;			// Windows handle of the Window
		HINSTANCE	m_hInstance;	// Program instance

		// Methods
	public:
		// Constructor
		D6Window(int x, int y, int width, int height, const char* title) { m_hWnd = NULL; m_hInstance = NULL, m_D6WinInstance = NULL; }

		// Initializer
		BOOL Create(HINSTANCE hInstance, Rect size, LPCTSTR szWindowTitle, LPCTSTR idIcon, LPCTSTR idIconSmall);

		// Show/Hide
		void ShowWindow();
		void HideWindow();

		// Repaint/Reshow
		void Refresh();

		// Get the Windows handle
		HWND GetHWnd() { return m_hWnd;  }

		// Operations to be overridden
		virtual void OnDraw(HDC hdc) {};					// Paint client area
		virtual void OnKeyDown(int key) {};					// Key pressed
		virtual void OnKeyUp(int key) {};					// Key released
		virtual void OnMouseLButtonDown(int x, int y) {};	// Mouse Button Left pressed
		
		virtual void OnTimer(int delta) {};					// Timer expired
		void StartTimer(int millis) { m_timeout = millis; }	// Timer setup

	protected:
		// WndProc 
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		LRESULT CALLBACK RealWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		// Run the message pump
		LONG Run();

	private:
		// Register the Window class
		ATOM RegisterClass(LPCTSTR idIcon, LPCTSTR idIconSmall);

	};

	//
	// Application - A simple abstract application
	//
	class Application {

	private:
		// Reference to the main derived D6Window
		D6Window* m_mainWin;

	public:
		// Application initialization
		void Init(HINSTANCE hInstance);

		// Run the application (start the main window message pump)
		void Run();

	};

	//
	// Color - RGB Color management class
	//
	class Color {

	private:
		unsigned char m_red = 0;
		unsigned char m_green = 0;
		unsigned char m_blue = 0;

	public: 
		Color() {};
		Color(unsigned char red, unsigned char green, unsigned char blue) { m_red = red; m_green = green; m_blue = blue; };
		Color(int red, int green, int blue) { m_red = red; m_green = green; m_blue = blue; }
		
		int GetR() { return m_red; }
		int GetG() { return m_green; }
		int GetB() { return m_blue; }
	};

	//
	// Drawing - Simple drawing operations class
	//
	class Drawing
	{
	private:
		HDC m_hdc;
		Color m_color;
		Color m_textColor;
		Color m_brushColor;

	public:
		// Constructor - from HDC
		Drawing(HDC hdc) { m_hdc = hdc; }

		// Set pen color
		void SetPen(Color color) { 
			
			m_color = color;

		}

		// Draw line
		void DrawLine(int x1, int y1, int x2, int y2) {

			HPEN pen = ::CreatePen(PS_SOLID, 1, RGB(m_color.GetR(), m_color.GetG(), m_color.GetB()));
			HGDIOBJ obj = ::SelectObject(m_hdc, pen);
			
			::MoveToEx(m_hdc, x1, y1, NULL);
			::LineTo(m_hdc, x2, y2);
			
			::SelectObject(m_hdc, obj);
			::DeleteObject(pen);
		}

		// Set brush (fill) color
		void SetBrush(Color color) {

			m_brushColor = color;

		}

		// Draw a filled rectangle
		void DrawRect(int x, int y, int width, int height) { 

			HBRUSH brush = ::CreateSolidBrush(RGB(m_color.GetR(), m_color.GetG(), m_color.GetB()));

			RECT rect; 
			rect.left = x; 
			rect.top = y; 
			rect.right = x + width; 
			rect.bottom = y + height; 
			::FillRect(m_hdc, &rect, brush); 
			
			::DeleteObject(brush);
		}

		// Set text color
		void SetTextColor(Color color) {

			m_textColor = color;

		}

		// Draw text
		void DrawText(int x, int y, std::string text, int align) 
		{
						
			RECT rect;
			rect.left = x;
			rect.top = y;
			rect.right = x;
			rect.bottom = y;

			COLORREF oldcol =::SetTextColor(m_hdc, RGB(m_textColor.GetR(), m_textColor.GetG(), m_textColor.GetB()));
			int oldmode = ::SetBkMode(m_hdc, TRANSPARENT);

			::DrawText(m_hdc, &text[0], text.length(), &rect, align| DT_NOCLIP);

			::SetTextColor(m_hdc, oldcol);
			::SetBkMode(m_hdc, oldmode);

		}

		// Return the underlying HDC
		HDC GetHDC() { return m_hdc; }

	};



}

#endif // !defined(D6_GUI_H)

