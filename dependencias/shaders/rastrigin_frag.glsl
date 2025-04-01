#version 460 core
out vec4 FragColor;

uniform vec2 resolution; // Resolución de la ventana
uniform vec2 xRange;
uniform vec2 yRange;

// Función Rastrigin en 2D
float rastrigin(vec2 pos) {
    float pi = 3.141592653589793;
    float A = 10.0;
    return 2 * A + (pos.x * pos.x - A * cos(2.0 * pi * pos.x)) +
                     (pos.y * pos.y - A * cos(2.0 * pi * pos.y));
}

void main() {
    // Normalizar las coordenadas del píxel al rango 
    //vec2 resolution_ = vec2(600,600);
    //vec2 xRange_ = vec2(-3,7);
    //vec2 yRange_ = vec2(-3,7);
    vec2 uv = vec2(
        (gl_FragCoord.x / resolution.x)*(xRange.y - xRange.x) + xRange.x,
        (gl_FragCoord.y / resolution.y)*(yRange.y - yRange.x) + yRange.x
    );

    // Calcular el valor de la función Rastrigin
    float value = rastrigin(uv);
    value /= 200;
    vec3 color = vec3(0.5, 0.1 ,value);
    // Asignar el color al píxel
    FragColor = vec4(color, 1.0);
}