varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent;
uniform sampler2D mytexture; 
uniform sampler2D mynormalmap;


void main()
{
mat3 tform;
vec3 P, N, L, V, H, mapN, tcolor, tcolor1;
vec4 diffuse_color, specular_color, diffuse_color1, diffuse_color2; 
float shininess = gl_FrontMaterial.shininess;
float pi = 3.14159;

// Create a 3x3 matrix from T, B, and N as columns:
tform = mat3(ec_vtangent,ec_vbitangent,ec_vnormal);
P = ec_vposition;
L = normalize(gl_LightSource[0].position - P);
V = normalize(-P);				
H = normalize(L+V);
		
mapN = vec3(texture2D(mynormalmap,gl_TexCoord[0].st));
// x, y, and z are in [0.0,1.0], but x and y should be in [-1.0,1.0].
mapN.xy = 2.0*mapN.xy - vec2(1.0,1.0);

N = normalize(tform*normalize(mapN));

tcolor = vec3(texture2D(mytexture,gl_TexCoord[0].st));
//tcolor1 = vec3(texture2D(mytexture1,gl_TexCoord[0].st));

diffuse_color = vec4(tcolor,1.0)*max(dot(N,L),0.0);
/*diffuse_color1 = vec4(tcolor1,1.0)*max(dot(N,L),0.0);
diffuse_color = (diffuse_color2 + diffuse_color1)/2;*/

specular_color = gl_FrontMaterial.specular*pow(max(dot(H,N),0.0),shininess);
specular_color *= (shininess+2.0)/(8.0*pi);

gl_FragColor = diffuse_color + specular_color;
//gl_FragColor = vec4(tcolor,1.0) ;
//gl_FragColor = vec4(N,1.0);
}

