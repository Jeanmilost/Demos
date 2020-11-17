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

/**
* DirectX .x model
*@author Jean-Milost Reymond
*/
class XModel
{
    public:
        /**
        * Animation key type
        */
        enum IEAnimKeyType
        {
            IE_KT_Unknown    = -1,
            IE_KT_Rotation   =  0,
            IE_KT_Scale      =  1,
            IE_KT_Position   =  2,
            IE_KT_MatrixKeys =  4
        };

        /**
        * Bone, it's a hierarchical local transformation to apply to a mesh
        */
        struct IBone
        {
            typedef std::vector<IBone*> IBones;

            std::string m_Name;      // bone name
            Matrix4x4F  m_Matrix;    // matrix containing the bone transformation to apply
            IBone*      m_pParent;   // bone parent, root bone if 0
            IBones      m_Children;  // bone children
            std::size_t m_MeshIndex; // mesh index in the mesh to bone dictionary

            IBone();
            virtual ~IBone();
        };

        /**
        * DirectX (.x) model mesh to bone dictionary item
        */
        struct IMeshToBoneItem
        {
            IBone*      m_pBone;
            std::size_t m_MeshIndex;

            IMeshToBoneItem();
            virtual ~IMeshToBoneItem();
        };

        /**
        * Skin weights index table
        */
        struct ISkinWeightIndexTable
        {
            typedef std::vector<std::size_t> IData;

            IData m_Data;

            ISkinWeightIndexTable();
            virtual ~ISkinWeightIndexTable();
        };

        /**
        * Skin weights, it's a group of vertices influenced by a bone
        */
        struct ISkinWeights
        {
            typedef std::vector<ISkinWeightIndexTable*> IIndexTable;
            typedef std::vector<float>                  IWeightsData;

            std::string  m_BoneName;   // linked bone name
            IBone*       m_pBone;      // linked bone
            Matrix4x4F   m_Matrix;     // matrix to transform the mesh vertices to the bone space
            std::size_t  m_MeshIndex;  // source mesh index
            IIndexTable  m_IndexTable; // table containing the indices of the vertices to modify in the source mesh
            IWeightsData m_Weights;    // weights indicating the bone influence on vertices, between 0.0f and 1.0f

            ISkinWeights();
            virtual ~ISkinWeights();
        };

        /**
        * Skin weights belonging to a mesh
        */
        struct IMeshSkinWeights
        {
            typedef std::vector<ISkinWeights*> ISkinWeightsData;

            ISkinWeightsData m_SkinWeights;

            IMeshSkinWeights();
            virtual ~IMeshSkinWeights();
        };

        /**
        * Animation key, may be a rotation, a translation, a scale, a matrix, ...
        */
        struct IAnimationKey
        {
            typedef std::vector<float> IValues;

            std::size_t m_Frame;
            IValues     m_Values;

            IAnimationKey();
            ~IAnimationKey();
        };

        /**
        * Animation key list
        */
        struct IAnimationKeys
        {
            typedef std::vector<IAnimationKey*> IKeys;

            IEAnimKeyType m_Type;
            IKeys         m_Keys;

            IAnimationKeys();
            ~IAnimationKeys();
        };

        /**
        * Animation
        */
        struct IAnimation
        {
            typedef std::vector<IAnimationKeys*> IKeys;

            std::string m_BoneName;
            IBone*      m_pBone;
            IKeys       m_Keys;

            IAnimation();
            ~IAnimation();
        };

        /**
        * Set of animations
        */
        struct IAnimationSet
        {
            typedef std::vector<IAnimation*> IAnimations;

            IAnimations m_Animations;

            IAnimationSet();
            ~IAnimationSet();
        };

        /**
        * DirectX (.x) model
        *@note Each mesh is connected to its own weights count and skeleton, sorted in the same order in each list
        */
        struct IModel
        {
            std::vector<Mesh*>             m_Mesh;           // meshes composing the model
            std::vector<VertexBuffer*>     m_Print;          // printed meshes (i.e ready to be painted)
            std::vector<IMeshSkinWeights*> m_MeshWeights;    // skin weights belonging to a mesh
            std::vector<IMeshToBoneItem*>  m_MeshToBoneDict; // mesh to bone dictionary
            std::vector<IAnimationSet*>    m_AnimationSet;   // set of animations to apply to bones
            IBone*                         m_pSkeleton;      // model skeleton
            bool                           m_MeshOnly;       // if activated, only the mesh will be drawn. All other data will be ignored
            bool                           m_PoseOnly;       // if activated, the model will take the default pose but will not be animated

            IModel();
            virtual ~IModel();
        };

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

        /**
        * Called when a skin should be applied to a model
        *@param index - skin index (in case the model contains several skins)
        *@param pMaterial - material containing the skin to apply
        *@param[in, out] pCanRelease - if 1, the skin content may be released after the skin is applied
        */
        //REM typedef void (*ITfOnApplySkin)(std::size_t index, const Material* pMaterial, int* pCanRelease);

        /**
        * Called when a texture should be deleted
        *@param pTexture - texture to delete
        */
        //REM typedef void (*ITfOnDeleteTexture)(const Texture* pTexture);

        XModel();
        virtual ~XModel();

        /**
        * Opens a .mhx2 file
        *@param fileName - mhx2 file to open
        *@return true on success, otherwise false
        */
        virtual bool Open(const std::string& fileName);

        /**
        * Reads a mhx2 data
        *@param data - mhx2 data to open
        *@return true on success, otherwise false
        */
        virtual bool Read(const std::string& data);

        /**
        * Gets the model
        *@return the model, nullptr if no model or on error
        */
        virtual IModel* GetModel() const;

        void Set_OnGetVertexColor(ITfOnGetVertexColor fOnGetVertexColor);

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
        * Dataset containing the skin weights
        */
        struct ISkinWeightsDataset : public IGenericDataset
        {
            typedef std::vector<std::size_t> IIndices;
            typedef std::vector<float>       IWeights;

            std::string m_BoneName;
            std::size_t m_ItemCount;
            IIndices    m_Indices;
            IWeights    m_Weights;
            Matrix4x4F  m_Matrix;
            std::size_t m_ReadValCount;
            std::size_t m_BoneIndex;
            std::size_t m_MeshIndex;

            ISkinWeightsDataset();
            virtual ~ISkinWeightsDataset();
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

            IEAnimKeyType m_Type;
            IKeys         m_Keys;
            std::size_t   m_KeyTotal;
            std::size_t   m_KeyIndex;
            std::size_t   m_ReadValCount;

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

        IModel*             m_pModel;
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

        void SkipSeparators(const std::string& data, std::size_t& offset) const;

        void SkipLine(const std::string& data, std::size_t& offset) const;

        void SkipCRLF(const std::string& data, std::size_t& offset) const;

        int TranslateWord(const std::string& data, std::size_t startOffset, std::size_t endOffset) const;

        bool ReadDatasetName(const std::string& data,
                                   std::size_t  startOffset,
                                   std::size_t  endOffset,
                                   IFileItem*   pItem) const;

        IEDataStructID GetDataStructureID(const std::string& word) const;

        std::string GetText(const std::string& data, std::size_t startOffset, std::size_t endOffset) const;

        IFileItem* AddChild(IFileItem* pItem, IEDataStructID id, IGenericDataset* pDataset) const;

        bool ItemToModel(const IFileItem*          pItem,
                               IModel*             pModel,
                               IBone*              pBone,
                         const VertexFormat*       pVertFormat,
                         const VertexCulling*      pVertCulling,
                         const Material*           pMaterial,
                         const ITfOnGetVertexColor fOnGetVertexColor,
                         const ITfOnLoadTexture    fOnLoadTexture) const;

        bool BuildMesh(const IFileItem*          pItem,
                             IModel*             pModel,
                             IBone*              pBone,
                       const VertexFormat*       pVertFormat,
                       const VertexCulling*      pVertCulling,
                       const Material*           pMaterial,
                       const ITfOnGetVertexColor fOnGetVertexColor,
                       const ITfOnLoadTexture    fOnLoadTexture) const;

        bool BuildVertex(const IFileItem*            pItem,
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
                         const ITfOnGetVertexColor   fOnGetVertexColor) const;

        bool BuildAnimationSet(const IFileItem* pItem, IModel* pModel) const;

        IFileItem* GetMaterial(const IFileItem* pItem, std::size_t index) const;

        /**
        * Adds a vertex to a vertex buffer
        *@param pVertex - vertex
        *@param pNormal - normal
        *@param pUV - texture coordinate
        *@param groupIndex - the vertex group index (e.g. the inner and outer vertices of a ring)
        *@param fOnGetVertexColor - get vertex color callback function to use, 0 if not used
        *@param[in, out] pVB - vertex buffer to add to
        *@return true on success, otherwise false
        */
        bool VertexBufferAdd(const Vector3F*           pVertex,
                             const Vector3F*           pNormal,
                             const IVector2*           pUV,
                                   std::size_t         groupIndex,
                             const ITfOnGetVertexColor fOnGetVertexColor,
                                   VertexBuffer*       pVB) const;

        void BuildParentHierarchy(IBone* pBone, IBone* pParent, IModel* pModel) const;

        /**
        * Finds a bone in the skeleton
        *@param pBone - root boneto search from
        *@param name - bone name to find
        *@return the bone, nullptr if not found or on error
        */
        IBone* FindBone(IBone* pBone, const std::string& name) const;
};
