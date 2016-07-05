#pragma once

#include <functional>
#include <GLM\glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

using namespace std;

//It is best to set particles active or inactive with a large buffer than constantly calling new and delete. We will reuse existing particles if necessary

namespace GameEngine2D {

	class Particle2D {
	public:
		glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		//Decide between global color per batch or specifying color for each particle (batch would be more cache friendly)
		ColorRGBA8 color;
		float life = 0.0f;
		float width = 0.0f;
	};

	inline void defaultParticleUpdate(Particle2D& particle, float deltaTime) //Must use inline when defining in a header file
	{
		particle.position += particle.velocity * deltaTime;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int maxParticles, float decayRate, GLTexture texture, function<void(Particle2D&, float)> updateFunction = defaultParticleUpdate);
		void update(float deltaTime);
		void draw(SpriteBatch* spriteBatch);

		void addParticle(const glm::vec2& position, const glm::vec2& velocity,const ColorRGBA8& color, float width);
		//void addParticles();

	private:
		int findFreeParticle();
		function<void(Particle2D&, float)> m_updateFunction;
	//Global properties for each batch
		float m_decayRate = 0.1f;
		Particle2D* m_particles = nullptr; 
		int m_maxParticles = 0; //Essentially max particles
		GLTexture m_texture;
		int m_lastFreeParticle = 0;
	};

}
