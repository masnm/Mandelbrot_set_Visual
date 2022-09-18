#version 330 core

in vec4 gl_FragCoord;
in vec4 ourColor;

out vec4 FragColor;
in float ftime;
in vec2 fresolution;
in vec2 fuv;

void main()
{
	float x0 = fuv.x;
	float y0 = fuv.y;
	float x = 0.0;
	float y = 0.0;
	float iteration = 0;
	float max_iteration = 2048;
	float xtemp;
	while ( x*x + y*y <= 4 && iteration < max_iteration ) {
		xtemp = x*x - y*y + x0;
		y = 2*x*y + y0;
		x = xtemp;
		iteration = iteration + 1;
	}
	float mcol = iteration / (max_iteration + 1);
	float a = 0.1f;
	float n = iteration;
	FragColor = vec4 (
		0.5f * sin(a * n) + 0.5f,
		0.5f * sin(a * n + 2.094f) + 0.5f,
		0.5f * sin(a * n + 4.188f) + 0.5f,
		1.0);
}
