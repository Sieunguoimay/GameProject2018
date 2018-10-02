attribute vec3 a_posL;
attribute vec3 a_norm;
attribute vec3 a_texcoord;
varying vec3 v_color;
//uniform mat4 model;
void main(){
	gl_Position = vec4(a_posL,1.0);
	v_color = a_norm;
}
