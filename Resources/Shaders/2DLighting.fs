precision mediump float;
varying vec4 v_color;

uniform sampler2D u_texture0;
void main()
{

	float alpha = pow(v_color.a,10.0);
	gl_FragColor = vec4(v_color.xyz,alpha);//vec4(1.0,0.0,0.0,1.0);
}
