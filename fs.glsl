#version 460 core

uniform int itr;
uniform double zoom;
uniform dvec2 screenSize;
uniform dvec2 offset;

out vec4 fragColour;

double mandelbrot(dvec2 c){
    dvec2 z = vec2(0.0, 0.0);
    dvec2 temp;
    double n = 0.0;
    while (n < itr && z.x*z.x + z.y*z.y <= 4.0){ 
        temp.x = (z.x * z.x) - (z.y * z.y) + c.x;
        temp.y = (2.0 * z.x * z.y) + c.y;
        z = temp;
        n++;
    }
    return n/float(itr);
}

vec4 colourMap(float t){
    vec3 a = vec3(0.0,0.0,0.0);
    vec3 b = vec3(0.59,0.55,0.75);
    vec3 c = vec3(0.1,0.2,0.3);
    vec3 d = vec3(0.75,0.75,0.75);

    vec3 RGB = a + b*cos(6.28318*(c*t+d));
    
    return vec4(RGB, 1.0);
}

void main(){
    double t = mandelbrot(((gl_FragCoord.xy - screenSize / 2) / zoom) - offset);
    fragColour = colourMap(float(t));
}