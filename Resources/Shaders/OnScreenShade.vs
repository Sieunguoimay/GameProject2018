attribute vec3 a_posL;

attribute vec2 a_texcoord;

varying vec2 v_texcoord;

uniform mat4 u_model;
uniform mat4 u_proj;
uniform float u_camZ;

void main(){
	gl_Position = u_proj*u_model*vec4(a_posL.x,a_posL.y,-u_camZ,1.0);
	//gl_Position=vec4(a_posL,1.0);
	v_texcoord = a_texcoord;
}
