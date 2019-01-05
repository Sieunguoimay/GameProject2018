attribute vec3 a_posL; //0
attribute vec3 a_norm; //1
attribute vec2 a_texcoord;//2

uniform mat4 u_model;//0
uniform mat4 u_view;//1
uniform mat4 u_proj;//2
uniform mat3 u_normalMatrix;//3


varying vec2 v_texcoord;
varying vec3 v_normalCoord, v_eyeCoord;

void main(){
	gl_Position = u_proj*u_view*u_model*vec4(a_posL,1.0);

	v_eyeCoord = vec3(u_view*u_model*vec4(a_posL,1.0));

	mat3 m = mat3(u_view*u_model);
	mat3 normalMatrix = mat3(m[0][0],m[0][1],m[0][2],
							m[1][0],m[1][1],m[1][2],
							m[2][0],m[2][1],m[2][2]);
	v_normalCoord = normalMatrix*a_norm;
	v_texcoord = a_texcoord;
}
