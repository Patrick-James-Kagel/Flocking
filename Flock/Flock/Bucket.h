#pragma once

using namespace sf;

class Bucket
{
private:

public:
	std::vector<Boid> boids;

	Bucket();
};

Bucket::Bucket()
{
	boids = std::vector<Boid>();
}
