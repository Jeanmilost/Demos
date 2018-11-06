/****************************************************************************
 * ==> QR_Resources --------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a texture class and his associated manager and    *
 *               helper                                                     *
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

#ifndef QR_ResourcesH
#define QR_ResourcesH

// qr engine
#include "QR_Texture.h"
#include "QR_ModelGroup.h"
#ifdef USE_SHADER
    #include "QR_Shader.h"
#endif

/**
* Shared 3D resources
*/
class QR_Resources
{
    public:
        /**
        * Texture dictionary, used to register the texture resources
        */
        typedef std::map<QR_SizeT, QR_Texture*> ITextureDictionary;

        /**
        * Model dictionary, used to register the model resources
        */
        typedef std::map<QR_SizeT, QR_ModelGroup*> IModelDictionary;

        /**
        * Shader dictionary, used to register the shader resources
        */
        #ifdef USE_SHADER
            typedef std::map<QR_SizeT, QR_Shader*> IShaderDictionary;
        #endif

        QR_Resources();
        virtual ~QR_Resources();

        /**
        * Gets the texture dictionary
        *@return texture dictionary
        */
        virtual inline ITextureDictionary* GetTextureDictionary() const;

        /**
        * Gets the model dictionary
        *@return model dictionary
        */
        virtual inline IModelDictionary* GetModelDictionary() const;

        /**
        * Gets the shader dictionary
        *@return shader dictionary
        */
        #ifdef USE_SHADER
            virtual inline IShaderDictionary* GetShaderDictionary() const;
        #endif

    private:
        ITextureDictionary    m_TextureDictionary;
        IModelDictionary      m_ModelDictionary;
        #ifdef USE_SHADER
            IShaderDictionary m_ShaderDictionary;
        #endif
};

//---------------------------------------------------------------------------
// QR_Resources
//---------------------------------------------------------------------------
QR_Resources::ITextureDictionary* QR_Resources::GetTextureDictionary() const
{
    return const_cast<ITextureDictionary*>(&m_TextureDictionary);
}
//---------------------------------------------------------------------------
QR_Resources::IModelDictionary* QR_Resources::GetModelDictionary() const
{
    return const_cast<IModelDictionary*>(&m_ModelDictionary);
}
//---------------------------------------------------------------------------
#ifdef USE_SHADER
    QR_Resources::IShaderDictionary* QR_Resources::GetShaderDictionary() const
    {
        return const_cast<IShaderDictionary*>(&m_ShaderDictionary);
    }
#endif
//---------------------------------------------------------------------------

#endif
