#version 120

//void main()
//{	
//	gl_TexCoord[0] = gl_MultiTexCoord0;
//	gl_Position = ftransform();
//} 

varying vec3 normal;
varying vec4 worldPosition;
varying vec4 worldN;
varying vec3 viewDir;

void main()
{
	normal = gl_NormalMatrix * gl_Normal;
	
	gl_Position = ftransform();
	worldPosition = gl_ModelViewMatrix * gl_Vertex;
	worldN = normalize( vec4( gl_NormalMatrix * gl_Normal, 0 ) );
	vec4 viewDir4 = normalize( worldPosition );
	viewDir = vec3( viewDir4.xyz );
    
	gl_TexCoord[0] = gl_MultiTexCoord0;
}

