#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Macro for detecting an openGL error.
// Only works if the program is compiled in debug mode.
// 
// Usage:
// GL_CHECK(myfunction); // In case of OpenGL call
// auto v = GL_CHECK(myfunction_withreturn); // In case of function returning values
// GL_CHECK("My checkpoint"); // In case of no function call
// 
// Note calling GL_CHECK(); will result to an error (arg stmt is missing)
inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        // Flush a error message in the error exit
        std::cerr << "OpenGL error " << (int)err << " at " << fname << ":" << line << " for " << stmt << "\n";
        std::cerr.flush();
        // Raise exception and terminate the program. 
        // Can be commented out if needed
        abort(); 
    }
}
#ifdef _DEBUG
#define GL_CHECK(stmt) stmt;  \
        CheckOpenGLError(#stmt, __FILE__, __LINE__);
#else
#define GL_CHECK(stmt) stmt
#endif

// Helper object that simplify the shader loading and interactions
// Can be extended if necessary
class ShaderProgram
{
public:
   // ------------------------------------------------------------------------
   // constructor
   ShaderProgram();
   
   // ------------------------------------------------------------------------
   // attach shader from sources 
   // return true if sucessfull
   bool addShaderFromSource(GLenum type, const std::string& path);
   
   // ------------------------------------------------------------------------
   // link the different shaders to make a full program 
   // return true if sucessfull
   bool link();

   // ------------------------------------------------------------------------
   // get program ID to interact directly with the shader program
   inline GLuint programId() const { return m_ID; }

   // ------------------------------------------------------------------------
   // use shader program
   inline void bind() const { 
       if(!m_linked) {
           // Warn user
           std::cerr << "Shader is not properly linked!\n";
       }
       glUseProgram(m_ID); 
   }
   

   // get id value corresponding to attribute
    // ------------------------------------------------------------------------
   inline int attributeLocation(const char* name) const { return glGetAttribLocation(m_ID, name); }
   inline int attributeLocation(const std::string name) const { return glGetAttribLocation(m_ID, name.c_str()); }

    // utility uniform functions
    // ------------------------------------------------------------------------
    inline void setBool(const std::string& name, bool value) const { glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value); }

    // ------------------------------------------------------------------------
    inline void setInt(const std::string& name, int value) const { glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value); }

    // ------------------------------------------------------------------------
    inline void setFloat(const std::string& name, float value) const { glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value); }

    // ------------------------------------------------------------------------
    inline void setMat4(const std::string& name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    // ------------------------------------------------------------------------
    inline void setMat3(const std::string& name, const glm::mat3& mat) const { glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]); }

    // ------------------------------------------------------------------------
    inline void setVec4(const std::string& name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]); }

    // ------------------------------------------------------------------------
    inline void setVec3(const std::string& name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]); }

private:
    // Shader program id
    GLuint m_ID;
    // Does the shader is link?
    bool m_linked = false;
    // List of the different shaders (can be reused if necessary)
    std::map<std::string, GLuint> m_shaders_ids;
};
#endif