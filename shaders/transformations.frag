#version 450 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;
in vec3 MyColor;
in vec2 TexturePos;
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec3 ka;
uniform vec3 ks;
uniform vec3 kd;

uniform int useMaterial;
uniform int useTexture;


// Texture sampler
uniform sampler2D ourTexture1;

void main()
{
    // Ambient
    float ambientStrength = 0.1f;

	vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);

	vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);

	vec3 specular = specularStrength * spec * lightColor;
		
	vec4 textura = texture(ourTexture1, TexturePos);
	vec4 textureAndColor = mix(textura, vec4(MyColor, 1.0), 0.5);
	color = vec4(ambient + diffuse + specular, 1.0f) * textura;
	
	//color = vec4(ambient + diffuse + specular, 1.0f) * vec4(MyColor, 1.0f);

	


} 

