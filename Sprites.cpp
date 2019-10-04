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

#include <math.h>
#include <vector>
#include "D6GUI/D6GUI.h"
#include "Sprites.h"

using namespace d6gui;
using namespace std;

StarShipSprite::StarShipSprite() 
{


	m_vertexArray.push_back(new Vertex( 10,   0));
	m_vertexArray.push_back(new Vertex( 10,  10));
	m_vertexArray.push_back(new Vertex( 30,  40));
	m_vertexArray.push_back(new Vertex( 10,  40));
	m_vertexArray.push_back(new Vertex( 10,  50));
	m_vertexArray.push_back(new Vertex(-10,  50));
	m_vertexArray.push_back(new Vertex(-10,  40));
	m_vertexArray.push_back(new Vertex(-30,  40));
	m_vertexArray.push_back(new Vertex(-10,  10));
	m_vertexArray.push_back(new Vertex(-10,   0));
	m_vertexArray.push_back(new Vertex(  0, -10));

	m_lineArray.push_back(new Line( 0,  1));
	m_lineArray.push_back(new Line( 1,  2));
	m_lineArray.push_back(new Line( 2,  3));
	m_lineArray.push_back(new Line( 3,  4));
	m_lineArray.push_back(new Line( 4,  5));
	m_lineArray.push_back(new Line( 5,  6));
	m_lineArray.push_back(new Line( 6,  7));
	m_lineArray.push_back(new Line( 7,  8));
	m_lineArray.push_back(new Line( 8,  9));
	m_lineArray.push_back(new Line( 9, 10));
	m_lineArray.push_back(new Line(10,  0));
	m_lineArray.push_back(new Line(10,  0));
		
}


ShootSprite::ShootSprite() 
{

	m_vertexArray.push_back(new Vertex(  3,   0));
	m_vertexArray.push_back(new Vertex(  0,   3));
	m_vertexArray.push_back(new Vertex( -3,   0));
	m_vertexArray.push_back(new Vertex(  0,  -3));

	m_lineArray.push_back(new Line( 0,  1));
	m_lineArray.push_back(new Line( 1,  2));
	m_lineArray.push_back(new Line( 2,  3));
	m_lineArray.push_back(new Line( 3,  0));
		
}


AsteroidBigSprite::AsteroidBigSprite() 
{

	m_vertexArray.push_back(new Vertex(  50,   30));
	m_vertexArray.push_back(new Vertex(  40,   50));
	m_vertexArray.push_back(new Vertex(  10,   60));
	m_vertexArray.push_back(new Vertex( -10,   50));
	m_vertexArray.push_back(new Vertex( -30,   30));
	m_vertexArray.push_back(new Vertex( -40,    0));
	m_vertexArray.push_back(new Vertex( -30,  -30));
	m_vertexArray.push_back(new Vertex(   0,  -40));
	m_vertexArray.push_back(new Vertex(  30,  -39));
	m_vertexArray.push_back(new Vertex(  50,  -20));
	m_vertexArray.push_back(new Vertex(  60,   10));

	m_lineArray.push_back(new Line( 0,  1, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 1,  2, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 2,  3, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 3,  4, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 4,  5, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 5,  6, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 6,  7, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 7,  8, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 8,  9, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 9, 10, Color(0, 255, 0)));
	m_lineArray.push_back(new Line(10,  0, Color(0, 255, 0)));

		
}


AsteroidMediumSprite::AsteroidMediumSprite() 
{

	m_vertexArray.push_back(new Vertex(  30,   0));
	m_vertexArray.push_back(new Vertex(  30,  25));
	m_vertexArray.push_back(new Vertex(  10,  40));
	m_vertexArray.push_back(new Vertex( -10,  30));
	m_vertexArray.push_back(new Vertex( -20,   0));
	m_vertexArray.push_back(new Vertex( -10, -20));
	m_vertexArray.push_back(new Vertex(  20, -19));

	m_lineArray.push_back(new Line( 0,  1, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 1,  2, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 2,  3, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 3,  4, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 4,  5, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 5,  6, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 6,  0, Color(0, 255, 0)));
		
}


AsteroidSmallSprite::AsteroidSmallSprite() 
{

	m_vertexArray.push_back(new Vertex( 20,   0));
	m_vertexArray.push_back(new Vertex( 10,  20));
	m_vertexArray.push_back(new Vertex(-10,  10));
	m_vertexArray.push_back(new Vertex( -5, -10));
	m_vertexArray.push_back(new Vertex( 10,  -9));

	m_lineArray.push_back(new Line( 0,  1, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 1,  2, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 2,  3, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 3,  4, Color(0, 255, 0)));
	m_lineArray.push_back(new Line( 4,  0, Color(0, 255, 0)));
		
}

ExplosionSprite::ExplosionSprite() {

	m_vertexArray.push_back(new Vertex(  15,   0));
	m_vertexArray.push_back(new Vertex(  16,   1));

	m_vertexArray.push_back(new Vertex(  7,    5));
	m_vertexArray.push_back(new Vertex(  8,    4));

	m_vertexArray.push_back(new Vertex(  -8,   -5));
	m_vertexArray.push_back(new Vertex(  -9,   -4));

	m_vertexArray.push_back(new Vertex(  13,  -10));
	m_vertexArray.push_back(new Vertex(  14,  -11));

	m_vertexArray.push_back(new Vertex( -17,  -10));
	m_vertexArray.push_back(new Vertex( -18,  -11));

	m_vertexArray.push_back(new Vertex(  -13,   8));
	m_vertexArray.push_back(new Vertex(  -14,   9));

	m_vertexArray.push_back(new Vertex(  -6,    3));
	m_vertexArray.push_back(new Vertex(  -5,    4));

	m_vertexArray.push_back(new Vertex(  10,   -3));
	m_vertexArray.push_back(new Vertex(  11,   -4));

	m_lineArray.push_back(new Line(  0,  1));
	m_lineArray.push_back(new Line(  2,  3));
	m_lineArray.push_back(new Line(  4,  5));
	m_lineArray.push_back(new Line(  6,  7));
	m_lineArray.push_back(new Line(  8,  9));
	m_lineArray.push_back(new Line( 10, 11));
	m_lineArray.push_back(new Line( 12, 13));
	m_lineArray.push_back(new Line( 14, 15));

}

ThrustSprite::ThrustSprite() {

	m_vertexArray.push_back(new Vertex(  10, 50));
	m_vertexArray.push_back(new Vertex(   0, 60));
	m_vertexArray.push_back(new Vertex( -10, 50));

	m_lineArray.push_back(new Line(  0,  1, Color(255, 0, 0)));
	m_lineArray.push_back(new Line(  1,  2, Color(255, 0, 0)));

}
