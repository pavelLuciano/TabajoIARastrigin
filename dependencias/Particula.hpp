#ifndef _PARTICULA_H
#define _PARTICULA_H
#include <glm/glm.hpp>
#include <Rastrigin2D.hpp>
#include <shaders/Shader.h>

#define PART_VERT "./dependencias/shaders/particle_vert.glsl"
#define PART_FRAG "./dependencias/shaders/particle_frag.glsl"

class Particula
{
public:
    static glm::vec2 gbest_pos;
    static float gbest;
    static float radio;
    static float maxVel;
    static float inercia;
    static float C1;
    static float C2;
    static int evals;
    static int evals_to_best;


    float fit, pfit;
    glm::vec2 velocidad;
    glm::vec2 pos, pPos;
    glm::vec3 color;

    Particula(glm::vec2 _pos)
    {
        if (shader == nullptr) initShader();
        pos = _pos;
        velocidad = glm::normalize(glm::vec2((static_cast<float>(std::rand()) / (float)RAND_MAX) * 2.0f - 1.0f,
                                             (static_cast<float>(std::rand()) / (float)RAND_MAX) * 2.0f - 1.0f))
                    * (static_cast<float>(std::rand()) / RAND_MAX) * maxVel;
        fit = Rastrigin2D::rastrigin(pos.x, pos.y);
        pPos = glm::vec2(x_max + 1, y_max + 1);
        pfit = Rastrigin2D::rastrigin(pPos.x, pPos.y);
        color = glm::vec3(0.1f, 0.5f, pfit);
    }

    ~Particula()
    {
        //if(shader != nullptr) delete shader;
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
    }

    void mover(float time)
    {
        float ran1, ran2, ran3, ran4;
        ran1 = static_cast<float>(std::rand()) / RAND_MAX;
        ran2 = static_cast<float>(std::rand()) / RAND_MAX;
        ran3 = static_cast<float>(std::rand()) / RAND_MAX;
        ran4 = static_cast<float>(std::rand()) / RAND_MAX;

        
        velocidad = glm::vec2(
            (inercia * velocidad.x) + (ran1 * C1 * (pPos.x - pos.x)) + (ran2 * C2 * (gbest_pos.x - pos.x)),
            (inercia * velocidad.y) + (ran3 * C1 * (pPos.y - pos.y)) + (ran4 * C2 * (gbest_pos.y - pos.y))
        );
        
        if (glm::length(velocidad) > maxVel) velocidad = glm::normalize(velocidad) * maxVel;
        // 
        pos = pos + velocidad * 0.025f;

        if (pos.x > x_max || pos.x < x_min)
        {
            pos.x = (pos.x > x_max) ? x_max : x_min;
            velocidad *= glm::vec2(-1, 1);
        }
        
        if (pos.y > y_max || pos.y < y_min)
        {
            pos.y = (pos.y > y_max) ? y_max : y_min;
            velocidad *= glm::vec2(1, -1);
        }
        
    
    }

    float evaluar()
    {
        evals++;
        fit = Rastrigin2D::rastrigin(pos.x, pos.y);
        color = glm::vec3(0.5f, 0.1f, pfit/200);
        if (fit < pfit)
        {
            pfit = fit;
            pPos = pos;
        }
        if (fit < gbest)
        {
            gbest = fit;
            gbest_pos = pos;
            evals_to_best = evals;
        }
        return fit;
    }

    static void drawBest()
    {
        shader->use();
        glUniform2f(xRangeLoc, x_min, x_max);
        glUniform2f(yRangeLoc, y_min, y_max);
        glUniform2f(posLoc, gbest_pos.x, gbest_pos.y);
        glUniform3f(colorLoc, 1.0, 1.0, 1.0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

/////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
    static void setRadio(float r)
    {
        shader->use();
        radio = r;
        glUniform1f(radiusLoc, radio);
    }
    static void setVelocidad(float v)
    {
        maxVel = v;
    }
    static void setInercia(float w)
    {
        inercia = w;
    }
    static void setC1C2(float c1, float c2)
    {
        C1 = c1;
        C2 = c2;
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
        setVelocidad(10.0f);
        setInercia(50.0f);
        setC1C2(50.0f,50.0f);
        evals = evals_to_best = 0;
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

    static float x_min;
    static float x_max;
    static float y_min;
    static float y_max;
};

Shader* Particula::shader = nullptr;
GLuint  Particula::resolutionLoc = 0;
GLuint  Particula::posLoc        = 0;
GLuint  Particula::radiusLoc     = 0;
GLuint  Particula::colorLoc      = 0; 
GLuint  Particula::xRangeLoc     = 0;
GLuint  Particula::yRangeLoc     = 0;
GLuint  Particula::VAO           = 0;
int     Particula::evals         = 0;
int     Particula::evals_to_best = 0;

glm::vec2 Particula::gbest_pos = glm::vec2(0,0);
float Particula::gbest = 3.399e38;
float Particula::radio = 0.015f;
float Particula::maxVel = 0.1f;
float Particula::inercia = 50.0f;
float Particula::C1 = 50.0f;
float Particula::C2 = 50.0f;

float Particula::x_min = -3;
float Particula::x_max = 7;
float Particula::y_min = -3;
float Particula::y_max = 7;

#endif // _PARTICULA_H