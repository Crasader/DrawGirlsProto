R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;

uniform vec2 u_blurSize;

void main() {
	vec4 sum = vec4(0.0);
	
	vec2 myBlurSize = u_blurSize;
	sum += texture2D(CC_Texture0, v_texCoord + (- 4.0, -4.0) * myBlurSize) * 0.05;
	sum += texture2D(CC_Texture0, v_texCoord + (- 3.0, -3.0) * myBlurSize) * 0.09;
	sum += texture2D(CC_Texture0, v_texCoord + (- 2.0, -2.0) * myBlurSize) * 0.12;
	sum += texture2D(CC_Texture0, v_texCoord + (- 1.0, -1.0) * myBlurSize) * 0.15;
	sum += texture2D(CC_Texture0, v_texCoord + (- 0.0, -0.0) * myBlurSize) * 0.16;
	sum += texture2D(CC_Texture0, v_texCoord + (+ 1.0, +1.0) * myBlurSize) * 0.15;
	sum += texture2D(CC_Texture0, v_texCoord + (+ 2.0, +2.0) * myBlurSize) * 0.12;
	sum += texture2D(CC_Texture0, v_texCoord + (+ 3.0, +3.0) * myBlurSize) * 0.09;
	sum += texture2D(CC_Texture0, v_texCoord + (+ 4.0, +4.0) * myBlurSize) * 0.05;

	
	gl_FragColor = sum;
}

)";