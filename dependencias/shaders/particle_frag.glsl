#version 330 core
out vec4 FragColor;


uniform vec3 color; //color del circulo 
uniform vec2 circlePosition; // Posición del circulo
uniform float circleRadius; // Radio común para todos los círculos

uniform vec2 resolution; // Resolución de la ventana
uniform vec2 xRange;
uniform vec2 yRange;


void main()
{
    vec2 min = vec2(xRange.x, yRange.x);
    vec2 max = vec2(yRange.y, xRange.y);
    vec2 fragCirclePosition = (circlePosition - min)/(max - min);
    vec2 uv = gl_FragCoord.xy / resolution; 
    float dist = length(uv - fragCirclePosition);

    // Dibujar el círculo si el píxel está dentro del radio
    if (dist < circleRadius) {
        if (dist < circleRadius - 0.0025) 
            FragColor = vec4(color, 1.0);
        else FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    } else {
        discard; // Descartar el píxel si no está dentro del círculo
    }
}