#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <math.h>

#define XRES 768
#define YRES 768

struct fv2{
	float x,y;
};

struct fv3{
	float x,y,z;
};

struct iv3{
	int x,y,z;
};

struct OBJ{
std::vector<fv3> vertex;
std::vector<fv2> tcoord;
std::vector<fv3> normal;
std::vector<fv3> tangent;
std::vector<fv3> bitangent;
//std::vector<std::vector<iv3> > face;
}obj;

int faceSize=0;
int sprogram;
int sprogram1;
GLfloat light0_position[] = { -4.0, 2.0, -3.0, 1.0 };
GLfloat light0_direction[] = { 0.5, -2.0, 0.5, 1.0};

void lights()
{
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
}

bool loadOBJ(const char* path){

	std::vector< unsigned int > vi;
	std::vector< unsigned int > ti;
        std::vector< unsigned int > ni;
	std::vector<fv3> tv;
	std::vector<fv2> tt;
	std::vector<fv3> tn;
	std::vector<fv3> ttan;
	std::vector<fv3> tbitan;

	FILE * file = fopen(path, "r");
	if(file==NULL){
        printf("cannot open the file!\n");
	}
	while(1){
	    char header[128];
	    int res = fscanf(file, "%s", header);
	    if(res==EOF)break;
    //v
	if(strcmp(header,"v")==0){
        fv3 v;
        fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z);
        tv.push_back(v);
        
	//vt
	}else if(strcmp(header, "vt")==0){
        fv2 t;
	    fscanf(file, "%f %f\n", &t.x, &t.y);
	    tt.push_back(t);
       
	//vn
	}else if(strcmp(header,"vn")==0){
	    fv3 n;
	    fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z);
	    tn.push_back(n);
        
	//vx
	}else if(strcmp(header,"vx")==0){
	fv3 vx;
	fscanf(file, "%f %f %f\n", &vx.x, &vx.y, &vx.z);
	ttan.push_back(vx);
    
	//vy
	}else if(strcmp(header,"vy")==0){
	    fv3 vy;
	    fscanf(file, "%f %f %f\n", &vy.x, &vy.y, &vy.z);
	    tbitan.push_back(vy);
        
	//f
	}else if(strcmp(header,"f")==0){
	    std::string v1,v2,v3,v4;
	    int vertexIndex[4],tcoordIndex[4],normalIndex[4];
	    fscanf(file, 
	    "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
	    &vertexIndex[0],&tcoordIndex[0],&normalIndex[0],
        &vertexIndex[1],&tcoordIndex[1],&normalIndex[1],
        &vertexIndex[2],&tcoordIndex[2],&normalIndex[2],
        &vertexIndex[3],&tcoordIndex[3],&normalIndex[3]);
       
        vi.push_back(vertexIndex[0]);
        vi.push_back(vertexIndex[1]);
        vi.push_back(vertexIndex[2]);
        vi.push_back(vertexIndex[3]);
        ti.push_back(tcoordIndex[0]);
        ti.push_back(tcoordIndex[1]);
        ti.push_back(tcoordIndex[2]);
        ti.push_back(tcoordIndex[3]);
        ni.push_back(normalIndex[0]);
        ni.push_back(normalIndex[1]);
        ni.push_back(normalIndex[2]);
        ni.push_back(normalIndex[3]);
        faceSize+=4;
	}
	}	

    for(unsigned int i=0;i<vi.size();i++){
        obj.vertex.push_back(tv[vi[i]-1]);
        obj.tangent.push_back(ttan[vi[i]-1]);
        obj.bitangent.push_back(tbitan[vi[i]-1]);
       
    }
    for(unsigned int i=0;i<ti.size();i++){
        obj.tcoord.push_back(tt[ti[i]-1]);
    }
    for(unsigned int i=0;i<ni.size();i++){
        obj.normal.push_back(tn[ni[i]-1]);
    }
    return true;
}

void setup_the_viewvol()
{
fv3 eye;
fv3 view;
fv3 up;

glEnable(GL_DEPTH_TEST);

/* specify size and shape of view volume */
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.6,0.1,20.0);

/* specify position for view volume */
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

eye.x = -4.0; eye.y = 2.0; eye.z = -3.0;
view.x = 0.0; view.y = 0.5; view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;

gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}


//------------------------------------------------------------
void set_uniform_parameters(unsigned int p,unsigned int id1, unsigned int id2)
{
int location;
location = glGetUniformLocation(p,"mytexture");
glUniform1i(location,id1);
location = glGetUniformLocation(p,"mynormalmap");
glUniform1i(location,id2);
/*location = glGetUniformLocation(p,"mytexture1");
glUniform1i(location,2);*/
}

void view_volume(float *ep, float *vp)
{
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(45.0,1.6,0.1,20.0);
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(ep[0],ep[1],ep[2],vp[0],vp[1],vp[2],0.0,1.0,0.0);
}

void save_matrix(float *ep, float *vp)
{
glMatrixMode(GL_TEXTURE); 
// This must match the unit used in the vertex shader.
glActiveTexture(GL_TEXTURE0);
glLoadIdentity();
glTranslatef(0.0,0.0,-0.005);
glScalef(0.5,0.5,0.5);
glTranslatef(1.0,1.0,1.0);
gluPerspective(45.0,1.6,0.1,20.0);
gluLookAt(ep[0],ep[1],ep[2],vp[0],vp[1],vp[2],0.0,1.0,0.0);
}

//------------------------------------------------------------
void draw_stuff()
{
//std::vector< fv3 > vertices;
//std::vector< fv2 > tc;
//std::vector< fv3 > normals;
//std::vector< fv3 > tangent;
//std::vector< fv3 > bitangent;
//std::vector<std::vector< iv3 > > faces;
//std::vector< unsigned int > vi,ti,ni;
float eyepoint[3], viewpoint[3];
int k;

int tanIndex=glGetUniformLocation(sprogram,"tangent");
int bitanIndex=glGetUniformLocation(sprogram,"bitangent");
bool loadSucceed = loadOBJ("data/teapot.605.obj");
if(!loadSucceed){
    std::cout<<"load .obj file failed!"<<std::endl;
}

 glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// Render first from the point of view of the light source. 
// This render will go to texture (#1), and it will not use shaders.
glBindFramebufferEXT(GL_FRAMEBUFFER,1); 
glUseProgram(0);
for(k=0;k<3;k++){ 
	eyepoint[k] = light0_position[k]; 
	viewpoint[k] = light0_direction[k]+light0_position[k];
	}
//view_volume(eyepoint,viewpoint);
// Set lights after vv so that shader can assume eye coordinates.
lights(); 

glBegin(GL_QUADS);
    for(int i=0;i<(faceSize);i++){
        glNormal3f(obj.normal[i].x,obj.normal[i].y,
              obj.normal[i].z);
            glTexCoord2f(obj.tcoord[i].x,obj.tcoord[i].y);
            glVertexAttrib3f(tanIndex,obj.tangent[i].x,
                obj.tangent[i].y,obj.tangent[i].z);
            glVertexAttrib3f(bitanIndex,obj.bitangent[i].x,
                obj.bitangent[i].y,obj.bitangent[i].z);
            glVertex3f(obj.vertex[i].x,obj.vertex[i].y,
             obj.vertex[i].z);
    }
glEnd(); 

float front[4][3]={{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};
float back[4][3]={{0.0,0.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0},{1.0,0.0,0.0}};
float left[4][3]={{0.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},{0.0,1.0,0.0}};
float right[4][3]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};
float top[4][3]={{2,0,2},{2,-0,-2},{-2,0,-2},{-2,0,2}};
float bottom[4][3]={{-2,0,-2},{-2,0,2},{2,0,2},{2,-0,-2}};
float mytexcoords[4][2] = {{0.0,1.0},{1.0,1.0},{1.0,0.0},{0.0,0.0}};
	
glUseProgram(sprogram1);	
set_uniform_parameters(sprogram1,2,3);
glActiveTexture(GL_TEXTURE2);
glBindTexture(GL_TEXTURE_2D,3);
glActiveTexture(GL_TEXTURE3);
glBindTexture(GL_TEXTURE_2D,4);

glEnable(GL_TEXTURE_2D);
   glBegin(GL_QUADS);
int i;
glNormal3f(0.0,0.0,1.0);
for(i=0;i<4;i++){
	glTexCoord2fv(mytexcoords[i]);
	glVertex3f(bottom[i][0],bottom[i][1],bottom[i][2]);
	}
glEnd();
glDisable(GL_TEXTURE_2D);

glBindFramebufferEXT(GL_FRAMEBUFFER,0);
// Save the "view from eyepoint" transformation.
save_matrix(eyepoint,viewpoint);

// Switch to the shader and use texture #1.

//use Texture
glUseProgram(sprogram);	
set_uniform_parameters(sprogram,0,1);
// This unit number must match that set as a uniform value for the
// fragment shader.
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D,2);

// Draw scene from the intended eye point, complete with shadows. 
eyepoint[0] = -4.0; eyepoint[1] = 2.0; eyepoint[2] = -3.0;
viewpoint[0] = 0.0; viewpoint[1] = 0.5; viewpoint[1] = 0.0;
//view_volume(eyepoint,viewpoint);
lights();
//glActiveTexture(GL_TEXTURE2);
//glBindTexture(GL_TEXTURE_2D,3);
glEnable(GL_TEXTURE_2D);

   glBegin(GL_QUADS);

    for(int i=0;i<(faceSize);i++){
        glNormal3f(obj.normal[i].x,obj.normal[i].y,
              obj.normal[i].z);
            glTexCoord2f(obj.tcoord[i].x,obj.tcoord[i].y);
            glVertexAttrib3f(tanIndex,obj.tangent[i].x,
                obj.tangent[i].y,obj.tangent[i].z);
            glVertexAttrib3f(bitanIndex,obj.bitangent[i].x,
                obj.bitangent[i].y,obj.bitangent[i].z);
            glVertex3f(obj.vertex[i].x,obj.vertex[i].y,
             obj.vertex[i].z);
    }  
glEnd();    
glDisable(GL_TEXTURE_2D);

//---------------------------


glFlush();
}

void build_shadowmap()
{
// Set properties of texture id #1.
glBindTexture(GL_TEXTURE_2D,3);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
// Declare size and type of texture; it has no data initially (last arg 0).
glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, XRES, YRES, 0, 
	GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
// Back to default.
glBindTexture(GL_TEXTURE_2D, 0);

glBindFramebufferEXT(GL_FRAMEBUFFER,1);
glDrawBuffer(GL_NONE); // No color buffers will be written.

// Attach this framebuffer (id #1 above) to texture (id #1 is penultimate arg),
// so that we can perform an offscreen render-to-texture.
glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,1,0);
// Back to default.
glBindFramebufferEXT(GL_FRAMEBUFFER,0);
}



char *read_shader_program(char const*filename) 
{
FILE *fp;
char *content = NULL;
int fd, count;
fd = open(filename,O_RDONLY);
count = lseek(fd,0,SEEK_END);
close(fd);
content = (char *)calloc(1,(count+1));
fp = fopen(filename,"r");
count = fread(content,sizeof(char),count,fp);
content[count] = '\0';
fclose(fp);
return content;
}
//--------------------------------------------------------------------
unsigned int set_shaders(char const* vert, char const* frag)
{
char *vs, *fs;
GLuint v, f, p;

v = glCreateShader(GL_VERTEX_SHADER);
f = glCreateShader(GL_FRAGMENT_SHADER);
vs = read_shader_program(vert);
fs = read_shader_program(frag);
glShaderSource(v,1,(const char **)&vs,NULL);
glShaderSource(f,1,(const char **)&fs,NULL);
free(vs);
free(fs); 
glCompileShader(v);
glCompileShader(f);
p = glCreateProgram();
glAttachShader(p,f);
glAttachShader(p,v);
glLinkProgram(p);
return(p);
}
//--------------------------------------------------------------------


void load_texture(char const*filename,unsigned int tid)
{
FILE  *fptr;
char buf[512], *parse;
int im_size, im_width, im_height, max_color;
unsigned char *texture_bytes;

fptr=fopen(filename,"r");
fgets(buf,512,fptr);
do{
	fgets(buf,512,fptr);
	} while(buf[0]=='#');
parse = (char *)strtok(buf," \t");
im_width = atoi(parse);

parse = (char *)strtok(NULL," \n");
im_height = atoi(parse);

fgets(buf,512,fptr);
parse = (char *)strtok(buf," \n");
max_color = atoi(parse);

im_size = im_width*im_height;
texture_bytes = (unsigned char *)calloc(3,im_size);
fread(texture_bytes,3,im_size,fptr);
fclose(fptr);

glBindTexture(GL_TEXTURE_2D,tid);
glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,im_width,im_height,0,GL_RGB, 
	GL_UNSIGNED_BYTE,texture_bytes);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
free(texture_bytes);
}

void do_lights()
{
/* white light */
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_specular[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_position[] = { -6.5, 2.0, 2.0, 1.0 };
float light0_direction[] = { -1.5, -2.0, -2.0, 1.0};

/* turn off scene default ambient */
glLightModelfv(GL_LIGHT_MODEL_AMBIENT,light0_ambient);

/* make specular correct for spots */
glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,1);

glLightfv(GL_LIGHT0,GL_AMBIENT,light0_ambient);
glLightfv(GL_LIGHT0,GL_DIFFUSE,light0_diffuse);
glLightfv(GL_LIGHT0,GL_SPECULAR,light0_specular);
glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,180.0);
glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.5);
glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.1);
glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.01);
glLightfv(GL_LIGHT0,GL_POSITION,light0_position);
glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,light0_direction);

glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
}

void do_material()
{
float mat_ambient[] = {0.0,0.0,0.0,1.0};
float mat_diffuse[] = {0.9,0.9,0.1,1.0};
float mat_specular[] = {0.33,0.65,1.0,1.0};
float mat_shininess[] = {2.0};

glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);
}

int mybuf = 1;

void initOGL(int argc, char **argv)
{

 glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE | GLUT_ACCUM);
   glutInitWindowSize(1280, 720);
   glutInitWindowPosition(100 , 50);
   glutCreateWindow("Teapot (Yu Li, Volodymyr Budnichenko)");

   load_texture("data/Crack-Texture.ppm",1);
   load_texture("data/coppernormal.ppm",2);
   load_texture("data/wall2.ppm",3);
   load_texture("data/wall2new.ppm",4);

setup_the_viewvol();
do_lights();
do_material();

build_shadowmap();

sprogram=set_shaders("tex.vert","tex.frag");
sprogram1=set_shaders("tex_floor.vert","tex_floor.frag");

}

void getout(unsigned char key,int x,int y)
{
switch(key) {
        case 'q':               
                exit(1);
        default:
                break;
    }
}

int main(int argc, char **argv)
{
initOGL(argc,argv);
glutDisplayFunc(draw_stuff);
//glutIdleFunc(update);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}
