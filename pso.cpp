#define PSO_RASTRIGIN_
#include <panconqueso.h>

int main(int argc, char **argv)
{
    ventana = initDepsAndCreateWin(WIDTH, HEIGHT);
    //Limites de la funcion rastrigin
    float x_range[2] = {-3, 7};
    float y_range[2] = {-3, 7};
    int c1_c2[2] = {50,10};
    float inercia = 1500;
    int num_particulas = 10;
    std::vector<Particula*> particulas(num_particulas);
    for (int i = 0; i < num_particulas; i++)
    {
        float randomValueX = static_cast<float>(rand()) / RAND_MAX;
        float randomValueY = static_cast<float>(rand()) / RAND_MAX;
        glm::vec2 pos(
            x_range[0] + randomValueX * (x_range[1] - x_range[0]),
            y_range[0] + randomValueY * (y_range[1] - y_range[0])
        );
        particulas[i] = new Particula(pos);
    }


    bool isStopped = false;
    flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    while(!glfwWindowShouldClose(ventana))
    {
        glfwPollEvents();
        if (glfwGetWindowAttrib(ventana, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }
        newFrame();
        int win_wid, win_hei;
        glfwGetWindowSize(ventana, &win_wid, &win_hei);
        ImVec2 pos(win_hei, 0);
        ImVec2 size(win_wid - win_hei, win_hei);
        ImGui::SetNextWindowPos(pos);
        ImGui::SetNextWindowSize(size);
        if (ImGui::Begin("PSO", NULL, flags))
        {
            ImGui::SeparatorText("Rangos de la funcion Rastring");
            ImGui::InputFloat2("X", x_range);
            ImGui::InputFloat2("Y", y_range);
            ImGui::SeparatorText("Parametros PSO");
            ImGui::InputInt2("C1 & C2", c1_c2);
            ImGui::InputFloat("Inercia", &inercia);


            Rastrigin->setXRange(x_range[0], x_range[1]);
            Rastrigin->setYRange(y_range[0], y_range[1]);
            Particula::setXRange(x_range[0], x_range[1]);
            Particula::setYRange(y_range[0], y_range[1]);
            Particula::setC1C2(c1_c2[0], c1_c2[1]);
            Particula::setInercia(inercia);

            ImGui::SeparatorText("Best");
            ImGui::Text("Best Pos:    %f, %f", Particula::gbest_pos.x, Particula::gbest_pos.y);
            ImGui::Text("Best:        %f", Particula::gbest);
            ImGui::Text("Evals:       %d", Particula::evals);
            ImGui::Text("EvalsToBest: %d", Particula::evals_to_best);

            ImGui::SeparatorText("Test");
            ImGui::InputInt("Num particulas", &num_particulas); 
            if (ImGui::Button("Stop")) isStopped = true;
            ImGui::SameLine();
            if (ImGui::Button("Restart"))
            {
                particulas.resize(num_particulas);
                for (int i = 0; i < num_particulas; i++)
                {
                    float randomValueX = static_cast<float>(rand()) / RAND_MAX;
                    float randomValueY = static_cast<float>(rand()) / RAND_MAX;
                    glm::vec2 pos(
                        x_range[0] + randomValueX * (x_range[1] - x_range[0]),
                        y_range[0] + randomValueY * (y_range[1] - y_range[0])
                    );
                    if (particulas[i] != nullptr) delete particulas[i];
                    particulas[i] = new Particula(pos);
                }
                Particula::setBest(glm::vec2(10000,10000));
                Particula::evals = Particula::evals_to_best = 0;
                isStopped = false;

            }

        }
        ImGui::End();
        //ImGui::ShowDemoWindow();

        static float intervalo_eval = 0.0f;
        intervalo_eval += delta_time;
        if (intervalo_eval >= 0.075f)
        {
            if (!isStopped)
            {
                for(auto particula : particulas)
                {
                    particula->evaluar();
                    particula->mover(delta_time);
                }
            }
            intervalo_eval = 0.0f;
        }
        

        Rastrigin->draw();
        
        for (auto particula : particulas) particula->draw();
        Particula::drawBest();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(ventana);
    }
    glfwTerminate();
    std::cout << "Terminado" << std::endl;
    for (auto particula : particulas) delete particula;
    delete Particula::shader;
    delete Rastrigin;
}