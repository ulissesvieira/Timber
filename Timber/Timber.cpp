// Timber.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>

using namespace sf;

void initGraphics(Texture *texture, Sprite *sprite);
void handleKeyboard(RenderWindow *window);
void updateScene();
void drawScene(RenderWindow *window, Sprite *sprite);

int main() {
	// Create a video mode object
	VideoMode vm(1920, 1080);
	// Create and open a window for the game 
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);
	// Create a texture to hold a graphic on the GPU 
	Texture textureBackgroud;
	// Create a sprite
	Sprite spriteBackground;

	initGraphics(&textureBackgroud, &spriteBackground);

	while (window.isOpen()) {
		/* Handle the player input */
		handleKeyboard(&window);

		/* Update the scene */
		updateScene();

		/* Draw the scene */
		drawScene(&window, &spriteBackground);
	}

    return 0;
}

void handleKeyboard(RenderWindow *window) {
	if (Keyboard::isKeyPressed(Keyboard::Escape)) {
		window->close();
	}
}

void drawScene(RenderWindow *window, Sprite *sprite) {
	// Clear verything form the last frame
	window->clear();

	// Draw our game scene here
	window->draw(*sprite);

	// Show everythig we just drew
	window->display();
}

void updateScene() {

}

void initGraphics(Texture *texture, Sprite *sprite) {
	// Load a graphic into the texture
	texture->loadFromFile("graphics/background.png");

	// Attach the texture to the sprite
	sprite->setTexture(*texture);
	// Set the spriteBackground to cover the screen
	sprite->setPosition(0, 0);
}