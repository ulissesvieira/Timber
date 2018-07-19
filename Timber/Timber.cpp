// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

const int CLOUD_NUMBER = 3;
const int NUMBER_CLOUD_PARAMETERS = 3;
const int NUM_BRANCHES = 6;

// Where is the player/branche
// Left or right
enum class side { LEFT, RIGHT, NONE };

std::array<Sprite, NUM_BRANCHES> branches;
std::array<side, NUM_BRANCHES> branchPositions;

void handleKeyboard(RenderWindow& window, bool& pauseGame, int& score, float& timeRemaining);
void updateScene();
void drawScene(RenderWindow& window, std::vector<Sprite>& sprites);
void initSprite(Sprite& sprite, Texture& texture, std::string fileName, int x, int y, bool loadTexture = true);
void initClouds(Texture& texture, std::array<Sprite, CLOUD_NUMBER>& sprites, std::array<bool, CLOUD_NUMBER>& activeClouds, std::array<float, CLOUD_NUMBER>& speedClouds);

void updateBee(Sprite& sprite, float& speed, bool& active, Time dt);
void updateClouds(Sprite& sprite, float& speed, bool& active, Time dt, std::array<int, NUMBER_CLOUD_PARAMETERS> params);

void initTextDisplay(Text& message, Text& score, Font& font);

void initBranches(Texture& texture, std::array<Sprite, NUM_BRANCHES>& branches, std::array<side, NUM_BRANCHES>& branchPositions);
void updateBranchesPositions(std::array<side, NUM_BRANCHES>& branchPositions, int seed);
void updateBranches(std::array<Sprite, NUM_BRANCHES>& branches, std::array<side, NUM_BRANCHES>& branchPositions);

int main() {
	// Will contain all sprites 
	//std::vector<Sprite> sprites;	

	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game 
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	// Create a texture to hold a graphic on the GPU 
	Texture textureBackgroud;
	// Create a sprite
	Sprite spriteBackground;
	// Make a tree sprite
	Texture textureTree;
	Sprite spriteTree;
	// Prepare the bee
	Texture textureBee;
	Sprite spriteBee;
	// Make 3 cloud sprites from 1 texture
	Texture textureCloud;
	std::array<Sprite, CLOUD_NUMBER> spriteClouds;
	std::array<bool, CLOUD_NUMBER> activeClouds;
	std::array<float, CLOUD_NUMBER> speedClouds;
	// Branches
	Texture textureBranch;

	initSprite(spriteBackground, textureBackgroud, "graphics/background.png", 0, 0);
	initSprite(spriteTree, textureTree, "graphics/tree.png", 810, 0);
	initSprite(spriteBee, textureBee, "graphics/bee.png", 0, 800);
	/*sprites.push_back(spriteBackground);
	sprites.push_back(spriteTree);
	sprites.push_back(spriteBee);*/

	initClouds(textureCloud, spriteClouds, activeClouds, speedClouds);
	// here for now... some logic will be added later
	//sprites.insert(sprites.end(), spriteClouds.begin(), spriteClouds.end());

	initBranches(textureBranch, branches, branchPositions);

	// Is the bee current moving?
	bool beeActive = false;
	// How fast can the bee fly 
	float beeSpeed = 0.0f;

	// VAriable to control time itself
	Clock clock;

	// Track whether the game is runing
	bool paused = false;

	// Draw some text
	int score = 0;
	Text messageText, scoreText;
	Font font;
	initTextDisplay(messageText, scoreText, font);

	// Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	// testing
	updateBranchesPositions(branchPositions, 1);
	updateBranchesPositions(branchPositions, 2);
	updateBranchesPositions(branchPositions, 3);
	updateBranchesPositions(branchPositions, 4);
	updateBranchesPositions(branchPositions, 5);

	while (window.isOpen()) {
		/* Handle the player input */
		handleKeyboard(window, paused, score, timeRemaining);

		/* Update the scene */
		if (!paused) {
			Time dt = clock.restart();

			updateScene();

			// Subtract from the amount of time remaining
			timeRemaining -= dt.asSeconds();

			// Size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f) {
				// Pause the game
				paused = true;

				// Changed the message shown to the player
				messageText.setString("Out of time!");

				// Reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			}

			// Update the bee
			updateBee(spriteBee, beeSpeed, beeActive, dt);

			// Manage the clouds
			std::array<int, NUMBER_CLOUD_PARAMETERS> params = { 10, 150, 0 };
			updateClouds(spriteClouds[0], speedClouds[0], activeClouds[0], dt, params);
			params[0] = 20; params[1] = 300; params[2] = -150;
			updateClouds(spriteClouds[1], speedClouds[1], activeClouds[1], dt, params);
			params[0] = 30; params[1] = 450; params[2] = -150;
			updateClouds(spriteClouds[2], speedClouds[2], activeClouds[2], dt, params);

			// Update the score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			updateBranches(branches, branchPositions);
		}		

		/* Draw the scene */
		//drawScene(window, sprites);
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteTree);
		window.draw(spriteBee);

		for (unsigned int i = 0; i < CLOUD_NUMBER; i++) {
			window.draw(spriteClouds[i]);
		}

		for (unsigned int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}

		// Draw the score
		window.draw(scoreText);
		// Draw the timebar
		window.draw(timeBar);
		if (paused) {
			// Draw our message
			window.draw(messageText);
		}

		window.display();
	}	

    return 0;
}

void handleKeyboard(RenderWindow& window, bool& pauseGame, int& score, float& timeRemaining) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window.close();
	}

	if (Keyboard::isKeyPressed(Keyboard::Return)) {
		pauseGame = !pauseGame;

		score = 0;
		timeRemaining = 5;
	}
}

void drawScene(RenderWindow& window, std::vector<Sprite>& sprites) {
	// Clear verything form the last frame
	window.clear();

	// Draw our game scene here
	for (Sprite& sprite : sprites) {
		window.draw(sprite);
	}

	// Show everythig we just drew
	window.display();
}

void updateScene() {

}

void initSprite(Sprite& sprite, Texture& texture, std::string fileName, int x, int y, bool loadTexture) {
	if (loadTexture) {
		// Load a graphic into the texture
		texture.loadFromFile(fileName);
	}	

	// Attach the texture to the sprite
	sprite.setTexture(texture);
	// Set the spriteBackground to cover the screen
	sprite.setPosition(x, y);
}

void initClouds(Texture& texture, std::array<Sprite, CLOUD_NUMBER>& sprites, std::array<bool, CLOUD_NUMBER>& activeClouds, std::array<float, CLOUD_NUMBER>& speedClouds) {
	std::array<std::pair<int, int>, CLOUD_NUMBER> coordinates = { {{0, 0}, {0, 250}, {0, 500}} };

	texture.loadFromFile("graphics/cloud.png");
	
	for (unsigned int i = 0; i < CLOUD_NUMBER; i++) {
		activeClouds[i] = false;
		speedClouds[i] = 0.0f;

		initSprite(sprites[i], texture, "", coordinates[i].first, coordinates[i].second, false);
	}
}

void updateBee(Sprite& sprite, float& speed, bool& active, Time dt) {
	if (!active) {
		// How fast is the bee
		srand((int)time(0));
		speed = (rand() % 200) + 200;

		// How high is the bee
		srand((int)time(0) * 10);
		float height = (rand() % 500) + 500;
		sprite.setPosition(2000, height);
		active = true;
	}
	else {
		float x = sprite.getPosition().x - (speed * dt.asSeconds());
		float y = sprite.getPosition().y;
		sprite.setPosition(x, y);

		// Has the bee reached the righthand edge of the screen?
		if (sprite.getPosition().x < -100) {
			// Set it up ready to be a whole new cloud next frame
			active = false;
		}
	}
}

void updateClouds(Sprite& sprite, float& speed, bool& active, Time dt, std::array<int, NUMBER_CLOUD_PARAMETERS> params) {
	if (!active) {
		// How fast is the cloud
		srand((int)time(0) * params[0]);
		speed = (rand() % 200);

		// How high is the cloud
		srand((int)time(0) * params[0]);
		float height = (rand() % params[1]) + params[2];
		sprite.setPosition(2000, height);
		active = true;
	}
	else {
		float x = sprite.getPosition().x - (speed * dt.asSeconds());
		float y = sprite.getPosition().y;
		sprite.setPosition(x, y);

		// Has the cloud reached the right hand edge of the screen?
		if (sprite.getPosition().x < -300) {
			// Set it up ready to be a whole new cloud next frame
			active = false;
		}
	}
}

void initTextDisplay(Text& message, Text& score, Font& font) {
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// Set the font to our message
	message.setFont(font);
	score.setFont(font);

	// Assign the actual message
	message.setString("Press Enter to start!");
	score.setString("Score = 0");

	// Make it really big
	message.setCharacterSize(75);
	score.setCharacterSize(100);

	// Choose the color
	message.setFillColor(Color::White);
	score.setFillColor(Color::White);

	// Float the text
	FloatRect textRect = message.getLocalBounds();

	message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	message.setPosition(1920 / 2.0f, 1080 / 2.0f);
	score.setPosition(20, 20);
}

void initBranches(Texture& texture, std::array<Sprite, NUM_BRANCHES>& branches, std::array<side, NUM_BRANCHES>& branchPositions) {
	// Prepare 6 branches	
	texture.loadFromFile("graphics/branch.png");

	// set texture for each branch
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(texture);
		branches[i].setPosition(-2000, -2000);

		// set the sprite's origin to dead center
		// We can then spin it around whithout changind its position
		branches[i].setOrigin(200, 20);		
	}	
}

void updateBranchesPositions(std::array<side, NUM_BRANCHES>& branchPositions, int seed) {
	// Move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--) {
		branchPositions[j] = branchPositions[j - 1];
	}

	// Spawn a new branch at position 0
	// LEFT, RIGHT or NONE
	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{	
		case 0:
			branchPositions[0] = side::LEFT;
			break;
		case 1: 
			branchPositions[0] = side::RIGHT;
			break;
		default:
			branchPositions[0] = side::NONE;
			break;
	}
}

void updateBranches(std::array<Sprite, NUM_BRANCHES>& branches, std::array<side, NUM_BRANCHES>& branchPositions) {
	for (int i = 0; i < NUM_BRANCHES; i++) {
		float height = i * 150;		

		switch (branchPositions[i])
		{
			case side::LEFT: 
			{
				// Move the sprite to the left side
				branches[i].setPosition(610, height);

				// Flip the sprite around the other way
				branches[i].setRotation(180);

				break;
			}				
			case side::RIGHT:
			{
				// Move the sprite to the right side
				branches[i].setPosition(1330, height);

				// Flip the sprite rotation to normal
				branches[i].setRotation(0);

				break;
			}
			default:
			{
				// Hide the branch
				branches[i].setPosition(3000, height);

				break;
			}
		}
	}
}