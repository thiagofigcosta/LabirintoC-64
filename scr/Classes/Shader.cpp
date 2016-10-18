#include "Shader.hpp"

Shader::Shader() {
}

Shader::Shader(const Shader& orig) {
}

Shader::~Shader() {
}

GLuint Shader::shaderID;
GLuint Shader::vertexID;
GLuint Shader::fragmentID;
bool Shader::isOn=true;

string Shader::loadStr(string path){
    path=Util::newPath((char*)path.c_str());
    fstream file(path);
    if(!file.is_open()) {
            cout << "Falha ao abrir shader " << path << endl;
            return "";
    }
    stringstream fileData;
    fileData << file.rdbuf();
    file.close();
    return fileData.str();
}

bool Shader::compileShader(string vertexPath, string fragmentPath, bool bindTexCoord0, bool bindNormal, bool bindColor){
   string vertex=loadStr(vertexPath);
   string fragment=loadStr(fragmentPath);
   int vSize=vertex.size(),fSize=fragment.size();
   const char *vAChar=vertex.c_str();
   const char *fAChar=fragment.c_str();
   if(fragment==""||vertex=="") return false;
   vertexID=glCreateShader(GL_VERTEX_SHADER);
   fragmentID=glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(vertexID, 1, (const GLchar **)&vAChar, &vSize);
   glShaderSource(fragmentID, 1, (const GLchar **)&fAChar, &fSize);
   GLint compiled;
    glCompileShader(vertexID);
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
            cout<<"Vertex shader nao compilou"<<endl;
//            printShaderInfoLog(vertexID);
            glDeleteShader(vertexID);
            vertexID=0;
            glDeleteShader(fragmentID);
            fragmentID=0;
            return false;
    }
    glCompileShader(fragmentID);
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &compiled);
    if(!compiled){
            cout<<"Fragment shader n compilu"<<endl;
 //           printShaderInfoLog(fragmentID);
            glDeleteShader(vertexID);
            vertexID=0;
            glDeleteShader(fragmentID);
            fragmentID=0;
            return false;
    }
    shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexID);
    glAttachShader(shaderID, fragmentID);
    glBindAttribLocation(shaderID, 0, "InVertex");
    if(bindTexCoord0) glBindAttribLocation(shaderID, 1, "InTexCoord0");
    if(bindNormal) glBindAttribLocation(shaderID, 2, "InNormal");
    if(bindColor) glBindAttribLocation(shaderID, 3, "InColor");
    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, (GLint *)&compiled);
    if(!compiled){
            cout<<"Shader n linkou"<<endl;
            glDetachShader(shaderID, vertexID);
            glDetachShader(shaderID, fragmentID);
            glDeleteShader(vertexID);
            vertexID=0;
            glDeleteShader(fragmentID);
            fragmentID=0;
            glDeleteProgram(shaderID);
            shaderID=0;
            return false;
    }
    return true;
}

void Shader::deleteShader(){
    glDeleteShader(vertexID);
    vertexID=0;
    glDeleteShader(fragmentID);
    fragmentID=0;
    glDeleteProgram(shaderID);
    shaderID=0;
    glUseProgram(0);
}