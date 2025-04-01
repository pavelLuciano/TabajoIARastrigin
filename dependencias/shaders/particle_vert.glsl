#version 330 core
layout(location = 0) in vec2 aPos; // Coordenadas del vértice

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0); // Pasar las coordenadas directamente
}