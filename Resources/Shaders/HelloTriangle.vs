attribute vec2 a_posL;
attribute vec4 a_color;
attribute vec2 a_texcoord;
uniform mat4 u_ortho;
varying vec4 v_color;
varying vec2 v_texcoord;
void main(){
	gl_Position = u_ortho*vec4(a_posL,0.0,1.0);
	v_color = a_color;//vec4(1.0,1.0,1.0,1.0);
	//v_color = vec4(u_ortho[0][0],0.0,0.0,1.0);
	v_texcoord = vec2(a_texcoord.x,1.0-a_texcoord.y);
}
