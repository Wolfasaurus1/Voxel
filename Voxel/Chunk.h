#pragma once

#include <vector>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include <PerlinNoise.hpp>

enum BlockType
{
	EMPTY,
	DIRT,
	GRASS,
	WATER
};


class Chunk
{
public:
	Chunk(int x, int y, int z, const siv::PerlinNoise& perlin)
	{
		for (int i = 0; i < 16; i++){
			for (int k = 0; k < 16; k++) {
				int height = (int)(perlin.octave2D_01(((x + i) * 0.008), ((z + k) * 0.008), 4) * 50);
				for (int j = 0; j < 16; j++) {
					if (abs(height - y - j) < 1)
						blocks[i][j][k] = GRASS;
					else
						blocks[i][j][k] = EMPTY;
				}
			}
		}
	}

	BlockType GetBlock(int x, int y, int z)
	{
		return blocks[x][y][z];
	}

	void SetBlock(int x, int y, int z, BlockType blockType)
	{
		blocks[x][y][z] = blockType;
	}

private:
	BlockType blocks[16][16][16]; 
};



using namespace std;
using namespace glm;



// mesh a chunk, given the chunk and its global position
void MeshChunk(Chunk* c, vec3 p, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
	int CHLOD = 16;

	int u, v, w;
	int n;

	BlockType* mask = new BlockType[16 * 16];
	BlockType current, facing;
	int s;

	vec3 color;

	for (int d = 0; d < 6; d++) 
	{
		u = (d / 2 + 0) % 3;    
		v = (d / 2 + 1) % 3;     
		w = (d / 2 + 2) % 3;  

		int x[3] = { 0 };
		int q[3] = { 0 };
		int y[3] = { 0 };

		n = 2 * (d % 2) - 1;	//Normal Direction
		q[u] = n;				//Normal Vector
		y[u] = 1;				//Simple Vector


		for (x[u] = 0; x[u] < CHLOD; x[u]++) {       //Loop Over Depth

			//construct the appropriate mask
			for (x[v] = 0; x[v] < CHLOD; x[v]++) {     //Loop Over Slice
				for (x[w] = 0; x[w] < CHLOD; x[w]++) 
				{
					s = x[w] + x[v] * CHLOD;      //Slice Index
					mask[s] = EMPTY;              //Set to Air

					current = c->GetBlock(x[0], x[1], x[2]);

					if (current == EMPTY) continue;

					//Basically, we are facing out of the chunk, so we do take over the surface.
					if (x[u] + q[u] < 0 || x[u] + q[u] >= CHLOD) {
						mask[s] = current;
						continue;
					}

					// what block is the extending surface facing? 
					// is it facing anything? 
					facing = c->GetBlock(x[0] + q[0], x[1] + q[1], x[2] + q[2]);

					if (facing == EMPTY) {
						mask[s] = current;
					}

				}
			}

			int width = 1, height = 1;
			bool quaddone;

			// how far can we extend this quad in each direction? 
			for (x[v] = 0; x[v] < CHLOD; x[v]++) {            //Evaluate Mask
				for (x[w] = 0; x[w] < CHLOD; x[w] += width) {   //Permissible Skip
					width = height = 1;       //Current Quad Dimensions

					s = x[w] + x[v] * CHLOD;    //Current Slice Index
					current = mask[s];        //Current Block Type

					if (current == EMPTY)  //We don't mesh air
						continue;

					while (mask[s + width] == current && x[w] + width < CHLOD)
						width++;

					quaddone = false;
					for (height = 1; x[v] + height < CHLOD; height++) {   //Find Height

						for (int k = 0; k < width; k++) {                   //Iterate Over Width
							if (mask[s + k + height * CHLOD] != current) {
								quaddone = true;
								break;
							}
						}
						if (quaddone) break;
					}

					//Zero the Mask
					for (int l = x[v]; l < x[v] + height; l++)   
						for (int k = x[w]; k < x[w] + width; k++)
							mask[k + l * CHLOD] = EMPTY;

					int du[3] = { 0 }; du[v] = height;
					int dv[3] = { 0 }; dv[w] = width;


					// this should be somewhere else. come back and refactor this
					BlockType type = c->GetBlock(x[0], x[1], x[2]);
					if (type == GRASS)
						color = vec3(0.251, 0.475, 0.549);
					else if (type == DIRT)
						color = vec3(0.122, 0.212, 0.239);
					else if (type == WATER)
						color = vec3(0.439, 0.663, 0.631);





					if (n < 0) {

						indices.push_back(vertexData.size() / 10 + 0);
						indices.push_back(vertexData.size() / 10 + 2);
						indices.push_back(vertexData.size() / 10 + 1);
						indices.push_back(vertexData.size() / 10 + 3);
						indices.push_back(vertexData.size() / 10 + 2);
						indices.push_back(vertexData.size() / 10 + 0);

						//Some need to go clock-wise, others need to go counterclockwise.
						// Vertex 0
						vertexData.push_back((p.x + x[0] - 0.5)  );
						vertexData.push_back((p.y + x[1] - 0.5)  );
						vertexData.push_back((p.z + x[2] - 0.5)  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 2
						vertexData.push_back((p.x + x[0] + du[0] - 0.5)  );
						vertexData.push_back((p.y + x[1] + du[1] - 0.5)  );
						vertexData.push_back((p.z + x[2] + du[2] - 0.5)  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 3
						vertexData.push_back((p.x + x[0] + du[0] + dv[0] - 0.5)  );
						vertexData.push_back((p.y + x[1] + du[1] + dv[1] - 0.5)  );
						vertexData.push_back((p.z + x[2] + du[2] + dv[2] - 0.5)  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 4
						vertexData.push_back((p.x + x[0] + dv[0] - 0.5)  );
						vertexData.push_back((p.y + x[1] + dv[1] - 0.5)  );
						vertexData.push_back((p.z + x[2] + dv[2] - 0.5)  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

					}
					else {

						indices.push_back(vertexData.size() / 10 + 0);
						indices.push_back(vertexData.size() / 10 + 2);
						indices.push_back(vertexData.size() / 10 + 1);
						indices.push_back(vertexData.size() / 10 + 1);
						indices.push_back(vertexData.size() / 10 + 3);
						indices.push_back(vertexData.size() / 10 + 0);
						//Vertex 0
						vertexData.push_back((p.x + x[0] - 0.5 + y[0])  );
						vertexData.push_back((p.y + x[1] - 0.5 + y[1])  );
						vertexData.push_back((p.z + x[2] - 0.5 + y[2])  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 1
						vertexData.push_back((p.x + x[0] + du[0] + dv[0] - 0.5 + y[0])  );
						vertexData.push_back((p.y + x[1] + du[1] + dv[1] - 0.5 + y[1])  );
						vertexData.push_back((p.z + x[2] + du[2] + dv[2] - 0.5 + y[2])  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 2
						vertexData.push_back((p.x + x[0] + du[0] - 0.5 + y[0])  );
						vertexData.push_back((p.y + x[1] + du[1] - 0.5 + y[1])  );
						vertexData.push_back((p.z + x[2] + du[2] - 0.5 + y[2])  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);

						//Vertex 3
						vertexData.push_back((p.x + x[0] + dv[0] - 0.5 + y[0])  );
						vertexData.push_back((p.y + x[1] + dv[1] - 0.5 + y[1])  );
						vertexData.push_back((p.z + x[2] + dv[2] - 0.5 + y[2])  );

						vertexData.push_back(q[0]);
						vertexData.push_back(q[1]);
						vertexData.push_back(q[2]);

						vertexData.push_back(color.x);
						vertexData.push_back(color.y);
						vertexData.push_back(color.z);
						vertexData.push_back(1.0);
					}
					// next quad
				}
			}
			// next slice
		}
	}
	delete[] mask;
}