R"(
																	
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;
uniform float u_pixelationSize;
uniform vec2 u_imageSize;
void main()
{
	/*vec4 sum = vec4(0.0);
	// v_texCoord 좌표의 CC_Texture0 위치의 rgba 값에 밝기 인자를 곱함.
	sum += texture2D(CC_Texture0, v_texCoord) * u_brighten; */
	//float m_ResX = 1.0 / u_imageSize.x;
	//float m_ResY = 1.0 / u_imageSize.y;
	vec2 temp = vec2(1.0 / u_imageSize.x, 1.0 / u_imageSize.y);
	vec2 uv = v_texCoord.xy;
	vec2 temp2 = uv * u_imageSize;
	temp2.x = floor(temp2.x / u_pixelationSize ) * u_pixelationSize;
	temp2.y = floor(temp2.y / u_pixelationSize ) * u_pixelationSize;

	vec2 coord = temp2 / u_imageSize;
	//vec2 coord = uv;

  vec4 tc = texture2D(CC_Texture0, coord).rgba;
	float gray = dot(tc.rgb, vec3(0.299, 0.587, 0.114));
  gl_FragColor = vec4(gray, gray, gray, tc.a);


}

)";
