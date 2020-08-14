#version 460 core

const int MAX_LIGHTS = 10;

struct Material {
  sampler2D texture_diffuse1;
  sampler2D texture_specular1;
  float shininess;
};

struct Light {
  vec3 position;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  vec3 attenuation;

  bool isDirectional;
  bool isSpotlight;
  vec3 spotlightDirection;
  float cut_off;
  float outer_cut_off;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform int nbLights;
uniform Light lights[MAX_LIGHTS];

vec3 calculateLighting(Light light, Material material, vec3 norm,
                       vec3 viewDir) {
  vec3 ambient =
      light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));

  vec3 lightDir;
  float attenuation = 1.0;
  float intensity = 1.0;

  if (light.isDirectional) {
    lightDir = normalize(-light.position);
  } else // positional
  {
    lightDir = normalize(light.position - FragPos);

    float dist = length(light.position - FragPos);
    attenuation = 1.0 / (light.attenuation.x + light.attenuation.y * dist +
                         light.attenuation.z * (dist * dist));
    if (light.isSpotlight) {
      float theta = dot(lightDir, normalize(-light.spotlightDirection));
      float epsilon = light.cut_off - light.outer_cut_off;
      intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    }
  }

  vec3 diffuse = light.diffuse * max(dot(norm, lightDir), 0.0) *
                 vec3(texture(material.texture_diffuse1, TexCoord));

  vec3 reflectDir = reflect(-lightDir, norm);
  float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = light.specular * specFactor *
                  vec3(texture(material.texture_specular1, TexCoord));

  return (ambient + (diffuse + specular) * intensity) * attenuation;
}

void main() {
  vec3 norm = normalize(Normal);
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 lighting = {0.0, 0.0, 0.0};
  for (int i = 0; i < min(nbLights, MAX_LIGHTS); i++) {
    lighting += calculateLighting(lights[i], material, norm, viewDir);
  }
  FragColor = vec4(lighting, 1.0);
}