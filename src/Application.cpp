#include "Application.h"
void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
  
	//player movement
	if (key == GLFW_KEY_W && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(W, true);
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(A, true);
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(S, true);
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(D, true);
	}

	if (key == GLFW_KEY_W && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(W, false);
	}

	if (key == GLFW_KEY_A && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(A, false);
	}
	if (key == GLFW_KEY_S && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(S, false);
	}

	if (key == GLFW_KEY_D && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(D, false);
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
		renderLines = true;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (key == GLFW_KEY_Z && action == GLFW_RELEASE) {
		renderLines = false;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		renderPostProcessing = false;
	}
	if (key == GLFW_KEY_P && action == GLFW_RELEASE) {
		renderPostProcessing = true;
	}

	// Falling toggle
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) {
		componentManager.GetPlayer().SetInput(LSHIFT, true);
	}
	if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
		componentManager.GetPlayer().SetInput(LSHIFT, false);
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS) {
		componentManager.GetCamera().alt_pressed = true;
	}

	if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
		componentManager.GetCamera().alt_pressed = false;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		componentManager.GetCamera().x_pressed = true;
	}

	if (key == GLFW_KEY_X&& action == GLFW_RELEASE) {
		componentManager.GetCamera().x_pressed = false;
	}
}

void Application::mouseCallback(GLFWwindow* window, int button, int action, int mods)
{
	double posX, posY;

	if (action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &posX, &posY);
		cout << "Pos X " << posX << " Pos Y " << posY << endl;
	}
}

void Application::mouseMovementCallback(GLFWwindow* window, double posX, double posY)
{
	
}

void Application::InitTerrain() {
	TerrainS terrain;
	terrain.numVerts = MESHSIZE * MESHSIZE * 6;
	//generate the VAO
	glGenVertexArrays(1, &terrain.VAOId);
	glBindVertexArray(terrain.VAOId);

	//generate vertex buffer to hand off to OGL
	GLuint terrainPosBuf;
	glGenBuffers(1, &terrainPosBuf);
	glBindBuffer(GL_ARRAY_BUFFER, terrainPosBuf);
	vec3* vertices = (vec3*)malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec3));
	for (int x = 0; x < MESHSIZE; x++)
		for (int z = 0; z < MESHSIZE; z++)
		{
			vertices[x * 4 + z * MESHSIZE * 4 + 0] = (vec3(0.0, 0.0, 0.0) + vec3(x, 0, z)) * 3.0f;
			vertices[x * 4 + z * MESHSIZE * 4 + 1] = (vec3(1.0, 0.0, 0.0) + vec3(x, 0, z)) * 3.0f;
			vertices[x * 4 + z * MESHSIZE * 4 + 2] = (vec3(1.0, 0.0, 1.0) + vec3(x, 0, z)) * 3.0f;
			vertices[x * 4 + z * MESHSIZE * 4 + 3] = (vec3(0.0, 0.0, 1.0) + vec3(x, 0, z)) * 3.0f;
		}
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * MESHSIZE * MESHSIZE * 4, vertices, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	free(vertices);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//tex coords
	float t = 1. / (float) MESHSIZE;
	vec2* tex = (vec2*)malloc(MESHSIZE * MESHSIZE * 4 * sizeof(vec2));
	for (int x = 0; x < MESHSIZE; x++)
		for (int y = 0; y < MESHSIZE; y++)
		{
			tex[x * 4 + y * MESHSIZE * 4 + 0] = vec2(0.0, 0.0) + vec2(x, y) * t;
			tex[x * 4 + y * MESHSIZE * 4 + 1] = vec2(t, 0.0) + vec2(x, y) * t;
			tex[x * 4 + y * MESHSIZE * 4 + 2] = vec2(t, t) + vec2(x, y) * t;
			tex[x * 4 + y * MESHSIZE * 4 + 3] = vec2(0.0, t) + vec2(x, y) * t;
		}

	GLuint terrainTexBuf;
	glGenBuffers(1, &terrainTexBuf);
	//set the current state to focus on our vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, terrainTexBuf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * MESHSIZE * MESHSIZE * 4, tex, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	free(tex);

	glGenBuffers(1, &terrain.IndexBuff);
	//set the current state to focus on our vertex buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IndexBuff);
	GLushort* elements = (GLushort*)malloc(MESHSIZE * MESHSIZE * 6 * sizeof(GLushort));
	int ind = 0;
	for (int i = 0; i < MESHSIZE * MESHSIZE * 6; i += 6, ind += 4)
	{
		elements[i + 0] = ind + 0;
		elements[i + 1] = ind + 1;
		elements[i + 2] = ind + 2;
		elements[i + 3] = ind + 0;
		elements[i + 4] = ind + 2;
		elements[i + 5] = ind + 3;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * MESHSIZE * MESHSIZE * 6, elements, GL_STATIC_DRAW);
	glBindVertexArray(0);
	free(elements);
	shaderManager.Terrain = terrain;
}

void Application::InitSkybox(const std::string& resourceDirectory)
{
	unsigned int textureID;

	vector<std::string> faces{
		"StarSkybox041.png",
		"StarSkybox042.png",
		"StarSkybox043.png",
		"StarSkybox044.png",
		"StarSkybox045.png",
		"StarSkybox046.png"
	};

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	for (GLuint i = 0; i < faces.size(); i++) {
		unsigned char *data =
			stbi_load((resourceDirectory + "/StarSkybox/" + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			cout << "failed to load: " << (resourceDirectory + "/" + faces[i]).c_str() << endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	cout << " creating cube map any errors : " << glGetError() << endl;
	shaderManager.skyboxTexId = textureID;
}

// Q: Why is shader manager all handled in application?
void Application::InitShaderManager(const std::string& resourceDirectory)
{
	shaderManager = ShaderManager::GetInstance();
	GLuint tex;
	int width, height, channels;
	
	//this lambda does a general loadTexture. Can add more parameters to change its behavior if needed.
	auto loadTexture = [this, resourceDirectory, &tex, &width, &height, &channels] //captures
	(string filename, string managerIdentifer, bool filterMipMap = true) { //parameters
		char filepath[1000];
		string str = resourceDirectory + filename;
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &tex);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (filterMipMap)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		shaderManager.SetTexture(managerIdentifer, tex);
	};

	// load all textures
	loadTexture("/cat.jpg", "Cat");
	loadTexture("/LUNA/new/lunaModelTextures/luna3_lambert1_BaseColor.png", "Luna");
	loadTexture("/LUNA/new/lunaModelTextures/luna3_lambert1_Emissive.png", "Luna Emissive");
	loadTexture("/LUNA/new/lunaModelTextures/luna3_lambert1_Normal.png", "Luna Normal");
	loadTexture("/LUNA/new/lunaModelTextures/luna3_lambert1_Roughness.png", "Luna Roughness");
	loadTexture("/LUNA/new/lunaModelTextures/luna3_lambert1_Metallic.png", "Luna Metal");
	loadTexture("/grass.jpg", "Grass");
	loadTexture("/alpha.bmp", "Alpha", false);
	loadTexture("/smoke_spritesheet.png", "SandPartTex");
	loadTexture("/noiseTex.png", "noiseTex", false);
	loadTexture("/sandShallow.jpg", "sandShallow");
	loadTexture("/sandSteep.jpg", "sandSteep");
	loadTexture("/CloudNoise/cloud_BaseNoise.png", "cloudBaseNoise");
	loadTexture("/CloudNoise/cloud_NoiseTexture.png", "cloudNoise");
	loadTexture("/CloudNoise/cloud_Distort.png", "cloudDistort");
	loadTexture("/drone_rockie/Robot_Rockie_BaseColor.png", "droneBase");
	loadTexture("/drone_rockie/Robot_Rockie_Emissive.png", "droneEmiss");
	loadTexture("/drone_rockie/Robot_Rockie_Normal.png", "droneNormal");
	loadTexture("/rainbow.jpg", "Rainbow");

	// HUD textures
	loadTexture("/HUD/starfragment.png", "starFragment");
	loadTexture("/HUD/MoonBar.png", "moonBar");
	loadTexture("/HUD/MoonIcon.png", "moonIcon");
	loadTexture("/HUD/num_slash.png", "slash");
	loadTexture("/HUD/num_x.png", "x");
	loadTexture("/HUD/winTex.png", "win");
	loadTexture("/HUD/loseTex.png", "lose");

	for (int i = 0; i <= 10; i++)
	{
		loadTexture("/HUD/num_" + to_string(i) + ".png", "num_" + to_string(i));
	}


	// used on Luna
	auto luna_body = make_shared<Program>();
	luna_body->setVerbose(true);
	luna_body->setShaderNames(resourceDirectory + "/luna_body_vert.glsl", resourceDirectory + "/luna_body_frag.glsl");
	luna_body->Init();
	luna_body->addUniform("P");
	luna_body->addUniform("V");
	luna_body->addUniform("M");
	luna_body->addUniform("flashAmt");
	luna_body->addUniform("flashCol");
	luna_body->addUniform("camPos");
	luna_body->addUniform("lightDir");
	luna_body->addAttribute("vertPos");
	luna_body->addAttribute("vertNor");
	luna_body->addAttribute("vertTex");
	luna_body->addAttribute("vertTan");
	luna_body->addAttribute("vertBN");

	GLuint TexLocation = glGetUniformLocation(luna_body->pid, "tex");
	glUseProgram(luna_body->pid);
	glUniform1i(TexLocation, 0);
	TexLocation = glGetUniformLocation(luna_body->pid, "emissiveTex");
	glUseProgram(luna_body->pid);
	glUniform1i(TexLocation, 1);
	TexLocation = glGetUniformLocation(luna_body->pid, "normalTex");
	glUseProgram(luna_body->pid);
	glUniform1i(TexLocation, 2);
	TexLocation = glGetUniformLocation(luna_body->pid, "roughness");
	glUseProgram(luna_body->pid);
	glUniform1i(TexLocation, 3);
	TexLocation = glGetUniformLocation(luna_body->pid, "metal");
	glUseProgram(luna_body->pid);
	glUniform1i(TexLocation, 4);

	shaderManager.SetShader("Luna Body", luna_body);

	auto head_prog = make_shared<Program>();
	head_prog->setVerbose(true);
	head_prog->setShaderNames(resourceDirectory + "/tex_vert.glsl", resourceDirectory + "/head_frag.glsl");
	head_prog->Init();
	head_prog->addUniform("P");
	head_prog->addUniform("V");
	head_prog->addUniform("M");
	head_prog->addUniform("flashAmt");
	head_prog->addUniform("flashCol");
	head_prog->addUniform("eye1Pos");
	head_prog->addUniform("eye1Radius");
	head_prog->addUniform("eyeOpenPct");
	head_prog->addUniform("lightDir");
	head_prog->addUniform("camPos");
	head_prog->addAttribute("vertPos");
	head_prog->addAttribute("vertNor");
	head_prog->addAttribute("vertTex");

	TexLocation = glGetUniformLocation(head_prog->pid, "tex");
	glUseProgram(head_prog->pid);
	glUniform1i(TexLocation, 0);

	shaderManager.SetShader("HeadShader", head_prog);

	//used for particle effects on star fragments
	auto partProg = make_shared<Program>();
	partProg->setVerbose(true);
	partProg->setShaderNames(
		resourceDirectory + "/particle_vert.glsl",
		resourceDirectory + "/particle_frag.glsl");
	partProg->Init();
	partProg->addUniform("P");
	partProg->addUniform("M");
	partProg->addUniform("V");
	partProg->addUniform("totalTime");
	partProg->addUniform("centerPos");
	partProg->addUniform("alphaTexture");
	partProg->addUniform("alphaMult");
	partProg->addUniform("rainbowTexture");
	partProg->addAttribute("pColor");
	partProg->addAttribute("pNormal");
	partProg->addAttribute("pRotation");

	GLuint PartLocation0 = glGetUniformLocation(partProg->pid, "alphaTexture");
	GLuint PartLocation1 = glGetUniformLocation(partProg->pid, "rainbowTexture");
	glUseProgram(partProg->pid);
	glUniform1i(PartLocation0, 0);
	glUniform1i(PartLocation1, 1);

	shaderManager.SetShader("particle", partProg);

	auto droneProg = make_shared<Program>();
	droneProg->setVerbose(true);
	droneProg->setShaderNames(
		resourceDirectory + "/drone_vertex.glsl",
		resourceDirectory + "/drone_frag.glsl"
	);
	droneProg->Init();
	droneProg->addUniform("P");
	droneProg->addUniform("M");
	droneProg->addUniform("V");
	droneProg->addAttribute("vertPos");
	droneProg->addAttribute("vertNor");
	droneProg->addAttribute("vertTex");
	droneProg->addAttribute("vertTan");
	droneProg->addAttribute("vertBN");

	TexLocation = glGetUniformLocation(droneProg->pid, "tex");
	glUseProgram(droneProg->pid);
	glUniform1i(TexLocation, 0);
	TexLocation = glGetUniformLocation(droneProg->pid, "emissiveTex");
	glUseProgram(droneProg->pid);
	glUniform1i(TexLocation, 1);
	TexLocation = glGetUniformLocation(droneProg->pid, "normalTex");
	glUseProgram(droneProg->pid);
	glUniform1i(TexLocation, 2);

	shaderManager.SetShader("drone", droneProg);

	//used for sand
	auto sandProg = make_shared<Program>();
	sandProg->setVerbose(true);
	sandProg->setShaderNames(
		resourceDirectory + "/sand_vert.glsl",
		resourceDirectory + "/sand_frag.glsl");
	sandProg->Init();
	sandProg->addUniform("P");
	sandProg->addUniform("M");
	sandProg->addUniform("V");
	sandProg->addUniform("totalTime");
	sandProg->addUniform("alphaTime");
	sandProg->addUniform("centerPos");
	sandProg->addUniform("playerSpeed");
	sandProg->addUniform("playerDirection");
	sandProg->addUniform("campos");
	sandProg->addUniform("alphaTexture");
	sandProg->addUniform("alphaMult");
	sandProg->addUniform("alphaSpeed");
	sandProg->addAttribute("pColor");
	sandProg->addAttribute("pNormal");
	sandProg->addAttribute("pRotation");

	sandProg->addUniform("Row");
	sandProg->addUniform("Column");


	GLuint SandLocation0 = glGetUniformLocation(sandProg->pid, "alphaTexture");
	
	glUseProgram(sandProg->pid);
	glUniform1i(SandLocation0, 0);

	shaderManager.SetShader("Sand", sandProg);



	//used for star fragments
	auto starProg = make_shared<Program>();
	starProg->setVerbose(true);
	starProg->setShaderNames(
		resourceDirectory + "/star_vert.glsl",
		resourceDirectory + "/star_frag.glsl");
	starProg->Init();
	starProg->addUniform("P");
	starProg->addUniform("M");
	starProg->addUniform("V");
	starProg->addUniform("starTexture");
	starProg->addUniform("totalTime");
	starProg->addUniform("centerPos");
	starProg->addUniform("playerPos");
	starProg->addUniform("campos");
	starProg->addUniform("lights");
	starProg->addAttribute("vertPos");
	starProg->addAttribute("vertNor");
	starProg->addAttribute("vertTex");

	GLuint StarLocation = glGetUniformLocation(starProg->pid, "Star");
	glUseProgram(starProg->pid);
	glUniform1i(StarLocation, 0);

	shaderManager.SetShader("Star", starProg);

	
	//used for explosion
	auto explosionProg = make_shared<Program>();
	explosionProg->setVerbose(true);
	explosionProg->setShaderNames(
		resourceDirectory + "/explosion_vert.glsl",
		resourceDirectory + "/explosion_frag.glsl");
	explosionProg->Init();
	explosionProg->addUniform("P");
	explosionProg->addUniform("M");
	explosionProg->addUniform("V");
	explosionProg->addUniform("starTexture");
	explosionProg->addUniform("totalTime");
	explosionProg->addUniform("centerPos");
	explosionProg->addAttribute("vertPos");
	explosionProg->addAttribute("vertNor");
	explosionProg->addAttribute("vertTex");

	GLuint ExplosionLocation = glGetUniformLocation(explosionProg->pid, "Explosion");
	glUseProgram(explosionProg->pid);
	glUniform1i(ExplosionLocation, 0);

	shaderManager.SetShader("Explosion", explosionProg);
	


	// Terrain Shader
	auto heightProg = make_shared<Program>();
	heightProg->setVerbose(true);
	heightProg->setShaderNames(resourceDirectory + "/height_vertex.glsl", resourceDirectory + "/height_frag.glsl",
		resourceDirectory + "/terrain_tesscontrol.glsl", resourceDirectory + "/terrain_tesseval.glsl");
	if (!heightProg->Init())

	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	heightProg->addUniform("P");
	heightProg->addUniform("V");
	heightProg->addUniform("M");

	//Terrain info
	heightProg->addUniform("baseHeight");

	// Camera/Player information
	heightProg->addUniform("campos");
	heightProg->addUniform("playerPos");
	heightProg->addUniform("lightDir");
	heightProg->addUniform("time");
	heightProg->addUniform("LS");

	heightProg->addUniform("diffuseContrast");
	heightProg->addUniform("shadowColor");
	heightProg->addUniform("terrainColor");
	heightProg->addUniform("sandStrength");
	heightProg->addUniform("shadowCastColor");
	// rim 
	heightProg->addUniform("rimStrength");
	heightProg->addUniform("rimPower");
	heightProg->addUniform("rimColor");
	// ocean spec
	heightProg->addUniform("oceanSpecularStrength");
	heightProg->addUniform("oceanSpecularPower");
	heightProg->addUniform("oceanSpecularColor");
	// sand ripples
	heightProg->addUniform("steepnessSharpnessPower");
	heightProg->addUniform("specularHardness");
	heightProg->addUniform("sandRipplesStrength");


	heightProg->addAttribute("vertPos");
	heightProg->addAttribute("vertTex");

	assert(glGetError() == GL_NO_ERROR);

	TexLocation = glGetUniformLocation(heightProg->pid, "tex");
	GLuint TexLocation2 = glGetUniformLocation(heightProg->pid, "tex2");
	GLuint TexLocation3 = glGetUniformLocation(heightProg->pid, "noiseTex");
	GLuint TexLocation4 = glGetUniformLocation(heightProg->pid, "sandShallow");
	GLuint TexLocation5 = glGetUniformLocation(heightProg->pid, "sandSteep");
	GLuint TexLocation6 = glGetUniformLocation(heightProg->pid, "shadowDepth");

	glUseProgram(heightProg->pid);
	glUniform1i(TexLocation, 0);
	glUniform1i(TexLocation2, 1);
	glUniform1i(TexLocation3, 2);
	glUniform1i(TexLocation4, 3);
	glUniform1i(TexLocation5, 4);
	glUniform1i(TexLocation6, 5);

	assert(glGetError() == GL_NO_ERROR);

	shaderManager.SetShader("Height", heightProg);

	// Skybox Shader
	auto skyboxProg = make_shared<Program>();
	skyboxProg->setVerbose(true);
	skyboxProg->setShaderNames(resourceDirectory + "/skybox_vert.glsl", resourceDirectory + "/skybox_frag.glsl");
	if (!skyboxProg->Init())

	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	skyboxProg->addUniform("P");
	skyboxProg->addUniform("V");
	skyboxProg->addUniform("M");
	skyboxProg->addUniform("time");
	skyboxProg->addUniform("dayBottomColor");
	skyboxProg->addUniform("dayTopColor");
	skyboxProg->addUniform("nightBottomColor");
	skyboxProg->addUniform("nightTopColor");
	skyboxProg->addUniform("horizonColor");
	skyboxProg->addUniform("sunDir");
	skyboxProg->addUniform("moonOffset");
	skyboxProg->addUniform("cloudScale");
	skyboxProg->addUniform("cloudSpeed");
	skyboxProg->addUniform("cloudCutoff");
	skyboxProg->addUniform("cloudFuzziness");
	skyboxProg->addUniform("cloudColorDayEdge");
	skyboxProg->addUniform("cloudColorDayMain");
	skyboxProg->addUniform("cloudColorNightEdge");
	skyboxProg->addUniform("cloudColorNightMain");

	skyboxProg->addAttribute("vertPos");

	TexLocation = glGetUniformLocation(skyboxProg->pid, "cloudBaseNoise");
	TexLocation2 = glGetUniformLocation(skyboxProg->pid, "cloudNoise");
	TexLocation3 = glGetUniformLocation(skyboxProg->pid, "cloudDistort");

	glUseProgram(skyboxProg->pid);
	glUniform1i(TexLocation, 1);
	glUniform1i(TexLocation2, 2);
	glUniform1i(TexLocation3, 3);

	assert(glGetError() == GL_NO_ERROR);
	shaderManager.SetShader("Skybox", skyboxProg);

	// HUD shader
	auto HUDprog = make_shared<Program>();
	HUDprog->setVerbose(true);
	HUDprog->setShaderNames(resourceDirectory + "/hud_vert.glsl", resourceDirectory + "/hud_frag.glsl");
	if (!HUDprog->Init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	HUDprog->addUniform("P");
	HUDprog->addUniform("M");
	HUDprog->addUniform("texBuf");
	HUDprog->addAttribute("vertPos");
	HUDprog->addAttribute("vertTex");


	TexLocation = glGetUniformLocation(HUDprog->pid, "Texture0");

	glUseProgram(HUDprog->pid);
	glUniform1i(TexLocation, 1);

	assert(glGetError() == GL_NO_ERROR);
	shaderManager.SetShader("HUD", HUDprog);

	// Shadow Depth
	auto shadowDepthProg = make_shared<Program>();
	shadowDepthProg->setVerbose(true);
	shadowDepthProg->setShaderNames(resourceDirectory + "/shadow_depth_vert.glsl", resourceDirectory + "/shadow_depth_frag.glsl");
	if (!shadowDepthProg->Init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}

	shadowDepthProg->addUniform("LP");
	shadowDepthProg->addUniform("LV");
	shadowDepthProg->addUniform("M");
	shadowDepthProg->addUniform("castShadows");
	shadowDepthProg->addAttribute("vertPos");

	assert(glGetError() == GL_NO_ERROR);
	shaderManager.SetShader("ShadowDepth", shadowDepthProg);

	// Terrain Depth
	auto terrainDepthProg = make_shared<Program>();
	terrainDepthProg->setVerbose(true);
	terrainDepthProg->setShaderNames(resourceDirectory + "/terrain_depth_vert.glsl", resourceDirectory + "/shadow_depth_frag.glsl",
		resourceDirectory + "/terrain_depth_tesscontrol.glsl", resourceDirectory + "/terrain_depth_tesseval.glsl");
	if (!terrainDepthProg->Init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	terrainDepthProg->addUniform("LP");
	terrainDepthProg->addUniform("LV");
	terrainDepthProg->addUniform("M");
	terrainDepthProg->addUniform("camoff");
	terrainDepthProg->addUniform("castShadows");
	terrainDepthProg->addAttribute("vertPos");
	//Terrain info
	terrainDepthProg->addUniform("baseHeight");
	// Camera/Player information
	terrainDepthProg->addUniform("campos");
	terrainDepthProg->addUniform("playerPos");


	assert(glGetError() == GL_NO_ERROR);
	shaderManager.SetShader("TerrainDepth", terrainDepthProg);

	// Texture Debug
	auto textureDebugProg = make_shared<Program>();
	textureDebugProg->setVerbose(true);
	textureDebugProg->setShaderNames(resourceDirectory + "/pass_vert.glsl", resourceDirectory + "/pass_texfrag.glsl");
	if (!textureDebugProg->Init())
	{
		std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
		exit(1);
	}
	textureDebugProg->addUniform("texBuf");
	textureDebugProg->addAttribute("vertPos");

	TexLocation = glGetUniformLocation(textureDebugProg->pid, "texBuf");
	glUseProgram(textureDebugProg->pid);
	glUniform1i(TexLocation, 1);

	assert(glGetError() == GL_NO_ERROR);
	shaderManager.SetShader("TextureDebug", textureDebugProg);

	//the obj files you want to load. Add more to read them all.
	vector<string> filenames = { "sphere", "Star Bit", "icoSphere", "LUNA/new/lunaModelTextures/right_arm",
		"LUNA/new/lunaModelTextures/left_arm", "LUNA/new/lunaModelTextures/body", "LUNA/new/lunaModelTextures/head", 
    "unit_cube", "quad", "drone_rockie/Robot_Rockie"};
	vector<bool> normalMapFlags = { false, false, false, true, true, true, false, false, false, true};	// not extensible, should be tied to the model but 1 week remaining.
	vec3 explosionScaleFactor = vec3(60.0f);
	//where the data is held
	vector<vector<tinyobj::shape_t>> TOshapes(filenames.size());
	vector<tinyobj::material_t> objMaterials; //not using for now.
	bool rc = false;
	string errStr;
	for (size_t i = 0; i < filenames.size(); ++i) {
		rc = tinyobj::LoadObj(TOshapes[i], objMaterials, errStr, (resourceDirectory + "/" + filenames[i] + ".obj").c_str());
		if (!rc) {
			cerr << errStr << endl;
			exit(EXIT_FAILURE);
		}
		else {
			//some obj files have multiple shapes, read just the first one for now.
			shared_ptr<Shape> shape = make_shared<Shape>();
			shape->createShape(TOshapes[i][0]); //the first (0'th) shape read in of the i'th file.
			shape->measure();
			if (filenames[i] == "icoSphere") {
				shape->scale(explosionScaleFactor);
			}
			shape->computeNormals();
			shape->Init(normalMapFlags[i]);

			shaderManager.SetModel(filenames[i], shape);
		}
	}

	InitTerrain();
	InitSkybox(resourceDirectory);
}

void Application::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowManager->getHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
}


void Application::Init(std::string resourceDirectory)
{
	GLSL::checkVersion();
	// lock the mouse cursor
	//glfwSetInputMode(windowManager->getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// Set background color.
	// glClearColor(.12f, .34f, .56f, 1.0f);
	// Enable z-buffer test.
	CHECKED_GL_CALL(glEnable(GL_DEPTH_TEST));
	CHECKED_GL_CALL(glEnable(GL_BLEND));
	CHECKED_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glPointSize(ParticleRenderer::originalPointSize);
	InitImGui();
	InitShaderManager(resourceDirectory);
	// do ComponentManager's init here
	componentManager.Init(resourceDirectory, &audioEngine);
	audioEngine.Init(resourceDirectory);
	audioEngine.Play("Climbing the Ginso Tree.mp3");

	postProcessing = make_shared<PostProcessing>(windowManager, &componentManager.GetCamera());
	hudRenderer = make_shared<HUDRenderer>();
	hudRenderer->Init(componentManager.GetGameState());
}


void Application::render(float frameTime)
{
	// imgui
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// local modelview matrix use this for later labs

	auto M = make_shared<MatrixStack>();
	auto V = make_shared<MatrixStack>();
	auto P = make_shared<MatrixStack>();

	// Get current frame buffer size.
	int width, height;
	glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
	

	// Clear framebuffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	componentManager.UpdateComponents(frameTime, width, height);

	// Render depth for shadows
	LightComponent::GetInstance(vec3(0)).SetupRenderShadows();
	componentManager.RenderToDepth();


	// clear all framebuffers + bind base one
	glViewport(0, 0, width, height);
	postProcessing->SetUpFrameBuffers();

	if (renderLines || !renderPostProcessing)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, width, height);
	}

	hudRenderer->Update(frameTime, componentManager.GetGameState());
	
	componentManager.Render(frameTime);

	// render post-processing
	if (renderPostProcessing && !renderLines)
    {
		// Check if we are going fast enough that we should flag blur
		float goalBlur = componentManager.GetGameState()->ShouldSpawnSand() ? .92 : 0.0;
    	// render post-processing
    	postProcessing->RenderPostProcessing(frameTime, goalBlur);
    }
	// render HUD
	hudRenderer->RenderHUD(width, height, componentManager.GetGameState());
	//LightComponent::GetInstance(vec3(0)).DebugDrawDepthTexture();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
