uniform vec3 u_light1Dir;
uniform vec3 u_light1Color;
uniform vec3 u_light2Dir;
uniform vec3 u_light2Color;
uniform sampler2D tex;

in vec4 v_normal;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    vec3 n = normalize(v_normal.xyz);
    vec3 l1 = u_light1Color * max(0, dot(n, u_light1Dir));
    vec3 l2 = u_light2Color * max(0, dot(n, u_light2Dir));
    PixelColor.rgb = l1 + l2;
    PixelColor.a = 1.0;
}


