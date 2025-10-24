#include "Resources/Program.h"

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
    /*
    std::vector<neu::vec3> points{ { 0, 0.5f, 0 }, { 0.3535f, -0.3535f, 0 }, { -0.3535f, -0.3535f, 0 }, { 0, -0.5f, 0 }, { 0.3535f, 0.3535f, 0 }, { -0.3535f, 0.3535f, 0 } };
    std::vector<neu::vec3> colors{ { 0, 0, 1 }, { 1, 0, 0 }, { 0, 1, 0 }, { 1, 1, 0 }, { 1, 0, 1 }, { 0, 1, 1 } };
	std::vector<neu::vec2> texcoords{ { 0.5f, 1 }, { 1, 0 }, { 0, 0 }, { 0.5f, 0 }, { 1, 1 }, { 0, 1 } };
    */

	//interleaved vertex data
    struct Vertex {
        neu::vec3 position;
		neu::vec3 color;
        neu::vec2 texcoord;
	};

    
    std::vector<Vertex> vertices{
        { { 0, 0.5f, 0 },            { 0, 0, 1 }, { 0.5f, 1 } },
        { { 0.3535f, -0.3535f, 0 },  { 1, 0, 0 }, { 1, (.5 - .3535f) } },
        { { -0.3535f, -0.3535f, 0 }, { 0, 1, 0 }, { 0, (.5 - .3535f) } },
        { { 0, -0.5f, 0 },           { 1, 1, 0 }, { 0.5f, 0 } },
        { { 0.3535f, 0.3535f, 0 },   { 1, 0, 1 }, { 1, .8535f}},
        { { -0.3535f, 0.3535f, 0 },  { 0, 1, 1 }, { 0, .8535f } }
    };
    

    /* square
    std::vector<Vertex> vertices{
        { {-0.5f, 0.5f, 0},  {1, 0, 0}, {0, 1} },
        { {0.5f, 0.5f, 0},   {0, 1, 0}, {1, 1} },
        { {0.5f, -0.5f, 0},  {0, 0, 1}, {1, 0} },
        { {-0.5f, -0.5f, 0}, {1, 1, 0}, {0, 0} }
	};
    */

	std::vector<GLushort> indices{ 0, 1, 2, 3, 4, 5 };
	//std::vector<GLuint> indices{ 0, 1, 2, 2, 3, 0 };

	//vertex buffer
	//neu::res_t<neu::VertexBuffer> vb = std::make_shared<neu::VertexBuffer>();
	//vb->CreateVertexBuffer((GLsizei)(vertices.size() * sizeof(Vertex)), (GLsizei)vertices.size(), vertices.data());
	//vb->CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), indices.data());
	//vb->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
	//vb->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));
	//vb->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texcoord));

    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/torus.obj");
    

    /*
    GLuint vbo[3];
    glGenBuffers(3, vbo);

	//vertex buffer (position)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(neu::vec3), points.data(), GL_STATIC_DRAW);

	//vertex buffer (color)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(neu::vec3), colors.data(), GL_STATIC_DRAW);

	//vertex buffer (texcoord)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(neu::vec2), texcoords.data(), GL_STATIC_DRAW);
    

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

	// texcoord
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
    */

    /*
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
    */
    
    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);
    

	auto program = neu::Resources().Get<neu::Program>("programs/basiclit.prog");
    program->Use();

    /*
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
    */

    //texture
	//neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/beast.png");
	neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("Textures/vacationmemories.png");

    
    //uniform
	//program->SetUniform("u_time", 0.0f);
	//GLint uniform = glGetUniformLocation(program->m_program, "u_time");
	//ASSERT(uniform != -1);

    program->SetUniform("u_texture", 0);

    //lights
    program->SetUniform("u_ambient_light", glm::vec3{0.5f});
	neu::Transform light{ { 0, .5, .5 } };

    glm::mat4 model = glm::mat4(1.0f);

    float v = model[3][3];
    glm::vec3 eye = { 0, 0, 6 };

    neu::Transform transform{ { 0,0,0 } };
    neu::Transform camera{ {0,0,5} };



    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        //model matrix
		/*glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        program->SetUniform("u_model", model);*/

		transform.rotation.x += glm::radians(45.0f) * dt;

		program->SetUniform("u_model", transform.GetMatrix());

		//view matrix
		/*eye.x = neu::GetEngine().GetInput().GetMousePosition().x / 1000.0f;
		eye.y = neu::GetEngine().GetInput().GetMousePosition().y / 1000.0f;
		glm::mat4 view = glm::lookAt(eye, eye + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		program->SetUniform("u_view", view);*/

		//view matrix camera controls
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_LSHIFT)) camera.position.y -= speed * dt;
		glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		program->SetUniform("u_view", view);

		program->SetUniform("u_light.color", glm::vec3{1, 1, 1});
		light.position.x = neu::math::sinf(neu::GetEngine().GetTime().GetTime()*2) * 5.0f;
		program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));


		float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
		program->SetUniform("u_projection", projection);

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

		//vb->Draw(GL_TRIANGLES);
        model3d->Draw(GL_TRIANGLES);


        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
