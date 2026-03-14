#version 330
layout (location = 0) in vec3 polozenie;
layout (location = 1) in vec4 kolorPrzychodzacyVS;
uniform mat4 macierz;
out vec4 kolorWychodzacyVS;
void main()
{
    gl_Position = macierz * vec4(polozenie, 1.0);
    kolorWychodzacyVS = kolorPrzychodzacyVS;
}
