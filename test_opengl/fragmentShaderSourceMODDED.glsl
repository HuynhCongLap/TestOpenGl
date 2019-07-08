#version 300 es
#undef lowp
#undef mediump
#undef highp

precision mediump float;

in highp vec3 vert;
in highp vec3 vertNormal;
in highp vec3 color;

out highp vec4 fragColor;

uniform highp vec3 lightPos;

highp float sphere(highp vec3 C, highp float R) // assuming origin is (0.0, 0.0, 0.0)
{
    highp vec3 D = normalize(vec3(1.0, 1.0, 1.0)); // should probably not stay as is...

    highp float DC = dot(D, C);
    highp float DD = dot(D, D);
    highp float CC = dot(C, C);

    highp float a = 1.0;
    highp float b = -2.0*DC;
    highp float c = CC - R*R;
    highp float d = b*b - 4.0*a*c;

    if (d < 0.0)
        return 1000.0;

    highp float t1 = (-b - sqrt(d))/(2.0*a);
    highp float t2 = (-b + sqrt(d))/(2.0*a);

    if (t1 >= 0.0 && t2 >= 0.0)
        return min(t1, t2);
    else if (t1 >= 0.0)
        return t1;
    else if (t2 >= 0.0)
        return t2;
    else
        return 1000.0;
}

void main() {
    // the layout of this function and the presence of
    // "float sphere(vec3, float)" above should be clue enough,
    // but feel free to change or replace anything

    highp vec3 light_dir = normalize(lightPos - vert);
    highp vec3 normal = vertNormal;

    if (true) {
        highp float light_dot = max(dot(normal, light_dir), 0.0);
        highp vec3 col = clamp(color*0.2 + color*0.8*light_dot, 0.0, 1.0);
        fragColor = vec4(col, 1.0);
    }
    else
        fragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
