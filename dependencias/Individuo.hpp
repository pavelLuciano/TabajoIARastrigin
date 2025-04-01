#ifndef _INDIVIDUO_HPP
#define _INDIVIDUO_HPP

#include <glm/glm.hpp>
#include <Rastrigin2D.hpp>
#include <vector>
#include <random>
#include <algorithm>

#define PART_VERT "./dependencias/shaders/particle_vert.glsl"
#define PART_FRAG "./dependencias/shaders/particle_frag.glsl"

class Individuo
{
public:
    static glm::vec2 gbest_pos;
    static float gbest;
    static int poblaciones_maximas;
    static int tamano_poblacion;
    static float rango_mutacion;
    static int k; //para los torneos 
    static float prob_mutacion;

    float fit;
    glm::vec2 pos;
    glm::vec3 color;

    Individuo(glm::vec2 _pos)
    {
        if (shader == nullptr) initShader();
        pos = _pos;
        fit = Rastrigin2D::rastrigin(pos.x, pos.y);
        color = glm::vec3(0.1f, 0.5f, fit);
    }

    ~Individuo()
    {
        //if(shader != nullptr) delete shader;
    }


    static Individuo* CruzamientoPonderado(Individuo* madre, Individuo* padre)
    {
        //promedia las posiciones de los padres
        return new Individuo((madre->pos + padre->pos)/2.0f);
    }
        
    static void Mutacion(Individuo* individuo)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dist(-rango_mutacion, rango_mutacion); // Rango de -a a a

        individuo->updatePos(individuo->pos + glm::vec2(dist(gen), dist(gen)));
    }


    //SELECCIONES
    static Individuo* RuletteWheel(std::vector<Individuo*>& poblacion)
    {
    
        std::vector<float> inv_fitness(poblacion.size());
        //float max_fit = *std::max_element(inv_fitness.begin(), inv_fitness.end());
        //float min_fit = *std::min_element(inv_fitness.begin(), inv_fitness.end());
        for (int i = 0; i < poblacion.size(); i++)
        {
            inv_fitness[i] = 1.0f/(poblacion[i]->fit + 1e-6);
            //inv_fitness[i] = (max_fit - poblacion[i]->fit) / (max_fit - min_fit + 1e-6);
            //epsilon para evitar divisiones por cero
        }
        std::random_device rd;
        std::mt19937 gen(rd()); // Generador de números aleatorios
        std::discrete_distribution<> dist(inv_fitness.begin(), inv_fitness.end()); // Distribución ponderada

        // Seleccionamos un índice según los pesos
        int indice = dist(gen);
        return poblacion[indice];
    }

    static Individuo* Torneo(std::vector<Individuo*>& poblacion)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(poblacion.begin(), poblacion.end(), gen);

        Individuo* ganador = poblacion[0];
        for(int i = 1; i < k; i++)
        {
            if (poblacion[i]->fit < ganador->fit) ganador = poblacion[i];
        }
        return ganador;
    }


    //REINSERCIONES
    static void Generacional(std::vector<Individuo*> &old_poblacion, std::vector<Individuo*> &new_poblacion)
    {
        for (int i = 0; i < old_poblacion.size(); i++)
            delete old_poblacion[i];
        old_poblacion = new_poblacion;
    };

    static void SteadyState(std::vector<Individuo*> &old_poblacion, std::vector<Individuo*> &new_poblacion)
    {

    }

    void updatePos(glm::vec2 newPos)
    {
        pos = newPos;
        fit = Rastrigin2D::rastrigin(pos.x, pos.y);
    }



/////////////////////////////////////////////////////////////
//IGNORAR
////////////////////////////////////////////////////////////
    static void lookUpAndSetBest(std::vector<Individuo*> pob)
    {
        for (auto ind: pob)
        {
            if (ind->fit < gbest)
            {
                gbest = ind->fit;
                gbest_pos = ind->pos;
            }
        }
    }

    static void drawBest()
    {
        shader->use();
        glUniform2f(xRangeLoc, x_min, x_max);
        glUniform2f(yRangeLoc, y_min, y_max);
        glUniform2f(posLoc, 0, 0);
        glUniform3f(colorLoc, 1.0, 1.0, 1.0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }


    static void setRadio(float r)
    {
        shader->use();
        radio = r;
        glUniform1f(radiusLoc, radio);
    }
    static void updateResolution(int width, int height)
    {
        std::cout << "Res: " << width << " " << height << "\n"<< std::endl;
        shader->use();
        glUniform2f(resolutionLoc, width, height);
    }
    static void setVAO(GLuint vao)
    {
        VAO = vao;
    }
    static void setXRange(float _x_min, float _x_max)
    {
        x_min =_x_min;
        x_max =_x_max;
    }
    
    static void setYRange(float _y_min, float _y_max)
    {
        y_min =_y_min;
        y_max =_y_max;
    }
    static void setBest(glm::vec2 pos)
    {
        gbest_pos = pos;
        gbest = Rastrigin2D::rastrigin(pos.x, pos.y);
    }

    static void initShader()
    {
        if (shader == nullptr) shader = new Shader(PART_VERT, PART_FRAG);
        xRangeLoc = glGetUniformLocation(shader->program_id, "xRange");
        yRangeLoc = glGetUniformLocation(shader->program_id, "yRange");
        resolutionLoc = glGetUniformLocation(shader->program_id, "resolution");
        radiusLoc = glGetUniformLocation(shader->program_id,"circleRadius");
        posLoc = glGetUniformLocation(shader->program_id,"circlePosition");
        colorLoc =  glGetUniformLocation(shader->program_id,"color");
        shader->use();
        setBest(glm::vec2(10000,10000));
        setRadio(0.015f);
    }

    void draw()
    {
        shader->use();
        glUniform2f(xRangeLoc, x_min, x_max);
        glUniform2f(yRangeLoc, y_min, y_max);
        glUniform2f(posLoc, pos.x, pos.y);
        glUniform3f(colorLoc, color.r, color.g, color.b);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
        std::cout << "deberia dibujar\r";
    }


    static Shader* shader;
private:
    
    static GLuint resolutionLoc;
    static GLuint posLoc;
    static GLuint radiusLoc;
    static GLuint colorLoc;
    static GLuint xRangeLoc;
    static GLuint yRangeLoc;
    static GLuint VAO;

    static float radio;
    static float x_min;
    static float x_max;
    static float y_min;
    static float y_max;
    

};


glm::vec2 Individuo::gbest_pos        = glm::vec2(0,0);
float  Individuo::gbest               = 0;
int    Individuo::poblaciones_maximas = 50;
int    Individuo::tamano_poblacion    = 100;
float  Individuo::rango_mutacion      = 0.1f;
float  Individuo::prob_mutacion       = 0.2f;

Shader* Individuo::shader = nullptr;
GLuint Individuo::resolutionLoc       = 0;
GLuint Individuo::posLoc              = 0;
GLuint Individuo::radiusLoc           = 0;
GLuint Individuo::colorLoc            = 0;
GLuint Individuo::xRangeLoc           = 0;
GLuint Individuo::yRangeLoc           = 0;
GLuint Individuo::VAO                 = 0;
float  Individuo::radio               = 0.015f;
float  Individuo::x_min               = -3.0f;
float  Individuo::x_max               = 7.0f;
float  Individuo::y_min               = -3.0f;
float  Individuo::y_max               = 7.0f;
int    Individuo::k                   = 5; //para los torneos 
#endif