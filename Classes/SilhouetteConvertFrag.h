R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor; // 일단 무시.
varying vec2 v_texCoord; // 넘어오는 텍스쳐 좌표.

uniform sampler2D CC_Texture0; // 텍스쳐

uniform int u_type;

void main() {
	// v_texCoord 좌표의 CC_Texture0 위치의 rgba 값에 밝기 인자를 곱함.
//	texture2D(CC_Texture0, v_texCoord)
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
	vec4 resultColor;
	if(u_type == 0)
		resultColor = vec4(0, 0, 1, texColor.a);
	else if(u_type == 1)
		resultColor = vec4(0, 0, texColor.b, 1);
	else if(u_type == 2)
		resultColor = vec4(240.0/255.0*texColor.b, 110.0/255.0*texColor.b, 170.0/255.0*texColor.b, 1);
	else if(u_type == 3)
		resultColor = vec4(255.0/255.0*texColor.b, 245.0/255.0*texColor.b, 104.0/255.0*texColor.b, 1);
	else if(u_type == 4)
		resultColor = vec4(57.0/255.0*texColor.b, 181.0/255.0*texColor.b, 74.0/255.0*texColor.b, 1);
	else if(u_type == 5)
		resultColor = vec4(168.0/255.0*texColor.b, 100.0/255.0*texColor.b, 168.0/255.0*texColor.b, 1);
	else if(u_type == 6)
		resultColor = vec4(0.0/255.0*texColor.b, 191.0/255.0*texColor.b, 243.0/255.0*texColor.b, 1);
	else if(u_type == 7)
		resultColor = vec4(242.0/255.0*texColor.b, 101.0/255.0*texColor.b, 34.0/255.0*texColor.b, 1);
	else
		resultColor = vec4(0, 0, texColor.b, 1);
	
	gl_FragColor = resultColor;
}

)";