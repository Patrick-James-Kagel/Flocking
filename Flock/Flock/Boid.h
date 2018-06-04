#pragma once

using namespace sf;

class Boid
{

private:


public:
	ConvexShape triangle;

	Vector2f target;

	Vector2f location;
	Vector2f velocity;
	Vector2f acceleration;

	float sepForce;
	float aliForce;
	float cohForce;

	Vector2f desiredVelocity;

	Vector2f steering;

	std::vector<Boid> myGrid;
	int myIndex;
	int myColumn;
	int myRow;

	float maximumSpeed;
	float maximumForce;

	float theta;
	float oldTheta;

	float desiredSeperation;

	int quadrant;

	std::vector<Boid> otherBoids;

	Boid();
	Boid(float p_locationx, float p_locationy);
	void Move(std::vector<Boid> p_boids, Vector2i target);
	void Normalize(Vector2f& p_vector);

	void Flock(std::vector<Boid> p_boids);
	Vector2f Align(std::vector<Boid> p_boids);
	Vector2f Cohesion(std::vector<Boid> p_boids);
	Vector2f Separate(std::vector<Boid> p_boids);
	Vector2f Noise();

	Vector2f LockOnTarget(Vector2f p_target);

	Clock deltaClock;
	Time timestep;

};

Boid::Boid()
{
	//triangle = CircleShape(80, 3);
	triangle.setPointCount(3);

	triangle.setPoint(0, Vector2f(0, 10));
	triangle.setPoint(2, Vector2f(40, 0));
	triangle.setPoint(1, Vector2f(0, -10));

	//triangle.setScale(Vector2f(.2, .2));

	quadrant = 0;

	maximumSpeed = 1;
	maximumForce = .5;

	desiredSeperation = 20;

	velocity.x = 1;
	velocity.y = 1;

	otherBoids = std::vector<Boid>();

}

Boid::Boid(float p_locationx, float p_locationy)
{
	triangle.setPointCount(3);

	triangle.setPosition(Vector2f(p_locationx, p_locationy));

	triangle.setPoint(0, Vector2f(0, 10));
	triangle.setPoint(2, Vector2f(40, 0));
	triangle.setPoint(1, Vector2f(0, -10));

	triangle.setOrigin(Vector2f(40, 0));

	//triangle.setScale(Vector2f(.2, .2));

	quadrant = 0;

	maximumSpeed = 1000;
	maximumForce = .075;

	desiredSeperation = 20;

	velocity.x = 1;
	velocity.y = 1;

	sepForce = 3;
	aliForce = 1;
	cohForce = .1;
}

void Boid::Normalize(Vector2f& p_vector)
{
	float magnitude;
	magnitude = std::sqrt((std::pow((p_vector.x), 2) + std::pow((p_vector.y), 2)));

	p_vector = p_vector / magnitude;
}

Vector2f Boid::LockOnTarget(Vector2f p_target)
{
	desiredVelocity = p_target - location; //Figure out the vector from us to them

	float d = std::sqrt((desiredVelocity.x * desiredVelocity.x) + (desiredVelocity.y * desiredVelocity.y));

	Normalize(desiredVelocity);
	desiredVelocity *= (maximumSpeed * (d / 50)); //Set our speed

	steering = desiredVelocity - velocity; //Dial in our steering and apply limiting force factor (steering radius)
	steering += Noise();
	steering *= maximumForce;

	//Vector2f steerToMouse(Mouse::getPosition().x, Mouse::getPosition().y);
	//Normalize(steerToMouse);
	return steering;
}

Vector2f Boid::Separate(std::vector<Boid> p_boids)
{

	Vector2f vectorSum(0, 0);
	int count = 0;

	for (int i = 0; i < p_boids.size(); i++)
	{
		float distance = std::sqrt(std::pow((p_boids[i].location.x - location.x), 2) + std::pow((p_boids[i].location.y - location.y), 2));

		if (distance > 0 && distance < desiredSeperation)
		{
			Vector2f difference = location - p_boids[i].location;
			difference = difference / distance;
			Normalize(difference);

			vectorSum += difference;
			count++;
		}
	}

	if (count > 0)
	{
		vectorSum = vectorSum / (float)count;
		Normalize(vectorSum);
		vectorSum *= maximumSpeed;
	}

	vectorSum *= maximumForce;
	return vectorSum - velocity;

}

Vector2f Boid::Align(std::vector<Boid> p_boids)
{
	float neighborDistance = 100;
	Vector2f sum(0, 0);
	int count = 0;

	for (int i = 0; i < p_boids.size(); i++)
	{
		float distance = std::sqrt(std::pow((p_boids[i].location.x - location.x), 2) + std::pow((p_boids[i].location.y - location.y), 2));
		if (distance > 0 && distance < neighborDistance)
		{
			sum += p_boids[i].velocity;
			count++;
		}
	}

	if (count > 0)
	{
		sum = sum / (float)count;
		Normalize(sum);
		sum *= maximumSpeed;

		sum -= velocity;
		sum *= maximumForce;
		return sum;
	}
	else
	{
		Vector2f empty(0, 0);
		return empty;
	}
}

Vector2f Boid::Cohesion(std::vector<Boid> p_boids)
{
	float neighborDistance = 1000;
	Vector2f sum(0, 0);
	int count = 0;

	for (int i = 0; i < p_boids.size(); i++)
	{
		float distance = std::sqrt(std::pow((p_boids[i].location.x - location.x), 2) + std::pow((p_boids[i].location.y - location.y), 2));
		if (distance > 0 && distance < neighborDistance)
		{
			sum += p_boids[i].location;
			count++;
		}
	}

	if (count > 0)
	{

		sum = (sum / (float)count);
		return LockOnTarget(sum);

	}
	else
	{
		Vector2f empty(0, 0);
		return empty;
	}

}

Vector2f Boid::Noise()
{
	Vector2f sine(std::sin(location.x), std::sin(location.y));
	Normalize(sine);
	return sine;
}

void Boid::Flock(std::vector<Boid> p_boids)
{
	Vector2f sep = Separate(p_boids);
	Vector2f ali = Align(p_boids);
	Vector2f coh = Cohesion(p_boids);

	sep *= sepForce;
	ali *= aliForce;
	coh *= cohForce;

	acceleration += sep;
	acceleration += ali;
	acceleration += coh;
}

void Boid::Move(std::vector<Boid> p_boids, Vector2i target)
{
	location = triangle.getPosition();

	Flock(p_boids);

	acceleration *= maximumForce;

	velocity += acceleration; //Go go go!
	velocity *= maximumForce;
	location += velocity;

	theta = ((std::atan2((location.y - triangle.getPosition().y), (location.x - triangle.getPosition().x))) * (180 / 3.14159)); //Calculate and set the heading
	triangle.setRotation(theta);

	triangle.setPosition(location);

	acceleration = Vector2f(0, 0); //Reset acceleration for the next push
}