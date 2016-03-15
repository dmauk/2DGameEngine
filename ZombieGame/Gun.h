#pragma once
#include <string>
#include <GLM\glm.hpp>
#include <vector>
#include "Bullet.h"
using namespace std;

class Gun
{
public:
	Gun(string name, int fireRate, int bulletsPerShot, float bulletSpeed, float bulletDamage, float bulletSpread);
	~Gun();
	void update(bool isMouseDown, const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position);
	
private:
	int _frameCounter; 
	void fire(const glm::vec2& direction, vector<Bullet>& bullets, const glm::vec2& position);
	string _name;
	int _bulletsPerShot; ///< per shot (e.g. shotgun)
	float _spread; ///<Accuracy
	int _fireRate; ///< per frame
	float _bulletDamage;
	float _bulletSpeed;
};
