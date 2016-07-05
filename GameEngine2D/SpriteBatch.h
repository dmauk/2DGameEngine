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
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color);
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle);
		GLuint texture;
		float depth;
		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;
	
	private:
		glm::vec2 rotatePoint(glm::vec2 position, float angle);
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
		
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle);
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);
		void renderBatch();
	private:
		void createRenderBatches();
		void createVertexArray();
		void sortGlyphs();

		//Sort functions:
		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);
		GlyphSortType m_sortType;
		GLuint m_vbo;
		GLuint m_vao;
		vector<Glyph*> m_glyphPointers; //for sorting glyphs for rendering
		vector<Glyph> m_glyphs;
		vector<RenderBatch> m_renderBatches;
	};
	
}

