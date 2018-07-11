#version 450 core
out vec4 color;

in vec3 FragPos;  
in vec3 Normal;
in vec3 MyColor;
in vec2 TexturePos;
  
void main()
{

	//color = vec4(ambient + diffuse + specular, 1.0f) * vec4(MyColor, 1.0f);
	color = vec4(MyColor, 1.0f);


} 

