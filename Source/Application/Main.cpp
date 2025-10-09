
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

    /* glColor3f(0,0,255);
 glVertex3f(0,.5,0);
 glColor3f(255,0,0);
 glVertex3f(.3535,-.3535,0);
 glColor3f(0,255,0);
 glVertex3f(-.3535,-.3535,0);

 glColor3f(255, 0, 255);
 glVertex3f(.3535, .3535, 0);
 glColor3f(0, 255, 255);
 glVertex3f(-.3535, .3535, 0);
 glColor3f(255, 255, 0);
 glVertex3f(0, -.5, 0);  */


    GLuint vbo;
    glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(neu::vec3), points.data(), GL_STATIC_DRAW);

    GLuint vao;
	glGenVertexArrays(1, &vao);
	glEnableVertexArrayAttrib(vao, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //vertex shader
    std::string vs_source;
	neu::file::ReadTextFile("Shaders/basic.vert", vs_source);
	const char* vs_cstr = vs_source.c_str();

    GLuint vs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_cstr, NULL);
	glCompileShader(vs);

	//fragment shader

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

		float angle = neu::GetEngine().GetTime().GetTime()*100;
		float scale = (std::sinf(SDL_GetTicks() / 500.0f) + .4f) / 2.0f * .1f; // 0
		neu::vec2 mousePos = neu::GetEngine().GetInput().GetMousePosition();
        neu::vec2 position;
		position.x = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetWidth(), -1.0f, 1.0f, mousePos.x);
		position.y = neu::math::Remap(0.0f, (float)neu::GetEngine().GetRenderer().GetHeight(), 1.0f, -1.0f, mousePos.y);

        // draw
        neu::vec3 color{ 0, 0, 0 };
        neu::GetEngine().GetRenderer().SetColor(color.r, color.g, color.b);
        neu::GetEngine().GetRenderer().Clear();

 
		glLoadIdentity();
		glPushMatrix();

		glTranslatef(position.x, position.y, 0);
		glRotatef(angle, 0, 0, 1);
		glScalef(scale, scale, 1);

		glBegin(GL_TRIANGLES);
		//glBegin(GL_LINES);
		//glBegin(GL_POINTS);
		//glBegin(GL_LINE_LOOP);
		//glBegin(GL_TRIANGLE_STRIP);

        for (int i = 0; i < points.size(); i++) {
            glColor3f(colors[i].r, colors[i].g, colors[i].b);
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
        
		glPopMatrix();

        glEnd();

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
