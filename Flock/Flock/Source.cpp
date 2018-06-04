#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>

#include <iostream>
#include <vector>

#include "Boid.h"
#include "BoidSystem.h"
#include "Target.h"
#include "Bucket.h"


using namespace sf;

void DrawBoids(RenderWindow& p_window);

BoidSystem boidSystem;
Target target;

int resolution;
int addCounter;
int currentMode;

bool mouseDown;
bool sepOff;
bool aliOff;
bool cohOff;

Text instructions;
Text mode;
Text scalar;
Text onOff;

Font font;

int main()
{
	//---------------Creating window--------------------
	sf::RenderWindow window(sf::VideoMode(1500, 1000), "Flocking");


	//---------------Creating Game Obj0ects---------------
	boidSystem = BoidSystem();
	resolution = 2;
	addCounter = 0;
	currentMode = 0;

	boidSystem.InitializeFlock(0, window); //Creating the Boid Flock
	mouseDown = false;
	sepOff = false;
	aliOff = false;
	cohOff = false;

	instructions.setFillColor(Color::White);
	mode.setFillColor(Color::White);
	scalar.setFillColor(Color::White);
	onOff.setFillColor(Color::White);

	font.loadFromFile("Bitwise.ttf");

	mode.setFont(font);
	mode.setCharacterSize(20);
	mode.setString("No Mode Selected");
	mode.setPosition(0, 50);

	scalar.setFont(font);
	scalar.setCharacterSize(20);
	scalar.setString(std::to_string(0));
	scalar.setPosition(300, 50);

	onOff.setFont(font);
	onOff.setCharacterSize(20);
	onOff.setString("Off");
	onOff.setPosition(200, 50);

	instructions.setFont(font);
	instructions.setCharacterSize(20);
	instructions.setString("A = Alignment, S = Separation, D = Cohesion. Z and X to Adjust Values. Up and Down to Add/Remove Boids");
	instructions.setPosition(0, window.getSize().y - 50);

	//-------------Game Loop--------------------
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			//--------Input Controls-------------

			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Up)
				{
					if (addCounter <= 70)
					{
						boidSystem.AddBoid(window);

						addCounter++;
					}
				}
				else if (event.key.code == Keyboard::Down)
				{
					if (addCounter > 0)
					{
						boidSystem.RemoveBoid();
					}
				}
				else if (event.key.code == Keyboard::A)
				{
					mode.setString("Alignment");
					currentMode = 0;

				}
				else if (event.key.code == Keyboard::S)
				{
					mode.setString("Seperation");
					currentMode = 1;

				}
				else if (event.key.code == Keyboard::D)
				{

					mode.setString("Cohesion");
					currentMode = 2;
				}
				else if (event.key.code == Keyboard::Z)
				{

					boidSystem.Adjust(currentMode, .01);

					if (currentMode == 0)
						scalar.setString(std::to_string(boidSystem.currentAlign));
					else if (currentMode == 1)
						scalar.setString(std::to_string(boidSystem.currentSeparate));
					if (currentMode == 2)
						scalar.setString(std::to_string(boidSystem.currentCohesion));
				}
				else if (event.key.code == Keyboard::X)
				{

					boidSystem.Adjust(currentMode, -.01);

					if (currentMode == 0)
						scalar.setString(std::to_string(boidSystem.currentAlign));
					else if (currentMode == 1)
						scalar.setString(std::to_string(boidSystem.currentSeparate));
					if (currentMode == 2)
						scalar.setString(std::to_string(boidSystem.currentCohesion));
				}

				else if (event.key.code == Keyboard::Return)
				{
					if (currentMode == 0)
					{
						if (aliOff)
						{
							boidSystem.Toggle(currentMode, true);
							aliOff = false;

							onOff.setString("On");
						}
						else if (!aliOff)
						{
							boidSystem.Toggle(currentMode, false);
							aliOff = true;

							onOff.setString("Off");
						}
					}
					else if (currentMode == 1)
					{
						if (sepOff)
						{
							boidSystem.Toggle(currentMode, true);
							sepOff = false;

							onOff.setString("On");
						}
						else if (!sepOff)
						{
							boidSystem.Toggle(currentMode, false);
							sepOff = true;

							onOff.setString("Off");
						}
					}
					else if (currentMode == 2)
					{
						if (cohOff)
						{
							boidSystem.Toggle(currentMode, true);
							cohOff = false;

							onOff.setString("On");
						}
						else if (!cohOff)
						{
							boidSystem.Toggle(currentMode, false);
							cohOff = true;

							onOff.setString("Off");
						}
					}
				}
			}
		}
		
		


		window.clear();
		window.draw(instructions);
		window.draw(mode);
		window.draw(scalar);
		window.draw(onOff);
		DrawBoids(window);
		window.display();
	}

	return 0;
}

void DrawBoids(RenderWindow& p_window)
{

	//for (int i = 0; i < resolution; i++)
	//{
	//	for (int j = 0; j < resolution; j++)
	//	{
	//		
	//	}
	//}

	//for (int i = 0; i < boidSystem.boidList.size(); i++)
	//{

	//	int column = int(boidSystem.boidList[i].location.x) / (resolution * resolution);
	//	int row = int(boidSystem.boidList[i].location.y) / (resolution * resolution);

	//	boidSystem.boidGrid[column][row].push_back(boidSystem.boidList[i]);
	//}

	//For every Boid, make sure it loops around within the window and doesn't ever go outside of the window borders

	for (int i = 0; i < boidSystem.boidList.size(); i++)
	{
		int column = int(boidSystem.boidList[i].location.x) / (resolution * resolution);
		int row = int(boidSystem.boidList[i].location.y) / (resolution * resolution);

		boidSystem.boidList[i].Move(boidSystem.boidList, Mouse::getPosition());

		if (boidSystem.boidList[i].triangle.getPosition().y >= p_window.getSize().y)
		{
			std::cout << "upper wrap" << std::endl;
			boidSystem.boidList[i].triangle.setPosition(boidSystem.boidList[i].triangle.getPosition().x, 10);
		}
		else if (boidSystem.boidList[i].triangle.getPosition().y < 0)
		{
			std::cout << "lower wrap" << std::endl;
			boidSystem.boidList[i].triangle.setPosition(boidSystem.boidList[i].triangle.getPosition().x, p_window.getSize().y - 10);
		}

		if (boidSystem.boidList[i].triangle.getPosition().x >= p_window.getSize().x)
		{
			boidSystem.boidList[i].triangle.setPosition(10, boidSystem.boidList[i].triangle.getPosition().y);
		}
		else if (boidSystem.boidList[i].triangle.getPosition().x < 0)
		{
			boidSystem.boidList[i].triangle.setPosition(p_window.getSize().x - 10, boidSystem.boidList[i].triangle.getPosition().y);
		}

		p_window.draw(boidSystem.boidList[i].triangle);
	}

	//The exact same treatment for the target boid
	/*
	if (target.triangle.getPosition().y > p_window.getSize().y)
	{
		target.triangle.setPosition(target.triangle.getPosition().x, 0);
	}
	else if (target.triangle.getPosition().y < 0)
	{
		target.triangle.setPosition(target.triangle.getPosition().x, p_window.getSize().y);
	}

	if (target.triangle.getPosition().x > p_window.getSize().x)
	{
		target.triangle.setPosition(0, target.triangle.getPosition().y);
	}
	else if (target.triangle.getPosition().x < 0)
	{
		target.triangle.setPosition(p_window.getSize().x, target.triangle.getPosition().y);
	}

	target.CalculateTurns();
	target.Move();
	p_window.draw(target.triangle);
	*/
}
