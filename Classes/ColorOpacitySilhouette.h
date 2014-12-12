R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor; // 일단 무시.
varying vec2 v_texCoord; // 넘어오는 텍스쳐 좌표.

uniform sampler2D CC_Texture0; // 텍스쳐

uniform vec4 color4;

void main() {
	// v_texCoord 좌표의 CC_Texture0 위치의 rgba 값에 밝기 인자를 곱함.
	//	texture2D(CC_Texture0, v_texCoord)
	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
	vec4 resultColor;
	
	resultColor = vec4(color4[0]*texColor.b, color4[1]*texColor.b, color4[2]*texColor.b, color4[3]*texColor.b);
	
	gl_FragColor = resultColor;
}

)";