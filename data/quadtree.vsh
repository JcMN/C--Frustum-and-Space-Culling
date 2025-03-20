uniform mat4 proj;
uniform mat4 view;

in vec4 a_position;


void main(){
    gl_Position = proj * view * a_position;
}