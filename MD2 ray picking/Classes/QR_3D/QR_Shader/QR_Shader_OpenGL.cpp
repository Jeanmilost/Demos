/****************************************************************************
 * ==> QR_Shader_OpenGL ----------------------------------------------------*
 ****************************************************************************
 * Description : Shader language implemented for OpenGL                     *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - QR Engine                                                  *
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

#include "QR_Shader_OpenGL.h"

// std
#include <fstream>
#include <sstream>

// qr engine
#include "QR_Exception.h"

//---------------------------------------------------------------------------
// QR_Shader_OpenGL
//---------------------------------------------------------------------------
QR_Shader_OpenGL::QR_Shader_OpenGL() :
    m_ProgramID(0),
    m_VertexID(0),
    m_FragmentID(0),
    m_fOnBindAttribute(NULL)
{}
//---------------------------------------------------------------------------
QR_Shader_OpenGL::~QR_Shader_OpenGL()
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
void QR_Shader_OpenGL::CreateProgram()
{
    // create new shader program
    m_ProgramID = glCreateProgram();

    // succeeded?
    if (!m_ProgramID)
        M_THROW_EXCEPTION("Failed to create shader program");
}
//---------------------------------------------------------------------------
QR_UIntPtrT QR_Shader_OpenGL::GetProgramID() const
{
    return m_ProgramID;
}
//---------------------------------------------------------------------------
QR_UIntPtrT QR_Shader_OpenGL::AttachFile(const std::string& fileName, IEType type)
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
        case IE_ST_Vertex:   m_VertexID   = shaderID; break;
        case IE_ST_Fragment: m_FragmentID = shaderID; break;
        default:             M_THROW_EXCEPTION("Unknown shader type");
    }

    return shaderID;
}
//---------------------------------------------------------------------------
QR_UIntPtrT QR_Shader_OpenGL::Attach(const std::string& source, IEType type)
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
        case IE_ST_Vertex:   m_VertexID   = shaderID; break;
        case IE_ST_Fragment: m_FragmentID = shaderID; break;
        default:             M_THROW_EXCEPTION("Unknown shader type");
    }

    return shaderID;
}
//---------------------------------------------------------------------------
bool QR_Shader_OpenGL::Link(bool use) const
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
void QR_Shader_OpenGL::Use(bool use) const
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
GLenum QR_Shader_OpenGL::ShaderTypeToOpenGLShaderType(IEType type)
{
    switch (type)
    {
        case IE_ST_Vertex:   return GL_VERTEX_SHADER;
        case IE_ST_Fragment: return GL_FRAGMENT_SHADER;
        default:             M_THROW_EXCEPTION("Unknown shader type");
    }
}
//---------------------------------------------------------------------------
QR_Shader::IEType QR_Shader_OpenGL::OpenGLTypeToShaderType(GLenum type)
{
    switch (type)
    {
        case GL_VERTEX_SHADER:   return IE_ST_Vertex;
        case GL_FRAGMENT_SHADER: return IE_ST_Fragment;
        default:                 M_THROW_EXCEPTION("Unknown shader type");
    }
}
//---------------------------------------------------------------------------
void QR_Shader_OpenGL::Set_OnBindAttribute(ITfOnBindAttribute fHandler)
{
    m_fOnBindAttribute = fHandler;
}
//---------------------------------------------------------------------------
GLuint QR_Shader_OpenGL::CompileFile(const std::string& fileName, GLenum type) const
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
GLuint QR_Shader_OpenGL::Compile(const std::string& source, GLenum type) const
{
    // create new shader
    GLuint shader = glCreateShader(type);

    // get source buffer
    const QR_Char* pSource = source.c_str();

    // compile shader
    glShaderSource(shader, 1, &pSource, NULL);
    glCompileShader(shader);

    // failed?
    if (!shader)
        return 0;

    // return shader index
    return shader;
}
//---------------------------------------------------------------------------
