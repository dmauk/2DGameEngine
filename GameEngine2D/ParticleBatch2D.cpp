#include "ParticleBatch2D.h"

namespace GameEngine2D {

	ParticleBatch2D::ParticleBatch2D()
	{
		//Empty
	}


	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] m_particles;
	}

	void ParticleBatch2D::init(int maxParticles, float decayRate, GLTexture texture, function<void(Particle2D&, float)> updateFunction /* = defaultParticleUpdate */)
	{
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayRate = decayRate;
		m_texture = texture;
		m_updateFunction = updateFunction;
	}

	void ParticleBatch2D::addParticle(const glm::vec2& position, const glm::vec2& velocity, const ColorRGBA8& color, float width)
	{
		int particleIndex = findFreeParticle();

		auto& p = m_particles[particleIndex];

		p.life = 1.0f;
		p.position = position;
		p.velocity = velocity;
		p.color = color;
		p.width = width;
	}

	void ParticleBatch2D::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++)
		{
			if (m_particles[i].life > 0.0f) //check if active
			{
				m_updateFunction(m_particles[i], deltaTime); //Using the function pointer boi
				m_particles[i].life -= m_decayRate;

			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* spriteBatch)
	{
		glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++)
		{
			if (m_particles[i].life > 0.0f)//still active?
			{
				auto& p = m_particles[i]; //So we don't have to keep typing m_particles[i] we just use a reference to it
				glm::vec4 destRect(p.position.x, p.position.y, p.width, p.width);
				spriteBatch->draw(destRect, uvRect, m_texture.id, 0.0f, p.color);
			}
		}
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = m_lastFreeParticle; i < m_maxParticles; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i; //Return index of free particle
			}
		}

		for (int i = 0; i < m_lastFreeParticle; i++)
		{
			if (m_particles[i].life <= 0.0f)
			{
				m_lastFreeParticle = i;
				return i; //Return index of free particle
			}
		}

		//No particles are free so overwrite first particle for now
		return 0;
	}
}