#version 430 core

in vec3 v_fragment_normal;
in vec3 v_fragment_position;

layout(location = 0) out vec4 o_color;

uniform vec3 u_view_position;

struct MaterialBasic {
    vec3 color;
};

uniform MaterialBasic u_material;

struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

uniform DirectionalLight u_directional_light;

vec3 phong(DirectionalLight directional_light, vec3 object_color, vec3 fragment_normal, vec3 fragment_position, vec3 view_position) {
    fragment_normal = normalize(fragment_normal);
    const vec3 light_direction = normalize(-directional_light.direction);

    const float ambient_strength = 0.1;
    const vec3 ambient_light = ambient_strength * directional_light.color;

    const float diffuse_strength = max(dot(fragment_normal, light_direction), 0.0);
    const vec3 diffuse_light = diffuse_strength * directional_light.color;

    const float specular_strength_ = 0.5;
    const float shininess_ = 32;
    const vec3 view_direction = normalize(view_position - fragment_position);
    const vec3 reflection_direction = reflect(-light_direction, fragment_normal);

    const float specular_strength = pow(max(dot(view_direction, reflection_direction), 0.0), shininess_);
    const vec3 specular_light = specular_strength * directional_light.color * specular_strength_;

    return (ambient_light + diffuse_light + specular_light) * object_color;
}

void main() {
    const vec3 color = phong(u_directional_light, u_material.color, v_fragment_normal, v_fragment_position, u_view_position);
    o_color = vec4(color, 1.0);
}
