#version 330 core


//for deciding which part of material will have ambient, diffuse, specular or shininess
//this is more like a color part
struct Material{
    vec4 diffuse;
    vec4 specular;
    float shininess;

};
uniform sampler2D diffuse0;
uniform sampler2D specular0;


struct DirLight{
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

//strength of the light
#define MAX_POINT_LIGHTS 20

struct PointLight{
    vec3 position;

    //attenuation constatnt (to fix the strength of the light)
    float k0;
    float k1;
    float k2;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform PointLight pointLight[MAX_POINT_LIGHTS];
uniform int noPointLights;

#define MAX_SPOT_LIGHTS 5
struct SpotLight{
    
    vec3 position;
    vec3 direction;

    //attenuation constatnt (to fix the strength of the light)
    float k0;
    float k1;
    float k2;
    
    float cutOff;
    float outerCutOff;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};
uniform SpotLight spotLight[MAX_SPOT_LIGHTS];
uniform int noSpotLights;

uniform DirLight dirLight;

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

//in vec3 ourColor;
in vec2 TexCoord;

uniform Material material;
uniform vec3 viewPos;
uniform int noTex;
//uniform sampler2D texture1; //it bring the sample coordinate/id of the texture
//uniform sampler2D texture2;

vec4 calcPointLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);
vec4 calcSpotLight(int idx, vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap);


void main(){
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec4 diffMap ;
    vec4 specMap ;

    if(noTex == 1){
        diffMap = material.diffuse;
        specMap = material.specular;
    }else{
        diffMap = texture(diffuse0, TexCoord);
        specMap = texture(specular0, TexCoord);
    }

    vec4 result;

    //directional

    result = calcDirLight(norm, viewDir, diffMap, specMap);

    //PointLight

    for(int i=0; i<noPointLights; i++){
        result += calcPointLight(i, norm, viewDir, diffMap, specMap);
    }

    // SpotLight
    for(int i=0; i<noSpotLights; i++){
        result += calcSpotLight(i, norm, viewDir, diffMap, specMap);
    }
    FragColor = result; 
}

vec4 calcPointLight(int idx,vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap){
    //ambient(constant)
    vec4 ambient = pointLight[idx].ambient * diffMap;

    //diffuse(here we are more towards the material part)
    //based on fragment postion
    vec3 lightDir = normalize(pointLight[idx].position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = pointLight[idx].diffuse * (diff * diffMap);

    //specular(it more of how viewer part)
    //based on the position of the viewer
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*128);
    vec4 specular = pointLight[idx].specular * (spec * specMap);

    //calc the attenuation constant

    float dist = length(pointLight[idx].position - FragPos);
    float attenuation = 1.0 / (pointLight[idx].k0 + pointLight[idx].k1 * dist + pointLight[idx].k2*(dist * dist));


    return vec4(ambient + diffuse + specular) * attenuation;
}

vec4 calcDirLight(vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap){
    //ambient(constant)
    vec4 ambient = dirLight.ambient * diffMap;

    //diffuse(here we are more towards the material part)
    //based on fragment postion
    vec3 lightDir = normalize(-dirLight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = dirLight.diffuse * (diff * diffMap);

    //specular(it more of how viewer part)
    //based on the position of the viewer
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*128);
    vec4 specular = dirLight.specular * (spec * specMap);

    return vec4(ambient + diffuse + specular);
}

vec4 calcSpotLight(int idx,vec3 norm, vec3 viewDir, vec4 diffMap, vec4 specMap){
    vec3 lightDir = normalize(spotLight[idx].position - FragPos);
    float theta = dot(lightDir, normalize(-spotLight[idx].direction));
    //ambient
    vec4 ambient = spotLight[idx].ambient * diffMap;

    if(theta > spotLight[idx].outerCutOff){ // > because using cosines and not degrees
        // if in cutoff, render light

        //diffuse
       float diff = max(dot(norm, lightDir), 0.0);
       vec4 diffuse = spotLight[idx].diffuse * (diff * diffMap);  

       //specular
       vec3 reflectDir = reflect(-lightDir, norm);
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess*128);
       vec4 specular = spotLight[idx].specular * (spec * specMap);

       float intensity = (theta - spotLight[idx].outerCutOff)/(spotLight[idx].cutOff - spotLight[idx].outerCutOff);
       intensity = clamp(intensity, 0.0, 1.0);
       diffuse *= intensity;
       specular *= intensity;

       float dist = length(spotLight[idx].position - FragPos);
       float attenuation = 1.0 / (spotLight[idx].k0 + spotLight[idx].k1 * dist + spotLight[idx].k2*(dist * dist));

       return vec4(ambient + diffuse + specular) * attenuation;
    }else{
        //render just ambient
        return ambient;
    }
}