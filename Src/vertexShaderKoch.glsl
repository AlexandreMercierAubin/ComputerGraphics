#version 430 core      
                    
layout(location = 0) in vec3 in_vertice; 
layout(location = 1) in vec3 in_color;
uniform mat4 matRotation;
uniform mat4 matScale;
uniform mat4 matTranslation;

out vec3 fragColor;

void main()
{

  gl_Position = matTranslation* matRotation* matScale* vec4(in_vertice, 1.0);
  fragColor= in_color;

}

