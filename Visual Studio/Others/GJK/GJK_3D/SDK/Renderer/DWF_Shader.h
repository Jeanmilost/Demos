/****************************************************************************
 * ==> DWF_Shader ----------------------------------------------------------*
 ****************************************************************************
 * Description: Basic shader language class                                 *
 * Developer:   Jean-Milost Reymond                                         *
 ****************************************************************************
 * MIT License - DwarfStar Game Engine                                      *
 *                                                                          *
 * Permission is hereby granted, free of charge, to any person obtaining a  *
 * copy of this software and associated documentation files (the            *
 * "Software"), to deal in the Software without restriction, including      *
 * without limitation the rights to use, copy, modify, merge, publish,      *
 * distribute, sub-license, and/or sell copies of the Software, and to      *
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

#pragma once

// std
#include <map>
#include <string>

namespace DWF_Renderer
{
    /**
    * Basic shader language class
    *@author Jean-Milost Reymond
    */
    class Shader
    {
        public:
            /**
            * Shader type enumeration
            */
            enum class IEType
            {
                IE_ST_Vertex,
                IE_ST_Fragment
            };

            /**
            * Shader attributes
            */
            enum class IEAttribute
            {
                IE_SA_Vertices,
                IE_SA_Normal,
                IE_SA_Texture,
                IE_SA_Color,
                IE_SA_ProjectionMatrix,
                IE_SA_ViewMatrix,
                IE_SA_ModelMatrix,
                IE_SA_TextureSampler
            };

            Shader();
            virtual ~Shader();

            /**
            * Gets attribute name
            *@param attribute - attribute to get name
            *@return attribute name
            */
            virtual std::string GetAttributeName(IEAttribute attribute) const;

            /**
            * Sets attribute name
            *@param attribute - attribute to set name
            *@param name - new attribute name
            */
            virtual void SetAttributeName(IEAttribute  attribute, const std::string& name);

            /**
            * Creates the program
            */
            virtual void CreateProgram() = 0;

            /**
            * Gets shader program identifier
            *@return shader program identifier
            */
            virtual std::uintptr_t GetProgramID() const = 0;

            /**
            * Attaches shader to program from file
            *@param fileName - shader file
            *@param type - shader type
            *@return compiled shader identifier
            */
            virtual std::uintptr_t AttachFile(const std::string& fileName, IEType type) = 0;

            /**
            * Attaches shader to program
            *@param source - shader source code
            *@param type - shader type
            *@return compiled shader identifier
            */
            virtual std::uintptr_t Attach(const std::string& source, IEType type) = 0;

            /**
            * Links all attached shader and keep program ready to run
            *@param use - if true, program will be used immediately (in case link succeeded)
            *@return true on success, otherwise false
            */
            virtual bool Link(bool use) const = 0;

            /**
            * Uses the program
            *@param use - if true, program will be used, released otherwise
            */
            virtual void Use(bool use) const = 0;

        private:
            typedef std::map<IEAttribute, std::string> IAttributeDictionary;

            IAttributeDictionary m_AttributeDictionary;

            /**
            * Populates default attribute dictionary
            */
            void PopulateAttributeDict();
    };
}
