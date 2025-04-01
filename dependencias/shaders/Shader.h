#ifndef _SHADER_H
#define _SHADER_H

#include <GL/glew.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>


#define NO_COMPILED_PROGRAM -1

class Shader
{
public:
    unsigned int program_id = NO_COMPILED_PROGRAM;

    //constructores
    Shader(const char*, const char*);
    //destructor
    ~Shader();
    //metodos
    //getters
    unsigned int getID() const;
    void use() const;
    bool isCompiled();

private:
    bool compileShaderProgram(const char*, const char*);
    unsigned int compileShader(unsigned int, std::string&);
    void checkCompileErrors(unsigned int, int) const;
    
};

#endif