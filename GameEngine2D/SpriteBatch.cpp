
#include "SpriteBatch.h"
#include <algorithm>

using namespace std;
namespace GameEngine2D {

	SpriteBatch::SpriteBatch() :_vbo(0), _vao(0)
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
		_sortType = sortType;
		_renderBatches.clear(); //Get rid of old batches when a new one is to begin
		_glyphs.clear(); //Get rid of glyphs
	}



	void SpriteBatch::end() //Post-process
	{
		_glyphPointers.resize(_glyphs.size());
		for (int i = 0; i < _glyphs.size(); i++)
		{
			_glyphPointers[i] = &_glyphs[i];
		}
		sortGlyphs();
		createRenderBatches();
	}




	//						x			y			z		w
	//destRect defined as (position.x, position.y, width, height)
	//uvRect defined as (position.x, position.y, width, height)
	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const Color& color)
	{	
		_glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}



	void SpriteBatch::renderBatch()
	{
		glBindVertexArray(_vao);

		for (int i = 0; i < _renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].numVertices);
		}
		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches()
	{
		vector<Vertex> vertices;
		vertices.resize(_glyphs.size() * 6); //Avoid using pushback because it does extra checking
		if (_glyphs.empty())
		{
			return;
		}
		//Could do RenderBatch batch(0, 6, _glyphs[0]->texture);
		//_renderBatches.push_back(batch); but this would create a batch on the stack then copy 
		//it into the vector and the original copy would be wasted immediately. alternative below
		int currentVertex = 0;
		int offset = 0;
		_renderBatches.emplace_back(offset, 6, _glyphPointers[0]->texture);
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomLeft;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->bottomRight;
		vertices[currentVertex++] = _glyphPointers[0]->topRight;
		vertices[currentVertex++] = _glyphPointers[0]->topLeft;
		offset += 6; //Used to keep track of which vertices to render in the batch. Ie. we use the same texture but all the vertices are different so we should be able to pickout the correct ones to render.

		for (int currentGlyph = 1; currentGlyph < _glyphPointers.size(); currentGlyph++)
		{
			if (_glyphPointers[currentGlyph]->texture != _glyphPointers[currentGlyph - 1]->texture)
			{
				_renderBatches.emplace_back(offset, 6, _glyphPointers[currentGlyph]->texture);
				
			}
			else
			{
				_renderBatches.back().numVertices += 6;
			}

			vertices[currentVertex++] = _glyphPointers[currentGlyph]->topLeft;
			vertices[currentVertex++] = _glyphPointers[currentGlyph]->bottomLeft;
			vertices[currentVertex++] = _glyphPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[currentGlyph]->bottomRight;
			vertices[currentVertex++] = _glyphPointers[currentGlyph]->topRight;
			vertices[currentVertex++] = _glyphPointers[currentGlyph]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//Orphan the buffer //Avoids having to overwrite data.
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload new data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
	}

	void SpriteBatch::createVertexArray()
	{
		if (_vao == 0) 
		{
			glGenVertexArrays(1, &_vao);
		}

		if (_vbo == 0) 
		{
			glGenBuffers(1, &_vbo);
		}

		glBindVertexArray(_vao);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

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
		switch (_sortType) {
			case GlyphSortType::BACK_TO_FRONT:
				stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareBackToFront); //Equal elements will retain the same order
					break;
			case GlyphSortType::FRONT_TO_BACK:
				stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareFrontToBack);
					break;
			case GlyphSortType::TEXTURE:
				stable_sort(_glyphPointers.begin(), _glyphPointers.end(), compareTexture);
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
