R"(
																	
varying vec2 v_texCoord;

uniform sampler2D CC_Texture0;
uniform vec2 u_imageSize;
void main()
{
	float dx = 10.*(1./u_imageSize.x);
	float dy = 10.*(1./u_imageSize.y);
	vec2 coord = vec2(dx*floor(v_texCoord.x/dx),
										dy*floor(v_texCoord.y/dy));
	gl_FragColor = texture2D(CC_Texture0, coord);
}

)";