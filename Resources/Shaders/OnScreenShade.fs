precision mediump float;
uniform sampler2D u_texture0;

varying vec2 v_texcoord;

uniform vec2 u_texSize;
uniform vec2 u_spriteOffset;
uniform float u_alpha;
vec2 processTexcoord(){
	vec2 newTexcoord = v_texcoord;
	newTexcoord.x = newTexcoord.x/u_texSize.x+u_spriteOffset.x*1.0/u_texSize.x;
	newTexcoord.y = newTexcoord.y/u_texSize.y+u_spriteOffset.y*1.0/u_texSize.y;
	return newTexcoord;
}


void main(){
	vec2 texcoord = processTexcoord();
	vec4 textureColor = texture2D(u_texture0,texcoord);
	gl_FragColor = vec4(textureColor.rgb,textureColor.a*u_alpha);
}
