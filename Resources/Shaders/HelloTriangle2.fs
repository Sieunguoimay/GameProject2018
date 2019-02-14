precision mediump float;
varying vec4 v_color;
varying vec2 v_texcoord;

uniform sampler2D u_texture0;
void main()
{
	vec4 textureColor = texture2D(u_texture0,v_texcoord);
	gl_FragColor = v_color*textureColor;//vec4(1.0,0.0,0.0,1.0);
}