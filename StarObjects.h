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

#if !defined(D6_STAROBJECTS_H)
#define D6_STAROBJECTS_H

#include "Geometry.h"

//
// Defines the generic StarObject (common to all "logic" elements
// further defined)
//
class StarObject {

protected:
	// Every object has a position
	Vertex m_position;
	// Every object has a speed
	Vertex m_speed;

	// Every object had a limited field of action inside the board
	Vertex m_limits;

public:

	// Costruction (no need of destruction)
	StarObject() { m_limits.setX(100); m_limits.setY(100); }
	
	// This must redefined inside descendant classes in order
	// to obtain the right behaviour of an element
	virtual void Update();

	// Get/Set fields
	void SetPosition(const Vertex& position) { m_position = position; }
	Vertex& GetPosition()					 { return m_position;     }
	void SetSpeed(const Vertex& speed)		 { m_speed = speed;		  }
	Vertex& GetSpeed()						 { return m_speed;		  }
	void SetLimits(const Vertex& limits)	 { m_limits = limits;	  }
	Vertex& GetLimits()					     { return m_limits;		  }
};

//
// Defines the starship class
//
class StarShip : public StarObject {

private:

	// Absolute maximum speed	
	static const int c_maxSpeed;

	// Position is handled by parent StarObject

	// Angle of rotation
	int m_angle;

	// Indicates when a key is pressed (this avoids the repeat glitches from kbd)
	bool m_bKeyLeft;
	bool m_bKeyRight;
	bool m_bKeyUp;

	// Indicates the current thrus power
	int  m_thrust;

	// Indicates if the ship is destroyed
	bool  m_isExploded;

public:

	// Returns if the thruster is active (*not* its power, it is
	// handled privately inside the class)
	bool GetThrust();
	
	// Update the ship fields (needed by parent)
	virtual void Update();
	
	// Keys processing
	void KeyRightPressed();
	void KeyRightReleased();
	void KeyLeftPressed();
	void KeyLeftReleased();
	void KeyUpPressed();
	void KeyUpReleased();

	// Construction (no need of destruction)
	StarShip()				{  m_angle = 0; 
							   m_bKeyLeft = FALSE; 
							   m_bKeyRight = FALSE; 
							   m_bKeyUp = FALSE;
							   m_thrust = 0;
							   m_isExploded = FALSE;
							}
	
	// Set/Get fields 
	int GetAngle()           { return m_angle;  }
	void SetAngle(int angle) { m_angle = angle; }

	// Set if ship is exploded ... but it's really needed ?!? However, it is here.
	void SetExploded(bool isExploded) { m_isExploded = isExploded; }
	// Returns if the ship is exploded (maybe IsExploded could be better ...)
	bool GetExploded()                { return m_isExploded;       }
 
};

//
// Defines a "logic" asteroid
//
class Asteroid : public StarObject {
private:
	int m_size;
public:
	Asteroid(Vertex& position, Vertex& speed, Vertex& limits, int size = 0) {
		m_position = position;
		m_speed = speed;
		m_limits = limits;
		m_size = size;
	}

	int GetSize()			{ return m_size; }
	void SetSize(int size)  { m_size = size; }

};

//
// Defines a "logic" shoot
//
class Shoot : public StarObject {
public:
	void SetTTL(int TTL);
	int GetTTL();
	Shoot(Vertex& position, int angle, int speed);
	virtual void Update();

private:
	int m_ttl;
};

//
// Defines a "logic" explosion
//
class Explosion : public StarObject {
public:
	Explosion(Vertex& position, int phase = 0, int rotation = 0) {
		m_phase = phase;
		m_position = position;
		m_rotation = rotation;
		m_limits = position;
	}

	int GetPhase()  { return m_phase; }
	void Update()   { m_phase++;      }
	int GetRotation() { return m_rotation; }

private:
	int m_phase;
	int m_rotation;
};

#endif // !defined(D6_BACKBUFFER_H)
