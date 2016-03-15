#include "Gun.h"
#include <random>
#include <ctime>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <GLM/gtx/rotate_vector.hpp>

using namespace std;

Gun::Gun(string name, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, float bulletSpread) : _name(name), _fireRate(fireRate), _bulletsPerShot(bulletsPerShot), _bulletDamage(bulletDamage), _bulletSpeed(bulletSpeed) , _frameCounter(0), _spread(bulletSpread)
{
}


Gun::~Gun()
{
}


void Gun::fire(const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position)
{
	const float DEG_TO_RAD = M_PI / 180.0f;
	static mt19937 randomEngine;
	uniform_real_distribution<float> randRotate(-_spread, _spread);


	for (int i = 0; i < _bulletsPerShot; i++)
	{

		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine) * DEG_TO_RAD), _bulletDamage, _bulletSpeed);
	}
}

void Gun::update(bool isMouseDown, const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position)
{
	cout << "UPDATE" << endl;

	_frameCounter++;
	if (_fireRate <= _frameCounter && isMouseDown)
	{
		fire(direction, bullets, position);
		_frameCounter = 0;
	}
}