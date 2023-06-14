#version 330 core
out vec4 frag_colour;
in vec2 position;
in vec3 out_color;

uniform vec2 center;
uniform float radius;
uniform float line;
uniform vec3 fill_color;

void main() {
	float distanceX = abs(position.x - center.x);
    float distanceY = abs(position.y - center.y);

    float d = sqrt(distanceX * distanceX + distanceY * distanceY);
    if(d <= radius)
    {
        float t1 = 1.0 - smoothstep(radius - line, radius, d);
        float t2 = 1.0 - smoothstep(radius, radius + line, d);
        frag_colour = vec4(mix(out_color.rgb, fill_color.rgb, t1), t2);
        //frag_colour = vec4(fill_color, 1.0f);
    }
    else
    {
        discard;
    }
};