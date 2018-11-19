#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
using namespace sf;

//function declaration
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//where is the player/branch?
//left or right?
enum class side { LEFT, RIGHT, NONE };
side branchPositions[NUM_BRANCHES];

int main()
{
	//create a video mode object
	VideoMode vm(1920, 1080);

	//create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	//create a texture to hold a graphic on the GPU
	Texture textureBackground;

	//load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");

	//create a sprite
	Sprite spriteBackground;

	//attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);

	//set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	//make a tree sprite
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");

	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);

	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree);
	spriteTree2.setPosition(30, -200);

	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree);
	spriteTree3.setPosition(1600, -120);

	//prepare the bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");

	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	//is the bee currently moving?
	bool beeActive = false;

	//how fast can the bee fly
	float beeSpeed = 0.0f;

	//make 3 cloud sprites from 1 texture
	Texture textureCloud;

	//load 1 new texture
	textureCloud.loadFromFile("graphics/cloud.png");

	//3 new sprites with the same texture
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	//are the clouds currently on screen?
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//how fast is each cloud?
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//variables to control time itself
	Clock clock;

	//time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

	//track whether the game is running
	bool paused = true;

	//draw some text
	int score = 0;
	Text messageText;
	Text scoreText;

	//we need to choose a font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//set the font to our message
	messageText.setFont(font);
	scoreText.setFont(font);

	//assign the actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("score = 0");

	//make it really big
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//choose a color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

	scoreText.setPosition(20, 20);

	//prepare 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");

	//set texture for each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the sprite's origin dead center
		//we can then spin it round without changing its position
		branches[i].setOrigin(220, 20);
	}

	//prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580, 720);

	//the player starts on the left
	side playerSide = side::LEFT;

	//prepare the gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 860);

	//prepare the axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//line up the axe with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//prepare the flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//control player input
	bool acceptInput = false;

	while (window.isOpen())
	{
		/*
		*********************************
		Handle the players input
		*********************************
		*/
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		//start the game
		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;

			//reset the time and the score
			score = 0;
			timeRemaining = 6;

			//make all the branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++)
			{
				branchPositions[i] = side::NONE;
			}

			//make sure the gravestone is hidden
			spriteRIP.setPosition(675, 2000);

			//move the player into position
			spritePlayer.setPosition(580, 720);
			
			acceptInput = true;
		}
		
		// Wrap the player controls to
		// Make sure we are accepting input
		if (acceptInput)
		{
			//first handle pressing the right cursor key
			if (Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D));
			{
				//make sure the player is on the right
				playerSide = side::RIGHT;
				score++;

				//add time to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				//update the branches
				updateBranches(score);

				//set the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;
			}
			//handle the left cursor key
			if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))
			{
				//make sure the player is on the left
				playerSide = side::LEFT;
				score++;
				
				//add time to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				
				//update the branches
				updateBranches(score);

				//set the log flying to the right
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
			}
		}

		/*
		*********************************
		Update the scene
		*********************************
		*/
		if (!paused)
		{

			//measure time
			Time dt = clock.restart();

			//substract from the amount of time remaining
			timeRemaining -= dt.asSeconds();
			//size up the time bar
			timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 0.0f)
			{
				//pause the game
				paused = true;

				//change the message shown to the player
				messageText.setString("out of time!");

				//reposition the text based on its new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
			}
			//setup the bee
			if (!beeActive)
			{
				//how fast is the bee
				srand((int)time(0));
				beeSpeed = (rand() % 200) + 200;

				//how high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
				//move the bee
			{
				spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed*dt.asSeconds()), spriteBee.getPosition().y);

				//has the bee reached the right hand edge of the screen?
				if (spriteBee.getPosition().x < -100)
					beeActive = false;
			}
			//manage the clouds
			//cloud 1
			if (!cloud1Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else
			{
				spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed*dt.asSeconds()), spriteCloud1.getPosition().y);

				//has the cloud reached the right hand edge of the screen?
				if (spriteCloud1.getPosition().x > 1920)
				{
					//set it up ready to be a whole new cloud net frame
					cloud1Active = false;
				}
			}
			//cloud 2
			if (!cloud2Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else
			{
				spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed*dt.asSeconds()), spriteCloud2.getPosition().y);

				//has the cloud reached the right hand edge of the screen?
				if (spriteCloud2.getPosition().x > 1920)
				{
					//set it up ready to be a whole new cloud net frame
					cloud1Active = false;
				}
			}
			//cloud 3
			if (!cloud3Active)
			{
				//how fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);

				//how high is the cloud
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else
			{
				spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed*dt.asSeconds()), spriteCloud3.getPosition().y);

				//has the cloud reached the right hand edge of the screen?
				if (spriteCloud3.getPosition().x > 1920)
				{
					//set it up ready to be a whole new cloud net frame
					cloud3Active = false;
				}
			}

			//update the score text
			std::stringstream ss;
			ss << "score = " << score;
			scoreText.setString(ss.str());

			//update the branch sprites
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT)
				{
					//move the sprite to the left side
					branches[i].setPosition(610, height);

					//flip the sprite round the other way
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT)
				{
					//move the sprite to the right side
					branches[i].setPosition(1330, height);
					
					//set the sprite rotation to normal
					branches[i].setRotation(0);
				}
				else
				{
					//hide the branch
					branches[i].setPosition(3000, height);
				}
			}
		} //end if (!paused)

		/*
		*********************************
		Draw the scene
		*********************************
		*/
		//clear everything from the last frame
		window.clear();

		//draw our game scene here
		window.draw(spriteBackground);

		//draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		//draw the branches
		for (int i = 0;i < NUM_BRANCHES;i++)
		{
			window.draw(branches[i]);
		}
		
		//draw the trees
		window.draw(spriteTree);
		window.draw(spriteTree2);
		window.draw(spriteTree3);

		//draw the player
		window.draw(spritePlayer);

		//draw the axe
		window.draw(spriteAxe);

		//draw the flying log
		window.draw(spriteLog);

		//draw the gravestone
		window.draw(spriteRIP);

		//draw the insect
		window.draw(spriteBee);

		//draw the score
		window.draw(scoreText);

		//draw the time
		window.draw(timeBar);
		if (paused)
		{

			//draw our messge
			window.draw(messageText);
		}
		//show everything we just drew
		window.display();
	}

	return 0;
}
//function definition
void updateBranches(int seed)
{
	//move all the branches down one place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPositions[j] = branchPositions[j - 1];
	}

	//spawn a new branch at position 0
	//LEFT, RIGHT or NONE
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