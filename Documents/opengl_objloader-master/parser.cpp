#define GL_GLEXT_PROTOTYPES

#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <vector>
#include <iostream>
#include <cstring>
#include <unistd.h>

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
	    int checkNum = fscanf(file, 
	    "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", 
	    &vertexIndex[0],&tcoordIndex[0],&normalIndex[0],
        &vertexIndex[1],&tcoordIndex[1],&normalIndex[1],
        &vertexIndex[2],&tcoordIndex[2],&normalIndex[2],
        &vertexIndex[3],&tcoordIndex[3],&normalIndex[3]);
        if(checkNum!=12){
            std::cout<<"cannot load the faces"<<std::endl;
            return false;
        }
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
        faceSize++;
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
gluPerspective(45.0,2.0,0.1,20.0);

/* specify position for view volume */
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();

eye.x = 10.0; eye.y = 2.0; eye.z = -2.0;
view.x = 0.0; view.y = 0.5; view.z = 0.0;
up.x = 0.0; up.y = 1.0; up.z = 0.0;

gluLookAt(eye.x,eye.y,eye.z,view.x,view.y,view.z,up.x,up.y,up.z);
}

void draw_stuff()
{
//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
//glDrawArrays(GL_QUADS,0,24);
//glutSwapBuffers();
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   //glActiveTexture(GL_TEXTURE1);
   
   //glEnable(GL_TEXTURE_2D);
   
   
std::vector< fv3 > vertices;
std::vector< fv2 > tc;
std::vector< fv3 > normals;
std::vector< fv3 > tangent;
std::vector< fv3 > bitangent;
std::vector<std::vector< iv3 > > faces;
std::vector< unsigned int > vi,ti,ni;
int vindex,tindex,nindex;
bool loadSucceed = loadOBJ("data/teapot.605.obj", vertices, tc, normals,
    tangent,bitangent,vi,ti,ni);
if(!loadSucceed){
    std::cout<<"load .obj file failed!"<<std::endl;
}

   glBegin(GL_QUADS);
    for(int i=0;i<faceSize;i++){
        for(int j=0;j<4;j++){
            vindex=vi[i+j];
            tindex=ti[i+j];
            nindex=ni[i+j];
            //std::cout<<vi[i+j]<<","<<ti[i+j]<<","<<ni[i+j]<<std::endl;

            glNormal3f(normals[nindex].x,normals[nindex].y,
              normals[nindex].z);
            glTexCoord2f(tc[tindex].x,tc[tindex].y);

            glVertex3f(vertices[vindex].x,vertices[vindex].y,
             vertices[vindex].z);
        }
    }
glEnd();
   //glDisable(GL_TEXTURE_2D);

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

void do_lights()
{
/* white light */
float light0_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
float light0_diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_specular[] = { 1.0, 1.0, 1.0, 0.0 };
float light0_position[] = { 1.5, 2.0, 2.0, 1.0 };
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
float mat_specular[] = {1.0,1.0,1.0,1.0};
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
   glutInitWindowSize(1024, 768);
   glutInitWindowPosition(100 , 50);
   glutCreateWindow("teapot test");

   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_MULTISAMPLE_ARB);

setup_the_viewvol();
do_lights();
do_material();

// for( int i=0; i<vertices.size(); ++i)
//   std::cout << vertices[i].x <<","<<vertices[i].y<<","<<vertices[i].z<<
//       std::endl;


/* gray background */
//glClearColor(0.35,0.35,0.35,0.0);
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
