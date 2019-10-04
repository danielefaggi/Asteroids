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

#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "D6GUI/D6GUI.h"
#include "Sprites.h"
#include "Game.h"

//
// Implementation for Game 
//

const int Game::c_maxBlinkTimer = 20;
const int Game::c_pauseBeweenLevels = 100;
const int Game::c_maxWaitTimer = 30;

Game::Game(int width, int height) {


	LoadSprites();

	// Zero the lives
	m_lives = 0;

	// Zero the score
	m_score = 0;

	// Zero the pause "ressurection" wait timer 
	m_waitTimer = 0;

	// Zero the blink Timer 
	m_blinkTimer = 0;

	// Zero the end level timer
	m_endLevelTimer = 0;

	// Setup the initial game status
	m_status = GameStatus::StatusGameOver; 

	m_width = width;
	m_height = height;

	// Setup Starship
	m_starship.SetLimits(Vertex(width, height));
	m_starship.SetPosition(Vertex(width/2, height/2));
	m_starship.SetSpeed(Vertex(1, 1));

	// Setup Asteroids
	InitAsteroids(4, 3);
}

void Game::LoadSprites() {

	// Load Sprites
	m_sprites.push_back(new StarShipSprite());
	m_sprites.push_back(new ShootSprite());
	m_sprites.push_back(new AsteroidBigSprite());
	m_sprites.push_back(new AsteroidMediumSprite());
	m_sprites.push_back(new AsteroidSmallSprite());
	m_sprites.push_back(new ExplosionSprite());
	m_sprites.push_back(new ThrustSprite());

}

void Game::DeleteSprites() {

	// Update Asteroids

	std::vector<VSprite*>::iterator vi;

	if(m_sprites.size() > 0) {
		for(vi = m_sprites.begin(); vi != m_sprites.end(); vi ++) {
			delete (*vi);
		}
	}

	m_sprites.clear();

}

void Game::Step()
{


	//
	// Update Procedures
	// =================
	//
	// These may be splitted into single funtions, but it
	// could worse maintenance, tidying the class declaration
	// ... so I prefer keep it clean and the result is this one ...
	// (please note the leading and trailing braces that 
	// identify the unnamed functions)
	//

	// Update the blink timer
	if(++m_blinkTimer > c_maxBlinkTimer) m_blinkTimer = 0;

	// Update Asteroids
	{
		std::vector<Asteroid*>::iterator vi;

		if(m_asteroids.size() > 0) {
			for(vi = m_asteroids.begin(); vi != m_asteroids.end(); vi ++) {
				(*vi)->Update();
			}
		} else {
			m_endLevelTimer++;
		}
	}

	// Update the wait "Resurrection" timer
	if(m_waitTimer < c_maxWaitTimer && m_starship.GetExploded()) 
		m_waitTimer ++;

	// If the time is elapsed
	if(m_waitTimer >= c_maxWaitTimer && m_starship.GetExploded()) {
		if(m_lives <= 0) {
			m_status = GameStatus::StatusGameOver;
		}
	}

	// Update Shoots
	{
		std::vector<Shoot*>::iterator vi;
		
		if(m_shoots.size() > 0) {
			for(vi = m_shoots.begin(); vi != m_shoots.end(); vi ++) {
				(*vi)->Update();
			}
		}

		bool repeat;
		
		do {
			if(m_shoots.size() == 0) break;

			repeat = FALSE;

			for(repeat = FALSE, vi = m_shoots.begin(); vi != m_shoots.end(); vi ++) {
				if((*vi)->GetTTL() < 0) {
					delete (*vi);
					m_shoots.erase(vi);
					repeat = TRUE;
					break;
				}
			}
		} while(repeat);
	}

	// Check Collisions Shoots/Asteroids
	{
		bool repeat;

		do {	

			if(m_shoots.size() == 0 || m_asteroids.size() == 0) break;

			repeat = FALSE;

			std::vector<Shoot*>::iterator vi;
			
			for(vi = m_shoots.begin(); vi != m_shoots.end(); vi ++) {

				std::vector<Asteroid*>::iterator ai;

				for(ai = m_asteroids.begin(); ai != m_asteroids.end(); ai ++) {

					if(m_sprites[SpriteAsteroidBig + (*ai)->GetSize()]->CheckCollision((*vi)->GetPosition(),
													(*ai)->GetPosition().getX(),
													(*ai)->GetPosition().getY())) {
						//
						// Hit!
						//
						
						// Gets info about the shoot
						Vertex shootspeed = (*vi)->GetSpeed();

						// Remove the shoot
						delete (*vi);
						m_shoots.erase(vi);
						
						// Gets info about the asteroid hit
						Vertex position = (*ai)->GetPosition();
						Vertex speed    = (*ai)->GetSpeed();
						Vertex limits   = (*ai)->GetLimits();

						int size = (*ai)->GetSize();

						// Update the score
						m_score += (size + 1) * 10;

						// Calculate the direction of new asteroids
						int x = (speed.getX() - shootspeed.getX()) / (3 - size);
						int y = (speed.getY() - shootspeed.getY()) / (3 - size);

						size++;
						
						delete (*ai);
						m_asteroids.erase(ai);

						// if asteroid is of size 0 or 1
						if(size <= 2) {

							//
							// Init new asteroids with random directions
							//

							double rndx;
							double rndy;
							double sx;
							double sy;
							double px;
							double py;
							do {
								rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
								rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
								sx = rndx * ((double)abs(x) - 1) + 1;
								sy = rndy * ((double)abs(y) - 1) + 1;
							} while(sx <= 0 && sy <= 0);

							rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							px = 7 * rndx - 4;
							py = 7 * rndy - 4;

							m_asteroids.push_back(new Asteroid(position + Vertex((int) px, (int)py), Vertex((int) -(x/sx), (int) (y/sy)), limits, size));

							do {
							rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							sx = rndx * ((double)abs(x) - 1) + 1;
							sy = rndy * ((double)abs(y) - 1) + 1;
							} while(sx <= 0 && sy <= 0);

							rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							px = 7 * rndx - 4;
							py = 7 * rndy - 4;
							
							m_asteroids.push_back(new Asteroid(position + Vertex((int)px, (int)py), Vertex( (int) (x/sx), (int) -(y/sy)), limits, size));

							do {
							rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							sx = rndx * ((double)abs(x) - 1) + 1;
							sy = rndy * ((double)abs(y) - 1) + 1;
							} while(sx <= 0 && sy <= 0);

							rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
							px = 7 * rndx - 4;
							py = 7 * rndy - 4;

							m_asteroids.push_back(new Asteroid(position + Vertex((int) px, (int) py), Vertex((int) -(x/sx), (int) -(y/sy)), limits, size));
						}

						m_explosions.push_back(new Explosion(position, 0));

						repeat = TRUE;
						break;
					}

				}

				if(repeat) break;
			}

		} while(repeat);
	}

	// Update Explosions
	{
		bool repeat;
		
		do {
			if(m_explosions.size() == 0) break;

			repeat = FALSE;
			std::vector<Explosion*>::iterator ei;
			for(ei = m_explosions.begin(); ei != m_explosions.end(); ei ++) {
				(*ei)->Update();
				if((*ei)->GetPhase() > 10) {
					delete (*ei);
					m_explosions.erase(ei);
					repeat = TRUE;
					break;
				}
			}
		} while(repeat);
	}

	//
	// Update the StarShip only if we are inside a game
	// *and* if the ship is not destroyed 
	//
	if(m_status == GameStatus::StatusInGame && !m_starship.GetExploded()) {

		m_starship.Update();

		// Checks Collision for StarShip to Asteroids
		// In other words, this checks if a ship's vertex is inside an asteroid
		// if so, the starship will be destroyed
		std::vector<Vertex*> _vertexes = m_sprites[0]->CreateTransform(
						   m_starship.GetPosition().getX(),
						   m_starship.GetPosition().getY(),
						   m_starship.GetAngle(), 100);
		{
			std::vector<Asteroid*>::iterator vi;

			if(m_asteroids.size() > 0) {
				for(vi = m_asteroids.begin(); vi != m_asteroids.end(); vi ++) {
					if(m_sprites[SpriteAsteroidBig + (*vi)->GetSize()]->CheckCollision(_vertexes,
														(*vi)->GetPosition().getX(),
														(*vi)->GetPosition().getY())) {
						// Starship hit!!!

						m_starship.SetExploded(TRUE);

						std::vector<Vertex*>::iterator ni;

						for(ni = _vertexes.begin(); ni != _vertexes.end(); ni ++) {
							double rotation = 360.0 * ((double) rand()) / (((double) RAND_MAX) + 1.0);
							m_explosions.push_back(new Explosion(**(ni), 0, ((int) rotation)));
						}

						// Reset the wait time (it's really needed to pause between
						// ship destructions)
						m_waitTimer = 0;

						// Decrease the lives
						m_lives--;

						break;
					}
				}
			}
		}

		m_sprites[0]->DeleteTransform(_vertexes);
	}

	if(m_endLevelTimer > c_pauseBeweenLevels) {
		StartNextLevel();
	}
}

//
// Main Renderer 
//
void Game::Render(BackBuffer* backbuffer)
{

	//
	// if we are not in a game, then display useful information
	//
	if(m_status == GameStatus::StatusGameOver) {

		d6gui::Drawing drw = d6gui::Drawing(backbuffer->GetBackBufferDC());
		drw.SetTextColor(d6gui::Color(255,255,255));

		// MinGUI has a BUG
		// It doesn't release the previous font handle ... this leads to deplete
		// the windows GDI resources (so we use the standard font)
		//drw.SetTextFont(MinGUI::TextFont(18, std::string("Lucida Console"), TRUE));

		if(m_blinkTimer > 5) {
			drw.DrawText(m_width/2, m_height/2 - 20, std::string("GAME OVER"), d6gui::CENTER);
		}

		drw.DrawText(m_width/2, m_height/2 +  20, std::string("PRESS <FIRE> TO START"), d6gui::CENTER);
		
		drw.DrawText(m_width/2 - 90, m_height/2 +  60, std::string("LEFT, RIGHT"), d6gui::LEFT);
		drw.DrawText(m_width/2 - 90, m_height/2 +  80, std::string("UP"), d6gui::LEFT);
		drw.DrawText(m_width/2 - 90, m_height/2 + 100, std::string("DOWN"), d6gui::LEFT);

		drw.DrawText(m_width/2 + 10, m_height/2 +  60, std::string("Turn she ship"), d6gui::LEFT);
		drw.DrawText(m_width/2 + 10, m_height/2 +  80, std::string("Thrust"), d6gui::LEFT);
		drw.DrawText(m_width/2 + 10, m_height/2 + 100, std::string("Fire"), d6gui::LEFT);

	}
	
	if(m_status == GameStatus::StatusInGame && m_waitTimer >= c_maxWaitTimer && m_starship.GetExploded()) {

		d6gui::Drawing drw = d6gui::Drawing(backbuffer->GetBackBufferDC());
		drw.SetTextColor(d6gui::Color(255,255,255));

		if(m_blinkTimer > 5) {
			drw.DrawText(m_width/2, m_height/2, std::string("PRESS <FIRE> TO CONTINUE"), d6gui::CENTER);
		}

	}

	{
		d6gui::Drawing drw = d6gui::Drawing(backbuffer->GetBackBufferDC());
		drw.SetTextColor(d6gui::Color(255,255,255));
		
		{
			std::ostringstream stm;
			stm << "LIVES: "<< m_lives;

			drw.DrawText(5, 20, stm.str(), d6gui::LEFT);
		}

		{
			std::ostringstream stm;
			stm << "SCORE: "<< std::setw(9) << m_score;

			drw.DrawText(m_width - 15, 20, stm.str(), d6gui::RIGHT);
		}
	}

	//
	// Renders the ship only if we are inside a game *and* it is not destroyed
	//
	if(m_status == GameStatus::StatusInGame && !m_starship.GetExploded()) {

		m_sprites[SpriteShip]->Draw(d6gui::Drawing(backbuffer->GetBackBufferDC()), 
								m_starship.GetPosition().getX(),
								m_starship.GetPosition().getY(),
								m_starship.GetAngle());
		if(m_starship.GetThrust()) {
			m_sprites[SpriteThrust]->Draw(d6gui::Drawing(backbuffer->GetBackBufferDC()), 
											m_starship.GetPosition().getX(),
											m_starship.GetPosition().getY(),
											m_starship.GetAngle());
		}
	}

	//
	// Renders the asteroids
	//
	{
		std::vector<Asteroid*>::iterator vi;
		
		for(vi = m_asteroids.begin(); vi != m_asteroids.end(); vi ++) {
			
			m_sprites[2 + (*vi)->GetSize()]->Draw(d6gui::Drawing(backbuffer->GetBackBufferDC()), 
						   (*vi)->GetPosition().getX(),
						   (*vi)->GetPosition().getY());
		}
	}
	
	//
	// Renders the shoots
	//
	{
		std::vector<Shoot*>::iterator vi;
		
		for(vi = m_shoots.begin(); vi != m_shoots.end(); vi ++) {
			m_sprites[1]->Draw(d6gui::Drawing(backbuffer->GetBackBufferDC()), 
						   (*vi)->GetPosition().getX(),
						   (*vi)->GetPosition().getY());
		}
	}

	//
	// Renders the explosions
	//
	{
		std::vector<Explosion*>::iterator vi;
		
		for(vi = m_explosions.begin(); vi != m_explosions.end(); vi ++) {
			m_sprites[5]->Draw(d6gui::Drawing(backbuffer->GetBackBufferDC()), 
						   (*vi)->GetPosition().getX(),
						   (*vi)->GetPosition().getY(),
						   (*vi)->GetRotation(),
						   (*vi)->GetPhase()*50 + 1);
		}
	}

}


void Game::KeyFirePressed()
{

	//
	// Allow fire only if are in a game and the ship is not destroyed
	//
	if(m_status == GameStatus::StatusInGame && !m_starship.GetExploded() && !m_prevPressed) {
		m_prevPressed = TRUE;
		Shoot* shoot = new Shoot(m_starship.GetPosition(), m_starship.GetAngle(), 10);
		shoot->SetSpeed(shoot->GetSpeed() + m_starship.GetSpeed());
		shoot->SetLimits(m_starship.GetLimits());
		shoot->Update();

		m_shoots.push_back(shoot);
	}

	// If we are in a game, the resurrection timer is elapsed and the ship
	// is destroyed then with this key we must place a new starship in the game
	if(m_status == GameStatus::StatusInGame && m_starship.GetExploded() && m_waitTimer >= c_maxWaitTimer) {
		SetupStarship();
	}

	//
	// If are outside a game, fire key starts a new one
	// 
	if(m_status == GameStatus::StatusGameOver) {
		StartGame();
	}
}

//
// Game::StartGame()
//
// Starts a new game but before cleanup all object allocated
// previously
//
void Game::StartGame()
{
	// Fill the lives avalible
	m_lives = 3;

	// Zero the score
	m_score = 0;

	// Cleanup previously allocated resources
	DeleteGame();

	// Setup Starship
	SetupStarship();

	// Setup 1st level
	StartFirstLevel();

	m_status = GameStatus::StatusInGame;

	// Reset the wai time (however, it not needed)
	m_waitTimer = 0;

}

//
// Game::DeleteGame()
//
// Cleanup resources of the current game
//
// Remember that all objects are allocated thought the new
// keyword, so they must released with appropriate delete
//
void Game::DeleteGame()
{
	// Clear Asteroids
	{
		std::vector<Asteroid*>::iterator vi;

		if(m_asteroids.size() > 0) {
			for(vi = m_asteroids.begin(); vi != m_asteroids.end(); vi ++) {
				delete (*vi);
			}
		}

		// Empty the vector (it is allocated on the stack)
		m_asteroids.clear();
	}

	// Clear Shoots
	{
		std::vector<Shoot*>::iterator vi;
		
		if(m_shoots.size() > 0) {
			for(vi = m_shoots.begin(); vi != m_shoots.end(); vi ++) {
				delete (*vi);
			}
		}
		// Empty the vector (it is allocated on the stack)
		m_shoots.clear();
	}

	// Clear Explosions
	{
		std::vector<Explosion*>::iterator vi;

		if(m_explosions.size() > 0) {
			for(vi = m_explosions.begin(); vi != m_explosions.end(); vi ++) {
				delete (*vi);
			}
		}

		// Empty the vector (it is allocated on the stack)
		m_explosions.clear();
	}

}

//
// Game::InitAsteroids(number, maxSpeed)
//
// Initialize randomly a <number> of asteroids with a maximum speed
// maxSpeed ...
//
void Game::InitAsteroids(int number, int maxSpeed)
{

	int n;

	for(n = 0; n < number; n ++) {
		
		int posx;
		int posy;

		bool repeat;

		do {
			repeat = FALSE;

			double rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
			double rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);
			posx = (int) ( rndx * ((double) m_width));
			posy = (int) ( rndy * ((double) m_height));

			if((posx > m_width / 6) && (posx < 5 * m_width / 6)) {
				repeat = TRUE;
			} else if((posy > m_height / 6) && (posy < 5 * m_height / 6)) {
				repeat = TRUE;
			}

		} while(repeat);

		int speedx;
		int speedy;
		
		{
			double rndx = ((double) rand()) / (((double) RAND_MAX) + 1.0);
			double rndy = ((double) rand()) / (((double) RAND_MAX) + 1.0);

			speedx = (int) ( rndx * ((double) maxSpeed));
			speedy = (int) (sqrt( ((double) maxSpeed) * ((double) maxSpeed) - 
								((double) speedx  ) * ((double) speedx  )   )   ); 
		}		

		m_asteroids.push_back(new Asteroid(Vertex(posx, posy), Vertex(speedx, speedy), Vertex(m_width, m_height)));

	}

}

void Game::StartFirstLevel()
{
	m_level = -1;

	StartNextLevel();
}

void Game::StartNextLevel()
{

	switch(++m_level) {
	case 0:
		InitAsteroids(1, 2);
		break;
	case 1:
		InitAsteroids(2, 2);
		break;
	case 3:
		InitAsteroids(3, 2);
		break;
	case 4:
		InitAsteroids(3, 4);
		break;
	case 5:
		InitAsteroids(4, 2);
		break;
	default:
		InitAsteroids(4, 4);
		break;
	}

	m_endLevelTimer = 0;
}

// Setup Starship
void Game::SetupStarship()
{

	m_starship = StarShip();

	m_starship.SetLimits(Vertex(m_width, m_height));
	m_starship.SetPosition(Vertex(m_width/2, m_height/2));
	m_starship.SetSpeed(Vertex(0, 0));
}
