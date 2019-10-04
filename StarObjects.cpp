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
#include "Constants.h"
#include "Geometry.h"
#include "StarObjects.h"

//
// StarObject
//

void StarObject::Update() {

	m_position = m_position + m_speed;
	while(m_position.getX() > m_limits.getX()) {
		m_position.setX(m_position.getX() - m_limits.getX());
	}
	while(m_position.getX() < 0) {
		m_position.setX(m_position.getX() + m_limits.getX());
	}

	while(m_position.getY() > m_limits.getY()) {
		m_position.setY(m_position.getY() - m_limits.getY());
	}
	while(m_position.getY() < 0) {
		m_position.setY(m_position.getY() + m_limits.getY());
	}
	
}

//
// StarShip
//

const int StarShip::c_maxSpeed = 20;

void StarShip::KeyLeftPressed()
{
	m_bKeyLeft = TRUE;

}

void StarShip::KeyLeftReleased()
{
	m_bKeyLeft = FALSE;

}

void StarShip::KeyUpPressed()
{
	m_bKeyUp = TRUE;

}

void StarShip::KeyUpReleased()
{
	m_bKeyUp = FALSE;

}

void StarShip::KeyRightPressed()
{
	m_bKeyRight = TRUE;

}

void StarShip::KeyRightReleased()
{
	m_bKeyRight = FALSE;

}

void StarShip::Update()
{

	if(m_bKeyRight) {
		m_angle += 10;
		while(m_angle > 360) m_angle -= 360;
	}

	if(m_bKeyLeft) {
		m_angle -= 10;
		while(m_angle < 0) m_angle += 360;
	}

	if(m_bKeyUp) {
		m_thrust ++;
	} else {
		m_thrust = 0;
	}

	double y = -cos((((double) m_angle)/360.0) * gc_pi * 2.0) * m_thrust;
	double x =  sin((((double) m_angle)/360.0) * gc_pi * 2.0) * m_thrust;

	m_speed = m_speed + Vertex((int) x, (int) y);
	
	//
	// Checks and, eventually, limits the speed
	//
	double rs = sqrt(pow(((double) m_speed.getX()), 2.0) + 
		             pow(((double) m_speed.getY()), 2.0)   );

	if( rs > ((double) c_maxSpeed) ) {

		double da;
		if(m_speed.getY() < 0) {
			da = atan( - ((double) m_speed.getX()) / ((double) m_speed.getY()) );
		} else if(m_speed.getY() == 0) {
			da = (gc_pi / 2.0) * (m_speed.getX() > 0) ? 1.0 : -1.0;
		} else { //if( m_speed.getY() > 0)
			da = gc_pi - atan(((double) m_speed.getX()) / ((double) m_speed.getY()) );
		}

		double dy =  cos(da) * (rs - ((double) c_maxSpeed));
		double dx = -sin(da) * (rs - ((double) c_maxSpeed));
		m_speed = m_speed + Vertex((int) dx, (int) dy);
	}

	StarObject::Update();


}

//
// Shoot
//

Shoot::Shoot(Vertex& position, int angle, int speed) {

	double y = -cos((((double) angle)/360.0)*gc_pi*2.0) * speed;
	double x = sin((((double) angle)/360.0)*gc_pi*2.0) * speed;

	m_position = position;
	m_speed = Vertex((int) x, (int) y);
	m_ttl = 20;

}

void Shoot::Update() {

	StarObject::Update();

	m_ttl -= 1;
}

int Shoot::GetTTL()
{

	return m_ttl;

}

void Shoot::SetTTL(int TTL)
{
	m_ttl = TTL;
}

bool StarShip::GetThrust()
{

	return (m_thrust > 0);

}
