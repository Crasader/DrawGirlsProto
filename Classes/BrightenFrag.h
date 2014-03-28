R"(

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;




void main() {
	vec4 sum = vec4(0.0);
	sum += texture2D(CC_Texture0, v_texCoord) * 1.2;
	gl_FragColor = sum;
}

)";