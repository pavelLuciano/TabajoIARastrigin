#version 460 core
layout(location = 0) in vec2 aPos; // Coordenadas del vértice

uniform vec2 resolution; // Resolución de la ventana
uniform vec2 xRange;
uniform vec2 yRange;

void main() {
    gl_Position = vec4(aPos, 0.0, 1.0); // Pasar las coordenadas sin cambios
}