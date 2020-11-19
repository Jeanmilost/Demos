/****************************************************************************
 * ==> XModel --------------------------------------------------------------*
 ****************************************************************************
 * Description : DirectX .x model                                           *
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

#include "XModel.h"

// std
#include <stdlib.h>
#include <math.h>
#include <string.h>

// classes
#include "Quaternion.h"

#pragma warning(disable: 26812)

//---------------------------------------------------------------------------
// Global defines
//---------------------------------------------------------------------------
#define M_X_FORMAT_MAGIC         ((' ' << 24) + ('f' << 16) + ('o' << 8) + 'x')
#define M_X_FORMAT_VERSION03     (('3' << 8)  +  '0')
#define M_X_FORMAT_VERSION02     (('2' << 8)  +  '0')
#define M_X_FORMAT_BINARY        ((' ' << 24) + ('n' << 16) + ('i' << 8) + 'b')
#define M_X_FORMAT_TEXT          ((' ' << 24) + ('t' << 16) + ('x' << 8) + 't')
#define M_X_FORMAT_COMPRESSED    ((' ' << 24) + ('p' << 16) + ('m' << 8) + 'c')
#define M_X_FORMAT_FLOAT_BITS_32 (('2' << 24) + ('3' << 16) + ('0' << 8) + '0')
#define M_X_FORMAT_FLOAT_BITS_64 (('4' << 24) + ('6' << 16) + ('0' << 8) + '0')
//---------------------------------------------------------------------------
// XModel::IBone
//---------------------------------------------------------------------------
XModel::IBone::IBone() :
    m_Matrix(Matrix4x4F::Identity()),
    m_pParent(nullptr),
    m_MeshIndex(0)
{}
//---------------------------------------------------------------------------
XModel::IBone::~IBone()
{
    const std::size_t count = m_Children.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Children[i];
}
//---------------------------------------------------------------------------
// XModel::IMeshToBoneItem
//---------------------------------------------------------------------------
XModel::IMeshToBoneItem::IMeshToBoneItem() :
    m_pBone(nullptr),
    m_MeshIndex(0)
{}
//---------------------------------------------------------------------------
XModel::IMeshToBoneItem::~IMeshToBoneItem()
{}
//---------------------------------------------------------------------------
// XModel::ISkinWeightIndexTable
//---------------------------------------------------------------------------
XModel::ISkinWeightIndexTable::ISkinWeightIndexTable()
{}
//---------------------------------------------------------------------------
XModel::ISkinWeightIndexTable::~ISkinWeightIndexTable()
{}
//---------------------------------------------------------------------------
// XModel::ISkinWeights
//---------------------------------------------------------------------------
XModel::ISkinWeights::ISkinWeights() :
    m_pBone(nullptr),
    m_Matrix(Matrix4x4F::Identity()),
    m_MeshIndex(0)
{}
//---------------------------------------------------------------------------
XModel::ISkinWeights::~ISkinWeights()
{
    const std::size_t count = m_IndexTable.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_IndexTable[i];
}
//---------------------------------------------------------------------------
// XModel::IMeshSkinWeights
//---------------------------------------------------------------------------
XModel::IMeshSkinWeights::IMeshSkinWeights()
{}
//---------------------------------------------------------------------------
XModel::IMeshSkinWeights::~IMeshSkinWeights()
{
    const std::size_t count = m_SkinWeights.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_SkinWeights[i];
}
//---------------------------------------------------------------------------
// XModel::IAnimationKey
//---------------------------------------------------------------------------
XModel::IAnimationKey::IAnimationKey() :
    m_Frame(0)
{}
//---------------------------------------------------------------------------
XModel::IAnimationKey::~IAnimationKey()
{}
//---------------------------------------------------------------------------
// XModel::IAnimationKeys
//---------------------------------------------------------------------------
XModel::IAnimationKeys::IAnimationKeys() :
    m_Type(IE_KT_Unknown)
{}
//---------------------------------------------------------------------------
XModel::IAnimationKeys::~IAnimationKeys()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// XModel::IAnimation
//---------------------------------------------------------------------------
XModel::IAnimation::IAnimation() :
    m_pBone(nullptr)
{}
//---------------------------------------------------------------------------
XModel::IAnimation::~IAnimation()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// XModel::IAnimationSet
//---------------------------------------------------------------------------
XModel::IAnimationSet::IAnimationSet()
{}
//---------------------------------------------------------------------------
XModel::IAnimationSet::~IAnimationSet()
{
    const std::size_t count = m_Animations.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Animations[i];
}
//---------------------------------------------------------------------------
// XModel::IModel
//---------------------------------------------------------------------------
XModel::IModel::IModel() :
    m_pSkeleton(nullptr),
    m_MeshOnly(false),
    m_PoseOnly(false)
{}
//---------------------------------------------------------------------------
XModel::IModel::~IModel()
{}
//---------------------------------------------------------------------------
// XModel::IVector2
//---------------------------------------------------------------------------
XModel::IVector2::IVector2() :
    m_X(0.0f),
    m_Y(0.0f)
{}
//---------------------------------------------------------------------------
XModel::IVector2::~IVector2()
{}
//---------------------------------------------------------------------------
// XModel::IFileHeader
//---------------------------------------------------------------------------
XModel::IFileHeader::IFileHeader() :
    m_Magic(0),
    m_Major_Version(0),
    m_Minor_Version(0),
    m_Format(0),
    m_Float_Size(0)
{}
//---------------------------------------------------------------------------
XModel::IFileHeader::~IFileHeader()
{}
//---------------------------------------------------------------------------
// XModel::IGenericDataset
//---------------------------------------------------------------------------
XModel::IGenericDataset::IGenericDataset()
{}
//---------------------------------------------------------------------------
XModel::IGenericDataset::~IGenericDataset()
{}
//---------------------------------------------------------------------------
// XModel::IHeaderDataset
//---------------------------------------------------------------------------
XModel::IHeaderDataset::IHeaderDataset() :
    IGenericDataset(),
    m_Major(0),
    m_Minor(0),
    m_Flags(0),
    m_ReadValCount(0)
{}
//---------------------------------------------------------------------------
XModel::IHeaderDataset::~IHeaderDataset()
{}
//---------------------------------------------------------------------------
// XModel::IMatrixDataset
//---------------------------------------------------------------------------
XModel::IMatrixDataset::IMatrixDataset() :
    IGenericDataset(),
    m_Matrix(Matrix4x4F::Identity()),
    m_ReadValCount(0)
{}
//---------------------------------------------------------------------------
XModel::IMatrixDataset::~IMatrixDataset()
{}
//---------------------------------------------------------------------------
// XModel::IVertexBufferDataset
//---------------------------------------------------------------------------
XModel::IVertexBufferDataset::IVertexBufferDataset() :
    IGenericDataset(),
    m_VerticeTotal(0),
    m_IndiceTotal(0)
{}
//---------------------------------------------------------------------------
XModel::IVertexBufferDataset::~IVertexBufferDataset()
{}
//---------------------------------------------------------------------------
// XModel::ITexCoordsDataset
//---------------------------------------------------------------------------
XModel::ITexCoordsDataset::ITexCoordsDataset() :
    IGenericDataset(),
    m_UVTotal(0)
{}
//---------------------------------------------------------------------------
XModel::ITexCoordsDataset::~ITexCoordsDataset()
{}
//---------------------------------------------------------------------------
// XModel::IMaterialListDataset
//---------------------------------------------------------------------------
XModel::IMaterialListDataset::IMaterialListDataset() :
    IGenericDataset(),
    m_MaterialCount(0),
    m_MaterialIndiceTotal(0)
{}
//---------------------------------------------------------------------------
XModel::IMaterialListDataset::~IMaterialListDataset()
{}
//---------------------------------------------------------------------------
// XModel::IMaterialDataset
//---------------------------------------------------------------------------
XModel::IMaterialDataset::IMaterialDataset() :
    IGenericDataset(),
    m_SpecularExp(0.0f),
    m_ReadValCount(0)
{}
//---------------------------------------------------------------------------
XModel::IMaterialDataset::~IMaterialDataset()
{}
//---------------------------------------------------------------------------
// XModel::ISkinWeightsDataset
//---------------------------------------------------------------------------
XModel::ISkinWeightsDataset::ISkinWeightsDataset() :
    IGenericDataset(),
    m_ItemCount(0),
    m_Matrix(Matrix4x4F::Identity()),
    m_ReadValCount(0),
    m_BoneIndex(0),
    m_MeshIndex(0)
{}
//---------------------------------------------------------------------------
XModel::ISkinWeightsDataset::~ISkinWeightsDataset()
{}
//---------------------------------------------------------------------------
// XModel::ITextureDataset
//---------------------------------------------------------------------------
XModel::ITextureDataset::ITextureDataset() :
    IGenericDataset()
{}
//---------------------------------------------------------------------------
XModel::ITextureDataset::~ITextureDataset()
{}
//---------------------------------------------------------------------------
// XModel::IAnimationKeyDataset
//---------------------------------------------------------------------------
XModel::IAnimationKeyDataset::IAnimationKeyDataset() :
    IGenericDataset(),
    m_Frame(0),
    m_Total(0)
{}
//---------------------------------------------------------------------------
XModel::IAnimationKeyDataset::~IAnimationKeyDataset()
{}
//---------------------------------------------------------------------------
// XModel::IAnimationKeysDataset
//---------------------------------------------------------------------------
XModel::IAnimationKeysDataset::IAnimationKeysDataset() :
    IGenericDataset(),
    m_Type(IE_KT_Unknown),
    m_KeyTotal(0),
    m_KeyIndex(0),
    m_ReadValCount(0)
{}
//---------------------------------------------------------------------------
XModel::IAnimationKeysDataset::~IAnimationKeysDataset()
{
    const std::size_t count = m_Keys.size();

    for (std::size_t i = 0; i < count; ++i)
        delete m_Keys[i];
}
//---------------------------------------------------------------------------
// XModel::IFileItem
//---------------------------------------------------------------------------
XModel::IFileItem::IFileItem() :
    m_ID(IE_DS_Unknown),
    m_pParent(nullptr),
    m_pDataset(nullptr),
    m_Opened(false),
    m_ContentRead(false)
{}
//---------------------------------------------------------------------------
XModel::IFileItem::~IFileItem()
{
    const std::size_t count = m_Children.size();

    // clear children
    for (std::size_t i = 0; i < count; ++i)
        delete m_Children[i];
}
//---------------------------------------------------------------------------
// XModel
//---------------------------------------------------------------------------
XModel::XModel() :
    m_pModel(nullptr),
    m_MeshOnly(false),
    m_PoseOnly(false),
    m_fOnGetVertexColor(nullptr),
    m_fOnLoadTexture(nullptr)
{
    // configure the default vertex format
    m_VertFormatTemplate.m_Format = (VertexFormat::IEFormat)(VertexFormat::IE_VF_Colors | VertexFormat::IE_VF_TexCoords);
    m_VertFormatTemplate.m_Type   =  VertexFormat::IE_VT_Triangles;

    // configure the default vertex culling
    m_VertCullingTemplate.m_Type = VertexCulling::IE_CT_Back;
    m_VertCullingTemplate.m_Face = VertexCulling::IE_CF_CCW;

    // configure the default material
    m_MaterialTemplate.m_Color = ColorF(1.0f, 1.0f, 1.0f, 1.0f);
}
//---------------------------------------------------------------------------
XModel::~XModel()
{
    if (m_pModel)
        delete m_pModel;
}
//---------------------------------------------------------------------------
bool XModel::Open(const std::string& fileName)
{
    // delete the previous model, if exists
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    // no file name?
    if (fileName.empty())
        return false;

    IFileHeader header;
    char*       pBuffer    = NULL;
    std::FILE*  pStream    = NULL;
    std::size_t fileSize   = 0;
    std::size_t headerSize = 0;
    std::size_t bufferSize = 0;
    std::size_t dataSize   = 0;
    bool        success    = true;

    try
    {
        // open file for read
        #ifdef _WINDOWS
            const errno_t error = fopen_s(&pStream, fileName.c_str(), "rb");

            // error occurred?
            if (error != 0)
                return false;
        #else
            pStream = std::fopen(fileName.c_str(), "rb");
        #endif

        // is file stream opened?
        if (!pStream)
            return false;

        // get file size
        std::fseek(pStream, 0, SEEK_END);
        fileSize = std::ftell(pStream);
        std::fseek(pStream, 0, SEEK_SET);

        // read the file header, in a binary form even if it's a text
        headerSize = std::fread(&header, 1, sizeof(IFileHeader), pStream);
        dataSize   = fileSize - headerSize;

        // copy file content to buffer
        pBuffer    = new char[dataSize + 1];
        bufferSize = std::fread(pBuffer, 1, dataSize, pStream);
    }
    catch (...)
    {
        success = false;
    }

    // close cached xml file
    if (pStream)
        std::fclose(pStream);

    std::string data;

    try
    {
        // file read succeeded?
        if (success)
            // add readed data to output
            data += std::string((const char*)pBuffer, bufferSize);
    }
    catch (...)
    {
        success = false;
    }

    // delete buffer, if needed
    if (pBuffer)
        delete[] pBuffer;

    // is a .x file?
    if (header.m_Magic != M_X_FORMAT_MAGIC)
        return false;

    // only 3.2 or 3.3 versions are supported
    if ((header.m_Major_Version != M_X_FORMAT_VERSION03) ||
        (header.m_Minor_Version != M_X_FORMAT_VERSION03) &&
        (header.m_Minor_Version != M_X_FORMAT_VERSION02))
        return false;

    // is .x file containing text?
    if (header.m_Format != M_X_FORMAT_TEXT)
        return false;

    return (success && (bufferSize == dataSize) && Read(data));
}
//---------------------------------------------------------------------------
bool XModel::Read(const std::string& data)
{
    // delete the previous model, if exists
    if (m_pModel)
    {
        delete m_pModel;
        m_pModel = nullptr;
    }

    // valid data?
    if (!data.length())
        return false;

    // create the root item
    std::unique_ptr<IFileItem> pRootItem(new IFileItem());

    // as the root pointer itself may change while parsing, keep a local copy of the root pointer
    IFileItem* pRoot = pRootItem.get();

    std::size_t offset = 0;

    // parse the file content
    if (!Parse(data, offset, pRoot))
        return false;

    // create the model
    m_pModel = new IModel();

    // succeeded?
    if (!m_pModel)
        return false;

    // configure it
    m_pModel->m_MeshOnly = m_MeshOnly;
    m_pModel->m_PoseOnly = m_PoseOnly;

    // convert the read item hierarchy to an x model
    if (!ItemToModel(pRootItem.get(),
                     m_pModel,
                     0,
                     &m_VertFormatTemplate,
                     &m_VertCullingTemplate,
                     &m_MaterialTemplate,
                     m_fOnGetVertexColor,
                     m_fOnLoadTexture))
    {
        delete m_pModel;
        m_pModel = nullptr;

        return false;
    }

    // build the bones parent hierarchy (could not simply keep the pointer while hierarchy was built
    // because the bone pointers may change several time while their hierarchy is built)
    if (m_pModel->m_pSkeleton)
    {
        BuildParentHierarchy(m_pModel->m_pSkeleton, 0, m_pModel);

        // skin weights?
        if (m_pModel->m_MeshWeights.size())
            // retrieve the bone linked with each skin weights
            for (std::size_t i = 0; i < m_pModel->m_MeshWeights.size(); ++i)
                for (std::size_t j = 0; j < m_pModel->m_MeshWeights[i]->m_SkinWeights.size(); ++j)
                    m_pModel->m_MeshWeights[i]->m_SkinWeights[j]->m_pBone =
                            FindBone(m_pModel->m_pSkeleton,
                                     m_pModel->m_MeshWeights[i]->m_SkinWeights[j]->m_BoneName);

        // animation set?
        if (!m_pModel->m_PoseOnly && m_pModel->m_AnimationSet.size())
            // find each bone linked to animation sets
            for (std::size_t i = 0; i < m_pModel->m_AnimationSet.size(); ++i)
                for (std::size_t j = 0; j < m_pModel->m_AnimationSet[i]->m_Animations.size(); ++j)
                    m_pModel->m_AnimationSet[i]->m_Animations[j]->m_pBone =
                            FindBone(m_pModel->m_pSkeleton,
                                     m_pModel->m_AnimationSet[i]->m_Animations[j]->m_BoneName);
    }

    return true;
}
//---------------------------------------------------------------------------
XModel::IModel* XModel::GetModel() const
{
    return m_pModel;
}
//---------------------------------------------------------------------------
void XModel::GetBoneMatrix(const IBone* pBone, const Matrix4x4F& initialMatrix, Matrix4x4F& matrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the output matrix as identity
    matrix = Matrix4x4F::Identity();

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(pBone->m_Matrix);

        // go to parent bone
        pBone = pBone->m_pParent;
    }

    // initial matrix provided?
    if (!initialMatrix.IsIdentity())
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the initial one
        matrix = localMatrix.Multiply(initialMatrix);
    }
}
//---------------------------------------------------------------------------
void XModel::GetBoneAnimMatrix(const IBone*         pBone,
                               const IAnimationSet* pAnimSet,
                                     std::size_t    frameIndex,
                               const Matrix4x4F&    initialMatrix,
                                     Matrix4x4F&    matrix) const
{
    // no bone?
    if (!pBone)
        return;

    // set the output matrix as identity
    matrix = Matrix4x4F::Identity();

    Matrix4x4F animMatrix;

    // iterate through bones
    while (pBone)
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // get the animated bone matrix matching with frame. If not found use the identity one
        if (!GetAnimationMatrix(pAnimSet, pBone, frameIndex, animMatrix))
            animMatrix = Matrix4x4F::Identity();

        // stack the previously calculated matrix with the current bone one
        matrix = localMatrix.Multiply(animMatrix);

        // go to parent bone
        pBone = pBone->m_pParent;
    }

    // initial matrix provided?
    if (!initialMatrix.IsIdentity())
    {
        // get the previously stacked matrix as base to calculate the new one
        const Matrix4x4F localMatrix = matrix;

        // stack the previously calculated matrix with the initial one
        matrix = localMatrix.Multiply(initialMatrix);
    }
}
//---------------------------------------------------------------------------
void XModel::Set_OnGetVertexColor(ITfOnGetVertexColor fOnGetVertexColor)
{
    m_fOnGetVertexColor = fOnGetVertexColor;
}
//---------------------------------------------------------------------------
void XModel::Set_OnLoadTexture(ITfOnLoadTexture fOnLoadTexture)
{
    m_fOnLoadTexture = fOnLoadTexture;
}
//---------------------------------------------------------------------------
bool XModel::Parse(const std::string& data, std::size_t& offset, IFileItem*& pItem) const
{
    std::size_t wordOffset    = offset;
    bool        readingString = false;

    /*
    xof 0303txt 0032

    Frame Root {
      FrameTransformMatrix {
         1.000000, 0.000000, 0.000000, 0.000000,
         0.000000,-0.000000, 1.000000, 0.000000,
         0.000000, 1.000000, 0.000000, 0.000000,
         0.000000, 0.000000, 0.000000, 1.000000;;
      }
      Frame Cube {
        FrameTransformMatrix {
           1.000000, 0.000000, 0.000000, 0.000000,
           0.000000, 1.000000, 0.000000, 0.000000,
           0.000000, 0.000000, 1.000000, 0.000000,
           0.000000, 0.000000, 0.000000, 1.000000;;
        }
        Mesh { // Cube mesh
          8;
           1.000000; 1.000000;-1.000000;,
           1.000000;-1.000000;-1.000000;,
          -1.000000;-1.000000;-1.000000;,
          -1.000000; 1.000000;-1.000000;,
           1.000000; 0.999999; 1.000000;,
           0.999999;-1.000001; 1.000000;,
          -1.000000;-1.000000; 1.000000;,
          -1.000000; 1.000000; 1.000000;;
          6;
          4;3,2,1,0;,
          4;5,6,7,4;,
          4;1,5,4,0;,
          4;2,6,5,1;,
          4;3,7,6,2;,
          4;7,3,0,4;;
          MeshNormals { // Cube normals
            6;
             0.000000; 0.000000;-1.000000;,
             0.000000;-0.000000; 1.000000;,
             1.000000;-0.000000; 0.000000;,
            -0.000000;-1.000000;-0.000000;,
            -1.000000; 0.000000;-0.000000;,
             0.000000; 1.000000; 0.000000;;
            6;
            4;0,0,0,0;,
            4;1,1,1,1;,
            4;2,2,2,2;,
            4;3,3,3,3;,
            4;4,4,4,4;,
            4;5,5,5,5;;
          } // End of Cube normals
          MeshMaterialList { // Cube material list
            1;
            6;
            0,
            0,
            0,
            0,
            0,
            0;
            Material Material {
               0.640000; 0.640000; 0.640000; 1.000000;;
               96.078431;
               0.500000; 0.500000; 0.500000;;
               0.000000; 0.000000; 0.000000;;
            }
          } // End of Cube material list
        } // End of Cube mesh
      } // End of Cube
    } // End of Root
    */
    while (offset < data.length())
        switch (data[offset])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':
            case ',':
            case ';':
                // reading a string?
                if (readingString)
                {
                    // ignore it in this case
                    ++offset;
                    continue;
                }

                // parse the next word
                if (offset > wordOffset)
                    ParseWord(data, wordOffset, offset, pItem);

                // skip the following separators since the current offset
                SkipSeparators(data, offset);

                // set the next word start offset
                wordOffset = offset;
                continue;

            case '{':
            case '}':
                // reading a string?
                if (readingString)
                {
                    // ignore it in this case
                    ++offset;
                    continue;
                }

                // parse the next word
                if (offset > wordOffset)
                    ParseWord(data, wordOffset, offset, pItem);

                // parse the opening or closing brace
                ParseWord(data, offset, offset + 1, pItem);

                // go to next char
                ++offset;

                // skip the following separators since the current offset
                SkipSeparators(data, offset);

                // set the next word start offset
                wordOffset = offset;
                continue;

            case '/':
                // reading a string?
                if (readingString)
                {
                    // ignore it in this case
                    ++offset;
                    continue;
                }

                // parse the next word
                if (offset > wordOffset)
                    ParseWord(data, wordOffset, offset, pItem);

                // next char should also be a slash, otherwise it's an error
                if ((offset + 1) >= data.length() || data[offset + 1] != '/')
                    return false;

                // skip the text until next line
                SkipLine(data, offset);

                // set the next word start offset
                wordOffset = offset;
                continue;

            case '#':
                // reading a string?
                if (readingString)
                {
                    // ignore it in this case
                    ++offset;
                    continue;
                }

                // parse the next word
                if (offset > wordOffset)
                    ParseWord(data, wordOffset, offset, pItem);

                // skip the text until next line
                SkipLine(data, offset);

                // set the next word start offset
                wordOffset = offset;
                continue;

            case '\"':
                // begin or end to read a string
                if (readingString)
                    readingString = 0;
                else
                    readingString = 1;

                ++offset;
                continue;

            default:
                ++offset;
                continue;
        }

    return true;
}
//---------------------------------------------------------------------------
bool XModel::ParseWord(const std::string& data, std::size_t startOffset, std::size_t endOffset, IFileItem*& pItem) const
{
    // should always have an item defined
    if (!pItem)
        return false;

    // translate the word
    switch (TranslateWord(data, startOffset, endOffset))
    {
        case IE_DS_Template_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Template_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Header_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IHeaderDataset> pDataset(new IHeaderDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Header_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Frame_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Frame_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Frame_Transform_Matrix_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IMatrixDataset> pDataset(new IMatrixDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Frame_Transform_Matrix_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Mesh_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IVertexBufferDataset> pDataset(new IVertexBufferDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Mesh_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Mesh_Texture_Coords_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<ITexCoordsDataset> pDataset(new ITexCoordsDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Mesh_Texture_Coords_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Mesh_Material_List_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IMaterialListDataset> pDataset(new IMaterialListDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Mesh_Material_List_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Material_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IMaterialDataset> pDataset(new IMaterialDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Material_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Skin_Mesh_Header_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IHeaderDataset> pDataset(new IHeaderDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Skin_Mesh_Header_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Skin_Weights_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<ISkinWeightsDataset> pDataset(new ISkinWeightsDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Skin_Weights_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Texture_Filename_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<ITextureDataset> pDataset(new ITextureDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Texture_Filename_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Mesh_Normals_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IVertexBufferDataset> pDataset(new IVertexBufferDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Mesh_Normals_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Animation_Set_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Animation_Set_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Animation_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Animation_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_DS_Animation_Key_ID:
        {
            // sometimes dataset name may be the same as another known dataset. Check if it's the
            // case here and read the template name if yes
            if (pItem                            &&
               !pItem->m_Opened                  &&
                pItem->m_ID >= IE_DS_Template_ID &&
                pItem->m_ID <= IE_DS_Animation_Key_ID)
                return ReadDatasetName(data, startOffset, endOffset, pItem);

            // create the dataset
            std::unique_ptr<IAnimationKeysDataset> pDataset(new IAnimationKeysDataset());

            // add a new template child item
            std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Animation_Key_ID, pDataset.get()));

            // set the newly added child item as the current one
            pDataset.release();
            pItem = pChild.release();

            return true;
        }

        case IE_T_Open_Brace:
            // found another open brace in an open dataset?
            if (pItem->m_Opened)
            {
                // maybe an anonymous dataset or a link. Create the data
                std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

                // add a new child item
                std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Unknown, pDataset.get()));

                // set the newly added child item as the current one
                pDataset.release();
                pItem = pChild.release();
            }

            pItem->m_Opened = true;
            return true;

        case IE_T_Close_Brace:
            // close the dataset
            pItem->m_Opened      = false;
            pItem->m_ContentRead = true;

            // get the parent back
            pItem = pItem->m_pParent;

            return true;

        case IE_T_String:
            // was the item opened?
            if (pItem->m_Opened)
                switch (pItem->m_ID)
                {
                    case IE_DS_Texture_Filename_ID:
                    {
                        // get item data
                        ITextureDataset* pDataset = static_cast<ITextureDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // get the texture file name (without the quotes)
                        pDataset->m_FileName = GetText(data, startOffset + 1, endOffset - 1);

                        return true;
                    }

                    case IE_DS_Skin_Weights_ID:
                    {
                        // get item data
                        ISkinWeightsDataset* pDataset = static_cast<ISkinWeightsDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // get the skin weight linked bone name (without the quotes)
                        pDataset->m_BoneName = GetText(data, startOffset + 1, endOffset - 1);

                        return true;
                    }

                    default:
                        return true;
                }

            return true;

        case IE_T_Float:
            // was the item opened?
            if (pItem->m_Opened)
                switch (pItem->m_ID)
                {
                    case IE_DS_Frame_Transform_Matrix_ID:
                    {
                        // get item data
                        IMatrixDataset* pDataset = static_cast<IMatrixDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // read values exceeded?
                        if (pDataset->m_ReadValCount < 16)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            if (value.empty())
                                return false;

                            // convert value
                            pDataset->m_Matrix.m_Table[pDataset->m_ReadValCount / 4][pDataset->m_ReadValCount % 4] = std::atof(value.c_str());

                            ++pDataset->m_ReadValCount;
                        }

                        return true;
                    }

                    case IE_DS_Mesh_ID:
                    case IE_DS_Mesh_Normals_ID:
                    {
                        // get item data
                        IVertexBufferDataset* pDataset = static_cast<IVertexBufferDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read a new vertex?
                        if (pDataset->m_Vertices.size() < pDataset->m_VerticeTotal * 3)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            if (value.empty())
                                return false;

                            // convert value
                            pDataset->m_Vertices.push_back(std::atof(value.c_str()));
                        }

                        return true;
                    }

                    case IE_DS_Mesh_Texture_Coords_ID:
                    {
                        // get item data
                        ITexCoordsDataset* pDataset = static_cast<ITexCoordsDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read a new texture coordinate?
                        if (pDataset->m_UV.size() < pDataset->m_UVTotal * 2)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            if (value.empty())
                                return false;

                            // convert value
                            pDataset->m_UV.push_back(std::atof(value.c_str()));
                        }

                        return true;
                    }

                    case IE_DS_Material_ID:
                    {
                        char* pValue;

                        // get item data
                        IMaterialDataset* pDataset = static_cast<IMaterialDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // get the value to convert
                        const std::string value = GetText(data, startOffset, endOffset);

                        if (value.empty())
                            return false;

                        // convert the next value
                        switch (pDataset->m_ReadValCount)
                        {
                            case 0:  pDataset->m_Color.m_R         = std::atof(value.c_str()); break;
                            case 1:  pDataset->m_Color.m_G         = std::atof(value.c_str()); break;
                            case 2:  pDataset->m_Color.m_B         = std::atof(value.c_str()); break;
                            case 3:  pDataset->m_Color.m_A         = std::atof(value.c_str()); break;
                            case 4:  pDataset->m_SpecularExp       = std::atof(value.c_str()); break;
                            case 5:  pDataset->m_SpecularColor.m_R = std::atof(value.c_str()); break;
                            case 6:  pDataset->m_SpecularColor.m_G = std::atof(value.c_str()); break;
                            case 7:  pDataset->m_SpecularColor.m_B = std::atof(value.c_str()); break;
                            case 8:  pDataset->m_EmisiveColor.m_R  = std::atof(value.c_str()); break;
                            case 9:  pDataset->m_EmisiveColor.m_G  = std::atof(value.c_str()); break;
                            case 10: pDataset->m_EmisiveColor.m_B  = std::atof(value.c_str()); break;
                        }

                        ++pDataset->m_ReadValCount;

                        return true;
                    }

                    case IE_DS_Skin_Weights_ID:
                    {
                        char* pValue;

                        // get item data
                        ISkinWeightsDataset* pDataset = static_cast<ISkinWeightsDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read a new skin weight?
                        if (pDataset->m_Weights.size() < pDataset->m_ItemCount)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            if (value.empty())
                                return false;

                            // convert value
                            pDataset->m_Weights.push_back(std::atof(value.c_str()));
                        }
                        else
                        if (pDataset->m_ReadValCount < 16)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            if (value.empty())
                                return false;

                            // convert value
                            pDataset->m_Matrix.m_Table[pDataset->m_ReadValCount / 4][pDataset->m_ReadValCount % 4] = std::atof(value.c_str());

                            ++pDataset->m_ReadValCount;
                        }

                        return true;
                    }

                    case IE_DS_Animation_Key_ID:
                    {
                        // get item data
                        IAnimationKeysDataset* pDataset = static_cast<IAnimationKeysDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // get the value to convert
                        const std::string value = GetText(data, startOffset, endOffset);

                        if (value.empty())
                            return false;

                        pDataset->m_Keys[pDataset->m_KeyIndex]->m_Values.push_back(std::atof(value.c_str()));

                        // if all data were read, go to next item
                        if (pDataset->m_Keys[pDataset->m_KeyIndex]->m_Values.size() == pDataset->m_Keys[pDataset->m_KeyIndex]->m_Total)
                            ++pDataset->m_KeyIndex;

                        return true;
                    }

                    default:
                        return true;
                }

            return true;

        case IE_T_Integer:
            // was the item opened?
            if (pItem->m_Opened)
                switch (pItem->m_ID)
                {
                    case IE_DS_Header_ID:
                    case IE_DS_Skin_Mesh_Header_ID:
                    {
                        char* pValue;

                        // get item data
                        IHeaderDataset* pDataset = static_cast<IHeaderDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // get the value to convert
                        const std::string value = GetText(data, startOffset, endOffset);

                        // convert value
                        switch (pDataset->m_ReadValCount)
                        {
                            case 0: pDataset->m_Major = std::atoi(value.c_str()); break;
                            case 1: pDataset->m_Minor = std::atoi(value.c_str()); break;
                            case 2: pDataset->m_Flags = std::atoi(value.c_str()); break;
                        }

                        ++pDataset->m_ReadValCount;

                        return true;
                    }

                    case IE_DS_Mesh_ID:
                    case IE_DS_Mesh_Normals_ID:
                    {
                        // get item data
                        IVertexBufferDataset* pDataset = static_cast<IVertexBufferDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read the vertices or indice count, or a new indice?
                        if (!pDataset->m_VerticeTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_VerticeTotal = std::atoi(value.c_str());
                        }
                        else
                        if (!pDataset->m_IndiceTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_IndiceTotal = std::atoi(value.c_str());
                        }
                        else
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_Indices.push_back(std::atoi(value.c_str()));
                        }

                        return true;
                    }

                    case IE_DS_Mesh_Texture_Coords_ID:
                    {
                        // get item data
                        ITexCoordsDataset* pDataset = static_cast<ITexCoordsDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read the texture coordinate count?
                        if (!pDataset->m_UVTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_UVTotal = std::atoi(value.c_str());
                        }

                        return true;
                    }

                    case IE_DS_Mesh_Material_List_ID:
                    {
                        // get item data
                        IMaterialListDataset* pDataset = static_cast<IMaterialListDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read the material count, the material indices count, or a new indice?
                        if (!pDataset->m_MaterialCount)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_MaterialCount = std::atoi(value.c_str());
                        }
                        else
                        if (!pDataset->m_MaterialIndiceTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_MaterialIndiceTotal = std::atoi(value.c_str());
                        }
                        else
                        if (pDataset->m_MaterialIndices.size() < pDataset->m_MaterialIndiceTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_MaterialIndices.push_back(std::atoi(value.c_str()));
                        }

                        return true;
                    }

                    case IE_DS_Skin_Weights_ID:
                    {
                        // get item data
                        ISkinWeightsDataset* pDataset = static_cast<ISkinWeightsDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read the skin weights item count, or a new indice?
                        if (!pDataset->m_ItemCount)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_ItemCount = std::atoi(value.c_str());
                        }
                        else
                        if (pDataset->m_Indices.size() < pDataset->m_ItemCount)
                        {

                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_Indices.push_back(std::atoi(value.c_str()));
                        }

                        return true;
                    }

                    case IE_DS_Animation_Key_ID:
                    {
                        // get item data
                        IAnimationKeysDataset* pDataset = static_cast<IAnimationKeysDataset*>(pItem->m_pDataset);

                        // found it?
                        if (!pDataset)
                            return false;

                        // do read the key total, the key frame or the key value count?
                        if (pDataset->m_Type == IE_KT_Unknown)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_Type = (IEAnimKeyType)std::atoi(value.c_str());
                        }
                        else
                        if (!pDataset->m_KeyTotal)
                        {
                            // get the value to convert
                            const std::string value = GetText(data, startOffset, endOffset);

                            // convert value
                            pDataset->m_KeyTotal = std::atoi(value.c_str());

                            // reserve the memory for the keys and initialize them
                            pDataset->m_Keys.resize(pDataset->m_KeyTotal);

                            for (std::size_t i = 0; i < pDataset->m_KeyTotal; ++i)
                                pDataset->m_Keys[i] = new IAnimationKeyDataset();

                            pDataset->m_KeyIndex     = 0;
                            pDataset->m_ReadValCount = 0;
                        }
                        else
                        {
                            // search for value to read
                            switch (pDataset->m_ReadValCount)
                            {
                                // read the key frame
                                case 0:
                                {
                                    // get the value to convert
                                    const std::string value = GetText(data, startOffset, endOffset);

                                    // convert value
                                    pDataset->m_Keys[pDataset->m_KeyIndex]->m_Frame = std::atoi(value.c_str());

                                    ++pDataset->m_ReadValCount;

                                    break;
                                }

                                // read the key value count
                                case 1:
                                {
                                    // get the value to convert
                                    const std::string value = GetText(data, startOffset, endOffset);

                                    // convert value
                                    pDataset->m_Keys[pDataset->m_KeyIndex]->m_Total = std::atoi(value.c_str());
                                    pDataset->m_ReadValCount                        = 0;

                                    break;
                                }
                            }
                        }

                        return true;
                    }

                    default:
                        return true;
                }

            return true;

        case IE_T_Name:
        {
            // get item data
            IGenericDataset* pDataset = static_cast<IGenericDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // the name is a special dataset which contains the linked name to something else
            pDataset->m_Name = GetText(data, startOffset, endOffset);
            pItem->m_ID      = IE_DS_Link_ID;

            return true;
        }

        default:
            // is the word the name of a dataset?
            if (pItem && !pItem->m_Opened)
                // item content was already read or is root node?
                if (pItem->m_ContentRead || !pItem->m_pParent)
                {
                    // probably the name of an unknown dataset. Create the data
                    std::unique_ptr<IGenericDataset> pDataset(new IGenericDataset());

                    // add a new child item
                    std::unique_ptr<IFileItem> pChild(AddChild(pItem, IE_DS_Unknown, pDataset.get()));

                    // keep the current word as name for the unknown dataset
                    pDataset->m_Name = GetText(data, startOffset, endOffset);

                    // set the newly added child item as the current one
                    pDataset.release();
                    pItem = pChild.release();

                    return true;
                }
                else
                    return ReadDatasetName(data, startOffset, endOffset, pItem);

            // if item is already opened, then the word isn't a dataset name but something else
            return true;
    }
}
//---------------------------------------------------------------------------
void XModel::SkipSeparators(const std::string& data, std::size_t& offset) const
{
    // skip all special chars
    while (offset < data.length())
        switch (data[offset])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':
            case ',':
            case ';': ++offset; continue;
            default:            return;
        }
}
//---------------------------------------------------------------------------
void XModel::SkipLine(const std::string& data, std::size_t& offset) const
{
    // skip all the remaining line content
    while (offset < data.length() && data[offset] != '\r' && data[offset] != '\n')
        ++offset;

    // also skip the CRLF itself
    SkipCRLF(data, offset);
}
//---------------------------------------------------------------------------
void XModel::SkipCRLF(const std::string& data, std::size_t& offset) const
{
    // skip all carriage return and line feed
    while (offset < data.length() && (data[offset] == '\r' || data[offset] == '\n'))
        ++offset;
}
//---------------------------------------------------------------------------
int XModel::TranslateWord(const std::string& data, std::size_t startOffset, std::size_t endOffset) const
{
    // is word empty or comtains just 1 char?
    if (endOffset <= startOffset)
        return 0;
    else
    if ((endOffset - startOffset) == 1)
        if (data[startOffset] == '{')
            return IE_T_Open_Brace;
        else
        if (data[startOffset] == '}')
            return IE_T_Close_Brace;

    // is a string?
    if (data[startOffset] == '\"' && data[endOffset - 1] == '\"')
        return IE_T_String;

    int         itemType = 0;
    std::size_t offset   = startOffset;

    // iterate through word chars to determine if word is a number
    while (offset < endOffset)
    {
        // found end of line?
        if (data[offset] == '\0')
            break;

        // char contains a number or negative symbol?
        if ((data[offset] >= '0' && data[offset] <= '9') || data[offset] == '-')
        {
            // already detected as a float?
            if (itemType != IE_T_Float)
                // set it as an integer
                itemType = IE_T_Integer;

            ++offset;
            continue;
        }

        // char contains a floating point separator?
        if (data[offset] == '.')
        {
            // set it as a float
            itemType = IE_T_Float;
            ++offset;
            continue;
        }

        itemType = IE_T_Unknown;
        break;
    }

    // is a number?
    if (itemType == IE_T_Float || itemType == IE_T_Integer)
        return itemType;

    bool foundOpeningBrace = false;
    bool foundClosingBrace = false;

    // check if the word is immediately preceded by a opening brace
    for (int i = (int)startOffset - 1; i >= 0; --i)
    {
        switch (data[i])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':                         continue;
            case '{':  foundOpeningBrace = 1; break;
            default:                          break;
        }

        break;
    }

    // check if the word is immediately followed by a closing brace
    for (std::size_t i = endOffset; i < data.length(); ++i)
    {
        switch (data[i])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':                         continue;
            case '}':  foundClosingBrace = 1; break;
            default:                          break;
        }

        break;
    }

    // is a reference name?
    if (foundOpeningBrace && foundClosingBrace)
        return IE_T_Name;

    // extract word from data
    const std::string word = GetText(data, startOffset, endOffset);

    // get template identifier
    itemType = GetDataStructureID(word);

    // is a known template identifier?
    if (itemType != IE_DS_Unknown)
        return itemType;

    // unknown name or identifier
    return IE_T_Unknown;
}
//---------------------------------------------------------------------------
bool XModel::ReadDatasetName(const std::string& data,
                                   std::size_t  startOffset,
                                   std::size_t  endOffset,
                                   IFileItem*   pItem) const
{
    switch (pItem->m_ID)
    {
        case IE_DS_Template_ID:
        case IE_DS_Frame_ID:
        case IE_DS_Animation_Set_ID:
        case IE_DS_Animation_ID:
        {
            // get item data
            IGenericDataset* pDataset = pItem->m_pDataset;

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Header_ID:
        case IE_DS_Skin_Mesh_Header_ID:
        {
            // get item data
            IHeaderDataset* pDataset = static_cast<IHeaderDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Frame_Transform_Matrix_ID:
        {
            // get item data
            IMatrixDataset* pDataset = static_cast<IMatrixDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Mesh_ID:
        case IE_DS_Mesh_Normals_ID:
        {
            // get item data data
            IVertexBufferDataset* pDataset = static_cast<IVertexBufferDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Mesh_Texture_Coords_ID:
        {
            // get item data
            ITexCoordsDataset* pDataset = static_cast<ITexCoordsDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Mesh_Material_List_ID:
        {
            // get item data data
            IMaterialListDataset* pDataset = static_cast<IMaterialListDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Material_ID:
        {
            // get item data
            IMaterialDataset* pDataset = static_cast<IMaterialDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Skin_Weights_ID:
        {
            // get item data
            ISkinWeightsDataset* pDataset = static_cast<ISkinWeightsDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Texture_Filename_ID:
        {
            // get item data
            ITextureDataset* pDataset = static_cast<ITextureDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        case IE_DS_Animation_Key_ID:
        {
            // get item data
            IAnimationKeyDataset* pDataset = static_cast<IAnimationKeyDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // was item data name already attributed?
            if (!pDataset->m_Name.empty())
                return false;

            // get the item data name
            pDataset->m_Name = GetText(data, startOffset, endOffset);

            // succeeded?
            return !pDataset->m_Name.empty();
        }

        default:
            // unknown word, just ignore it
            return true;
    }
}
//---------------------------------------------------------------------------
XModel::IEDataStructID XModel::GetDataStructureID(const std::string& word) const
{
    if (word == "template")
        return IE_DS_Template_ID;
    else
    if (word == "Header")
        return IE_DS_Header_ID;
    else
    if (word == "Frame")
        return IE_DS_Frame_ID;
    else
    if (word == "FrameTransformMatrix")
        return IE_DS_Frame_Transform_Matrix_ID;
    else
    if (word == "Mesh")
        return IE_DS_Mesh_ID;
    else
    if (word == "MeshTextureCoords")
        return IE_DS_Mesh_Texture_Coords_ID;
    else
    if (word == "MeshMaterialList")
        return IE_DS_Mesh_Material_List_ID;
    else
    if (word == "Material")
        return IE_DS_Material_ID;
    else
    if (word == "XSkinMeshHeader")
        return IE_DS_Skin_Mesh_Header_ID;
    else
    if (word == "SkinWeights")
        return IE_DS_Skin_Weights_ID;
    else
    if (word == "TextureFilename")
        return IE_DS_Texture_Filename_ID;
    else
    if (word == "MeshNormals")
        return IE_DS_Mesh_Normals_ID;
    else
    if (word == "AnimationSet")
        return IE_DS_Animation_Set_ID;
    else
    if (word == "Animation")
        return IE_DS_Animation_ID;
    else
    if (word == "AnimationKey")
        return IE_DS_Animation_Key_ID;

    return IE_DS_Unknown;
}
//---------------------------------------------------------------------------
std::string XModel::GetText(const std::string& data, std::size_t startOffset, std::size_t endOffset) const
{
    return data.substr(startOffset, endOffset - startOffset);
}
//---------------------------------------------------------------------------
XModel::IFileItem* XModel::AddChild(IFileItem* pItem, IEDataStructID id, IGenericDataset* pDataset) const
{
    std::unique_ptr<IFileItem> pChild(new IFileItem());
    pChild->m_ID       = id;
    pChild->m_pParent  = pItem;
    pChild->m_pDataset = pDataset;
    pItem->m_Children.push_back(pChild.get());

    return pChild.release();
}
//---------------------------------------------------------------------------
bool XModel::ItemToModel(const IFileItem*          pItem,
                               IModel*             pModel,
                               IBone*              pBone,
                         const VertexFormat*       pVertFormat,
                         const VertexCulling*      pVertCulling,
                         const Material*           pMaterial,
                         const ITfOnGetVertexColor fOnGetVertexColor,
                         const ITfOnLoadTexture    fOnLoadTexture) const
{
    // must have a root item...
    if (!pItem)
        return false;

    // ...and a destination x model
    if (!pModel)
        return false;

    IBone* pCurrent = pBone;

    switch (pItem->m_ID)
    {
        case IE_DS_Frame_ID:
        {
            // get item data
            IGenericDataset* pDataset = static_cast<IGenericDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // no current bone?
            if (!pBone)
            {
                // then the skeleton should also be not initalized
                if (pModel->m_pSkeleton)
                    return false;

                // create the root bone
                pModel->m_pSkeleton = new IBone();

                // succeeded?
                if (!pModel->m_pSkeleton)
                    return false;

                // set it as the current bone
                pCurrent = pModel->m_pSkeleton;
            }
            else
            {
                // create a new bone
                std::unique_ptr<IBone> pChild(new IBone());

                // set bone parent
                pChild->m_pParent = pBone;

                // add it to the parent's children
                pBone->m_Children.push_back(pChild.get());
                pCurrent = pChild.release();
            }

            // from now current bone should always exist
            if (!pCurrent)
                return false;

            // get the bone name
            pCurrent->m_Name = pDataset->m_Name;

            break;
        }

        case IE_DS_Frame_Transform_Matrix_ID:
        {
            // get item data
            IMatrixDataset* pDataset = static_cast<IMatrixDataset*>(pItem->m_pDataset);

            // found it?
            if (!pDataset)
                return false;

            // should always have a parent bone
            if (!pBone)
                return false;

            // get the bone matrix
            pBone->m_Matrix = pDataset->m_Matrix;
            return true;
        }

        case IE_DS_Mesh_ID:
            // build the mesh
            return BuildMesh(pItem,
                             pModel,
                             pBone,
                             pVertFormat,
                             pVertCulling,
                             pMaterial,
                             fOnGetVertexColor,
                             fOnLoadTexture);

        case IE_DS_Animation_Set_ID:
            // don't load the animation data if pose only is required
            if (pModel->m_PoseOnly)
                return true;

            // build the animation set
            return BuildAnimationSet(pItem, pModel);

        default:
            break;
    }

    // convert all children items
    for (std::size_t i = 0; i < pItem->m_Children.size(); ++i)
        if (!ItemToModel(pItem->m_Children[i],
                         pModel,
                         pCurrent,
                         pVertFormat,
                         pVertCulling,
                         pMaterial,
                         fOnGetVertexColor,
                         fOnLoadTexture))
            return false;

    return true;
}
//---------------------------------------------------------------------------
bool XModel::BuildMesh(const IFileItem*          pItem,
                             IModel*             pModel,
                             IBone*              pBone,
                       const VertexFormat*       pVertFormat,
                       const VertexCulling*      pVertCulling,
                       const Material*           pMaterial,
                       const ITfOnGetVertexColor fOnGetVertexColor,
                       const ITfOnLoadTexture    fOnLoadTexture) const
{
    // get the dataset containing the mesh
    IVertexBufferDataset* pMeshDataset = static_cast<IVertexBufferDataset*>(pItem->m_pDataset);

    // found them?
    if (!pMeshDataset)
        return false;

    std::size_t meshWeightsIndex = 0;

    // is model supporting animations?
    if (!pModel->m_MeshOnly)
    {
        // add a new mesh skin weights to the model
        std::unique_ptr<IMeshSkinWeights> pMeshWeights(new IMeshSkinWeights());

        // keep the mesh weights index
        meshWeightsIndex = pModel->m_MeshWeights.size();

        // update the model
        pModel->m_MeshWeights.push_back(pMeshWeights.get());
        pMeshWeights.release();
    }

    IVertexBufferDataset* pNormalsDataset = nullptr;
    ITexCoordsDataset*    pUVDataset      = nullptr;
    IMaterialListDataset* pMatListDataset = nullptr;
    IFileItem*            pMatListItem    = nullptr;

    // search in the mesh children which are useful and should be used to build the mesh
    for (std::size_t i = 0; i < pItem->m_Children.size(); ++i)
        switch (pItem->m_Children[i]->m_ID)
        {
            case IE_DS_Mesh_Normals_ID:
                // found the normals dataset, get it
                pNormalsDataset = static_cast<IVertexBufferDataset*>(pItem->m_Children[i]->m_pDataset);

                // succeeded?
                if (!pNormalsDataset)
                    return false;

                continue;

            case IE_DS_Mesh_Texture_Coords_ID:
                // found the texture coordinates dataset, get it
                pUVDataset = static_cast<ITexCoordsDataset*>(pItem->m_Children[i]->m_pDataset);

                // succeeded?
                if (!pUVDataset)
                    return false;

                continue;

            case IE_DS_Mesh_Material_List_ID:
                // found the material list, get it
                pMatListItem = pItem->m_Children[i];

                // succeeded?
                if (!pMatListItem)
                    return false;

                // and get its dataset
                pMatListDataset = static_cast<IMaterialListDataset*>(pMatListItem->m_pDataset);

                // succeeded?
                if (!pMatListDataset)
                    return false;

                continue;

            case IE_DS_Skin_Weights_ID:
            {
                // mesh only should be drawn, ignore the skin weights
                if (pModel->m_MeshOnly)
                    continue;

                // found the skin weights dataset, get it
                ISkinWeightsDataset* pSkinWeightsDataset = static_cast<ISkinWeightsDataset*>(pItem->m_Children[i]->m_pDataset);

                // succeeded?
                if (!pSkinWeightsDataset)
                    return false;

                // add a new skin weights to the mesh skin weights
                std::unique_ptr<ISkinWeights> pSkinWeights(new ISkinWeights());

                // get the bone link name
                if (!pSkinWeightsDataset->m_BoneName.empty())
                    pSkinWeights->m_BoneName = pSkinWeightsDataset->m_BoneName;
                else
                    // orphan skin weights? This should be a bug...
                    return false;

                // get the matrix
                pSkinWeights->m_Matrix = pSkinWeightsDataset->m_Matrix;

                const std::size_t weightsCount = pSkinWeightsDataset->m_Weights.size();

                // reserve the memory for the keys and initialize them
                pSkinWeights->m_Weights.resize(weightsCount);

                for (std::size_t j = 0; j < weightsCount; ++j)
                    pSkinWeights->m_Weights[j] = pSkinWeightsDataset->m_Weights[j];

                const std::size_t indiceCount = pSkinWeightsDataset->m_Indices.size();

                // reserve the memory for the index table and initialize them
                pSkinWeights->m_IndexTable.resize(indiceCount);

                for (std::size_t j = 0; j < indiceCount; ++j)
                    pSkinWeights->m_IndexTable[j] = new ISkinWeightIndexTable();

                // update the model mesh skin weights list
                pModel->m_MeshWeights[meshWeightsIndex]->m_SkinWeights.push_back(pSkinWeights.get());
                pSkinWeights.release();

                continue;
            }

            default:
                continue;
        }

    // keep the current mesh index
    const std::size_t index = pModel->m_Mesh.size();

    // model contains bones?
    if (pBone)
    {
        // allocate memory for the new mesh-to-bone dictionary item
        std::unique_ptr<IMeshToBoneItem> pMeshBoneItem(new IMeshToBoneItem());

        // update the model mesh data
        pModel->m_MeshToBoneDict.push_back(pMeshBoneItem.get());
        pMeshBoneItem.release();

        // link the mesh to the bone owning it
        pModel->m_MeshToBoneDict[index]->m_MeshIndex = index;
        pBone->m_MeshIndex                           = index;
    }

    // allocate memory for the new vertex buffer
    std::unique_ptr<VertexBuffer> pVB(new VertexBuffer());

    // apply the user wished vertex format
    if (pVertFormat)
        pVB->m_Format = *pVertFormat;

    // apply the user wished vertex culling
    if (pVertCulling)
        pVB->m_Culling = *pVertCulling;

    // apply the user wished material
    if (pMaterial)
        pVB->m_Material = *pMaterial;

    // set the vertex format type
    pVB->m_Format.m_Type = VertexFormat::IE_VT_Triangles;

    // calculate the stride
    pVB->m_Format.CalculateStride();

    // is model supporting animations?
    if (!pModel->m_MeshOnly)
    {
        // if the animation is used, also create the associated mesh print
        std::unique_ptr<VertexBuffer> pPrint(new VertexBuffer());

        // copy the vertex format, culling and material from the source mesh
        pPrint->m_Format   = pVB->m_Format;
        pPrint->m_Culling  = pVB->m_Culling;
        pPrint->m_Material = pVB->m_Material;

        // update the model mesh print data
        pModel->m_Print.push_back(pPrint.get());
        pPrint.release();
    }

    bool hasTexture = false;

    // mesh contains materials?
    if (pMatListItem)
        // load all the material textures
        for (std::size_t i = 0; i < pMatListItem->m_Children.size(); ++i)
        {
            // get the next material item
            IFileItem* pMaterialItem = GetMaterial(pMatListItem, i);

            // found it and is really a material?
            if (!pMaterialItem || pMaterialItem->m_ID != IE_DS_Material_ID)
                return false;

            // iterate through material children
            for (std::size_t j = 0; j < pMaterialItem->m_Children.size(); ++j)
                // has a texture?
                if (pMaterialItem->m_Children[j]->m_ID == IE_DS_Texture_Filename_ID)
                {
                    std::size_t length;
                    bool        canRelease;

                    // get the texture dataset
                    ITextureDataset* pTextureDataset = static_cast<ITextureDataset*>(pMaterialItem->m_Children[j]->m_pDataset);

                    // found it?
                    if (!pTextureDataset)
                        return false;

                    // load the texture
                    if (fOnLoadTexture)
                        pVB->m_Material.m_pTexture = fOnLoadTexture(pTextureDataset->m_FileName);

                    // normally each material should contain only one texture
                    hasTexture = true;
                    break;
                }

            // for now only one texture is allowed per mesh. NOTE don't know if it's a
            // limitation, should be monitored in the future
            if (hasTexture)
                break;
        }

    // build the new mesh which will contain the vertex buffer
    std::unique_ptr<Mesh> pMesh(new Mesh());
    pMesh->m_VB.push_back(pVB.get());
    pVB.release();

    // keep the previous color, it may change while the mesh is created
    ColorF      prevColor     = pMesh->m_VB[pMesh->m_VB.size() - 1]->m_Material.m_Color;
    std::size_t materialIndex = 0;

    // iterate through indice table
    for (std::size_t i = 0; i < pMeshDataset->m_Indices.size(); i += pMeshDataset->m_Indices[i] + 1)
    {
        // iterate through source vertices
        for (std::size_t j = 0; j < pMeshDataset->m_Indices[i] - 1; ++j)
        {
            // calculate the next polygon index
            const std::size_t polygonIndex = i + j + 1;

            // build the polygon
            for (std::size_t k = 0; k < 3; ++k)
            {
                std::size_t vertIndex;

                // calculate the next vertex index
                if (!k)
                    vertIndex = i + 1;
                else
                    vertIndex = polygonIndex + (k - 1);

                // build the vertex
                if (!BuildVertex(pItem,
                                 pModel,
                                 pMesh.get(),
                                 index,
                                 vertIndex,
                                 materialIndex,
                                 prevColor,
                                 pMeshDataset,
                                 pNormalsDataset,
                                 pUVDataset,
                                 pMatListItem,
                                 pMatListDataset,
                                 fOnGetVertexColor))
                    return false;
            }
        }

        ++materialIndex;
    }

    // add the mesh to the model
    pModel->m_Mesh.push_back(pMesh.get());
    pMesh.release();

    return true;
}
//---------------------------------------------------------------------------
bool XModel::BuildVertex(const IFileItem*            pItem,
                               IModel*               pModel,
                               Mesh*                 pMesh,
                               std::size_t           meshIndex,
                               std::size_t           vertexIndex,
                               std::size_t           matListIndex,
                         const ColorF&               prevColor,
                         const IVertexBufferDataset* pMeshDataset,
                         const IVertexBufferDataset* pNormalsDataset,
                         const ITexCoordsDataset*    pUVDataset,
                         const IFileItem*            pMatList,
                         const IMaterialListDataset* pMatListDataset,
                         const ITfOnGetVertexColor   fOnGetVertexColor) const
{
    // calculate the vertex index from the indice table
    const std::size_t indiceIndex = pMeshDataset->m_Indices[vertexIndex] * 3;

    // is index out of bounds?
    if (indiceIndex >= pMeshDataset->m_Vertices.size())
        return false;

    Vector3F vertex;

    // build the vertex
    vertex.m_X = pMeshDataset->m_Vertices[indiceIndex];
    vertex.m_Y = pMeshDataset->m_Vertices[indiceIndex + 1];
    vertex.m_Z = pMeshDataset->m_Vertices[indiceIndex + 2];

    Vector3F  normal;
    Vector3F* pNormal = nullptr;

    // mesh contains normals?
    if ((pMesh->m_VB[0]->m_Format.m_Format & VertexFormat::IE_VF_Normals) && pNormalsDataset)
    {
        // calculate the normal index from the indice table
        const std::size_t nIndiceIndex = pNormalsDataset->m_Indices[vertexIndex] * 3;

        // is index out of bounds?
        if (nIndiceIndex >= pNormalsDataset->m_Indices.size())
            return false;

        // build the normal
        normal.m_X = pNormalsDataset->m_Vertices[nIndiceIndex];
        normal.m_Y = pNormalsDataset->m_Vertices[nIndiceIndex + 1];
        normal.m_Z = pNormalsDataset->m_Vertices[nIndiceIndex + 2];

        pNormal = &normal;
    }

    IVector2  uv;
    IVector2* pUV = nullptr;

    // mesh contains texture coordinates?
    if ((pMesh->m_VB[0]->m_Format.m_Format & VertexFormat::IE_VF_TexCoords) && pUVDataset)
    {
        // calculate the uv index from the indice table
        const std::size_t uvIndex = pMeshDataset->m_Indices[vertexIndex] * 2;

        // is index out of bounds?
        if (uvIndex >= pUVDataset->m_UV.size())
            return false;

        // build the texture coordinate. NOTE several files contain negative values, force them to
        // be positive (assume that the user will correct its texture in this case)
        uv.m_X = std::fabs(pUVDataset->m_UV[uvIndex]);
        uv.m_Y = std::fabs(pUVDataset->m_UV[uvIndex + 1]);

        pUV = &uv;
    }

    // do apply a material to the vertex?
    if (pMatList && pMatListDataset)
    {
        std::size_t materialIndex;

        // get the material index to apply to this vertex
        if (pMatListDataset->m_MaterialCount == 1)
            materialIndex = pMatListDataset->m_MaterialIndices[0];
        else
            materialIndex = pMatListDataset->m_MaterialIndices[matListIndex];

        // get the material item
        IFileItem* pMaterialItem = GetMaterial(pMatList, materialIndex);

        // succeeded?
        if (pMaterialItem && pMaterialItem->m_ID == IE_DS_Material_ID)
        {
            // get the material dataset
            IMaterialDataset* pMaterialDataset = static_cast<IMaterialDataset*>(pMaterialItem->m_pDataset);

            // found it?
            if (pMaterialDataset)
            {
                // change the vertex color to match with the material one
                pMesh->m_VB[0]->m_Material.m_Color = pMaterialDataset->m_Color;

                // do the same thing for the print
                if (!pModel->m_MeshOnly)
                    pModel->m_Print[meshIndex]->m_Material.m_Color = pMesh->m_VB[0]->m_Material.m_Color;
            }
        }
    }

    // keep the newly added vertex index
    const std::size_t vbIndex = pMesh->m_VB[0]->m_Data.size();

    // add the next vertex to the buffer
    if (!VertexBufferAdd(&vertex,
                          pNormal,
                          pUV,
                          0,
                          fOnGetVertexColor,
                          pMesh->m_VB[0]))
        return false;

    // reset the previous vertex color
    pMesh->m_VB[0]->m_Material.m_Color = prevColor;

    // do draw mesh only, nothing more is required
    if (pModel->m_MeshOnly)
        return true;

    // also add the new vertex to the print if mesh is animated
    if (!VertexBufferAdd(&vertex,
                          pNormal,
                          pUV,
                          0,
                          fOnGetVertexColor,
                          pModel->m_Print[meshIndex]))
        return false;

    // reset the previous print vertex color
    pModel->m_Print[meshIndex]->m_Material.m_Color = prevColor;

    std::size_t weightIndex = 0;

    // link the newly added vertice to the mesh skin weights
    for (std::size_t i = 0; i < pItem->m_Children.size(); ++i)
        switch (pItem->m_Children[i]->m_ID)
        {
            case IE_DS_Skin_Weights_ID:
            {
                // found the skin weights dataset, get it
                ISkinWeightsDataset* pSkinWeightsDataset = static_cast<ISkinWeightsDataset*>(pItem->m_Children[i]->m_pDataset);

                // succeeded?
                if (!pSkinWeightsDataset)
                    return false;

                // iterate through the indices to link to mesh vertices
                for (std::size_t j = 0; j < pSkinWeightsDataset->m_Indices.size(); ++j)
                    // is current vertex index matching with one in the current skin weights?
                    if (pSkinWeightsDataset->m_Indices[j] == pMeshDataset->m_Indices[vertexIndex])
                        // add the new indice
                        pModel->m_MeshWeights[meshIndex]->m_SkinWeights[weightIndex]->m_IndexTable[j]->m_Data.push_back(vbIndex);

                ++weightIndex;
                continue;
            }

            default:
                continue;
        }

    return true;
}
//---------------------------------------------------------------------------
bool XModel::BuildAnimationSet(const IFileItem* pItem, IModel* pModel) const
{
    // allocate memory for a new animation set
    std::unique_ptr<IAnimationSet> pAnimationSet(new IAnimationSet());

    // keep the animation set index
    const std::size_t index = pModel->m_AnimationSet.size();

    // iterate through source animations
    for (std::size_t i = 0; i < pItem->m_Children.size(); ++i)
    {
        // allocate memory for a new animation
        std::unique_ptr<IAnimation> pAnimation(new IAnimation());

        // iterate through source animation keys
        for (std::size_t j = 0; j < pItem->m_Children[i]->m_Children.size(); ++j)
        {
            // is a link?
            if (pItem->m_Children[i]->m_Children[j]->m_ID == IE_DS_Link_ID)
            {
                size_t nameLength;

                // get the dataset containing the animation keys
                IGenericDataset* pDataset = static_cast<IGenericDataset*>(pItem->m_Children[i]->m_Children[j]->m_pDataset);

                if (!pDataset)
                    return false;

                // get the bone name to link with
                pAnimation->m_BoneName = pDataset->m_Name;

                continue;
            }

            // allocate memory for a new animation keys
            std::unique_ptr<IAnimationKeys> pAnimationKeys(new IAnimationKeys());

            // get the dataset containing the animation keys
            IAnimationKeysDataset* pDataset = static_cast<IAnimationKeysDataset*>(pItem->m_Children[i]->m_Children[j]->m_pDataset);

            // succeeded?
            if (!pDataset)
                return false;

            // update the animation key type
            pAnimationKeys->m_Type = pDataset->m_Type;

            // iterate through keys
            for (std::size_t k = 0; k < pDataset->m_Keys.size(); ++k)
            {
                // allocate memory for a new animation key
                std::unique_ptr<IAnimationKey> pAnimationKey(new IAnimationKey());

                // get the key frame
                pAnimationKey->m_Frame = pDataset->m_Keys[k]->m_Frame;

                const std::size_t valueCount = pDataset->m_Keys[k]->m_Values.size();

                // assign memory for values
                pAnimationKey->m_Values.resize(valueCount);

                for (std::size_t l = 0; l < valueCount; ++l)
                    pAnimationKey->m_Values[l] = pDataset->m_Keys[k]->m_Values[l];

                // set the key to the keys list
                pAnimationKeys->m_Keys.push_back(pAnimationKey.get());
                pAnimationKey.release();
            }

            // set the keys list to the animations
            pAnimation->m_Keys.push_back(pAnimationKeys.get());
            pAnimationKeys.release();
        }

        // set the animations to the animation set
        pAnimationSet->m_Animations.push_back(pAnimation.get());
        pAnimation.release();
    }

    // set the animation set to the model
    pModel->m_AnimationSet.push_back(pAnimationSet.get());
    pAnimationSet.release();

    return true;
}
//---------------------------------------------------------------------------
XModel::IFileItem* XModel::GetMaterial(const IFileItem* pItem, std::size_t index) const
{
    // is index out of bounds?
    if (index >= pItem->m_Children.size())
        return nullptr;

    // return the material matching with the indice. NOTE assume that the material list object only
    // contains materials as children and that the read order was the correct one
    return pItem->m_Children[index];
}
//---------------------------------------------------------------------------
bool XModel::VertexBufferAdd(const Vector3F*           pVertex,
                             const Vector3F*           pNormal,
                             const IVector2*           pUV,
                                   std::size_t         groupIndex,
                             const ITfOnGetVertexColor fOnGetVertexColor,
                                   VertexBuffer*       pVB) const
{
    // no vertex buffer to add to?
    if (!pVB)
        return false;

    // the stride should be already calculated
    if (!pVB->m_Format.m_Stride)
        return false;

    // keep the current offset
    std::size_t offset = pVB->m_Data.size();

    // allocate space for new vertex
    pVB->m_Data.resize(pVB->m_Data.size() + pVB->m_Format.m_Stride);

    // source vertex exists?
    if (!pVertex)
    {
        // cannot add a nonexistent vertex, fill with empty data in this case
        pVB->m_Data[offset]     = 0.0f;
        pVB->m_Data[offset + 1] = 0.0f;
        pVB->m_Data[offset + 2] = 0.0f;
    }
    else
    {
        // copy vertex from source
        pVB->m_Data[offset]     = pVertex->m_X;
        pVB->m_Data[offset + 1] = pVertex->m_Y;
        pVB->m_Data[offset + 2] = pVertex->m_Z;
    }

    offset += 3;

    // vertex has a normal?
    if (pVB->m_Format.m_Format & VertexFormat::IE_VF_Normals)
    {
        // source normal exists?
        if (!pNormal)
        {
            // cannot add a nonexistent normal, fill with empty data in this case
            pVB->m_Data[offset]     = 0.0f;
            pVB->m_Data[offset + 1] = 0.0f;
            pVB->m_Data[offset + 2] = 0.0f;
        }
        else
        {
            // copy normal from source
            pVB->m_Data[offset]     = pNormal->m_X;
            pVB->m_Data[offset + 1] = pNormal->m_Y;
            pVB->m_Data[offset + 2] = pNormal->m_Z;
        }

        offset += 3;
    }

    // vertex has UV texture coordinates?
    if (pVB->m_Format.m_Format & VertexFormat::IE_VF_TexCoords)
    {
        // source texture coordinates exists?
        if (!pUV)
        {
            // cannot add nonexistent texture coordinates, fill with empty data in this case
            pVB->m_Data[offset]     = 0.0f;
            pVB->m_Data[offset + 1] = 0.0f;
        }
        else
        {
            // copy texture coordinates from source
            pVB->m_Data[offset]     = pUV->m_X;
            pVB->m_Data[offset + 1] = pUV->m_Y;
        }

        offset += 2;
    }

    // vertex has color?
    if (pVB->m_Format.m_Format & VertexFormat::IE_VF_Colors)
    {
        ColorF color;

        // get the vertex color
        if (fOnGetVertexColor)
            color = fOnGetVertexColor(pVB, pNormal, groupIndex);
        else
            color = pVB->m_Material.m_Color;

        // set color data
        pVB->m_Data[offset]     = color.m_R;
        pVB->m_Data[offset + 1] = color.m_G;
        pVB->m_Data[offset + 2] = color.m_B;
        pVB->m_Data[offset + 3] = color.m_A;
    }

    return true;
}
//---------------------------------------------------------------------------
void XModel::BuildParentHierarchy(IBone* pBone, IBone* pParent, IModel* pModel) const
{
    // no model?
    if (!pModel)
        return;

    // set bone parent
    pBone->m_pParent = pParent;

    // link the bone to the mesh
    for (std::size_t i = 0; i < pModel->m_MeshToBoneDict.size(); ++i)
        if (pModel->m_MeshToBoneDict[i]->m_MeshIndex == pBone->m_MeshIndex)
            pModel->m_MeshToBoneDict[i]->m_pBone = pBone;

    // build children hierarchy
    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
        BuildParentHierarchy(pBone->m_Children[i], pBone, pModel);
}
//---------------------------------------------------------------------------
bool XModel::GetAnimationMatrix(const IAnimationSet* pAnimSet,
                                const IBone*         pBone,
                                      std::size_t    frame,
                                      Matrix4x4F&    matrix) const
{
    // no animation set?
    if (!pAnimSet)
        return false;

    // no bone?
    if (!pBone)
        return false;

    // iterate through animations
    for (std::size_t i = 0; i < pAnimSet->m_Animations.size(); ++i)
    {
        // found the animation matching with the bone for which the matrix should be get?
        if (pAnimSet->m_Animations[i]->m_pBone != pBone)
            continue;

        std::size_t rotFrame       = 0;
        std::size_t nextRotFrame   = 0;
        std::size_t posFrame       = 0;
        std::size_t nextPosFrame   = 0;
        std::size_t scaleFrame     = 0;
        std::size_t nextScaleFrame = 0;
        QuaternionF rotation;
        QuaternionF nextRotation;
        QuaternionF finalRotation;
        Vector3F    position;
        Vector3F    nextPosition;
        Vector3F    finalPosition;
        Vector3F    scaling;
        Vector3F    nextScaling;
        Vector3F    finalScaling;

        // iterate through animation keys
        for (std::size_t j = 0; j < pAnimSet->m_Animations[i]->m_Keys.size(); ++j)
        {
            std::size_t keyIndex = 0;

            // iterate through animation key items
            for (std::size_t k = 0; k < pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size(); ++k)
                if (frame >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[k]->m_Frame)
                    keyIndex = k;
                else
                    break;

            // search for keys type
            switch (pAnimSet->m_Animations[i]->m_Keys[j]->m_Type)
            {
                case IE_KT_Rotation:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 4)
                        return false;

                    // get the rotation quaternion at index
                    rotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    rotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    rotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    rotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[3];
                    rotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextRotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextRotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextRotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextRotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[3];
                        nextRotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextRotation.m_W = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextRotation.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextRotation.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextRotation.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[3];
                        nextRotFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case IE_KT_Scale:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 3)
                        return false;

                    // get the scale values at index
                    scaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    scaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    scaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    scaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextScaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextScaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextScaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextScaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextScaling.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextScaling.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextScaling.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextScaleFrame  = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case IE_KT_Position:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 3)
                        return false;

                    // get the position values at index
                    position.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[0];
                    position.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[1];
                    position.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[2];
                    posFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Frame;

                    // get the next rotation quaternion
                    if (keyIndex + 1 >= pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys.size())
                    {
                        nextPosition.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[0];
                        nextPosition.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[1];
                        nextPosition.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Values[2];
                        nextPosFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[0]->m_Frame;
                    }
                    else
                    {
                        nextPosition.m_X = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[0];
                        nextPosition.m_Y = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[1];
                        nextPosition.m_Z = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Values[2];
                        nextPosFrame     = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex + 1]->m_Frame;
                    }

                    continue;

                case IE_KT_MatrixKeys:
                    if (pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values.size() != 16)
                        return false;

                    // get the key matrix
                    for (std::size_t k = 0; k < 16; ++k)
                        matrix.m_Table[k / 4][k % 4] = pAnimSet->m_Animations[i]->m_Keys[j]->m_Keys[keyIndex]->m_Values[k];

                    return true;

                default:
                    continue;
            }
        }

        // calculate the frame delta, the frame length and the interpolation for the rotation
        float frameDelta    = frame        - rotFrame;
        float frameLength   = nextRotFrame - rotFrame;
        float interpolation = frameDelta / frameLength;

        bool error = false;

        // interpolate the rotation
        finalRotation = rotation.Slerp(nextRotation, interpolation, error);

        // calculate the frame delta, the frame length and the interpolation for the scaling
        frameDelta    = frame          - scaleFrame;
        frameLength   = nextScaleFrame - scaleFrame;
        interpolation = frameDelta / frameLength;

        // interpolate the scaling
        finalScaling.m_X = scaling.m_X + ((nextScaling.m_X - scaling.m_X) * interpolation);
        finalScaling.m_Y = scaling.m_Y + ((nextScaling.m_Y - scaling.m_Y) * interpolation);
        finalScaling.m_Z = scaling.m_Z + ((nextScaling.m_Z - scaling.m_Z) * interpolation);

        // calculate the frame delta, the frame length and the interpolation for the rotation
        frameDelta    = frame        - posFrame;
        frameLength   = nextPosFrame - posFrame;
        interpolation = frameDelta / frameLength;

        // interpolate the position
        finalPosition.m_X = position.m_X + ((nextPosition.m_X - position.m_X) * interpolation);
        finalPosition.m_Y = position.m_Y + ((nextPosition.m_Y - position.m_Y) * interpolation);
        finalPosition.m_Z = position.m_Z + ((nextPosition.m_Z - position.m_Z) * interpolation);

        Matrix4x4F  scaleMatrix = Matrix4x4F::Identity();
        Matrix4x4F  rotateMatrix;
        Matrix4x4F  translateMatrix = Matrix4x4F::Identity();

        // get the rotation quaternion and the scale and translate vectors
        scaleMatrix.Scale(finalScaling);
        rotateMatrix = finalRotation.ToMatrix();
        translateMatrix.Translate(finalPosition);

        Matrix4x4F buildMatrix;

        // build the final matrix
        buildMatrix = scaleMatrix.Multiply(rotateMatrix);
        matrix      = buildMatrix.Multiply(translateMatrix);

        return true;
    }

    return false;
}
//---------------------------------------------------------------------------
XModel::IBone* XModel::FindBone(IBone* pBone, const std::string& name) const
{
    // no bone?
    if (!pBone)
        return nullptr;

    // found the bone?
    if (!pBone->m_Name.empty() && pBone->m_Name == name)
        return pBone;

    // iterate through the bone children
    for (std::size_t i = 0; i < pBone->m_Children.size(); ++i)
    {
        // search in next children bone
        IBone* pChildBone = FindBone(pBone->m_Children[i], name);

        // found the bone?
        if (pChildBone)
            return pChildBone;
    }

    return nullptr;
}
//---------------------------------------------------------------------------
