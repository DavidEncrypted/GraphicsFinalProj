#include "global.h"
#include "Renderer.h"



Renderer::Renderer(RenderData & renderdata) : renderdata(renderdata), fbo(0), fboms(0)
{}

bool Renderer::initRenderer(int rendersizex, int rendersizey, const std::string & setupfile)
{
	this->rendersizex = rendersizex;
	this->rendersizey = rendersizey;

	std::ifstream file;
	bool cameradefined = false;

	if (!rendercontext.create(rendersizex, rendersizey))
		return false;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	
	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE);

	useAA = true;
	AAcolorsamples = 4;
	AAcoveragesamples = 16;
	if (GLEW_NV_multisample_coverage)
		useCSAA = true;
	else
		useCSAA = false;

	if (!createMainFBO())
	{
		return false;
	}
	glViewport(0, 0, rendersizex, rendersizey);

	if (!tonemapshader.loadShaderProgram("tonemap", false, false))
	{
		std::cerr << tonemapshader.getErrorMessage() << std::endl;
		return false;
	}
	
	if (!skyboxshader.loadShaderProgram("skybox", false, false))
	{
		std::cerr << skyboxshader.getErrorMessage() << std::endl;
		return false;
	}

	if (!billboardshader.loadShaderProgram("billboard", false, true))
	{
		std::cerr << billboardshader.getErrorMessage() << std::endl;
		return false;
	}

	const float quaddata[12] =
	{ -1.0f, -1.0f,
	   1.0f, -1.0f,
	   1.0f,  1.0f,
	   1.0f,  1.0f,
	  -1.0f,  1.0f,
	  -1.0f, -1.0f };
	glGenBuffers(1, &vboquad);
	glBindBuffer(GL_ARRAY_BUFFER, vboquad);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), quaddata, GL_STATIC_DRAW);
	
	std::string skyboxnum;

	file.open(setupfile.c_str());
	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		if (line[line.size()-1] == '\r')
			line.resize(line.size()-1);

		if (line.substr(0, 6) == "Shader")
		{
			if (!usershader.loadShaderProgram(line.substr(7), true, false))
			{
				std::cerr << usershader.getErrorMessage() << std::endl;
				return false;
			}
		}
		else if (line.substr(0, 5) == "Model")
		{
			if (!renderdata.loadModel(line.substr(6)))
			{
				return false;
			}
		}
		else if (line.substr(0, 5) == "Light")
		{
			if (!renderdata.addLight(line.substr(6)))
			{
				return false;
			}
		}
		else if (line.substr(0, 6) == "Camera")
		{
			if (!cameradefined)
			{
				if (!renderdata.setCamera(line.substr(7)))
				{
					return false;
				}
				cameradefined = true;
			}
			else
			{
				std::cerr << "Double Camera definition in setup file" << std::endl;
				return false;
			}
		}
		else if (line.substr(0,6) == "Skybox"){
			std::cerr << "SKYBOX: " << std::stoi(line.substr(7));
			skyboxnum = line.substr(7);
		}
	}
	file.close();

	//std::string sky = "Skybox/";

	std::vector<std::string> skybox1 = 	{"sky-" + skyboxnum + ".back.png","sky-" + skyboxnum + ".front.png","sky-" + skyboxnum + ".right.png",
										 "sky-" + skyboxnum + ".left.png","sky-" + skyboxnum + ".up.png","sky-" + skyboxnum + ".down.png",};


	std::vector<std::string> faces = {"rainbow_bk.png", "rainbow_ft.png" , "rainbow_lf.png",
									   "rainbow_rt.png","rainbow_up.png",  "rainbow_dn.png"};

	renderdata.loadSkybox(skybox1);
	renderdata.loadBillboard("dot.png", "Black/blackSmoke00.png");
	renderdata.getParticles().loadParticles();
	renderdata.loadFireworkController();
	
	//std::cerr << "Skyboxid: " << renderdata.getSkybox().id << std::endl;
	return true;
}

Renderer::~Renderer()
{
	destroyMainFBO();
	rendercontext.destroy();
}

bool Renderer::resize(int rendersizex, int rendersizey)
{
	this->rendersizex = rendersizex;
	this->rendersizey = rendersizey;
	glViewport(0, 0, rendersizex, rendersizey);

	rendercontext.recreate(rendersizex, rendersizey);

	if (!resizeMainFBO())
	{
		return false;
	}

	return true;
}


void Renderer::render()
{
	time = (float)SDL_GetTicks() / 1000.0f;
	delta = time - prevtime;
	prevtime = time;

	const Vector4 & cameraposition = renderdata.getCameraPosition();
	const Vector4 & camerarotation = renderdata.getCameraRotation();
	const Vector4 emptyposition = Vector4(0.0f,0.0f,0.0f,0.0f);

	matprojection = Matrix4::PerspectiveMatrix((float)M_PI_4, (float)rendersizex / (float)rendersizey, 0.1f, 1000.0f);
	matmodelview = Matrix4::LookAtMatrix(cameraposition, camerarotation);
	rotmodelview = Matrix4::LookAtMatrix(emptyposition, camerarotation);

	int ParticlesCount = renderdata.getParticles().Update(delta,cameraposition);

	
	glBindFramebuffer(GL_FRAMEBUFFER, fboms);

	
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	

	glUseProgram(usershader.getProgram());
	
	//drawUserModelDepth();
	
	glClear(GL_COLOR_BUFFER_BIT);

	//drawUserModel();
	//glEnable(GL_BLEND);
	//
	renderdata.getFireworkController().UpdateTime(delta); 

	//
	//
	//std::cerr << ParticlesCount << std::endl;
	//
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	//glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(skyboxshader.getProgram());
	
	drawSkybox();

	glUseProgram(billboardshader.getProgram());

	drawBillboard(ParticlesCount);

	
	// //glDisable(GL_DEPTH_TEST);
	//glClear(GL_DEPTH_BUFFER_BIT);

	//glEnable(GL_DEPTH_TEST);

	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	//glUseProgram(usershader.getProgram());

	//







	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, fboms);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glBlitFramebuffer(0, 0, rendersizex, rendersizey, 0, 0, rendersizex, rendersizey, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, rbcolortexture);

	glUseProgram(tonemapshader.getProgram());

	drawScreenQuad();

	
}

void Renderer::drawBillboard(int ParticlesCount){
	

	Particles par = renderdata.getParticles();

	// Particle stuff
	glBindBuffer(GL_ARRAY_BUFFER, par.getPositionVBO());
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, par.getParticlePositionPtr());

	glBindBuffer(GL_ARRAY_BUFFER, par.getColorVBO());
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, par.getParticleColorPtr());








	// Billboard stuff
	
	// ... set view and projection matrix
	glUniformMatrix4fv(glGetUniformLocation(billboardshader.getProgram(), "matmodelview"), 1, GL_TRUE, matmodelview.elements());
	glUniformMatrix4fv(glGetUniformLocation(billboardshader.getProgram(), "matprojection"), 1, GL_TRUE, matprojection.elements());
	
	GLuint id = renderdata.getBillboard().textureid;
	GLuint smokeid = renderdata.getBillboard().smokeid;


	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	//glUniform1i(glGetUniformLocation(billboardshader.getProgram(), "billboardTextureSampler"), 7);

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, smokeid);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	//glUniform1i(glGetUniformLocation(billboardshader.getProgram(), "smokeTextureSampler"), 6);
	// // // This is the only interesting part of the tutorial.
	// This is equivalent to mlutiplying (1,0,0) and (0,1,0) by inverse(ViewMatrix).
	// ViewMatrix is orthogonal (it was made this way), 
	// so its inverse is also its transpose, 
	// and transposing a matrix is "free" (inversing is slooow)
	Vector4 rightvector = matmodelview.RightVector();
	Vector4 upvector = matmodelview.UpVector();
	//std::cerr << "right: " << rightvector.x() << " " << rightvector.y() << " " << rightvector.z() << std::endl;
	//std::cerr << "up: " << upvector.x() << " " << upvector.y() << " " << upvector.z() << std::endl;

	glUniform3f(glGetUniformLocation(billboardshader.getProgram(), "CameraRight_worldspace"), rightvector.x(), rightvector.y(), rightvector.z());
	glUniform3f(glGetUniformLocation(billboardshader.getProgram(), "CameraUp_worldspace"), upvector.x(), upvector.y(), upvector.z());
	

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, renderdata.getBillboard().vbo);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	
	

	// 2nd attribute buffer : positions of particles' centers
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, par.getPositionVBO());
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : x + y + z + size => 4
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : particles' colors
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, par.getColorVBO());
	glVertexAttribPointer(
		2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		4,                                // size : r + g + b + a => 4
		GL_UNSIGNED_BYTE,                 // type
		GL_TRUE,                          // normalized?    *** YES, this means that the unsigned char[4] will be accessible with a vec4 (floats) in the shader ***
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
	glVertexAttribDivisor(1, 1); // positions : one per quad (its center)                 -> 1
	glVertexAttribDivisor(2, 1); // color : one per quad                                  -> 1

	// GLfloat *posarray = new GLfloat[40];
	// GLfloat *intensityarray = new GLfloat[40];

	// for (int i = 0; i < 40; i++){
	// 	posarray[i] = 0.0f;
	// 	intensityarray[i] = 0.0f;
	// }
	// // light 0: 0.0f,0.0f,0.0f,0.0f
	
	// // light 1: 0.0f,50.0f,0.0f,0.0f
	// posarray[5] = -80.0f;
	
	// // 0,1,2,3 = 1.0f,0.0f,0.0f,0.0f
	// intensityarray[0] = 1.0f;

	// // 4,5,6,7 = 0.0f,1.0f,0.0f,0.0f
	// intensityarray[5] = 1.0f;
	


	glUniform4fv(glGetUniformLocation(billboardshader.getProgram(), 
		"light_position"), 10, par.getLightPositions());
	glUniform4fv(glGetUniformLocation(billboardshader.getProgram(), 
		"light_intensity"), 10, par.getLightIntensities());




	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
}

void Renderer::drawSkybox(){

	
	
	// ... set view and projection matrix
	glUniformMatrix4fv(glGetUniformLocation(skyboxshader.getProgram(), "matmodelview"), 1, GL_TRUE, rotmodelview.elements());
	glUniformMatrix4fv(glGetUniformLocation(skyboxshader.getProgram(), "matprojection"), 1, GL_TRUE, matprojection.elements());
	glUniform3f(glGetUniformLocation(skyboxshader.getProgram(), "colorfix"), 0.1f,0.2f,0.2f);
	//glUniform3f(glGetUniformLocation(skyboxshader.getProgram(), "colorfix"), upvector.x(), upvector.y(), upvector.z());
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, renderdata.getSkybox().vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindVertexArray(renderdata.getSkybox().vao);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, renderdata.getSkybox().id);
	glUniform1i(glGetUniformLocation(skyboxshader.getProgram(), "skybox"), 0);

	


	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	

	glDisableVertexAttribArray(0);
	glActiveTexture(0);

}


bool Renderer::toggleAA()
{
	useAA = !useAA;
	if (!resizeMainFBO())
	{
		std::cout << "Failed to enable anti-aliasing" << std::endl;
		useAA = !useAA;
		if (!resizeMainFBO())
		{
			return false;
		}
	}
	return true;
}



void Renderer::drawScreenQuad()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboquad);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(0);
}

void Renderer::drawUserModelDepth()
{
	size_t i;

	glUniformMatrix4fv(glGetUniformLocation(usershader.getProgram(), "matmodelview"), 1, GL_TRUE, matmodelview.elements());
	glUniformMatrix4fv(glGetUniformLocation(usershader.getProgram(), "matprojection"), 1, GL_TRUE, matprojection.elements());
	
	glUniform1fv(glGetUniformLocation(usershader.getProgram(), "time"), 1, &time);

	for (i = 0; i < renderdata.getNumMeshes(); ++i)
	{
		glBindTexture(GL_TEXTURE_2D, 0);

		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		//enable position
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//bind the face indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderdata.getMesh(i).vbo_faces);
		
		glDrawElements(GL_TRIANGLES, 3 * renderdata.getMesh(i).numfaces, GL_UNSIGNED_INT, 0);
		
		glDisableVertexAttribArray(0);
		
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	}
}

void Renderer::drawUserModel()
{
	
	size_t i, j;
	
	glUniformMatrix4fv(glGetUniformLocation(usershader.getProgram(), "matmodelview"), 1, GL_TRUE, matmodelview.elements());
	glUniformMatrix4fv(glGetUniformLocation(usershader.getProgram(), "matprojection"), 1, GL_TRUE, matprojection.elements());

	glUniform1fv(glGetUniformLocation(usershader.getProgram(), "time"), 1, &time);
	
	glUniform3fv(glGetUniformLocation(usershader.getProgram(), "camera_position"), 1, renderdata.getCameraPosition().elements());

	for (i = 0; i < renderdata.getNumMeshes(); ++i)
	{
		
		glUniform3fv(glGetUniformLocation(usershader.getProgram(), "material_diffuse"), 1, renderdata.getMesh(i).material->diffuse.elements());
		glUniform3fv(glGetUniformLocation(usershader.getProgram(), "material_specular"), 1, renderdata.getMesh(i).material->specular.elements());
		glUniform1fv(glGetUniformLocation(usershader.getProgram(), "material_shininess"), 1, &renderdata.getMesh(i).material->specularlevel);
		glUniform1fv(glGetUniformLocation(usershader.getProgram(), "material_glossiness"), 1, &renderdata.getMesh(i).material->glossiness);
		
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, renderdata.getMesh(i).material->texdiffuse);
		glUniform1i(glGetUniformLocation(usershader.getProgram(), "material_diffuse_map"), 4);
		glUniform1i(glGetUniformLocation(usershader.getProgram(), "material_diffuse_usemap"), renderdata.getMesh(i).material->texdiffuse);

		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, renderdata.getMesh(i).material->texbump);
		glUniform1i(glGetUniformLocation(usershader.getProgram(), "material_normal_map"), 5);
		glUniform1i(glGetUniformLocation(usershader.getProgram(), "material_normal_usemap"), renderdata.getMesh(i).material->texbump);

		//enable position
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_vertex);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		
		//enable normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_tbn);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
		
		//enable tangents
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_tbn);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(0 * sizeof(float)));
		
		//enable bitangents
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_tbn);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
		
		//enable texcoords
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, renderdata.getMesh(i).vbo_texcoord);
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 0, 0);
		
		//bind the face indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderdata.getMesh(i).vbo_faces);
		
		for (j = 0; j < renderdata.getNumLights(); ++j)
		{
			glUniform3fv(glGetUniformLocation(usershader.getProgram(), "light_position"), 1, renderdata.getLight(j).position.elements());
			glUniform3fv(glGetUniformLocation(usershader.getProgram(), "light_targetposition"), 1, renderdata.getLight(j).target.elements());
			glUniform3fv(glGetUniformLocation(usershader.getProgram(), "light_intensity"), 1, renderdata.getLight(j).intensity.elements());
			glUniform4fv(glGetUniformLocation(usershader.getProgram(), "light_params"), 1, renderdata.getLight(j).params.elements());

			glDrawElements(GL_TRIANGLES, 3 * renderdata.getMesh(i).numfaces, GL_UNSIGNED_INT, 0);
		}
		
		
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
		
		glDisableVertexAttribArray(0);
		
	}
	
	glActiveTexture(GL_TEXTURE0);

	
}


bool Renderer::createMainFBO()
{
	GLuint status;

	glGenFramebuffers(1, &fboms);
	glBindFramebuffer(GL_FRAMEBUFFER, fboms);

	glGenRenderbuffers(1, &rbcolor);
	glBindRenderbuffer(GL_RENDERBUFFER, rbcolor);
	if (useAA)
	{
		if (useCSAA)
			glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER, AAcoveragesamples, AAcolorsamples, GL_RGBA16F, rendersizex, rendersizey);
		else
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAcolorsamples, GL_RGBA16F, rendersizex, rendersizey);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, rendersizex, rendersizey);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbcolor);

	glGenRenderbuffers(1, &rbdepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbdepth);
	if (useAA)
	{
		if (useCSAA)
			glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER, AAcoveragesamples, AAcolorsamples, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
		else
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAcolorsamples, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbdepth);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer Error (AA): " << std::hex << status << std::endl;
		return false;
	}
	

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &rbcolortexture);
	glBindTexture(GL_TEXTURE_2D, rbcolortexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, rendersizex, rendersizey, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rbcolortexture, 0);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer Error: " << std::hex << status << std::endl;
		return false;
	}

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

bool Renderer::resizeMainFBO()
{
	GLuint status;

	glBindFramebuffer(GL_FRAMEBUFFER, fboms);
	
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

	glDeleteRenderbuffers(1, &rbcolor);
	glDeleteRenderbuffers(1, &rbdepth);

	glGenRenderbuffers(1, &rbcolor);
	glBindRenderbuffer(GL_RENDERBUFFER, rbcolor);
	if (useAA)
	{
		if (useCSAA)
			glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER, AAcoveragesamples, AAcolorsamples, GL_RGBA16F, rendersizex, rendersizey);
		else
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAcolorsamples, GL_RGBA16F, rendersizex, rendersizey);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA16F, rendersizex, rendersizey);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, rbcolor);

	glGenRenderbuffers(1, &rbdepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbdepth);
	if (useAA)
	{
		if (useCSAA)
			glRenderbufferStorageMultisampleCoverageNV(GL_RENDERBUFFER, AAcoveragesamples, AAcolorsamples, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
		else
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, AAcolorsamples, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
	}
	else
	{
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, rendersizex, rendersizey);
	}

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbdepth);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer Error: " << std::hex << status << std::endl;
		return false;
	}


	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, 0, 0);
	
	glDeleteTextures(1, &rbcolortexture);

	glGenTextures(1, &rbcolortexture);
	glBindTexture(GL_TEXTURE_2D, rbcolortexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, rendersizex, rendersizey, 0, GL_RGBA, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rbcolortexture, 0);

	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer Error: " << std::hex << status << std::endl;
		return false;
	}

	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void Renderer::destroyMainFBO()
{
	if (fbo)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);

		glDeleteTextures(1, &rbcolortexture);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteFramebuffers(1, &fbo);
	}
	
	if (fboms)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, fboms);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

		glDeleteTextures(1, &rbcolor);
		glDeleteRenderbuffers(1, &rbdepth);
	
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDeleteFramebuffers(1, &fboms);
	}
}
