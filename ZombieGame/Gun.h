#pragma once
#include <string>
#include <GLM\glm.hpp>
#include <vector>
#include "Bullet.h"
#include <GameEngine2D\AudioEngine.h>
using namespace std;

class Gun
{
public:
	Gun(string name, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, float bulletSpread, float bulletDuration, GameEngine2D::SoundEffect soundEffect);
	~Gun();
	void update(bool isMouseDown, const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position, float deltaTime);
	
private:
	GameEngine2D::SoundEffect m_soundEffect;
	float m_frameCounter; 
	void fire(const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position);
	string m_name;
	int m_bulletsPerShot; ///< per shot (e.g. shotgun)
	float m_spread; ///<Accuracy
	int m_fireRate; ///< per frame
	float m_bulletDamage;
	float m_bulletSpeed;
	float m_bulletDuration;
};
