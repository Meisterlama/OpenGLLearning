#version 460 core

const uint MAX_LIGHTS = 10;

out vec4 fragment_color;

uniform sampler2D texture_diffuse_1;

struct Light {
  vec4 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 attenuation;

  vec3 spotlight_direction;
  float cut_off;
  float outer_cut_off;
};

in vec3 position;
in vec3 normal;
in vec2 texture_coords;

uniform vec3 cameraPos;
uniform int nb_lights;
uniform Light lights[MAX_LIGHTS];

vec3 calculateLight(Light light, vec3 color, vec3 v_normal, vec3 v_view) {
  // todo normal matrix

  const vec3 ambient = light.ambient * color;

  vec3 v_light;
  float attenuation = 1.0;
  float intensity = 1.0; // spotlight

  if (light.position.w == 0.0) // directional light (sun)
  {
    v_light = normalize(-light.position.xyz);
  } else // positional light
  {
    v_light = normalize(light.position.xyz - position);

    const float dist = length(light.position.xyz - position);
    attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist +
                         light.attenuation.z * (dist * dist));

    if (light.cut_off != 0.0) {
      const float theta = dot(v_light, normalize(-light.spotlight_direction));
      const float epsilon = light.cut_off - light.outer_cut_off;
      intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    }
  }

  const vec3 diffuse =
      (light.diffuse * max(dot(v_normal, v_light), 0.0)) * color;

  // specular
  const vec3 v_reflect = reflect(-v_light, v_normal);
  const float specular_factor = pow(max(dot(v_view, v_reflect), 0.0), 32);
  const vec3 specular = light.specular * specular_factor *
                        texture(texture_diffuse_1, texture_coords).rgb;
  // todo replace texture by spec

  return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main() {

  const vec3 color = texture(texture_diffuse_1, texture_coords).rgb;
  const vec3 v_normal = normalize(normal);
  const vec3 v_view = normalize(cameraPos - position);

  vec3 result = {0.0, 0.0, 0.0};

  for (int i = 0; i < min(nb_lights, MAX_LIGHTS); i++) {

    result += calculateLight(lights[i], color, v_normal, v_view);
  }

  fragment_color = vec4(result, 1.0);
}