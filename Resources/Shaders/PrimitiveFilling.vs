attribute vec2 a_posL;
attribute vec4 a_color;
uniform mat4 u_ortho;
varying vec4 v_color;
void main(){
	gl_Position = u_ortho*vec4(a_posL,0.0,1.0);
	v_color = a_color;
}
