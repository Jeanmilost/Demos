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

#pragma once

// classes
#include "Color.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "Vertex.h"
#include "Model.h"

/**
* DirectX .x model
*@author Jean-Milost Reymond
*/
class XModel
{
    public:
        /**
        * Called when a vertex color should be get
        *@param pVB - vertex buffer that will contain the vertex for which the color should be get
        *@param pNormal - vertex normal
        *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
        *@return RGBA color to apply to the vertex
        *@note This callback will be called only if the per-vertex color option is activated in the vertex
        *      buffer
        */
        typedef ColorF (*ITfOnGetVertexColor)(const VertexBuffer* pVB, const Vector3F* pNormal, std::size_t groupIndex);

        /**
        * Called when a texture should be loaded
        *@param textureName - texture name to load
        *@return the loaded texture
        *@note The loaded texture will be deleted internally, and should no longer be deleted from outside
        */
        typedef Texture* (*ITfOnLoadTexture)(const std::string& textureName);

        XModel();
        virtual ~XModel();

        /**
        * Opens a .x file
        *@param fileName - x file to open
        *@return true on success, otherwise false
        */
        virtual bool Open(const std::string& fileName);

        /**
        * Reads a x data
        *@param data - x data to open
        *@return true on success, otherwise false
        */
        virtual bool Read(const std::string& data);

        /**
        * Gets the model
        *@return the model, nullptr if no model or on error
        */
        virtual Model* GetModel() const;

        /**
        * Changes the vertex format template
        *@param vertFormatTemplate - new vertex format template
        */
        virtual void SetVertFormatTemplate(const VertexFormat& vertFormatTemplate);

        /**
        * Changes the vertex culling template
        *@param vertCullingTemplate - new vertex culling template
        */
        virtual void SetVertCullingTemplate(const VertexCulling& vertCullingTemplate);

        /**
        * Changes the material template
        *@param materialTemplate - new material template
        */
        virtual void SetMaterial(const Material& materialTemplate);

        /**
        * Sets the OnGetVertexColor callback
        *@param fOnGetVertexColor - callback function handle
        */
        void Set_OnGetVertexColor(ITfOnGetVertexColor fOnGetVertexColor);

        /**
        * Sets the OnLoadTexture callback
        *@param fOnLoadTexture - callback function handle
        */
        void Set_OnLoadTexture(ITfOnLoadTexture fOnLoadTexture);

    private:
        /**
        * X file tokens
        *@note Tokens MSDN reference:
        *      https://docs.microsoft.com/en-us/windows/desktop/direct3d9/tokens
        */
        enum IEToken
        {
            // custom tokens
            IE_T_Unknown       = 0,
            // record-bearing tokens
            IE_T_Name          = 1,
            IE_T_String        = 2,
            IE_T_Integer       = 3,
            IE_T_GUID          = 5,
            IE_T_Integer_List  = 6,
            IE_T_Float_List    = 7,
            // standalone tokens
            IE_T_Open_Brace    = 10,
            IE_T_Close_Brace   = 11,
            IE_T_Open_Parent   = 12,
            IE_T_Close_Parent  = 13,
            IE_T_Open_Bracket  = 14,
            IE_T_Close_Bracket = 15,
            IE_T_Open_Angle    = 16,
            IE_T_Close_Angle   = 17,
            IE_T_Dot           = 18,
            IE_T_Comma         = 19,
            IE_T_Semicolon     = 20,
            IE_T_Template      = 31,
            IE_T_Word          = 40,
            IE_T_DWord         = 41,
            IE_T_Float         = 42,
            IE_T_Double        = 43,
            IE_T_Char          = 44,
            IE_T_UChar         = 45,
            IE_T_SWord         = 46,
            IE_T_SDWord        = 47,
            IE_T_Void          = 48,
            IE_T_LPSTR         = 49,
            IE_T_Unicode       = 50,
            IE_T_CString       = 51,
            IE_T_Array         = 52
        };

        /**
        * X file data structure identifiers
        */
        enum IEDataStructID
        {
            IE_DS_Link_ID                   = -1, // set to -1 because not part of the official IDs
            IE_DS_Unknown                   =  0,
            IE_DS_Template_ID               =  60,
            IE_DS_Header_ID                 =  61,
            IE_DS_Frame_ID                  =  62,
            IE_DS_Frame_Transform_Matrix_ID =  63,
            IE_DS_Mesh_ID                   =  64,
            IE_DS_Mesh_Texture_Coords_ID    =  65,
            IE_DS_Mesh_Material_List_ID     =  66,
            IE_DS_Material_ID               =  67,
            IE_DS_Skin_Mesh_Header_ID       =  68,
            IE_DS_Skin_Weights_ID           =  69,
            IE_DS_Texture_Filename_ID       =  70,
            IE_DS_Mesh_Normals_ID           =  71,
            IE_DS_Animation_Set_ID          =  72,
            IE_DS_Animation_ID              =  73,
            IE_DS_Animation_Key_ID          =  74
        };

        /**
        * 2D vector
        */
        struct IVector2
        {
            float m_X;
            float m_Y;

            IVector2();
            ~IVector2();
        };

        /**
        * File header
        */
        struct IFileHeader
        {
            unsigned       m_Magic;
            unsigned short m_Major_Version;
            unsigned short m_Minor_Version;
            unsigned       m_Format;
            unsigned       m_Float_Size;

            IFileHeader();
            ~IFileHeader();
        };

        /**
        * Generic dataset containing only a name
        */
        struct IGenericDataset
        {
            std::string m_Name;

            IGenericDataset();
            virtual ~IGenericDataset();
        };

        /**
        * Dataset containing a header
        */
        struct IHeaderDataset : public IGenericDataset
        {
            std::size_t m_Major;
            std::size_t m_Minor;
            std::size_t m_Flags;
            std::size_t m_ReadValCount;

            IHeaderDataset();
            virtual ~IHeaderDataset();
        };

        /**
        * Dataset containing a matrix
        */
        struct IMatrixDataset : public IGenericDataset
        {
            Matrix4x4F  m_Matrix;
            std::size_t m_ReadValCount;

            IMatrixDataset();
            virtual ~IMatrixDataset();
        };

        /**
        * Dataset containing an indexed vertex buffer. Used to read the meshes and normals
        */
        struct IVertexBufferDataset : public IGenericDataset
        {
            typedef std::vector<float>       IVertices;
            typedef std::vector<std::size_t> IIndices;

            IVertices   m_Vertices;
            std::size_t m_VerticeTotal;
            IIndices    m_Indices;
            std::size_t m_IndiceTotal;

            IVertexBufferDataset();
            virtual ~IVertexBufferDataset();
        };

        /**
        * Dataset containing the texture coordinates
        */
        struct ITexCoordsDataset : public IGenericDataset
        {
            typedef std::vector<float> IUVCoords;

            IUVCoords   m_UV;
            std::size_t m_UVTotal;

            ITexCoordsDataset();
            virtual ~ITexCoordsDataset();
        };

        /**
        * Dataset containing the mesh material list
        */
        struct IMaterialListDataset : public IGenericDataset
        {
            typedef std::vector<std::size_t> IMaterialIndices;

            std::size_t      m_MaterialCount;
            IMaterialIndices m_MaterialIndices;
            std::size_t      m_MaterialIndiceTotal;

            IMaterialListDataset();
            virtual ~IMaterialListDataset();
        };

        /**
        * Dataset containing the mesh material list
        */
        struct IMaterialDataset : public IGenericDataset
        {
            ColorF      m_Color;
            float       m_SpecularExp;
            ColorF      m_SpecularColor;
            ColorF      m_EmisiveColor;
            std::size_t m_ReadValCount;

            IMaterialDataset();
            virtual ~IMaterialDataset();
        };

        /**
        * Index dictionary, allows an index to match with a sorted number.
        * Used to retrieve the weight index matching with a vertex index
        */
        typedef std::map<std::size_t, std::size_t> IIndexDictionary;

        /**
        * Dataset containing the skin weights
        */
        struct ISkinWeightsDataset : public IGenericDataset
        {
            typedef std::vector<std::size_t> IIndices;
            typedef std::vector<float>       IWeights;

            std::string      m_BoneName;
            std::size_t      m_ItemCount;
            IIndices         m_Indices;
            IWeights         m_Weights;
            IIndexDictionary m_IndexDictionary;
            Matrix4x4F       m_Matrix;
            std::size_t      m_ReadValCount;

            ISkinWeightsDataset();
            virtual ~ISkinWeightsDataset();

            /**
            * Builds the index dictionary
            */
            virtual void BuildDictionary();
        };

        /**
        * Dataset containing the mesh texture
        */
        struct ITextureDataset : public IGenericDataset
        {
            std::string m_FileName;

            ITextureDataset();
            virtual ~ITextureDataset();
        };

        /**
        * Dataset containing the animation key
        */
        struct IAnimationKeyDataset : public IGenericDataset
        {
            typedef std::vector<float> IValues;

            int         m_Frame;
            IValues     m_Values;
            std::size_t m_Total;

            IAnimationKeyDataset();
            virtual ~IAnimationKeyDataset();
        };

        /**
        * Dataset containing the animation keys
        */
        struct IAnimationKeysDataset : public IGenericDataset
        {
            typedef std::vector<IAnimationKeyDataset*> IKeys;

            Model::IEAnimKeyType m_Type;
            IKeys                m_Keys;
            std::size_t          m_KeyTotal;
            std::size_t          m_KeyIndex;
            std::size_t          m_ReadValCount;

            IAnimationKeysDataset();
            virtual ~IAnimationKeysDataset();
        };

        /**
        * X file item
        */
        struct IFileItem
        {
            typedef std::vector<IFileItem*> IFileItems;

            IEDataStructID   m_ID;
            IFileItem*       m_pParent;
            IFileItems       m_Children;
            IGenericDataset* m_pDataset;
            bool             m_Opened;
            bool             m_ContentRead;

            IFileItem();
            ~IFileItem();
        };

        Model*              m_pModel;
        VertexFormat        m_VertFormatTemplate;
        VertexCulling       m_VertCullingTemplate;
        Material            m_MaterialTemplate;
        bool                m_MeshOnly;
        bool                m_PoseOnly;
        ITfOnGetVertexColor m_fOnGetVertexColor;
        ITfOnLoadTexture    m_fOnLoadTexture;

        /**
        * Parses the x file content
        *@param data - the x file data to parse
        *@param[in, out] offset - offset
        *@param[in, out] pItem - x item
        *@return true on success, otherwise false
        */
        bool Parse(const std::string& data, std::size_t& offset, IFileItem*& pItem) const;

        /**
        * Parses a X word
        *@param data - the x file data to parse
        *@param startOffset - word start offset
        *@param endOffset - word end offset
        *@param[in, out] pItem - x item
        *@return true on success, otherwise false
        */
        bool ParseWord(const std::string& data, std::size_t startOffset, std::size_t endOffset, IFileItem*& pItem) const;

        /**
        * Skips the separators
        *@param data - the file data
        *@param offset - the offset from which the separators should be skipped
        */
        void SkipSeparators(const std::string& data, std::size_t& offset) const;

        /**
        * Skips a line
        *@param data - the file data
        *@param offset - the offset from which the line should be skipped
        */
        void SkipLine(const std::string& data, std::size_t& offset) const;

        /**
        * Skips the carriage return and line feed chars
        *@param data - the file data
        *@param offset - the offset from which the chars should be skipped
        */
        void SkipCRLF(const std::string& data, std::size_t& offset) const;

        /**
        * Translates a word
        *@param data - the file data
        *@param startOffset - word start offset
        *@param endOffset - word end offset
        *@return true on success, otherwise false
        */
        int TranslateWord(const std::string& data, std::size_t startOffset, std::size_t endOffset) const;

        /**
        * Reads the dataset name
        *@param data - the file data
        *@param startOffset - name start offset
        *@param endOffset - name end offset
        *@return true on success, otherwise false
        */
        bool ReadDatasetName(const std::string& data,
                                   std::size_t  startOffset,
                                   std::size_t  endOffset,
                                   IFileItem*   pItem) const;

        /**
        * Gets the data structure identifier
        *@param word - word to convert to data structure identifier
        *@return the data structure identifier
        */
        IEDataStructID GetDataStructureID(const std::string& word) const;

        /**
        * Gets a text from a data
        *@param startOffset - text start offset
        *@param endOffset - text end offset
        *@return the text, empty string if not found or on error
        */
        std::string GetText(const std::string& data, std::size_t startOffset, std::size_t endOffset) const;

        /**
        * Adds a child to a dataset
        *@param pItem - file item in which the child will be added
        *@param id - dataset identifier
        *@param pDataset - dataset to add to
        *@return the child dataset
        */
        IFileItem* AddChild(IFileItem* pItem, IEDataStructID id, IGenericDataset* pDataset) const;

        /**
        * Converts the opened file item to a ready-to-use x model
        *@param pItem - file item containing the data
        *@param pModel - target x model
        *@param pBone - skeleton root bone
        *@param pVertFormat - template vertex format
        *@param pVertCulling - template vertex culling
        *@param pMaterial - material
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        *@param fOnLoadTexture - load texture callback function to use, nullptr if not used
        *@return true on success, otherwise false
        */
        bool ItemToModel(const IFileItem*          pItem,
                               Model*              pModel,
                               Model::IBone*       pBone,
                         const VertexFormat*       pVertFormat,
                         const VertexCulling*      pVertCulling,
                         const Material*           pMaterial,
                         const ITfOnGetVertexColor fOnGetVertexColor,
                         const ITfOnLoadTexture    fOnLoadTexture) const;

        /**
        * Builds a mesh
        *@param pItem - file item containing the data
        *@param pModel - target x model
        *@param pBone - skeleton root bone
        *@param pVertFormat - template vertex format
        *@param pVertCulling - template vertex culling
        *@param pMaterial - material
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        *@param fOnLoadTexture - load texture callback function to use, nullptr if not used
        *@return true on success, otherwise false
        */
        bool BuildMesh(const IFileItem*          pItem,
                             Model*              pModel,
                             Model::IBone*       pBone,
                       const VertexFormat*       pVertFormat,
                       const VertexCulling*      pVertCulling,
                       const Material*           pMaterial,
                       const ITfOnGetVertexColor fOnGetVertexColor,
                       const ITfOnLoadTexture    fOnLoadTexture) const;

        /**
        * Builds a vertex
        *@param pItem - file item containing the data
        *@param pModel - target x model
        *@param pMesh - template mesh
        *@param meshIndex - mesh index
        *@param vertexIndex - vertex index
        *@param matListIndex - material list index
        *@param prevColor - previous color
        *@param pMeshDataset - the mesh dataset
        *@param pNormalsDataset- the normal dataset
        *@param pUVDataset - then texture coordinates dataset
        *@param pMatList - material list
        *@param pMatListDataset - the material list dataset
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        *@return true on success, otherwise false
        */
        bool BuildVertex(const IFileItem*            pItem,
                               Model*                pModel,
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
                         const ITfOnGetVertexColor   fOnGetVertexColor) const;

        /**
        * Builds the animation set
        *@param pItem - file item containing the animations
        *@param pModel - x model
        *@return true on success, otherwise false
        */
        bool BuildAnimationSet(const IFileItem* pItem, Model* pModel) const;

        /**
        * Gets the material
        *@param pItem - file item containing the material
        *@param index - material index
        *@return the material, nullptr if not found or on error
        */
        IFileItem* GetMaterial(const IFileItem* pItem, std::size_t index) const;

        /**
        * Adds a vertex to a vertex buffer
        *@param pVertex - vertex
        *@param pNormal - normal
        *@param pUV - texture coordinate
        *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
        *@param fOnGetVertexColor - get vertex color callback function to use, nullptr if not used
        *@param pVB - vertex buffer to add to
        *@return true on success, otherwise false
        */
        bool VertexBufferAdd(const Vector3F*           pVertex,
                             const Vector3F*           pNormal,
                             const IVector2*           pUV,
                                   std::size_t         groupIndex,
                             const ITfOnGetVertexColor fOnGetVertexColor,
                                   VertexBuffer*       pVB) const;

        /**
        * Builds the parent hierarchy
        *@param pBone - bone to link with parent
        *@param pParent - parent bone
        *@param pModel - x model
        */
        void BuildParentHierarchy(Model::IBone* pBone, Model::IBone* pParent, Model* pModel) const;

        /**
        * Gets the animation matrix
        *@param pAnimSet - animation set containing the animation to search
        *@param pBone - skeleton root bone
        *@param frame - animation frame
        *@param[out] matrix - animation matrix
        *@return true on success, otherwise false
        */
        bool GetAnimationMatrix(const Model::IAnimationSet* pAnimSet,
                                const Model::IBone*         pBone,
                                      std::size_t           frame,
                                      Matrix4x4F&           matrix) const;
};
