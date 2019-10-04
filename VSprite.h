/*

Asteroids - A simple asteroids clone game

Copyright (C) 2007, 2019  Daniele Faggi

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

#if !defined(D6_VSPRITE_H)
#define D6_VSPRITE_H

#include <vector>
#include "D6GUI/D6GUI.h"
#include "Geometry.h"

//
// Generic VSprite (Vector Sprite) object
//
// Every sprite must be derived from this object ... because this
// allow to handle all sprites in the same way despite their 
// differencies
//
class VSprite
{
protected:
	// Every sprite has two arrays: one for the vertexes and one
	// for the line connecting pairs of them
	std::vector<Vertex*> m_vertexArray;
	std::vector<Line*>   m_lineArray;

public:
	// Creates a transform: REMEMBER TO DEALLOCATE IT WITH DeleteTransform (!!!)
	std::vector<Vertex*> CreateTransform(int x, int y, int rotation, int scale);
	// Deletes a transform: USE THIS TO DEALLOCATE A TRANSFORM (!!!)
	void DeleteTransform( std::vector<Vertex*>& transform);

	// Construction / Destruction
	VSprite()  { }
	~VSprite(); // Destruction take care of deleting all obj allocated
				// inside into the vectors by the descentants

	// Draws the sprite (it can be ovverriden) 
	virtual void Draw(d6gui::Drawing &drw, int x, int y, int rotation = 0, int scale = 100);
	
	// Checks for collisions
	virtual bool CheckCollision(Vertex& v, int x, int y, int rotation = 0, int scale = 100);
	virtual bool CheckCollision(std::vector<Vertex*>& v, int x, int y, int rotation = 0, int scale = 100);

};

#endif // !defined(D6_VSPRITE_H)