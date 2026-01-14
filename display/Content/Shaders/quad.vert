
#version 330 core

uniform mat4 _Transform;
uniform vec3 _Color;

layout (location = 0) in vec3 _Position;
out vec4 _VertexColor;

void main(void) {    

    _VertexColor = vec4(_Color, 1.0);
    gl_Position =  _Transform * vec4(_Position, 1.0);
}
