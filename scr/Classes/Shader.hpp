#ifndef SHADER_HPP
#define SHADER_HPP
#include "Util.hpp"
#include "GL.hpp"
class Shader {
public:
    Shader();
    Shader(const Shader& orig);
    virtual ~Shader();
    static string loadStr(string path);
    static bool compileShader(string vertexPath, string fragmentPath, bool bindTexCoord0, bool bindNormal, bool bindColor);
    static void deleteShader();
    static GLuint shaderID;
    static GLuint vertexID;
    static GLuint fragmentID;
    static bool isOn;
private:

};

#endif /* SHADER_HPP */

