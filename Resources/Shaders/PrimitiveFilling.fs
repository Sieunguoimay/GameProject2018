precision mediump float;
varying vec4 v_color;

uniform sampler2D u_texture0;
void main()
{
	gl_FragColor = v_color;//vec4(1.0,0.0,0.0,1.0);
}
