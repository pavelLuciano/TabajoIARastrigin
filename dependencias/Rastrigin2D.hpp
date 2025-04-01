#ifndef _RASTRIGIN_2D_HPP
#define _RASTRIGIN_2D_HPP

#include <shaders/Shader.h>

#define RASTRIGIN_VERT "./dependencias/shaders/rastrigin_vert.glsl"
#define RASTRIGIN_FRAG "./dependencias/shaders/rastrigin_frag.glsl"

//diseño de singleton
class Rastrigin2D
{
private:
    Shader* shader;
    GLuint VAO, VBO;
    float x_min;
    float x_max;
    float y_min;
    float y_max;
    GLuint xRangeLoc;
    GLuint yRangeLoc;
    GLuint resolutionLoc;

public:
    Rastrigin2D()
    {
        shader = new Shader(RASTRIGIN_VERT, RASTRIGIN_FRAG);
        
        xRangeLoc = glGetUniformLocation(shader->program_id, "xRange");
        yRangeLoc = glGetUniformLocation(shader->program_id, "yRange");
        resolutionLoc = glGetUniformLocation(shader->program_id, "resolution");
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        // Definir los vértices de un cuadro que cubre todo el viewport
        float vertices[] = {
            -1.0f, -1.0f, // Esquina inferior izquierda
            1.0f, -1.0f, // Esquina inferior derecha
            -1.0f,  1.0f, // Esquina superior izquierda
            1.0f,  1.0f  // Esquina superior derecha
        };
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Especificar el formato de los vértices
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        x_min = y_min = -3.0f;
        x_max = y_max = 7.0f;
    };

    ~Rastrigin2D() {
        //Liberar recursos de OpenGL
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        delete shader;
    }


    void draw()
    {
        shader->use();
        glUniform2f(xRangeLoc, x_min, x_max);
        glUniform2f(yRangeLoc, y_min, y_max);
        //std::cout << "Range: " << x_min << " " << x_max << " , " << y_min << " " << y_max << "\r";

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

    }

    void setXRange(float _x_min, float _x_max)
    {
        x_min =_x_min;
        x_max =_x_max;
    }
    
    void setYRange(float _y_min, float _y_max)
    {
        y_min =_y_min;
        y_max =_y_max;
    }
    void updateResolution(int width, int height)
    {
        std::cout << "Res: " << width << " " << height << "\n"<< std::endl;
        shader->use();
        glUniform2f(resolutionLoc, width, height);
    }


    static float rastrigin(double x, double y)
    {
        const double A = 10.0;
        return 2 * A + (x * x - A * std::cos(2 * M_PI * x)) + (y * y - A * std::cos(2 * M_PI * y));
    }
    GLuint getVAO() {return VAO;}
};

#endif //_RASTRIGIN_2D_HPP