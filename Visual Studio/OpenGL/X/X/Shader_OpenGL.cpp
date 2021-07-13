/****************************************************************************
 * ==> Shader_OpenGL -------------------------------------------------------*
 ****************************************************************************
 * Description : Shader language implemented for OpenGL                     *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - x model reader                                             *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sublicense, and/or sell copies of the Software, and to       *
 * permit persons to whom the Software is furnished to do so, subject to    *
 * the following conditions:                                                *
 *                                                                          *
 * The above copyright notice and this permission notice shall be included  *
 * in all copies or substantial portions of the Software.                   *
 *                                                                          *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  *
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               *
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   *
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     *
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        *
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   *
 ****************************************************************************/

#include "Shader_OpenGL.h"

 // std
#include <fstream>
#include <sstream>

//---------------------------------------------------------------------------
// Shader_OpenGL
//---------------------------------------------------------------------------
Shader_OpenGL::Shader_OpenGL() :
    Shader(),
    m_ProgramID(0),
    m_VertexID(0),
    m_FragmentID(0),
    m_fOnBindAttribute(nullptr)
{}
//---------------------------------------------------------------------------
Shader_OpenGL::~Shader_OpenGL()
{
    // delete vertex shader, if needed
    if (m_VertexID)
    {
        glDetachShader(m_ProgramID, m_VertexID);
        glDeleteShader(m_VertexID);
        m_VertexID = 0;
    }

    // delete fragment shader, if needed
    if (m_FragmentID)
    {
        glDetachShader(m_ProgramID, m_FragmentID);
        glDeleteShader(m_FragmentID);
        m_FragmentID = 0;
    }

    // delete program, if needed
    if (m_ProgramID)
    {
        glDeleteProgram(m_ProgramID);
        m_ProgramID = 0;
    }
}
//---------------------------------------------------------------------------
void Shader_OpenGL::CreateProgram()
{
    // create new shader program
    m_ProgramID = glCreateProgram();

    // succeeded?
    if (!m_ProgramID)
        throw new std::exception("Failed to create shader program");
}
//---------------------------------------------------------------------------
std::uintptr_t Shader_OpenGL::GetProgramID() const
{
    return m_ProgramID;
}
//---------------------------------------------------------------------------
std::uintptr_t Shader_OpenGL::AttachFile(const std::string& fileName, IEType type)
{
    // compile shader
    GLuint shaderID = CompileFile(fileName, ShaderTypeToOpenGLShaderType(type));

    // succeeded?
    if (!shaderID)
        return 0;

    // attach shader to program
    glAttachShader(m_ProgramID, shaderID);

    // do bind attributes?
    if (m_fOnBindAttribute)
        m_fOnBindAttribute(m_ProgramID, type);

    // search for shader type
    switch (type)
    {
        case IEType::IE_ST_Vertex:   m_VertexID   = shaderID; break;
        case IEType::IE_ST_Fragment: m_FragmentID = shaderID; break;
        default:                     throw new std::exception("Unknown shader type");
    }

    return shaderID;
}
//---------------------------------------------------------------------------
std::uintptr_t Shader_OpenGL::Attach(const std::string& source, IEType type)
{
    // compile shader
    const GLuint shaderID = Compile(source, ShaderTypeToOpenGLShaderType(type));

    // succeeded?
    if (!shaderID)
        return 0;

    // attach shader to program
    glAttachShader(m_ProgramID, shaderID);

    // do bind attributes?
    if (m_fOnBindAttribute)
        m_fOnBindAttribute(m_ProgramID, type);

    // search for shader type
    switch (type)
    {
        case IEType::IE_ST_Vertex:   m_VertexID   = shaderID; break;
        case IEType::IE_ST_Fragment: m_FragmentID = shaderID; break;
        default:                     throw new std::exception("Unknown shader type");
    }

    return shaderID;
}
//---------------------------------------------------------------------------
bool Shader_OpenGL::Link(bool use) const
{
    // link program
    glLinkProgram(m_ProgramID);

    GLint linked;

    // query program to know if link succeeded
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &linked);

    // succeeded?
    if (!linked)
        return false;

    // do use linked program immediately?
    if (use)
        Use(true);

    return true;
}
//---------------------------------------------------------------------------
void Shader_OpenGL::Use(bool use) const
{
    // do use program and program exists?
    if (use && m_ProgramID)
        // bind program
        glUseProgram(m_ProgramID);
    else
        // unbind program
        glUseProgram(0);
}
//---------------------------------------------------------------------------
GLenum Shader_OpenGL::ShaderTypeToOpenGLShaderType(IEType type)
{
    switch (type)
    {
        case IEType::IE_ST_Vertex:   return GL_VERTEX_SHADER;
        case IEType::IE_ST_Fragment: return GL_FRAGMENT_SHADER;
        default:                     throw new std::exception("Unknown shader type");
    }
}
//---------------------------------------------------------------------------
Shader::IEType Shader_OpenGL::OpenGLTypeToShaderType(GLenum type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER:   return IEType::IE_ST_Vertex;
        case GL_FRAGMENT_SHADER: return IEType::IE_ST_Fragment;
        default:                 throw new std::exception("Unknown shader type");
    }
}
//---------------------------------------------------------------------------
void Shader_OpenGL::Set_OnBindAttribute(ITfOnBindAttribute fHandler)
{
    m_fOnBindAttribute = fHandler;
}
//---------------------------------------------------------------------------
GLuint Shader_OpenGL::CompileFile(const std::string& fileName, GLenum type) const
{
    // open shader file
    std::ifstream shaderFile(fileName.c_str());

    std::ostringstream sstr;
    std::string        line;

    // iterate through file lines
    while (std::getline(shaderFile, line))
        // add line to source buffer to compile
        sstr << line << "\n";

    return Compile(sstr.str(), type);
}
//---------------------------------------------------------------------------
GLuint Shader_OpenGL::Compile(const std::string& source, GLenum type) const
{
    // create new shader
    GLuint shader = glCreateShader(type);

    // get source buffer
    const char* pSource = source.c_str();

    // compile shader
    glShaderSource(shader, 1, &pSource, nullptr);
    glCompileShader(shader);

    // failed?
    if (!shader)
        return 0;

    // return shader index
    return shader;
}
//---------------------------------------------------------------------------
