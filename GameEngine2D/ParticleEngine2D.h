#pragma once
#include <vector>

using namespace std;



namespace GameEngine2D {

	class ParticleBatch2D; //Forward declare inside gameEngine2D because they belong to game engine 2D
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		//Decision does one batch hold all particles? Or should I separate them for readability
		//After adding a particle batch, the particle engine becomes responsible for cleaning up the batch
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void update(float deltaTime);

		void draw(SpriteBatch* spriteBatch);

	private:
		vector<ParticleBatch2D*> m_batches;

	};

}

