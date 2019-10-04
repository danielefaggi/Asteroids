/*
Copyright (C) 2007,  Daniele Faggi

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
#include <math.h>
#include "Geometry.h"

Vertex* Vertex::getRotated(float angle, Vertex* pivot)
{

	float cosx = (float) cos((double) angle);
	float sinx = (float) sin((double) angle);

	return getRotated(cosx, sinx, pivot);
	
}

Vertex* Vertex::getRotated(float cosx, float sinx, Vertex *pivot) {

	Vertex _pivot;

	if(pivot != NULL) {
		_pivot.setX(pivot->getX());
		_pivot.setY(pivot->getY());
	}

	double _xv = ((double) m_x) - ((double) _pivot.getX());
	double _yv = ((double) m_y) - ((double) _pivot.getY());

	double _x = _xv * cosx - _yv * sinx;
	double _y = _xv * sinx + _yv * cosx;

	return new Vertex((int) (_x + _pivot.getX()), 
					  (int) (_y + _pivot.getY()) );

}

Vertex* Vertex::getScaled(float zoomx, float zoomy, Vertex* pivot)
{

	Vertex _pivot;

	if(pivot != NULL) {
		_pivot.setX(pivot->getX());
		_pivot.setY(pivot->getY());
	}

	double _x = (double) m_x - (double) _pivot.getX();
	double _y = (double) m_y - (double) _pivot.getY();

	_x = ((double) zoomx) * _x;
	_y = ((double) zoomy) * _y;

	return new Vertex((int) (_x + _pivot.getX()), 
					  (int) (_y + _pivot.getY()) );

}

Vertex& Vertex::operator +=  (const Vertex& v) {

	this->m_x += v.m_x;
	this->m_y += v.m_y;

	return *this;
}

Vertex& Vertex::operator -=  (const Vertex& v) {

	this->m_x -= v.m_x;
	this->m_y -= v.m_y;

	return *this;
}

Vertex operator+ (Vertex& v1, Vertex& v2) {
	
	Vertex v;

	v.setX(v1.getX() + v2.getX());
	v.setY(v1.getY() + v2.getY());

	return v;
}

Vertex operator- (Vertex& v1, Vertex& v2) {
	
	Vertex v;

	v.setX(v1.getX() - v2.getX());
	v.setY(v1.getY() - v2.getY());

	return v;
}

