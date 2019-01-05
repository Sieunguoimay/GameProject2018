precision mediump float;
//diffuse light and material infomation


varying vec3 v_normalCoord, v_eyeCoord;
varying vec2 v_texcoord;

vec3 normalizeNormal, normalizeEyeCoord, normalizeLightVec;
vec3 V, R;
vec4 ambient, diffuse, specular;
float intensity, cosAngle;



#define MAX_LIGHT_NUM 4
#define SUN_LIGHT 0
#define BULB_LIGHT 1

struct Light{
	int type;

	vec3 diffuse;
	vec3 pos;

	float constant;
	float linear;
	float quadratic;
};


uniform float u_shininess;//0
uniform int u_lightNum;//1
uniform Light u_lights[MAX_LIGHT_NUM];//2->24
uniform sampler2D u_texture0;//25


vec4 PointLight(int index,vec4 textureColor,bool frontSide){
	normalizeNormal = normalize(v_normalCoord);
	normalizeEyeCoord = normalize(v_eyeCoord);
	normalizeLightVec = normalize(u_lights[index].pos - v_eyeCoord);

	if(frontSide){
		cosAngle = max(0.0,dot(normalizeNormal, normalizeLightVec));
	}
	else{
		cosAngle = max(0.0,dot(-normalizeNormal, normalizeLightVec));
	}

	V = -normalizeEyeCoord;
	R = reflect(-normalizeLightVec,normalizeNormal);
	intensity = pow(max(0.0,dot(V,R)),u_shininess);


	float distance = length(u_lights[index].pos - v_eyeCoord);
	float attenuation = 1.0/(u_lights[index].constant 
						+ u_lights[index].linear*distance
						+ u_lights[index].quadratic*(distance*distance));

	ambient = 0.2*textureColor*vec4(u_lights[index].diffuse,1.0);
	diffuse = textureColor*vec4(u_lights[index].diffuse,1.0);
	specular = textureColor*vec4(u_lights[index].diffuse,1.0);

	vec4 outColor = attenuation*(ambient+(cosAngle*diffuse)+(intensity*specular));
	outColor.a = textureColor.a;
	return outColor;
}



vec4 PhongShading(int index,vec4 textureColor, bool frontSide){
	normalizeNormal = normalize(v_normalCoord);
	normalizeEyeCoord = normalize(v_eyeCoord);
	normalizeLightVec = normalize(u_lights[index].pos - v_eyeCoord);

	if(frontSide){
		cosAngle = max(0.0,dot(normalizeNormal, normalizeLightVec));
	}
	else{
		cosAngle = max(0.0,dot(-normalizeNormal, normalizeLightVec));
	}

	V = -normalizeEyeCoord;
	R = reflect(-normalizeLightVec,normalizeNormal);
	intensity = pow(max(0.0,dot(V,R)),u_shininess);


	ambient = 0.2*textureColor*vec4(u_lights[index].diffuse,1.0);
	diffuse = textureColor*vec4(u_lights[index].diffuse,1.0);
	specular = textureColor*vec4(u_lights[index].diffuse,1.0);

	vec4 outColor = (ambient+(cosAngle*diffuse)+(intensity*specular));
	outColor.a = textureColor.a;
	return outColor;
}

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
	vec4 multipleLightColor = vec4(0.0);
	vec2 texcoord = processTexcoord();
	vec4 textureColor = texture2D(u_texture0,texcoord);
	for(int i = 0; i<u_lightNum; i++){

		if(dot (v_normalCoord, v_eyeCoord) < 0.0){//gl_FrontFacing
			if(u_lights[i].type == 0)
				multipleLightColor += PhongShading(i,textureColor,true);
			else if(u_lights[i].type == 1)
				multipleLightColor += PointLight(i,textureColor,true);
		}
		else{
			if(u_lights[i].type == 0)
				multipleLightColor += PhongShading(i,textureColor,false);
			else if(u_lights[i].type == 1)
				multipleLightColor += PointLight(i,textureColor,false);
		}
		
	}
	//if(multipleLightColor.a<0.1)
		//discard;
	gl_FragColor = vec4(multipleLightColor.rgb,textureColor.a*u_alpha);
}
