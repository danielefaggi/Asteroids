/*

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

#include <windows.h>
#include "D6GUI/D6GUI.h"
#include "BackBuffer.h"

//BackBuffer::BackBuffer(d6gui::D6Window& win, MinGUI::Color& color) {
//
//	BackBuffer(win, color.GetR(), color.GetG(), color.GetB());
//
//}

//
// Costruction of a backbuffer
//
BackBuffer::BackBuffer(d6gui::D6Window& win, int red, int green, int blue) {
	
	// Gets the screen device context (where the window is placed)	
	HDC dc = ::GetDC(win.GetHWnd());
	
	// Creates the device contexts
	m_bufferHDC[0] = ::CreateCompatibleDC(dc);
	m_bufferHDC[1] = ::CreateCompatibleDC(dc);

	// Setup the bitmaps
	RECT rect;
	::GetClientRect(win.GetHWnd(), &rect);

	m_width = rect.right - rect.left;
	m_height = rect.bottom - rect.top;

	m_bufferBMP[0] = ::CreateCompatibleBitmap(dc, m_width, m_height);
	m_bufferBMP[1] = ::CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);

	// Selects the bitmaps in the DC's
	m_lastObjects[0] = ::SelectObject(m_bufferHDC[0], m_bufferBMP[0]);
	m_lastObjects[1] = ::SelectObject(m_bufferHDC[1], m_bufferBMP[1]);

	// Clear the bitmaps (useful in debug to test contexts)
	{
		d6gui::Drawing drw(m_bufferHDC[0]);
		drw.SetBrush(d6gui::Color(red, green, blue));
		drw.DrawRect(0, 0, m_width, m_height);
	}

	{
		d6gui::Drawing drw(m_bufferHDC[1]);
		drw.SetBrush(d6gui::Color(red, green, blue));
		drw.DrawRect(0, 0, m_width, m_height);
	}

	m_red	= red;
	m_green = green;
	m_blue	= blue;

	m_state = FALSE;
}

//
// Clears the backbuffers
//
void BackBuffer::Clear(int buffer) {
	
	int _buffer = 0;

	if(buffer < 0 || buffer > 1) {
		_buffer = m_state ? 0 : 1;
	} 

	d6gui::Drawing drw(m_bufferHDC[_buffer]);
	drw.SetPen(d6gui::Color(m_red, m_green, m_blue));
	drw.SetBrush(d6gui::Color(m_red, m_green, m_blue));
	drw.DrawRect(0, 0, m_width, m_height);

}

//
// Destroys the backbuffer
//
BackBuffer::~BackBuffer() {
		
	::SelectObject(m_bufferHDC[0], m_lastObjects[0]);
	::SelectObject(m_bufferHDC[1], m_lastObjects[1]);

	::DeleteObject(m_bufferBMP[0]);
	::DeleteObject(m_bufferBMP[1]);
	::DeleteDC(m_bufferHDC[0]);
	::DeleteDC(m_bufferHDC[1]);
}

void BackBuffer::SetClearColor(d6gui::Color& color) {

	m_red   = color.GetR();
	m_green = color.GetG();
	m_blue  = color.GetB();

}

d6gui::Color BackBuffer::GetClearColor() {

	return d6gui::Color(m_red, m_green, m_blue);

}
