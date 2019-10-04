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

#if !defined(D6_BACKBUFFER_H)
#define D6_BACKBUFFER_H

#include <windows.h>
#include "D6GUI/D6GUI.h"

//
// Defines a Backbuffer class
//
// A backbuffer is composed by a drawing surface and a displaying surface
// all drawing operations are performed on the first one and all the display
// operations are performed on the last one.
// 
// Please note that this class relies on MS Windows: if you need to port it
// to another OS, che handling of the buffers and device context must be
// change accordling
//

//
// Definition of the backbuffer class
//

class BackBuffer {
private:
	// Windows O.S. handles
	HDC		m_bufferHDC[2];
	HBITMAP m_bufferBMP[2];
	
	// Indicates which is the currently used buffer for backdrawing
	// and for displaying
	bool	m_state;

	// Last selected object inside the device contexts (needed to
	// deallocate HDC and other objects)
	HGDIOBJ m_lastObjects[2];
	
	// Bitmap (working) sizes
	int		m_width;
	int		m_height;

	// Background colors
	int		m_red;
	int		m_green;
	int		m_blue;

public:
	// Constructors (accepts either Color object or separate rgb colors)
	//BackBuffer(MinGUI::FrameWindow& win, MinGUI::Color& color);
	BackBuffer(d6gui::D6Window& win, int red = 0, int green = 0, int blue = 0);

	~BackBuffer();

public:

	// Gets the underlying HDC of the currently backbuffer
	HDC GetBackBufferDC() {
		return m_bufferHDC[m_state ? 0 : 1];		
	}

	// Renders the front buffer to the device context HDC
	void RenderFrontBuffer(HDC ddc) {
		 HDC sdc = m_bufferHDC[m_state ? 1 : 0];
		 ::BitBlt(ddc, 0, 0, m_width, m_height, sdc, 0, 0, SRCCOPY);
	}

	// Exchanges the front buffer with the backbuffer
	void FlipBuffers() {
		m_state = !m_state;
	}

	// Clears the selected buffer 
	void Clear(int buffer = -1);

	// Returns the sizes
	int GetWidth()  { return m_width;  }
	int GetHeight() { return m_height; }

	// Set/Get the backgroud color
	d6gui::Color GetClearColor();
	void  SetClearColor(d6gui::Color &color);
};

#endif // !defined(D6_BACKBUFFER_H)
