//this shader is to serve for render texture purpose only

attribute vec4 a_posL;

uniform mat4 u_ortho;

varying vec2 v_texcoord;
void main(){
	gl_Position = u_ortho*vec4(a_posL.xy,0.0,1.0);
	v_texcoord = vec2(a_posL.z,1.0-a_posL.w);
}
