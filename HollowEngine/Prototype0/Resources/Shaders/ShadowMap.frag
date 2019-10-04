#version 330 core
out vec4 FragColor;

in vec4 position;

void main()
{
   FragColor = position;
   //FragColor = vec4(position.w/50,position.w/50,position.w/50,1.0);
}