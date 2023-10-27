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

#include "DWF_Shader.h"

using namespace DWF_Renderer;

 //---------------------------------------------------------------------------
 // Shader
 //---------------------------------------------------------------------------
Shader::Shader()
{
    PopulateAttributeDict();
}
//---------------------------------------------------------------------------
Shader::~Shader()
{}
//---------------------------------------------------------------------------
std::string Shader::GetAttributeName(IEAttribute attribute) const
{
    // search for attribute to get name
    IAttributeDictionary::const_iterator it = m_AttributeDictionary.find(attribute);

    // found it?
    if (it == m_AttributeDictionary.end())
        return "";

    return it->second;
}
//---------------------------------------------------------------------------
void Shader::SetAttributeName(IEAttribute  attribute, const std::string& name)
{
    // is name empty?
    if (!name.length())
        return;

    // change attribute name
    m_AttributeDictionary[attribute] = name;
}
//---------------------------------------------------------------------------
void Shader::PopulateAttributeDict()
{
    m_AttributeDictionary[IEAttribute::IE_SA_Vertices]         = "aVertices";
    m_AttributeDictionary[IEAttribute::IE_SA_Normal]           = "aNormal";
    m_AttributeDictionary[IEAttribute::IE_SA_Texture]          = "aTexCoord";
    m_AttributeDictionary[IEAttribute::IE_SA_Color]            = "aColor";
    m_AttributeDictionary[IEAttribute::IE_SA_ProjectionMatrix] = "uProjection";
    m_AttributeDictionary[IEAttribute::IE_SA_ViewMatrix]       = "uView";
    m_AttributeDictionary[IEAttribute::IE_SA_ModelMatrix]      = "uModel";
    m_AttributeDictionary[IEAttribute::IE_SA_TextureSampler]   = "sTexture";
}
//---------------------------------------------------------------------------
