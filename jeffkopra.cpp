#include "initShaders.h"
using namespace std;
int counter=0;//counter to increase how many vertices are going to be drawn at a given time

GLuint vaoID, vboID[2];//vao and vbo names

GLfloat vertexarray[]={	0.5f,0.5f,0.0f,//vertice array
					0.85f,0.35f,0.0f,
				1.0f,0.0f,0.0f,
					0.85f,-0.35f,0.0f,
                       	0.5f,-0.5f,0.0f, 
                       		0.35f,-0.85f,0.0f,
                       	0.0f,-1.0f,0.0f,
                       		-0.35f,-0.85f,0.0f,
                       	-0.5f,-0.5f,0.0f, 
                       		-0.85f,-0.35f,0.0f,
                        -1.0f,0.0f,0.0f,
                        	-0.85f,0.35f,0.0f,
                       	-0.5f,0.5f,0.0f,
                       		-0.35f,0.85f,0.0f,
                      	0.0f,1.0f,0.0f,
                      		0.35f,0.85f,0.0f
                      };
                       
GLfloat colorarray[]={ 1.0f,1.0f,0.0f,1.0f,//color array
			     		1.0f,1.0f,0.0f,1.0f,
                       0.0f,1.0f,0.0f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       1.0f,0.0f,1.0f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       0.5f,0.5f,1.0f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       1.0f,0.5f,0.5f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       0.0f,1.0f,0.5f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       0.5f,0.5f,0.5f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       1.0f,0.5f,1.0f,1.0f,
                       		1.0f,1.0f,0.0f,1.0f,
                       };                       

void init(){

  ShaderInfo shaders[]={
  { GL_VERTEX_SHADER , "vertexshader.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);
  
  glGenVertexArrays(1, &vaoID);
  glBindVertexArray(vaoID);
  
  glGenBuffers(2, vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertexarray),vertexarray,GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
  glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(colorarray),colorarray,GL_STATIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

void drawscene(){
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_POLYGON,0,3+(counter%14));
  glFlush();
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    counter++;
    drawscene();
  }
}


int main(int argc,char ** argv){

  glutInit(&argc, argv);
  glutCreateWindow("Shapes");//creates the window with the specified name
  
  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  glutInitContextVersion(4, 3);//specifies the version of opengl
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);//specifies what profile your using
	
	init();

  //retruns what version of opengl and glsl your computer can use
  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version=glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);//displays callback draws the shapes
  glutMouseFunc(mousepress);//control callback specifies the mouse controls
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
