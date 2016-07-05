
#include "SpriteBatch.h"
#include <algorithm>

using namespace std;
namespace GameEngine2D {

	glm::vec2 Glyph::rotatePoint(glm::vec2 position, float angle)
	{
		glm::vec2 newVector;
		newVector.x = position.x * cos(angle) - position.y * sin(angle);
		newVector.y = position.x * sin(angle) + position.y * cos(angle);
		return newVector;
	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color) : texture(Texture), depth(Depth)
	{

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

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint Texture, float Depth, const ColorRGBA8& color, float angle) : texture(Texture), depth(Depth)
	{
		glm::vec2 halfDimensions(destRect.z / 2.0f, destRect.w / 2.0f); // Z and Y are width and heigh respectively

		//Get points centered at origin
		glm::vec2 tl(-halfDimensions.x, halfDimensions.y);
		glm::vec2 bl(-halfDimensions.x, -halfDimensions.y);
		glm::vec2 br(halfDimensions.x, -halfDimensions.y);
		glm::vec2 tr(halfDimensions.x, halfDimensions.y);

		//Rotate the points
		tl = rotatePoint(tl, angle) + halfDimensions;
		bl = rotatePoint(bl, angle) + halfDimensions;
		br = rotatePoint(br, angle) + halfDimensions;
		tr = rotatePoint(tr, angle) + halfDimensions;

		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y); //
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y); //
		bottomLeft.setUV(uvRect.x, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y); //
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y); //
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);
	}

	SpriteBatch::SpriteBatch() :m_vbo(0), m_vao(0)
	{
	}


	SpriteBatch::~SpriteBatch()
	{
	}



	void SpriteBatch::init() 
	{
		createVertexArray();
	}



	void SpriteBatch::begin(GlyphSortType sortType /*= GlyphSortType::TEXTURE*/) 
	{
		m_sortType = sortType;
		m_renderBatches.clear(); //Get rid of old batches when a new one is to begin
		m_glyphs.clear(); //Get rid of glyphs
	}



	void SpriteBatch::end() //Post-process
	{
		m_glyphPointers.resize(m_glyphs.size());
		for (int i = 0; i < m_glyphs.size(); i++)
		{
			m_glyphPointers[i] = &m_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}




	//						x			y			z		w
	//destRect defined as (position.x, position.y, width, height)
	//uvRect defined as (position.x, position.y, width, height)
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color)
	{	
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float angle)
	{
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir)
	{
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir)); // dot product will always return a position angle up to 180 degrees, so if we were to rotate 270 we would get 90 instead of -90
		if (dir.y < 0.0f) angle = -angle;
		m_glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}


	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(m_vao);

		for (int i = 0; i < m_renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, m_renderBatches[i].offset, m_renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		vector<Vertex> vertices;
		vertices.resize(m_glyphPointers.size() * 6); //Avoid using pushback because it does extra checking
		if (m_glyphPointers.empty())
		{
			return;
		}
		//Could do RenderBatch batch(0, 6, _glyphs[0]->texture);
		//_renderBatches.push_back(batch); but this would create a batch on the stack then copy 
		//it into the vector and the original copy would be wasted immediately. alternative below
		int currentVertex = 0;
		int offset = 0;
		m_renderBatches.emplace_back(offset, 6, m_glyphPointers[0]->texture);
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topRight;
		vertices[currentVertex++] = m_glyphPointers[0]->topLeft;
		offset += 6; //Used to keep track of which vertices to render in the batch. Ie. we use the same texture but all the vertices are different so we should be able to pickout the correct ones to render.

		for (int currentGlyph = 1; currentGlyph < m_glyphPointers.size(); currentGlyph++)
		{
			if (m_glyphPointers[currentGlyph]->texture != m_glyphPointers[currentGlyph - 1]->texture)
			{
				m_renderBatches.emplace_back(offset, 6, m_glyphPointers[currentGlyph]->texture);
				
			}
			else
			{
				m_renderBatches.back().numVertices += 6;
			}

			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topLeft;
			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topRight;
			vertices[currentVertex++] = m_glyphPointers[currentGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//Orphan the buffer //Avoids having to overwrite data.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload new data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	}

	void SpriteBatch::createVertexArray()
	{
		if (m_vao == 0) 
		{
			glGenVertexArrays(1, &m_vao);
		}

		if (m_vbo == 0) 
		{
			glGenBuffers(1, &m_vbo);
		}

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//Point opengl to the data
		//Position attribute pointer
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//Color attribute pointer
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//UV attribute pointer
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		glBindVertexArray(0);
	}

	void SpriteBatch::sortGlyphs()
	{
		switch (m_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareBackToFront); //Equal elements will retain the same order
					break;
			case GlyphSortType::FRONT_TO_BACK:
				stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareFrontToBack);
					break;
			case GlyphSortType::TEXTURE:
				stable_sort(m_glyphPointers.begin(), m_glyphPointers.end(), compareTexture);
					break;
		}
		 
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b)
	{
		if (a->depth > b->depth)
			return true;
		else
			return false;
	}


	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b)
	{
		if (a->depth < b->depth)
			return true;
		else
			return false;
	}


	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) 
	{
		if (a->texture > b->texture)
			return true;
		else
			return false;
	}


}
