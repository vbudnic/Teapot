// Vertex shader for normal mapping.
varying vec3 ec_vnormal, ec_vposition, ec_vtangent, ec_vbitangent;
varying vec4 tcoords;
attribute vec3 tangent, bitangent;

void main()
{	
ec_vnormal = gl_NormalMatrix*gl_Normal;
ec_vtangent = gl_NormalMatrix*tangent;
ec_vbitangent = gl_NormalMatrix*bitangent;
ec_vposition = gl_ModelViewMatrix*gl_Vertex;
gl_Position = gl_ProjectionMatrix*gl_ModelViewMatrix*gl_Vertex;
gl_TexCoord[0] = gl_MultiTexCoord0;
tcoords=gl_TextureMatrix[7]*gl_Vertex;
}
