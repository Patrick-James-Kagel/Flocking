#pragma once

using namespace sf;

class BoidSystem
{
private:


public:

	std::vector<Boid> boidList;
	std::vector<Boid> boidGrid[10][10];

	float currentAlign;
	float currentSeparate;
	float currentCohesion;

	BoidSystem();

	void AddBoid(RenderWindow& p_window);
	void RemoveBoid();
	void InitializeFlock(int amount, RenderWindow& p_window);
	void Adjust(int mode, float amount);
	void Toggle(int mode, bool on);
};

BoidSystem::BoidSystem()
{

	currentAlign = 1;
	currentSeparate = 1;
	currentCohesion = .1;

	boidList = std::vector<Boid>();
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			std::vector<Boid>* vectorPointer = new std::vector<Boid>();
			boidGrid[i][j] = *vectorPointer;

		}
	}
}

void BoidSystem::AddBoid(RenderWindow& p_window)
{
	Boid* boidMaker = new Boid((int)rand() % p_window.getSize().x, (int)rand() % p_window.getSize().y);

	boidMaker->aliForce = currentAlign;
	boidMaker->sepForce = currentSeparate;
	boidMaker->cohForce = currentCohesion;

	boidList.push_back(*boidMaker);
}

void BoidSystem::RemoveBoid()
{
	boidList.pop_back();
}

void BoidSystem::Adjust(int mode, float amount)
{
	switch (mode)
	{
	case 0:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (currentAlign >= .01 && currentAlign < 2)
			{
				boidList[i].aliForce += amount;
				currentAlign = boidList[i].aliForce;
			}

			if (currentAlign >= 2)
				currentAlign = 1.99;
		}
		break;

	case 1:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (currentSeparate >= .01 && currentSeparate < 2)
			{
				boidList[i].sepForce += amount;
				currentSeparate = boidList[i].sepForce;
			}

			if (currentSeparate >= 2)
				currentSeparate = 1.99;
		}
		break;

	case 2:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (currentCohesion >= .01 && currentCohesion < 2)
			{
			boidList[i].cohForce += amount;
			currentCohesion = boidList[i].cohForce;
			}

			if (currentCohesion >= 2)
				currentCohesion = 1.99;
		}
		break;

	default:
		std::cout << "Say What?" << std::endl;
		break;
	}
}

void BoidSystem::Toggle(int mode, bool on)
{
	switch (mode)
	{
	case 0:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (on)
				boidList[i].aliForce = currentAlign;
			else
				boidList[i].aliForce = 0;
		}
		break;

	case 1:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (on)
				boidList[i].sepForce = currentSeparate;
			else
				boidList[i].sepForce = 0;
		}
		break;

	case 2:
		for (int i = 0; i < boidList.size(); i++)
		{
			if (on)
				boidList[i].cohForce = currentCohesion;
			else
				boidList[i].cohForce = 0;
		}
		break;

	default:
		std::cout << "Say What?" << std::endl;
		break;
	}
}

void BoidSystem::InitializeFlock(int amount, RenderWindow& p_window)
{
	//Creates a custom-sized flock for initialization and places it
	//In the center of the screen
	//Will only create flock of size 150 or lower;

	int rowCounter = 0;
	int row = 0;
	int column = 0;
	int spacing = 200;

	if (amount > 0 && amount <= 150)
	{
		for (int i = 0; i < amount; i++)
		{
			Boid* boidPointer = new Boid((column + (p_window.getSize().x) / 2 - (p_window.getSize().x) / 5), (row + (p_window.getSize().y) / 2 - (p_window.getSize().y) / 5));


			//boidPointer->triangle.setPosition((column + (p_window.getSize().x) / 2 - (p_window.getSize().x) / 5), (row + (p_window.getSize().y) / 2 - (p_window.getSize().y) / 5));

			boidList.push_back(*boidPointer);

			rowCounter++;
			column += boidPointer->triangle.getScale().x * spacing;
			if (rowCounter % 10 == 0)
			{
				column = 0;
				row += boidPointer->triangle.getScale().y * spacing;
			}
		}
	}
	else if (amount > 150)
	{
		std::cout << "Amount of initial flock too high!" << std::endl;
	}
	else if (amount < 0)
	{
		std::cout << "Amount of initial flock too low!" << std::endl;
	}
}