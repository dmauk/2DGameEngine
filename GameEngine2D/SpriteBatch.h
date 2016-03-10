#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "Vertex.h"
#include <vector>

using namespace std;
//Goals to minimize texture switching and draw calls. IE batches of only same texture

namespace GameEngine2D {

	enum class GlyphSortType {NONE,FRONT_TO_BACK,BACK_TO_FRONT,TEXTURE};

	class Glyph {
	public:
		Glyph() {};
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const Color& color) : texture(Texture), depth(Depth) {

			topLeft.color = color;
			topLeft.setPosition(destRect.x, destRect.y + destRect.w); //
			topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

			bottomLeft.color = color;
			bottomLeft.setPosition(destRect.x, destRect.y); //
			bottomLeft.setUV(uvRect.x, uvRect.y);

			topRight.color = color;
			topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w); //
			topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

			bottomRight.color = color;
			bottomRight.setPosition(destRect.x + destRect.z, destRect.y); //
			bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
		}
		GLuint texture;
		float depth;
		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	};

	class RenderBatch 
	{
	public:
		RenderBatch(GLuint Offset, GLuint NumVertices, GLuint Texture) : offset(Offset), numVertices(NumVertices), texture(Texture) { }
		GLuint offset;
		GLuint numVertices;
		GLuint texture;

	};


	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();
		void init();
		void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
		void end(); //Post-process
		
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color);
		void renderBatch();
	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		//Sort functions:
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);
		GlyphSortType _sortType;
		GLuint _vbo;
		GLuint _vao;
		vector<Glyph*> _glyphPointers; //for sorting glyphs for rendering
		vector<Glyph> _glyphs;
		vector<RenderBatch> _renderBatches;
	};
	
}

