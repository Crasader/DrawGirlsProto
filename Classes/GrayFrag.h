R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor; // 일단 무시.
varying vec2 v_texCoord; // 넘어오는 텍스쳐 좌표.

uniform sampler2D CC_Texture0; // 텍스쳐

void main() {
	vec4 sum = vec4(0.0);
	// v_texCoord 좌표의 CC_Texture0 위치의 rgba 값에 밝기 인자를 곱함.
	vec4 color = texture2D(CC_Texture0, v_texCoord).rgba;
	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
	gl_FragColor = vec4(gray, gray, gray, color.a);
}

)";
