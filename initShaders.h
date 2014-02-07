#ifndef INITSHADERS_H_
#define INITSHADERS_H_

//#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <vector>
#include <cstdio>
#include <iostream>
using namespace std;

//prototypes
GLuint createShader(GLenum type, const GLchar* shadeSource);
const GLchar* inputShader(const char* filename);
GLuint createProgram(const vector<GLuint> shadeList);

typedef struct{
  GLenum type;
  const char* filename;
} ShaderInfo;

//create the shaders for your program
void initShaders(ShaderInfo* shaders){
  
  ShaderInfo* shade=shaders;
  vector<GLuint> Shadelist;//initialize list
  
  while(shade->type != GL_NONE){//goes through all the shaders
    Shadelist.push_back(createShader(shade->type,inputShader(shade->filename)));//adding shaders
    ++shade;
  }
  
  GLuint program=createProgram(Shadelist);//creates the program to link to shaders
  glUseProgram(program);//installs a program object
}

//loads the shader from vertex and fragments shaders 
const GLchar* inputShader(const char* filename){

  FILE* fshade = fopen(filename, "rb");
  
  if(!fshade){//is file opened ??
    fprintf(stderr,"unable to open file '%s'\n",filename);
    return NULL;
  }
  
  //length of the file == ??
  fseek(fshade, 0, SEEK_END);
  long filesize=ftell(fshade);
  fseek(fshade, 0, SEEK_SET);
  
  //allocates memory and read in file
  GLchar* shadingSource= new GLchar[filesize+1];//
  fread(shadingSource, 1, filesize, fshade);
  
  if(ftell(fshade) == 0){//is file empty ??
    fprintf(stderr, "File '%s' is empty.\n",filename);
    return NULL;
  }

  fclose(fshade);//close file
  
  shadingSource[filesize] = 0;//set end of file
  
  return const_cast<const GLchar*>(shadingSource);//overloads the const so the value with change per file  
  
  //NOTE: if the file is unable to open or is empty this function will segmentation fault your program
}

//creates shader
GLuint createShader(GLenum type, const GLchar* shadeSource){
  
  GLuint shader = glCreateShader(type);//create shader based on type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
  glShaderSource(shader, 1, &shadeSource, NULL);
  
  glCompileShader(shader);//compiles shader
  
  GLint compileStatus;//compilation status
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);//comiple status
  
  if(!compileStatus){//did shader compile?
    GLint logSize;//debug info size
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);//source file size
    
    GLchar* infoLog = new GLchar[logSize+1];//allocating memory for the debug info
    glGetShaderInfoLog(shader,logSize,&logSize,infoLog);//error messages
    
    const char *shadeInfo= NULL;//char array of shaders that have errors
    switch(type){// what shader has error??
      case GL_VERTEX_SHADER: shadeInfo = "vertex"; break;
      case GL_GEOMETRY_SHADER_EXT: shadeInfo = "geometric"; break;
      case GL_FRAGMENT_SHADER: shadeInfo = "fragment"; break;
    }
    fprintf(stderr,"\nCompile failure in %u shader: %s\n Error message:\n%s\n",type,shadeInfo,infoLog);
    delete[] infoLog;//memory management
  }
  return shader;
}

//creates the shading program to link the shader too
GLuint createProgram(const vector<GLuint> shadeList){

  GLuint program = glCreateProgram();//creates program
  
  for(GLuint i=0;i<shadeList.size();i++){glAttachShader(program,shadeList[i]);}//attaches shaders to program
  
  glBindAttribLocation(program, 0, "in_position");//binds attribute to a program
  glBindAttribLocation(program, 1, "in_color");
  glLinkProgram(program);//links program to your program
  
  GLint linkStatus;//linking variable status
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  
  if(!linkStatus){//is program linked to the program ??
    GLint logSize;//size of debug info
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);//linking status
    
    GLchar *infoLog = new GLchar[logSize+1];//allocating memory for debug info
    glGetProgramInfoLog(program,logSize,&logSize,infoLog);/
    
    fprintf(stderr,"\nShader linking failed: %s\n",infoLog);
    delete[] infoLog;//memory management

    for(GLuint i=0;i<shadeList.size();i++){glDeleteShader(shadeList[i]);}//memory management
  }
  return program;//self explanatory
}
#endif
