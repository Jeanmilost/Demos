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

#ifndef QR_SHADER_OPENGLH
#define QR_SHADER_OPENGLH

// std
#include <string>

// qr engine
#include "QR_Types.h"
#include "QR_Shader.h"

// openGL
#if defined (OS_WIN)
    #define GLEW_STATIC
    #include <GL/glew.h>
#elif defined (OS_OSX)
    #include <ES2/gl.h>
#endif

/**
* OpenGL shader language
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_Shader_OpenGL : public QR_Shader
{
    public:
        /**
        * Called when a generic attribute should be associated with a named variable
        *@param programID - shader program identifier
        *@param type - shader type
        */
        typedef void (*ITfOnBindAttribute)(QR_UIntPtrT programID, IEType type);

        QR_Shader_OpenGL();
        virtual ~QR_Shader_OpenGL();

        /**
        * Creates the program
        */
        virtual void CreateProgram();

        /**
        * Gets shader program identifier
        *@return shader program identifier
        */
        virtual QR_UIntPtrT GetProgramID() const;

        /**
        * Attaches shader to program from file
        *@param fileName - shader file
        *@param type - shader type
        *@return compiled shader identifier
        */
        virtual QR_UIntPtrT AttachFile(const std::string& fileName, IEType type);

        /**
        * Attaches shader to program
        *@param source - shader source code
        *@param type - shader type
        *@return compiled shader identifier
        */
        virtual QR_UIntPtrT Attach(const std::string& source, IEType type);

        /**
        * Links all attached shader and keep program ready to run
        *@param use - if true, program will be used immediately (in case link succeeded)
        *@return true on success, otherwise false
        */
        virtual bool Link(bool use) const;

        /**
        * Uses the program
        *@param use - if true, program will be used, released otherwise
        */
        virtual void Use(bool use) const;

        /**
        * Converts shader type to OpenGL shader type
        *@param type - shader type to convert
        *@return converted OpenGL shader type
        */
        static GLenum ShaderTypeToOpenGLShaderType(IEType type);

        /**
        * Converts OpenGL shader type to shader type
        *@param type - OpenGL shader type to convert
        *@return converted shader type
        */
        static IEType OpenGLTypeToShaderType(GLenum type);

        /**
        * Sets OnBindAttribute callback
        *@param fHandler - function handler
        */
        virtual void Set_OnBindAttribute(ITfOnBindAttribute fHandler);

    protected:
        /**
        * Compiles shader from file
        *@param shaderFile - shader file to compile
        *@param type - shader type
        *@return compiled shader identifier
        */
        virtual GLuint CompileFile(const std::string& shaderFile, GLenum type) const;

        /**
        * Compiles shader
        *@param source - shader source code
        *@param type - shader type
        *@return compiled shader identifier
        */
        virtual GLuint Compile(const std::string& source, GLenum type) const;

    private:
        GLuint             m_ProgramID;
        GLuint             m_VertexID;
        GLuint             m_FragmentID;
        ITfOnBindAttribute m_fOnBindAttribute;
};

#endif
