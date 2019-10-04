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

#if !defined(D6_SPRITES_H)
#define D6_SPRITES_H

#include "D6GUI/D6GUI.h"
#include "VSprite.h"

//
// Define the common vectorized sprites used inside the game
//

class StarShipSprite : public VSprite
{

public:
	StarShipSprite();
};

class ShootSprite : public VSprite 
{
public:
	ShootSprite();
};

class AsteroidBigSprite : public VSprite
{
public:
	AsteroidBigSprite();
};

class AsteroidMediumSprite : public VSprite
{
public:
	AsteroidMediumSprite();
};

class AsteroidSmallSprite : public VSprite
{
public:
	AsteroidSmallSprite();
};

class ExplosionSprite : public VSprite
{
public:
	ExplosionSprite();
};

class ThrustSprite : public VSprite
{
public:
	ThrustSprite();
};

#endif // !defined(D6_SPRITES_H)
