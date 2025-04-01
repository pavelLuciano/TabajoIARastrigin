#define AE_RASTRIGIN_
#include <panconqueso.h>
#include <functional>

int main()
{
    ventana = initDepsAndCreateWin(WIDTH, HEIGHT);
    float x_range[2] = {-3, 7};
    float y_range[2] = {-3, 7};
    
    std::function<Individuo*(std::vector<Individuo*>&)> seleccion = Individuo::RuletteWheel;
    std::function<void(std::vector<Individuo*>&, std::vector<Individuo*>&)> reinsercion = Individuo::Generacional;
    bool ruleta = true;

    Individuo::setBest(glm::vec2(10000,10000));
    std::vector<Individuo*>poblacion(Individuo::tamano_poblacion);
    for (int i = 0; i < poblacion.size(); i++)
    {
        float randomValueX = static_cast<float>(rand()) / RAND_MAX;
        float randomValueY = static_cast<float>(rand()) / RAND_MAX;
        glm::vec2 pos(
            x_range[0] + randomValueX * (x_range[1] - x_range[0]),
            y_range[0] + randomValueY * (y_range[1] - y_range[0])
        );
        poblacion[i] = new Individuo(pos);
    }



    flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
    float mutacion = Individuo::rango_mutacion;
    int num_generaciones = Individuo::poblaciones_maximas;
    int tam_pob = Individuo::tamano_poblacion;
    bool evalua = false;
    float prob_mut = Individuo::prob_mutacion;
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
        static int generacion = 0;
        if (ImGui::Begin("AE", NULL, flags))
        {
            ImGui::SeparatorText("Rangos de la funcion Rastring");
            ImGui::InputFloat2("X", x_range);
            ImGui::InputFloat2("Y", y_range);
            Rastrigin->setXRange(x_range[0], x_range[1]);
            Rastrigin->setYRange(y_range[0], y_range[1]);
            Individuo::setXRange(x_range[0], x_range[1]);
            Individuo::setYRange(y_range[0], y_range[1]);
            ////////////////////////////////////////
            ImGui::SeparatorText("Prueba Actual");
            ImGui::Text("Generacion atual:      %i", generacion);
            ImGui::Text("Generaciones Totales:  %i", Individuo::poblaciones_maximas);
            ImGui::Text("Tamaño Poblacion:      %i", Individuo::tamano_poblacion);
            ImGui::Text("Mutacion:              %f", Individuo::rango_mutacion);
            ImGui::Text("Probabilidad Mutacion: %f", Individuo::prob_mutacion);
            ImGui::Text("Seleccion:           "); ImGui::SameLine();
            if (ruleta) ImGui::Text("Ruleta");
            else {ImGui::Text("Torneo"); ImGui::Text("K = %i", Individuo::k);}
            ImGui::Text("GBest:                 %f", Individuo::gbest);
            if (ImGui::Button("Sig Generacion")) evalua = true;
            ////////////////////////////////////////
            ImGui::SeparatorText("Nueva Prueba");
            static float mutacion_nueva = 0.1f;
            static int tam_pob_nueva = 100;
            static int num_generaciones_nueva = 100;
            static int k_nueva = 5;
            ImGui::InputFloat("Mutacion", &mutacion_nueva);
            ImGui::InputInt("Tamaño Poblacion", &tam_pob_nueva);
            ImGui::Text("Probabilidad de Mutacion");
            ImGui::SliderFloat(" ", &prob_mut, 0.0f, 1.0f, "prob = %.2f");
            ImGui::InputInt("Generaciones", &num_generaciones_nueva);
            ImGui::Text("SELECCION:");
            static const char* opciones[] = {"Ruleta","Torneo"};
            static int selected_sel = 0;
            if (ImGui::Selectable(opciones[0], selected_sel == 0)) selected_sel = 0;
            if (ImGui::Selectable(opciones[1], selected_sel == 1)) selected_sel = 1;
            if (selected_sel == 1) ImGui::InputInt("K", &k_nueva);
            if (ImGui::Button("Empezar Prueba"))
            {
                //setea las nuevas variables
                Individuo::poblaciones_maximas = num_generaciones_nueva;
                Individuo::tamano_poblacion = tam_pob_nueva;
                Individuo::rango_mutacion = mutacion_nueva;
                Individuo::k = k_nueva;
                Individuo::prob_mutacion = prob_mut;
                Individuo::setBest(glm::vec2(10000,10000));
                if (selected_sel == 0){ seleccion = Individuo::RuletteWheel; ruleta = true;}
                else {seleccion = Individuo::Torneo; ruleta = false;}
                //reemplaza la poblacion
                for (int i = 0; i < poblacion.size(); i++) delete poblacion[i];
                poblacion.resize(Individuo::tamano_poblacion);
                for (int i = 0; i < poblacion.size(); i++)
                {
                    float randomValueX = static_cast<float>(rand()) / RAND_MAX;
                    float randomValueY = static_cast<float>(rand()) / RAND_MAX;
                    glm::vec2 pos(
                        x_range[0] + randomValueX * (x_range[1] - x_range[0]),
                        y_range[0] + randomValueY * (y_range[1] - y_range[0])
                    );
                    poblacion[i] = new Individuo(pos);
                }
                evalua = false;
                generacion = 0;
            }

        }
        ImGui::End();

        
        Rastrigin->draw();
        for (auto ind: poblacion) ind->draw();
        //mejor teorico
        Individuo::drawBest();

        //ALGORITMO EVOLUTIVO
        if (evalua && generacion <= Individuo::poblaciones_maximas)
        {

            std::vector <Individuo*> nueva_poblacion(poblacion.size());
            for(int i = 0; i < nueva_poblacion.size(); i++)
            {
                //selecciona a los padres
                Individuo * padre1 = seleccion(poblacion);
                Individuo * padre2 = seleccion(poblacion);

                nueva_poblacion[i] = Individuo::CruzamientoPonderado(padre1, padre2);
                float dice = static_cast<float>(std::rand()) / RAND_MAX;
                if (dice < Individuo::prob_mutacion)
                    Individuo::Mutacion(nueva_poblacion[i]);
            }
            reinsercion(poblacion, nueva_poblacion);
            Individuo::lookUpAndSetBest(poblacion);
            evalua = false;
            generacion++;
        }

        


        

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(ventana);
    }
    delete Particula::shader;
    delete Rastrigin;

}