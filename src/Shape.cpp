#include "Shape.h"
#include <iostream>
#include <assert.h>

#include "GLSL.h"
#include "Program.h"

using namespace std;

Shape::Shape() :
	eleBufID(0),
	posBufID(0),
	norBufID(0),
	texBufID(0), 
   vaoID(0)
{
	min = glm::vec3(0);
	max = glm::vec3(0);
}

Shape::~Shape()
{
}


void Shape::reverseNormals() {
	for (int i = 0; i < norBuf.size(); i++) {
		norBuf.at(i) = -norBuf.at(i);
	}
}

//places 
void Shape::populateNorBuf(size_t i, glm::vec3 normal) {
	//the first point
	norBuf.at(3 * eleBuf.at(i)) += normal.x;
	norBuf.at(3 * eleBuf.at(i) + 1) += normal.y;
	norBuf.at(3 * eleBuf.at(i) + 2) += normal.z;
	//the second point
	norBuf.at(3 * eleBuf.at(i + 1)) += normal.x;
	norBuf.at(3 * eleBuf.at(i + 1) + 1) += normal.y;
	norBuf.at(3 * eleBuf.at(i + 1) + 2) += normal.z;
	//the third point
	norBuf.at(3 * eleBuf.at(i + 2)) += normal.x;
	norBuf.at(3 * eleBuf.at(i + 2) + 1) += normal.y;
	norBuf.at(3 * eleBuf.at(i + 2) + 2) += normal.z;
}

void Shape::normalizeNorBuf() {
	//vertex by vertex

	for (size_t i = 0; i < norBuf.size(); i = i + 3) {//work with 3 coordinates at a time
		glm::vec3 vn = glm::vec3(norBuf.at(i), norBuf.at(i + 1), norBuf.at(i + 2));
		vn = glm::normalize(vn);
		//put the normalized components back
		norBuf.at(i) = vn.x;
		norBuf.at(i + 1) = vn.y;
		norBuf.at(i + 2) = vn.z;

	}

}

void Shape::computeNormals() {

	//if there were normals there, return and don't change them.
	if (!norBuf.empty()) return;
	norBuf.clear();
	norBuf.resize(posBuf.size());
	for (size_t i = 0; i < norBuf.size(); i++) {
		norBuf.at(i) = 0.0f;
	}
	//for every triangle
	for (size_t i = 0; i < eleBuf.size(); i = i + 3) {//do 3 elements of ele buf at a time
		//        2
		//       / \
		//      v   \
		//    0/__u__\1
		//grab a total of 9 floats from posBuf. eleBuf contains the location of the x-coord of each triangle, add 1 to get y,z.
		glm::vec3 p0 = glm::vec3(posBuf.at(3 * eleBuf.at(i)), posBuf.at(3 * eleBuf.at(i) + 1), posBuf.at(3 * eleBuf.at(i) + 2));
		glm::vec3 p1 = glm::vec3(posBuf.at(3 * eleBuf.at(i + 1)), posBuf.at(3 * eleBuf.at(i + 1) + 1), posBuf.at(3 * eleBuf.at(i + 1) + 2));
		glm::vec3 p2 = glm::vec3(posBuf.at(3 * eleBuf.at(i + 2)), posBuf.at(3 * eleBuf.at(i + 2) + 1), posBuf.at(3 * eleBuf.at(i + 2) + 2));
		//compute the vectors that minimally describe the triangle
		glm::vec3 u = p1 - p0;
		glm::vec3 v = p2 - p0;
		glm::vec3 normal = glm::cross(u, v);

		populateNorBuf(i, normal);
	}
	//all done with triangles. Now normalize the summed normal vectors.
	normalizeNorBuf();
}

/* copy the data from the shape to this object */
void Shape::createShape(tinyobj::shape_t & shape)
{
		posBuf = shape.mesh.positions;
		norBuf = shape.mesh.normals;
		texBuf = shape.mesh.texcoords;
		eleBuf = shape.mesh.indices;
}

void Shape::measure() {
  float minX, minY, minZ;
   float maxX, maxY, maxZ;

   minX = minY = minZ = 1.1754E+38F;
   maxX = maxY = maxZ = -1.1754E+38F;

   //Go through all vertices to determine min and max of each dimension
   for (size_t v = 0; v < posBuf.size() / 3; v++) {
		if(posBuf[3*v+0] < minX) minX = posBuf[3*v+0];
		if(posBuf[3*v+0] > maxX) maxX = posBuf[3*v+0];

		if(posBuf[3*v+1] < minY) minY = posBuf[3*v+1];
		if(posBuf[3*v+1] > maxY) maxY = posBuf[3*v+1];

		if(posBuf[3*v+2] < minZ) minZ = posBuf[3*v+2];
		if(posBuf[3*v+2] > maxZ) maxZ = posBuf[3*v+2];
	}

	min.x = minX;
	min.y = minY;
	min.z = minZ;
   max.x = maxX;
   max.y = maxY;
   max.z = maxZ;
}

void Shape::Init()
{
   // Initialize the vertex array object
   glGenVertexArrays(1, &vaoID);
   glBindVertexArray(vaoID);

	// Send the position array to the GPU
	glGenBuffers(1, &posBufID);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glBufferData(GL_ARRAY_BUFFER, posBuf.size()*sizeof(float), &posBuf[0], GL_STATIC_DRAW);
	
	// Send the normal array to the GPU
	if(norBuf.empty()) {
		norBufID = 0;
	} else {
		glGenBuffers(1, &norBufID);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glBufferData(GL_ARRAY_BUFFER, norBuf.size()*sizeof(float), &norBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the texture array to the GPU
	if(texBuf.empty()) {
		texBufID = 0;
	} else {
		glGenBuffers(1, &texBufID);
		glBindBuffer(GL_ARRAY_BUFFER, texBufID);
		glBufferData(GL_ARRAY_BUFFER, texBuf.size()*sizeof(float), &texBuf[0], GL_STATIC_DRAW);
	}
	
	// Send the element array to the GPU
	glGenBuffers(1, &eleBufID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eleBuf.size()*sizeof(unsigned int), &eleBuf[0], GL_STATIC_DRAW);
	
	// Unbind the arrays
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	auto error = glGetError();
	assert(error == GL_NO_ERROR);
}

void Shape::draw(const shared_ptr<Program> prog) const
{
	int h_pos, h_nor, h_tex;
	h_pos = h_nor = h_tex = -1;

   glBindVertexArray(vaoID);
	// Bind position buffer
	h_pos = prog->getAttribute("vertPos");
	GLSL::enableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, posBufID);
	glVertexAttribPointer(h_pos, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	
	// Bind normal buffer
	h_nor = prog->getAttribute("vertNor");
	if(h_nor != -1 && norBufID != 0) {
		GLSL::enableVertexAttribArray(h_nor);
		glBindBuffer(GL_ARRAY_BUFFER, norBufID);
		glVertexAttribPointer(h_nor, 3, GL_FLOAT, GL_FALSE, 0, (const void *)0);
	}

	if (texBufID != 0) {	
		// Bind texcoords buffer
		h_tex = prog->getAttribute("vertTex");
		if(h_tex != -1 && texBufID != 0) {
			GLSL::enableVertexAttribArray(h_tex);
			glBindBuffer(GL_ARRAY_BUFFER, texBufID);
			glVertexAttribPointer(h_tex, 2, GL_FLOAT, GL_FALSE, 0, (const void *)0);
		}
	}
	
	// Bind element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eleBufID);
	
	// Draw
	glDrawElements(GL_TRIANGLES, (int)eleBuf.size(), GL_UNSIGNED_INT, (const void *)0);
	
	// Disable and unbind
	if(h_tex != -1) {
		GLSL::disableVertexAttribArray(h_tex);
	}
	if(h_nor != -1) {
		GLSL::disableVertexAttribArray(h_nor);
	}
	GLSL::disableVertexAttribArray(h_pos);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
