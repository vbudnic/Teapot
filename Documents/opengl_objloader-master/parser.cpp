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
#include <stdio.h>
struct fv2{
	float x,y;
};

struct fv3{
	float x,y,z;
};

struct iv3{
	int x,y,z;
};



// std::vector<fv3> vertex;
// std::vector<fv2> tcoord;
// std::vector<fv3> normal;
// std::vector<fv3> tangent;
// std::vector<fv3> bitangent;
// std::vector<std::vector<iv3> > face;
// 
int faceSize=0;
int sprogram;
int sprogram1;

bool loadOBJ(const char* path,std::vector<fv3> &vertex, 
    std::vector<fv2> &tcoord,std::vector<fv3> &normal, 
    std::vector<fv3> &tangent,std::vector<fv3> &bitangent,
    	std::vector< unsigned int > &vi,std::vector< unsigned int > &ti,
        std::vector< unsigned int > &ni){


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
        vertex.push_back(tv[vi[i]-1]);
        tangent.push_back(ttan[vi[i]-1]);
        bitangent.push_back(tbitan[vi[i]-1]);
       
    }
    for(unsigned int i=0;i<ti.size();i++){
        tcoord.push_back(tt[ti[i]-1]);
    }
    for(unsigned int i=0;i<ni.size();i++){
        normal.push_back(tn[ni[i]-1]);
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
void set_uniform_parameters(unsigned int p, int type)
{

int location;
//---if type is 1 -> teapot; 2 -> floor; 3 -> top;
switch (type) {
case 1:
	
	location = glGetUniformLocation(p,"mytexture");
	glUniform1i(location,0);
	location = glGetUniformLocation(p,"mynormalmap");
	glUniform1i(location,1);
	break;
case 2:
	
	location = glGetUniformLocation(p,"mytexture");
	glUniform1i(location,2);
	location = glGetUniformLocation(p,"mynormalmap");
	glUniform1i(location,3);
	break;
default:
	
	location = glGetUniformLocation(p,"mytexture");
	glUniform1i(location,0);
	location = glGetUniformLocation(p,"mynormalmap");
	glUniform1i(location,1);
	break;
}
	
}



//------------------------------------------------------------
void draw_stuff()
{
std::vector< fv3 > vertices;
std::vector< fv2 > tc;
std::vector< fv3 > normals;
std::vector< fv3 > tangent;
std::vector< fv3 > bitangent;
std::vector<std::vector< iv3 > > faces;
std::vector< unsigned int > vi,ti,ni;

//int vindex,tindex,nindex;
int tanIndex=glGetUniformLocation(sprogram,"tangent");
int bitanIndex=glGetUniformLocation(sprogram,"bitangent");
bool loadSucceed = loadOBJ("data/teapot.605.obj", vertices, tc, normals,
    tangent,bitangent,vi,ti,ni);
if(!loadSucceed){
    std::cout<<"load .obj file failed!"<<std::endl;
}

 glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//use Texture
glUseProgram(sprogram);	
set_uniform_parameters(sprogram, 1);
glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D,1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D,2);

//glActiveTexture(GL_TEXTURE2);
//glBindTexture(GL_TEXTURE_2D,3);
glEnable(GL_TEXTURE_2D);

   glBegin(GL_QUADS);

     /* glVertex3f( 3.0f, 0.0f, -3.0f);
      glVertex3f(-3.0f, 0.0f, -3.0f);
      glVertex3f(-3.0f, 0.0f,  3.0f);
      glVertex3f( 3.0f, 0.0f,  3.0f);
     */
    
     
 /*    for (i = -20; i < 20; i++)
 for (j = -20; j < 20; j++)
 {
 float startX=  TS*(i+20);
 float startY = TS*(j+20);
 glTexCoord2f( startX + 0.0f, startY + 0 );  glVertex2f(i, j);
 glTexCoord2f( startX + TS, startY + 0 );  glVertex2f(i + 1, j);
 glTexCoord2f( startX + TS, startY + TS );  glVertex2f(i + 1, j + 1);
 glTexCoord2f( startX + 0.0f, startY + TS );  glVertex2f(i, j + 1);
 }
      */
    for(int i=0;i<(faceSize);i++){
        glNormal3f(normals[i].x,normals[i].y,
              normals[i].z);
            glTexCoord2f(tc[i].x,tc[i].y);
            glVertexAttrib3f(tanIndex,tangent[i].x,
                tangent[i].y,tangent[i].z);
            glVertexAttrib3f(bitanIndex,bitangent[i].x,
                bitangent[i].y,bitangent[i].z);
            glVertex3f(vertices[i].x,vertices[i].y,
             vertices[i].z);
    }
      
      
   
glEnd();    
glDisable(GL_TEXTURE_2D);

//---------------------------
float front[4][3]={{0.0,0.0,1.0},{1.0,0.0,1.0},{1.0,1.0,1.0},{0.0,1.0,1.0}};
float back[4][3]={{0.0,0.0,0.0},{0.0,1.0,0.0},{1.0,1.0,0.0},{1.0,0.0,0.0}};
float left[4][3]={{0.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,1.0},{0.0,1.0,0.0}};
float right[4][3]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};
float top[4][3]={{2,0,2},{2,-0,-2},{-2,0,-2},{-2,0,2}};
float bottom[4][3]={{-2,0,-2},{-2,0,2},{2,0,2},{2,-0,-2}};
float mytexcoords[4][2] = {{0.0,1.0},{1.0,1.0},{1.0,0.0},{0.0,0.0}};
	
glUseProgram(sprogram1);	
set_uniform_parameters(sprogram1, 2);
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
	

       // Floor 
 /*      
glVertex3f(-2,-0,-2);
glVertex3f(-2,-0,2);
glVertex3f(2,-0,2);
glVertex3f(2,-0,-2);
*/

/* Ceiling */
/*
glVertex3f(-1,1,-1);
glVertex3f(1,1,-1);
glVertex3f(1,1,1);
glVertex3f(-1,1,1);
    // Walls 
glVertex3f(-1,-1,1);
glVertex3f(1,-1,1);
glVertex3f(1,1,1);
glVertex3f(-1,1,1);

glVertex3f(-1,-1,-1);
glVertex3f(1,-1,-1);
glVertex3f(1,1,-1);
glVertex3f(-1,1,-1);

glVertex3f(1,1,1);
glVertex3f(1,-1,1);
glVertex3f(1,-1,-1);
glVertex3f(1,1,-1);

glVertex3f(-1,1,1);
glVertex3f(-1,-1,1);
glVertex3f(-1,-1,-1);
glVertex3f(-1,1,-1);
*/
glEnd();
glDisable(GL_TEXTURE_2D);
/*
glUseProgram(sprogram1);
glBegin(GL_QUADS); 
glNormal3f(0.0,0.0,-1.0);
for(i=0;i<4;i++) glVertex3f(back[i][0],back[i][1],back[i][2]);
glNormal3f(-1.0,0.0,0.0);
for(i=0;i<4;i++) glVertex3f(left[i][0],left[i][1],left[i][2]);
glNormal3f(1.0,0.0,0.0);
for(i=0;i<4;i++) glVertex3f(right[i][0],right[i][1],right[i][2]);
glNormal3f(0.0,1.0,0.0);
for(i=0;i<4;i++) glVertex3f(top[i][0],top[i][1],top[i][2]);
glNormal3f(0.0,-1.0,0.0);
for(i=0;i<4;i++) glVertex3f(bottom[i][0],bottom[i][1],bottom[i][2]);
glEnd();
glutSwapBuffers();*/
//-------------------------
        
glFlush();
}

void update()
{
//usleep(10000);
//glTranslatef(0.5,0.0,0.5);
//glRotatef(1.0,0.0,1.0,0.0);
//glTranslatef(-0.5,0.0,-0.5);
//glutPostRedisplay();
}

char *read_shader_program(char *filename) 
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
unsigned int set_shaders(char *vert, char *frag)
{
GLint vertCompiled, fragCompiled;
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
// glUseProgram(p);
return(p);
}



//--------------------------------------------------------------------


void load_texture(char *filename,unsigned int tid)
{
//FILE *fopen(), *fptr;
FILE  *fptr;
char buf[512], *parse;
int im_size, im_width, im_height, max_color;
unsigned char *texture_bytes, *tb_alpha, *tb_src, *tb_dst, *tb_final, *aarg; 
//------------last edit save----------------

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
float light0_position[] = { -10.5, 2.0, 2.0, 1.0 };
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

   load_texture("data/copper.ppm",1);
   load_texture("data/coppernormal.ppm",2);
   load_texture("data/wood.ppm",3);
   load_texture("data/fieldstoneN.ppm",4);

setup_the_viewvol();
do_lights();
do_material();

sprogram=set_shaders("tex.vert", "tex.frag");
sprogram1=set_shaders("tex_floor.vert", "tex_floor.frag");

}

void getout(unsigned char key, int x, int y)
{
switch(key) {
        case 'q':               
                //glDeleteBuffers(1,&mybuf);
                exit(1);
        default:
                break;
    }
}

int main(int argc, char **argv)
{
initOGL(argc,argv);
glutDisplayFunc(draw_stuff);
glutIdleFunc(update);
glutKeyboardFunc(getout);
glutMainLoop();
return 0;
}
