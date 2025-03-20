uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform vec3 u_matDiffuse;

in vec4 v_normal;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    vec3 diffuseLight = u_lightColor * max(0, dot(v_normal.xyz, u_lightDir));
    vec3 diffuse = u_matDiffuse * diffuseLight;
    PixelColor.rgb = diffuse;
    PixelColor.a = 1.0;
}


