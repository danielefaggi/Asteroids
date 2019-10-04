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

#if !defined(D6_GEOMETRY_H)
#define D6_GEOMETRY_H

#include <windows.h>
#include "D6GUI/D6GUI.h"

//
// Class Vertex
//
// This is a class used to describe either points and speeds
// is coordinates are integers but the calculation is performed
// as double precision
//
class Vertex {

private:
	// Coordinates, stored as integers
	int m_x;
	int m_y;

public:
	// Construction/Descrtuction
	Vertex()             { m_x = 0; m_y = 0; }
	Vertex(int x, int y) { m_x = x; m_y = y; }
	
	// General Operations
	void setX(int x)     { m_x = x; }
	int  getX() const    { return m_x; }
	void setY(int y)     { m_y = y; }
	int  getY() const    { return m_y; }	

	//
	// Transformation operations 
	//

	// Rotation
	Vertex* getRotated(float angle, Vertex* pivot = NULL);
	Vertex* getRotated(float cosx, float sinx, Vertex* pivot = NULL);
	
	// Scale
	Vertex* getScaled(float zoom, Vertex* pivot = NULL) 
			{ return getScaled(zoom, zoom, pivot); }
	Vertex* getScaled(float zoomx, float zoomy, Vertex* pivot = NULL);

	// Translation
	Vertex* getTranslated(float translx, float transly) 
			{ return new Vertex((int) (m_x + translx), (int) (m_y + transly)); }

	// Operators
	Vertex& operator+= (const Vertex& v); 
	Vertex& operator-= (const Vertex& v); 
	
};

// Simple operators
Vertex operator+ (Vertex& v1, Vertex& v2);
Vertex operator- (Vertex& v1, Vertex& v2);

//
// Class line
//
// Used (as vector) to build the sprites by connecting two Vertexes
//
class Line 
{
private:
	// Positonal Vertex indexes inside a source vector of Vertex elements
	int m_fromVertex;
	int m_toVertex;
	// Setup the color of the line
	d6gui::Color m_color;

public:
	// Construction (Destruction is not needed (there are not any "new" allocated objs)
	Line(int from, int to, d6gui::Color color = d6gui::Color(255,255,255))       
						{ m_fromVertex = from; m_toVertex = to; m_color = color; }
	
	// Get/Set fields
	int  getFromVertex()         { return m_fromVertex; }
	void setFromVertex(int from) { m_fromVertex = from; }
	int  getToVertex()           { return m_toVertex; }
	void setToVertex(int to)     { m_toVertex = to; }
	d6gui::Color getColor()	 { return m_color; }
	void setColor(d6gui::Color& color) { m_color = color; }
};

#endif // !defined(D6_GEOMETRY_H)
