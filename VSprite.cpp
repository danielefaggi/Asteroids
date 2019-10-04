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

#include <vector>
#include "D6GUI/D6GUI.h"
#include <math.h>
#include "Geometry.h"
#include "VSprite.h"


void VSprite::Draw(d6gui::Drawing &drw, int x, int y, int rotation, int scale) {

	std::vector<Vertex*> _vertexes = CreateTransform(x, y, rotation, scale);

	std::vector<Line*>:: iterator li;

	for(li = m_lineArray.begin(); li != m_lineArray.end(); li ++) {	
		
		int x1 = _vertexes[(*(li))->getFromVertex()]->getX();
		int y1 = _vertexes[(*(li))->getFromVertex()]->getY();
		int x2 = _vertexes[(*(li))->getToVertex()]->getX();
		int y2 = _vertexes[(*(li))->getToVertex()]->getY();
		
		drw.SetPen((*li)->getColor());
		drw.DrawLine(_vertexes[(*(li))->getFromVertex()]->getX(), 
					 _vertexes[(*(li))->getFromVertex()]->getY(),
					 _vertexes[(*(li))->getToVertex()]->getX(),
					 _vertexes[(*(li))->getToVertex()]->getY()   );
	}

	DeleteTransform(_vertexes);
}

//
// Creates an array of vertexes traslated, rotated and scaled
//
std::vector<Vertex*> VSprite::CreateTransform(int x, int y, int rotation, int scale) {
	
	std::vector<Vertex*> _vertexes;
	
	std::vector<Vertex*>::iterator vi;
	
	float cosx = (float) cos(((double)rotation) * ((float) (2 * 3.14159/360.0)));
	float sinx = (float) sin(((double)rotation) * ((float) (2 * 3.14159/360.0)));

	for(vi = m_vertexArray.begin(); vi != m_vertexArray.end(); vi ++) {

		Vertex* r = (*(vi))->getRotated(cosx, sinx);
		Vertex* s = r->getScaled((float)(((double) scale) / 100.0));
		Vertex* t = s->getTranslated((float)x, (float)y);

		_vertexes.push_back(t);

		delete s;
		delete r;
	}

	return _vertexes;
}

//
// Delete a previously allocated transform
//
void VSprite::DeleteTransform(std::vector<Vertex*>& transform) {

	std::vector<Vertex*>::iterator vi;

	for(vi = transform.begin(); vi != transform.end(); vi ++) {
		delete *(vi);
	}

	transform.clear();

}

/*
	Check Collision
	===============

	Algorithm used:

	Consider a ray exiting from the test point (v) in a direction (here is used
	a line going from the point to the right).
	If the number of time it intersects a line segment is even, the point is
	outside the polygon, otherwise it is inside.

	Before performing the test, the function provide to transform the vertexes
	according with the position, rotation and scale of the displayed object.

	The equation used are those related to the systems:
	
	|  y = y1 + (y2 - y1) * m
	|  x = x1 + (x2 - x1) * m

	and

	|  y = v.getY()
	|  x = v.getX() + n
	
	Merging them, we obtain:

	     v.getY() - y1
	 m = -------------
		    y2 - y1

	and
	          (x2 - x1) * (v.getY() - y1)
	 n = x1 + --------------------------- + v.getX()
	                   y2 - y1

	with the constaint that:

	m must between 0 and 1 and n must be greather or equal than 0

	In order to optimize collisions and avoid false positives when irregular forms
	are used as shapes for the polygons, the same procedure is performed on the 
	other axis, considering a ray exiting from the given point and proceeding toward 
	the top.
	The conditions and procedure are the same already exposed.

	When both conditions are true, collision has occurred.

*/
bool VSprite::CheckCollision(Vertex& v, int x, int y, int rotation, int scale) {

	std::vector<Vertex*> _vertexes = CreateTransform(x, y, rotation, scale);

	std::vector<Line*>:: iterator li;

	bool isInsideX = FALSE;

	for(li = m_lineArray.begin(); li != m_lineArray.end(); li ++) {	
		
		int x1 = _vertexes[(*(li))->getFromVertex()]->getX();
		int y1 = _vertexes[(*(li))->getFromVertex()]->getY();
		int x2 = _vertexes[(*(li))->getToVertex()]->getX();
		int y2 = _vertexes[(*(li))->getToVertex()]->getY();

		// Only a border is "touched", ignore it
		if((y2 - y1) == 0) { 
			continue; 
		}

		double m =  (((double)v.getY()) - ((double)y1)) / (((double)y2) - ((double) y1));

		// Solution outside the segment, ignore it
		if( (m > ((double) 1.0)) || (m < ((double) 0.0)) ) {
			continue;
		}

		double n = ((double)x1) + (((double)x2) - ((double)x1)) * m - ((double) v.getX());

		// Solution on the other side, ignore it
		if(n < 0.0) { 
			continue;
		}

		isInsideX = (!isInsideX);
	}

	bool isInsideY = FALSE;

	for(li = m_lineArray.begin(); li != m_lineArray.end(); li ++) {	
		
		int x1 = _vertexes[(*(li))->getFromVertex()]->getX();
		int y1 = _vertexes[(*(li))->getFromVertex()]->getY();
		int x2 = _vertexes[(*(li))->getToVertex()]->getX();
		int y2 = _vertexes[(*(li))->getToVertex()]->getY();

		// Only a border is "touched", ignore it
		if((x2 - x1) == 0) { 
			continue; 
		}

		double m =  (((double)v.getX()) - ((double)x1)) / (((double)x2) - ((double)x1));

		// Solution outside the segment, ignore it
		if( (m > ((double) 1.0)) || (m < ((double) 0.0)) ) {
			continue;
		}

		double n = ((double)y1) + (((double)y2) - ((double)y1)) * m - ((double) v.getY());

		// Solution on the other side, ignore it
		if(n < 0.0) { 
			continue;
		}

		isInsideY = (!isInsideY);
	}

	DeleteTransform(_vertexes);
	
	return isInsideX && isInsideY;
}

// Check for a vector of vertexes

bool VSprite::CheckCollision(std::vector<Vertex*>& v, int x, int y, int rotation, int scale) {

	std::vector<Vertex*>::iterator vi;

	for(vi = v.begin(); vi != v.end(); vi ++) {	
		
		if(CheckCollision(*(*vi), x, y, rotation, scale))
			return TRUE;
	}

	return FALSE;

}

//
// Destrution deletes all elements eventually added by descrendant
//
VSprite::~VSprite() {

	std::vector<Vertex*>::iterator vi;

	if(m_vertexArray.size() > 0)
	for(vi = m_vertexArray.begin(); vi != m_vertexArray.end(); vi ++) {			
		delete (*vi);
	}

	std::vector<Line*>::iterator li;

	if(m_lineArray.size() > 0)
	for(li = m_lineArray.begin(); li != m_lineArray.end(); li ++) {			
		delete (*li);
	}

}