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

#if !defined(D6_GAME_H)
#define D6_GAME_H

#include <vector>
#include "BackBuffer.h"
#include "VSprite.h"
#include "StarObjects.h"

enum class GameStatus {
	StatusInGame = 1,
	StatusGameOver = 0
};

//
// Class Game
//
// It handles all the game logic: state changes, updates of elements,
// container for the vector sprites and has a method to be used to
// render a game frame to the output device
//
// It is platform-indipendent
//

class Game {

public:
	

private:
	void SetupStarship();

	// Lives available
	int m_lives;

	// Game level	 
	int m_level;

	// Game Score
	int m_score;

	// Timer to pause between levels
	int m_endLevelTimer;

	// Timer to blink text and other things need to blinking
	int m_blinkTimer;

	// Timer to wait a restart after the ship is once destroyed
	int m_waitTimer;

	// Start the firs level
	void StartFirstLevel();
	// Start the next level
	void StartNextLevel();
	// Initialize a "number" asteroids with a "maxSpeed" maxSpeed
	void InitAsteroids(int number, int maxSpeed);
	// Delete all objects allocated by this object (needed to start
	// a new game) minus the vectorized sprites
	void DeleteGame();
	
	// Loads the sprites
	void LoadSprites();
	// Deletes the sprites
	void DeleteSprites();

	// Sarts a new game
	void StartGame();

	// Constant for text blinking (used with m_blinkTimer)
	static const int c_maxBlinkTimer;
	// Constant for pause between levels (used with m_endLevelTimer);
	static const int c_pauseBeweenLevels;
	// Constant for wait a "resurrection" of the ship (used with m_waitRestart);
	static const int c_maxWaitTimer;

	//
	// Position of sprites inside the sprite vector
	//
	enum Sprites {
		SpriteShip = 0,
		SpriteShoot = 1,
		SpriteAsteroidBig = 2,
		SpriteAsteroidMedium = 3,
		SpriteAsteroidSmall = 4,
		SpriteExplosion = 5,
		SpriteThrust = 6
	};

	//
	// vectors of game elements 
	//
	// Display sprites
	std::vector<VSprite*>	m_sprites;

	// Logic asteroids
	std::vector<Asteroid*>	m_asteroids;
	// Logic shoots
	std::vector<Shoot*>		m_shoots;
	// Logic Explosions
	std::vector<Explosion*> m_explosions;
	// Logic Ship
	StarShip				m_starship;

	// Board size 
	int m_width;
	int m_height;

	// Indicate the status of the game
	enum GameStatus m_status;

	int m_prevPressed = FALSE;

public:
	
	// The fire key has been pressed (this is the fire processor)
	// and a proxy if the ship is alive
	void KeyFirePressed();
	void KeyFireReleased() { m_prevPressed = FALSE; }

	// Invoke the frame renderer to the passed backbuffer
	void Render(BackBuffer* backbuffer);

	// Construction/Destruction
	Game(int width, int height);
	~Game()					{ DeleteGame(); DeleteSprites(); }

	// Update all the logic element inside the game accordly with
	// the game status (this must be called at regular time)
	void Step();

	// Game keys proxies
	void KeyLeftPressed()   { m_starship.KeyLeftPressed();   }
	void KeyRightPressed()  { m_starship.KeyRightPressed();  }
	void KeyLeftReleased()  { m_starship.KeyLeftReleased();  }
	void KeyRightReleased() { m_starship.KeyRightReleased(); }

	void KeyUpPressed()		{ m_starship.KeyUpPressed();  }
	void KeyUpReleased()	{ m_starship.KeyUpReleased(); }

};

#endif // !defined(D6_GAME_H)