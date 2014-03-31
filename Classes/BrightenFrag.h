R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor; // 일단 무시.
varying vec2 v_texCoord; // 넘어오는 텍스쳐 좌표.

uniform sampler2D CC_Texture0; // 텍스쳐

uniform float u_brighten; // C++ 에서 넘겨주는 변수값.



void main() {
	vec4 sum = vec4(0.0);
	// v_texCoord 좌표의 CC_Texture0 위치의 rgba 값에 밝기 인자를 곱함. 
	sum += texture2D(CC_Texture0, v_texCoord) * u_brighten;
	gl_FragColor = sum; // 출력될 rgba  값을 gl_FragColor 에 넣어줌.
}

)";