int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();

    // initialize scene
     

    SDL_Event e;
    bool quit = false;


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

	std::vector<GLushort> indices{ 0, 1, 2, 3, 4, 5 };
	//std::vector<GLuint> indices{ 0, 1, 2, 2, 3, 0 };

    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/teapot.obj");

    //material
    auto material = neu::Resources().Get<neu::Material>("materials/spot.mat");
    material->Bind();
    
    

    //lights
    material->program->SetUniform("u_ambient_light", glm::vec3{0.5f});
	neu::Transform light{ { 0, .5, .5 } };
	glm::vec3 lightColor = { 1, 1, 1 }; 

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
			ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;


		transform.rotation.x += glm::radians(45.0f) * dt;

        material->program->SetUniform("u_model", transform.GetMatrix());

		//view matrix camera controls
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_SPACE)) camera.position.y += speed * dt;
		if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_LSHIFT)) camera.position.y -= speed * dt;
		glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
		material->program->SetUniform("u_view", view);

        material->program->SetUniform("u_light.color", lightColor);
		light.position.x = neu::math::sinf(neu::GetEngine().GetTime().GetTime()*2) * 5.0f;
        material->program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));


		float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
		glm::mat4 projection = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
        material->program->SetUniform("u_projection", projection);


        neu::GetEngine().GetRenderer().Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Info");
		ImGui::DragFloat3("Position", glm::value_ptr(light.position), 0.1f);
		ImGui::ColorEdit3("Color", glm::value_ptr(lightColor), 0.1f);
        light.UpdateGui();
        transform.UpdateGui();
        material->UpdateGui();
		ImGui::End();

		//vb->Draw(GL_TRIANGLES);
        model3d->Draw(GL_TRIANGLES);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
