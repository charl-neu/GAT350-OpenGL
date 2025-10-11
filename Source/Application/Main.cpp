
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
     

    SDL_Event e;
    bool quit = false;

    //opengl init
    std::vector<neu::vec3> points{ { 0, 0.5f, 0 }, { 0.3535f, -0.3535f, 0 }, { -0.3535f, -0.3535f, 0 }, { 0, -0.5f, 0 }, { 0.3535f, 0.3535f, 0 }, { -0.3535f, 0.3535f, 0 } };
    std::vector<neu::vec3> colors{ { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 } };

    GLuint vbo[2];
    glGenBuffers(2, vbo);

	//vertex buffer (position)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(neu::vec3), points.data(), GL_STATIC_DRAW);

	//vertex buffer (color)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(neu::vec3), colors.data(), GL_STATIC_DRAW);


    //vertex array
    GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// position
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // color
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
	neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

    GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_cstr, NULL);
	glCompileShader(vs);
    int success;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(vs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

	//fragment shader
    std::string fs_source;
	neu::file::ReadTextFile("Shaders/basic.frag", fs_source);
	const char* fs_cstr = fs_source.c_str();

    GLuint fs;
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_cstr, NULL);
	glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetShaderInfoLog(fs, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fs);
	glAttachShader(shaderProgram, vs);
	glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        std::string infoLog(512, '\0');  // pre-allocate space
        GLsizei length;
        glGetProgramInfoLog(shaderProgram, (GLsizei)infoLog.size(), &length, &infoLog[0]);
        infoLog.resize(length);

        LOG_WARNING("Shader compilation failed: {}", infoLog);
    }
	glUseProgram(shaderProgram);

    //uniform
	GLint uniform = glGetUniformLocation(shaderProgram, "u_time");
	ASSERT(uniform != -1);


    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

		glUniform1f(uniform, neu::GetEngine().GetTime().GetTime());

        /*
		float angle = neu::GetEngine().GetTime().GetTime()*100;
		float scale = (std::sinf(SDL_GetTicks() / 500.0f) + .4f) / 2.0f * .25f; // 0
		neu::vec2 mousePos = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;
		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mousePos.x);
		position.y = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), 1.0f, -1.0f, mousePos.y);

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        */
        neu::GetEngine().GetRenderer().Clear();

		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)points.size());

        /*
		glLoadIdentity();
		glPushMatrix();

		glTranslatef(position.x, position.y, 0);
		glRotatef(angle, 0, 0, 1);
		glScalef(scale, scale, 1);

		glBegin(GL_TRIANGLES); //as designed, makes two overlapping triangles
		//glBegin(GL_LINES); //makes 3 lines of irregular lengths
		//glBegin(GL_POINTS); //creates 6 points
		//glBegin(GL_LINE_LOOP); //makes a crown-like shape
		//glBegin(GL_TRIANGLE_STRIP); //makes a star shape like GL_triangles but deformed

        for (int i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
        
		glPopMatrix();

        glEnd();
        */

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
