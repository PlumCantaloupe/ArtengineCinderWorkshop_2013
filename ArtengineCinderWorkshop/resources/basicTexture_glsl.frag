#version 120

//uniform sampler2D tex0;
//
//void main()
//{
//    vec2 uv = gl_TexCoord[0].st;
//    uv.y = 1.0 - uv.y;
//	vec4 baseTex = texture2D( tex0, uv );
//    gl_FragColor = baseTex;
//}

uniform sampler2D tex0;
varying vec3 normal;
varying vec3 worldPosition;
varying vec3 viewDir;

void main()
{
    vec2 uv = gl_TexCoord[0].st;
    uv.y = 1.0 - uv.y;
	vec3 sample = texture2D( tex0, uv).rgb;
    
	vec3 ppNormal = normalize( normal );
	vec3 L = normalize( vec3( 0.0, 0.0, 1.0 ) );
	vec3 eye = vec3( 0.0, 0.0, 1.0 );
	vec3 R = ppNormal * 2.0 * ( dot( L, ppNormal ) ) - L;
	vec3 V = normalize( viewDir );
	
	float ppDiffuse = max( dot( eye, ppNormal ), 0.0 );
	float ppSpecular = pow( ppDiffuse, 32.0 ) * 1.0;
    
	gl_FragColor.rgb = sample * ppDiffuse;// + vec3( ppSpecular, ppSpecular, ppSpecular );
	gl_FragColor.a = 1.0;
}
