uniform mat4 world;
uniform mat4 view;
uniform mat4 proj;

in vec4 a_position;

void main(){
    gl_Position = proj * view * world * a_position; 
}