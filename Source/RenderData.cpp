#include "global.h"
#include "RenderData.h"

RenderData::RenderData() : cameraposition(1.0f, 0.0f, 0.0f), camerarotation((float)M_PI, 0.0f, 0.0f) 
{}



bool RenderData::loadSkybox(const std::vector<std::string> & facefilenames){
	//sb = Skybox();
	sb.id = texturemanager.loadCubemap(facefilenames);

	const float skyboxVertices[] = {
	    // positions          
	    -1.0f,  1.0f, -1.0f,
	    -1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,

	    -1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f, -1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,

	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,

	    -1.0f, -1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f,
	    -1.0f, -1.0f,  1.0f,

	    -1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f, -1.0f,
	     1.0f,  1.0f,  1.0f,
	     1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f,  1.0f,
	    -1.0f,  1.0f, -1.0f,

	    -1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f, -1.0f,
	     1.0f, -1.0f, -1.0f,
	    -1.0f, -1.0f,  1.0f,
	     1.0f, -1.0f,  1.0f
	};
	

	// skybox VAO
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    sb.vao = skyboxVAO;
    sb.vbo = skyboxVBO;
	return true;
}

bool RenderData::loadBillboard(const std::string & texturename, const std::string & smoketexturename){
	bb = Billboard();
	GLuint texid = texturemanager.loadTexture("Models/Textures/" + texturename, false);
	std::cerr << "Loaded texid: " << texid << std::endl;
	bb.textureid = texid;
	GLuint smokeid = texturemanager.loadTexture("Models/Textures/" + smoketexturename, false);
	std::cerr << "Loaded texid: " << smokeid << std::endl;
	bb.smokeid = smokeid;

	// The VBO containing the 4 vertices of the particles.
	static const GLfloat g_vertex_buffer_data[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	GLuint billboard_vertex_buffer;
	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);
	bb.vbo = billboard_vertex_buffer;




	//particles = Particles();
	//particles.printparticles();

	return true;
}

bool RenderData::loadFireworkController(){
	fc.AttachParticles(&particles);
	fc.SetOrigin(Vector4(0.0f, 0.0f,-40.0f));
	float z = 0.3f;
	for (int i = 0; i < 10; i++){

		//
		fc.SetDirection(Vector4(0.0f,1.0f - (i/5.0f),z).normalize());
		fc.AddFireworkTrigger(5.0f + ((float)i/3.0f), 0.7f, 1, 255 - (i*25), (i*25), (i*25));
		if (i <= 5) z += 0.07;
		else z -= 0.08;
	}
	fc.SetDirection(Vector4(0.0f,0.0f,1.0f).normalize());
	fc.AddFireworkTrigger(15.0f, 0.7f, 2, 0,0,255);
	fc.AddFireworkTrigger(18.0f, 0.7f, 3, 0,255,0);

	for (int i = 0; i < 6; i++){
		fc.SetOrigin(Vector4(0.0f, -90.0f + (float)i*30.0f,-60.0f));
		fc.AddFireworkTrigger(22.0f, 0.7f, 3, 0,255,0);
	}
	for (int i = 0; i < 6; i++){
		fc.SetOrigin(Vector4(0.0f, -90.0f + (float)i*30.0f,-60.0f));
		fc.AddFireworkTrigger(24.0f, 0.7f, 3, 255,0,0);
	}
	for (int i = 0; i < 6; i++){
		fc.SetOrigin(Vector4(0.0f, -90.0f + (float)i*30.0f,-60.0f));
		fc.AddFireworkTrigger(26.0f, 0.7f, 3, 0,0,255);
	}

	fc.SetOrigin(Vector4(0.0f, 0.0f,-60.0f));
	fc.AddFireworkTrigger(30.0f, 3.2f, 1.1f, 3000, 5, 255, 10, 10);
	fc.AddFireworkTrigger(30.0f, 3.2f, 1.1f, 3000, 5, 10, 10, 255);
	
}

bool RenderData::loadModel(const std::string & modelfile)
{
	size_t i, j, numi, numj;

	size_t materialstart = materials.size();

	std::ifstream file(("Models/" + modelfile).c_str(), std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "Failed to open modelfile " << modelfile << std::endl;
		return false;
	}

	//modelname
	readString(file);

	// Materials
	numi = readInt(file);
	for (i = 0; i < numi; ++i)
	{
		Material m;
		//material name
		readString(file);

		m.diffuse = readColor(file);
		m.specular = readColor(file);
		m.specularlevel = readFloat(file);
		m.glossiness = readFloat(file);
		readFloat(file);

		m.texdiffuse = m.texspecular = m.texbump = 0;

		numj = readInt(file);
		for (j = 0; j < numj; ++j)
		{
			//map type
			int maptype = readInt(file);
			//map texture filename
			std::string filename = readString(file);

			
			if (maptype == 2)
			{
				m.texdiffuse = texturemanager.loadTexture("Models/Textures/" + filename + ".png", true);
			}
			else if (maptype == 4)
			{
				m.texspecular = texturemanager.loadTexture("Models/Textures/" + filename + ".png", true);
			}
			else if (maptype == 9)
			{
				m.texbump = texturemanager.loadTexture("Models/Textures/" + filename + ".png", false);
			}
		}
		materials.push_back(m);
	}

	numi = readInt(file);
	for (i = 0; i < numi; ++i)
	{
		Mesh m;
		float * floatarray;
		unsigned int * intarray;
		size_t matid = readInt(file);
		m.material = &(materials[materialstart + matid]);

		numj = readInt(file);

		floatarray = new float[numj * 3];
		for (j = 0; j < numj; ++j)
		{
			readPoint3(file, floatarray + j * 3);
		}
		glGenBuffers(1, &m.vbo_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo_vertex);
		glBufferData(GL_ARRAY_BUFFER, numj * 3 * sizeof(float), floatarray, GL_STATIC_DRAW);
		delete [] floatarray;

		floatarray = new float[numj * 9];
		for (j = 0; j < numj * 3; ++j)
		{
			readPoint3(file, floatarray + j * 3);
		}
		glGenBuffers(1, &m.vbo_tbn);
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo_tbn);
		glBufferData(GL_ARRAY_BUFFER, numj * 9 * sizeof(float), floatarray, GL_STATIC_DRAW);
		delete [] floatarray;

		floatarray = new float[numj * 2];
		for (j = 0; j < numj; ++j)
		{
			readPoint2(file, floatarray + j * 2);
		}
		glGenBuffers(1, &m.vbo_texcoord);
		glBindBuffer(GL_ARRAY_BUFFER, m.vbo_texcoord);
		glBufferData(GL_ARRAY_BUFFER, numj * 2 * sizeof(float), floatarray, GL_STATIC_DRAW);
		delete [] floatarray;

		numj = readInt(file);
		intarray = new unsigned int[numj * 3];
		for (j = 0; j < numj; ++j)
		{
			readIndex3(file, intarray + j * 3);
		}
		glGenBuffers(1, &m.vbo_faces);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.vbo_faces);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numj * 3 * sizeof(unsigned int), intarray, GL_STATIC_DRAW);
		delete [] intarray;
		m.numfaces = numj;

		meshes.push_back(m);
	}


	file.close();

	return true;
}

bool RenderData::addLight(const std::string & description)
{
	try {
		std::stringstream ss(description);
		char c;
		Light l;
		float mult;

		ss >> c;
		if (c != '(')
			throw(std::invalid_argument(std::string("Expected '('")));
		ss >> l.position.x() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.position.y() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.position.z() >> c;
		if (c != ')')
			throw(std::invalid_argument(std::string("Expected ')'")));

		ss >> c;
		if (c != '(')
			throw(std::invalid_argument(std::string("Expected '('")));
		ss >> l.target.x() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.target.y() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.target.z() >> c;
		if (c != ')')
			throw(std::invalid_argument(std::string("Expected ')'")));

		ss >> mult;
		
		ss >> c;
		if (c != '(')
			throw(std::invalid_argument(std::string("Expected '('")));
		ss >> l.intensity.x() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.intensity.y() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> l.intensity.z() >> c;
		if (c != ')')
			throw(std::invalid_argument(std::string("Expected ')'")));

		l.intensity *= mult / 255.0f;

		ss >> l.params.x() >> l.params.y() >> l.params.z() >> l.params.w();

		lights.push_back(l);

		return true;
	}
	catch (std::exception & e)
	{
		std::cerr << "Syntax error at Light: " << e.what() << std::endl;
		return false;
	}
}


bool RenderData::setCamera(const std::string & description)
{
	try {
		std::stringstream ss(description);
		char c;
		Vector4 position, target;

		ss >> c;
		if (c != '(')
			throw(std::invalid_argument(std::string("Expected '('")));
		ss >> position.x() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> position.y() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> position.z() >> c;
		if (c != ')')
			throw(std::invalid_argument(std::string("Expected ')'")));

		ss >> c;
		if (c != '(')
			throw(std::invalid_argument(std::string("Expected '('")));
		ss >> target.x() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> target.y() >> c;
		if (c != ',')
			throw(std::invalid_argument(std::string("Expected ','")));
		ss >> target.z() >> c;
		if (c != ')')
			throw(std::invalid_argument(std::string("Expected ')'")));

		target = target - position;

		cameraposition = position;
		camerarotation.x() = 0.0f;
		camerarotation.y() = atan2(-target.z(), sqrt(target.x() * target.x() + target.y() * target.y()));
		camerarotation.z() = atan2(target.y(), target.x());

		return true;
	}
	catch (std::exception & e)
	{
		std::cerr << "Syntax error at Camera: " << e.what() << std::endl;
		return false;
	}
}


size_t RenderData::getNumMeshes() const
{
	return meshes.size();
}
Mesh & RenderData::getMesh(size_t i)
{
	return meshes.at(i);
}

size_t RenderData::getNumLights() const
{
	return lights.size();
}
Light & RenderData::getLight(size_t i)
{
	return lights.at(i);
}

const Vector4 & RenderData::getCameraPosition()
{
	return cameraposition;
}
const Vector4 & RenderData::getCameraRotation()
{
	return camerarotation;
}


void RenderData::updateCamera(bool forward, bool backward, bool left, bool right, int turnright, int turndown, int deltatime)
{
	//std::cerr << "right: " << turnright << std::endl;
	//std::cerr << "down: " << turndown << std::endl;


	Vector4 dir = Vector4();
	if (forward ^ backward)
	{
		if (forward)
			dir += Vector4(1.0f, 0.0f, 0.0f);
		else
			dir += Vector4(-1.0f, 0.0f, 0.0f);
	}
	if (left ^ right)
	{
		if (left)
			dir += Vector4(0.0f, 1.0f, 0.0f);
		else
			dir += Vector4(0.0f, -1.0f, 0.0f);
	}
	if (dir.length() > 0.0f)
		dir = dir.normalize() * ((float)deltatime * 0.025f);
	//std::cerr << "pre x: " << cameraposition.x() << " y: " << cameraposition.y() << " z: " << cameraposition.z() << std::endl;
	//cameraposition = cameraposition.rotateX(turndown * 0.1f);
	cameraposition = cameraposition.rotateZ(-1 * turnright * 0.01f);
	//cameraposition = cameraposition.rotateZ(-1 * turnright * 0.01f);
	//std::cerr << "post x: " << cameraposition.x() << " y: " << cameraposition.y() << " z: " << cameraposition.z() << std::endl;
	//camerarotation = camerarotation.normalize();
	
	//std::cerr << "rotpre x: " << camerarotation.x() << " y: " << camerarotation.y() << " z: " << camerarotation.z() << std::endl;
	//cameraposition = cameraposition.rotateX(turndown * 0.1f);
	camerarotation.z() -= (float)turnright * 0.01f;
	//std::cerr << "rotpost x: " << camerarotation.x() << " y: " << camerarotation.y() << " z: " << camerarotation.z() << std::endl;
	//camerarotation.y() -= (float)turndown * 0.01f;
	if (camerarotation.y() >= M_PI_2)
		camerarotation.y() = (float)M_PI_2 - 0.0001f;
	if (camerarotation.y() <= -M_PI_2)
		camerarotation.y() = -(float)M_PI_2 + 0.0001f;
	if (camerarotation.z() >= M_PI)
		camerarotation.z() -= (float)M_PI * 2.0f;
	if (camerarotation.z() < M_PI)
		camerarotation.z() += (float)M_PI * 2.0f;

	//cameraposition = cameraposition.rotate(Vector4(0, 0, -(float)turnright * 0.01f));
	//cameraposition = cameraposition.rotate(Vector4(-(float)turndown * 0.01f, 0 	, 0));
	//cameraposition = cameraposition.rotate(Vector4(0, 0, -(float)turnright * 0.01f));
	
	cameraposition += dir.rotate(camerarotation);
}


unsigned char RenderData::readByte(std::ifstream & file)
{
	unsigned char result;
	file.read(reinterpret_cast<char*>(&result), 1);

	return result;
}

int RenderData::readInt(std::ifstream & file)
{
	int result;
	file.read(reinterpret_cast<char*>(&result), 4);
	return result;
}

float RenderData::readFloat(std::ifstream & file)
{
	float result;
	file.read(reinterpret_cast<char*>(&result), 4);
	return result;
}

std::string RenderData::readString(std::ifstream & file)
{
	std::string result;
	size_t length = readInt(file);
	result.resize(length, 0);
	file.read(&result[0], length);
	file.get();
	return result;
}

Vector4 RenderData::readColor(std::ifstream & file)
{
	Vector4 result;
	result.x() = (float)readByte(file) / 255.0f;
	result.y() = (float)readByte(file) / 255.0f;
	result.z() = (float)readByte(file) / 255.0f;

	return result;
}
Vector4 RenderData::readPoint2(std::ifstream & file)
{
	Vector4 result;
	result.x() = readFloat(file);
	result.y() = readFloat(file);
	return result;
}
Vector4 RenderData::readPoint3(std::ifstream & file)
{
	Vector4 result;
	result.x() = readFloat(file);
	result.y() = readFloat(file);
	result.z() = readFloat(file);
	return result;
}

void RenderData::readPoint2(std::ifstream & file, float * point2)
{
	point2[0] = readFloat(file);
	point2[1] = readFloat(file);
}
void RenderData::readPoint3(std::ifstream & file, float * point3)
{
	point3[0] = readFloat(file);
	point3[1] = readFloat(file);
	point3[2] = readFloat(file);
}
void RenderData::readIndex3(std::ifstream & file, unsigned int * index3)
{
	index3[0] = readInt(file);
	index3[1] = readInt(file);
	index3[2] = readInt(file);
}
