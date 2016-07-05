#include "Gun.h"
#include <random>
#include <ctime>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <GLM/gtx/rotate_vector.hpp>

using namespace std;

Gun::Gun(string name, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, float bulletSpread, float bulletDuration, GameEngine2D::SoundEffect soundEffect) : 
	m_name(name), 
	m_fireRate(fireRate), 
	m_bulletsPerShot(bulletsPerShot), 
	m_bulletDamage(bulletDamage), 
	m_bulletSpeed(bulletSpeed) , 
	m_frameCounter(0), 
	m_spread(bulletSpread), 
	m_bulletDuration(bulletDuration),
	m_soundEffect(soundEffect) {}


Gun::~Gun()
{
}


void Gun::fire(const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position)
{
	const float DEG_TO_RAD = M_PI / 180.0f;
	static mt19937 randomEngine;
	uniform_real_distribution<float> randRotate(-m_spread, m_spread);

	m_soundEffect.play();

	for (int i = 0; i < m_bulletsPerShot; i++)
	{

		bullets.emplace_back(position, glm::rotate(direction, randRotate(randomEngine)*DEG_TO_RAD), m_bulletDamage, m_bulletSpeed, m_bulletDuration);
	}
}

void Gun::update(bool isMouseDown, const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position, float deltaTime)
{
	m_frameCounter += 1.0f * deltaTime;
	if (m_fireRate <= m_frameCounter && isMouseDown)
	{
		fire(direction, bullets, position);
		m_frameCounter = 0;
	}
}