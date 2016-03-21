#pragma once
#include <string>
#include <GLM\glm.hpp>
#include <vector>
#include "Bullet.h"
using namespace std;

class Gun
{
public:
	Gun(string name, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, float bulletSpread, float bulletDuration);
	~Gun();
	void update(bool isMouseDown, const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position);
	
private:
	int m_frameCounter; 
	void fire(const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position);
	string m_name;
	int m_bulletsPerShot; ///< per shot (e.g. shotgun)
	float m_spread; ///<Accuracy
	int m_fireRate; ///< per frame
	float m_bulletDamage;
	float m_bulletSpeed;
	float m_bulletDuration;
};
