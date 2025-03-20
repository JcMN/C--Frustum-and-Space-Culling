uniform vec3 u_lightDir;
uniform vec3 u_lightColor;
uniform sampler2D u_diffuseTex;

in vec4 v_normal;
in vec2 v_uv1;
out vec4 PixelColor;

void main()
{
    vec3 diffuseLight = u_lightColor * max(0, dot(v_normal.xyz, u_lightDir));
    vec3 diffuseMat = texture(u_diffuseTex, v_uv1).rgb;
    PixelColor.rgb = diffuseMat * diffuseLight;
    PixelColor.a = 1.0;
}


