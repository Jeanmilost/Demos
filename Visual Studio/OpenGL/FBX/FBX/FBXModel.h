/****************************************************************************
 * ==> FBXModel ------------------------------------------------------------*
 ****************************************************************************
 * Description : Filmbox .fbx model                                         *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - fbx model reader                                           *
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
#include "Quaternion.h"
#include "Vertex.h"
#include "Model.h"

// std
#include <set>

/**
* FilmBox (.fbx) model
*@author Jean-Milost Reymond
*/
class FBXModel
{
    public:
        /**
        * FBX data type
        */
        enum class IEDataType
        {
            IE_DT_Unknown = 0,
            IE_DT_Data,
            IE_DT_Int,
            IE_DT_Float,
            IE_DT_String,
            IE_DT_KeySeparator,
            IE_DT_ValueSeparator,
            IE_DT_StartNodeSeparator,
            IE_DT_EndNodeSeparator,
            IE_DT_Comment
        };

        /**
        * FBX data
        */
        struct IFBXData
        {
            IEDataType         m_Type;
            const std::string* m_pData;
            std::size_t        m_Start;
            std::size_t        m_End;
            bool               m_IsCounter;

            /**
            * Constructor
            *@param pData - data to read from
            */
            IFBXData(const std::string* pData);

            /**
            * Constructor
            *@param type - data type
            *@param pData - data to read from
            *@param start - data start index
            *@param end - data end index
            */
            IFBXData(IEDataType type, const std::string* pData, std::size_t start, std::size_t end);

            /**
            * Gets raw value as a string
            *@return string containing raw value, empty string if not found or on error
            */
            virtual std::string GetRaw() const;

            /**
            * Gets data content as a boolean value
            *@return value, false if not found or on error
            */
            virtual bool GetBool() const;

            /**
            * Gets data content as an integer value
            *@return value, 0 if not found or on error
            */
            virtual int GetInt() const;

            /**
            * Gets data content as a long integer value
            *@return value, 0L if not found or on error
            */
            virtual long long GetLongLong() const;

            /**
            * Gets data content as a float value
            *@return value, 0.0f if not found or on error
            */
            virtual float GetFloat() const;

            /**
            * Gets data content as a double value
            *@return value, 0.0 if not found or on error
            */
            virtual double GetDouble() const;

            /**
            * Gets data content as a string
            *@return string, empty string if not found or on error
            */
            virtual std::string GetStr() const;
        };

        /**
        * FBX dataset
        */
        typedef std::vector<IFBXData*> IFBXDataset;

        /**
        * FBX item
        */
        class IFBXItem
        {
            public:
                IFBXItem();
                virtual ~IFBXItem();

                /**
                * Sets the item name
                *@param name - item name
                */
                virtual void SetName(const std::string& name);

                /**
                * Gets the item name
                *@return the item name
                */
                virtual std::string GetName() const;

                /**
                * Sets the item parent
                *@param pParent - item parent
                */
                virtual void SetParent(IFBXItem* pParent);

                /**
                * Gets the item parent
                *@return the item parent
                */
                virtual IFBXItem* GetParent() const;

                /**
                * Adds a value to a node
                *@param pValue - value to add
                *@note The value will be deleted internally, do not delete it from outside
                */
                virtual void AddValue(IFBXData* pValue);

                /**
                * Gets a value contained in the node
                *@param index - value index
                *@return value (as data), nullptr if not found or on error
                */
                virtual IFBXData* GetValue(std::size_t index) const;

                /**
                * Gets the value count
                *@return the value count
                */
                virtual std::size_t GetValueCount() const;

            private:
                IFBXItem*   m_pParent;
                IFBXDataset m_Values;
                std::string m_Name;
        };

        /**
        * FBX items
        */
        typedef std::vector<IFBXItem*> IFBXItems;

        /**
        * FBX property type
        */
        enum class IEPropType
        {
            IE_PT_Unknown = 0,
            IE_PT_Bool,
            IE_PT_Int,
            IE_PT_Double,
            IE_PT_ULongLong,
            IE_PT_String,
            IE_PT_Number,
            IE_PT_Color,
            IE_PT_ColorRGB,
            IE_PT_Vector,
            IE_PT_Vector3D,
            IE_PT_Time,
            IE_PT_DateTime,
            IE_PT_Enum,
            IE_PT_LclTranslation,
            IE_PT_LclRotation,
            IE_PT_LclScaling,
            IE_PT_Visibility,
            IE_PT_VisibilityInheritance,
            IE_PT_Compound,
            IE_PT_Object,
            IE_PT_Array
        };

        /**
        * FBX property
        */
        class IFBXProperty : public IFBXItem
        {
            public:
                /**
                * Constructor
                *@param propType - property type
                */
                IFBXProperty(IEPropType propType);

                virtual ~IFBXProperty();

                /**
                * Gets the item name
                *@return the item name, empty string if not found or on error
                */
                virtual std::string GetPropName() const;

                /**
                * Gets the property type
                *@return the property type
                */
                virtual IEPropType GetType() const;

            protected:
                bool m_Cached;

            private:
                std::string m_PropName;
                IEPropType  m_PropType;
        };

        /**
        * FBX properties
        */
        typedef std::vector<IFBXProperty*> IFBXProperties;

        /**
        * FBX unknown property
        */
        class IFBXUnknownProperty : public IFBXProperty
        {
            public:
                IFBXUnknownProperty();
                virtual ~IFBXUnknownProperty();
        };

        /**
        * FBX boolean property
        */
        class IFBXBoolProperty : public IFBXProperty
        {
            public:
                IFBXBoolProperty();
                virtual ~IFBXBoolProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual bool Get() const;

            private:
                bool m_Value;
        };

        /**
        * FBX integer property
        */
        class IFBXIntProperty : public IFBXProperty
        {
            public:
                IFBXIntProperty();
                virtual ~IFBXIntProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual int Get() const;

            private:
                int m_Value;
        };

        /**
        * FBX double property
        */
        class IFBXDoubleProperty : public IFBXProperty
        {
            public:
                IFBXDoubleProperty();
                virtual ~IFBXDoubleProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual double Get() const;

            private:
                double m_Value;
        };

        /**
        * FBX long long property
        */
        class IFBXLongLongProperty : public IFBXProperty
        {
            public:
                IFBXLongLongProperty();
                virtual ~IFBXLongLongProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual long long Get() const;

            private:
                long long m_Value;
        };

        /**
        * FBX string property
        */
        class IFBXStringProperty : public IFBXProperty
        {
            public:
                IFBXStringProperty();
                virtual ~IFBXStringProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual std::string Get() const;

            private:
                std::string m_Value;
        };

        /**
        * FBX number property
        */
        class IFBXNumberProperty : public IFBXProperty
        {
            public:
                IFBXNumberProperty();
                virtual ~IFBXNumberProperty();

                /**
                * Gets the value
                *@param index - the value index
                *@return the value, 0.0 if not found or on error
                */
                virtual double Get(std::size_t index) const;

                /**
                * Gets the value count
                *@return the value count
                */
                virtual std::size_t GetCount() const;

            private:
                std::vector<double> m_Values;

                /**
                * Gets and caches all the values
                */
                void GetValues() const;
        };

        /**
        * FBX color property
        */
        class IFBXColorProperty : public IFBXProperty
        {
            public:
                IFBXColorProperty();
                virtual ~IFBXColorProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual ColorF Get() const;

            private:
                ColorF m_Value;
        };

        /**
        * FBX color RGB property
        */
        class IFBXColorRGBProperty : public IFBXProperty
        {
            public:
                IFBXColorRGBProperty();
                virtual ~IFBXColorRGBProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual ColorF Get() const;

            private:
                ColorF m_Value;
        };

        /**
        * FBX vector property
        */
        class IFBXVectorProperty : public IFBXProperty
        {
            public:
                IFBXVectorProperty();
                virtual ~IFBXVectorProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual Vector3F Get() const;

            private:
                Vector3F m_Value;
        };

        /**
        * FBX vector 3D property
        */
        class IFBXVector3DProperty : public IFBXProperty
        {
            public:
                IFBXVector3DProperty();
                virtual ~IFBXVector3DProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual Vector3F Get() const;

            private:
                Vector3F m_Value;
        };

        /**
        * FBX time property
        */
        class IFBXTimeProperty : public IFBXProperty
        {
            public:
                IFBXTimeProperty();
                virtual ~IFBXTimeProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual time_t Get() const;

            private:
                time_t m_Value;
        };

        /**
        * FBX date and time property
        */
        class IFBXDateTimeProperty : public IFBXProperty
        {
            public:
                IFBXDateTimeProperty();
                virtual ~IFBXDateTimeProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual std::string Get() const;

            private:
                std::string m_Value;
        };

        /**
        * FBX enumerated value property
        */
        class IFBXEnumProperty : public IFBXProperty
        {
            public:
                IFBXEnumProperty();
                virtual ~IFBXEnumProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual int Get() const;

            private:
                int m_Value;
        };

        /**
        * FBX lcl translation property
        */
        class IFBXLclTranslationProperty : public IFBXProperty
        {
            public:
                IFBXLclTranslationProperty();
                virtual ~IFBXLclTranslationProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual Vector3F Get() const;

            private:
                Vector3F m_Value;
        };

        /**
        * FBX lcl rotation property
        */
        class IFBXLclRotationProperty : public IFBXProperty
        {
            public:
                IFBXLclRotationProperty();
                virtual ~IFBXLclRotationProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual QuaternionF Get() const;

            private:
                QuaternionF m_Value;
        };

        /**
        * FBX lcl scaling property
        */
        class IFBXLclScalingProperty : public IFBXProperty
        {
            public:
                IFBXLclScalingProperty();
                virtual ~IFBXLclScalingProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual Vector3F Get() const;

            private:
                Vector3F m_Value;
        };

        /**
        * FBX visibility property
        */
        class IFBXVisibilityProperty : public IFBXProperty
        {
            public:
                IFBXVisibilityProperty();
                virtual ~IFBXVisibilityProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual bool Get() const;

            private:
                bool m_Value;
        };

        /**
        * FBX visibility inheritance property
        */
        class IFBXVisibilityInheritanceProperty : public IFBXProperty
        {
            public:
                IFBXVisibilityInheritanceProperty();
                virtual ~IFBXVisibilityInheritanceProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual bool Get() const;

            private:
                bool m_Value;
        };

        /**
        * FBX compound property
        */
        class IFBXCompoundProperty : public IFBXProperty
        {
            public:
                IFBXCompoundProperty();
                virtual ~IFBXCompoundProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual std::string Get() const;

            private:
                std::string m_Value;
        };

        /**
        * FBX object property
        */
        class IFBXObjectProperty : public IFBXProperty
        {
            public:
                IFBXObjectProperty();
                virtual ~IFBXObjectProperty();

                /**
                * Gets the value
                *@return the value
                */
                virtual std::string Get() const;

            private:
                std::string m_Value;
        };

        /**
        * FBX array property
        */
        class IFBXArrayProperty : public IFBXProperty
        {
            public:
                /**
                * Constructor
                *@param capacity - array capacity, ignored if 0
                */
                IFBXArrayProperty(std::size_t capacity);

                virtual ~IFBXArrayProperty();

                /**
                * Gets the value as integer
                *@param index - the value index
                *@return the value, 0 if not found or on error
                */
                virtual int GetI(std::size_t index) const;

                /**
                * Gets the value as float
                *@param index - the value index
                *@return the value, 0.0f if not found or on error
                */
                virtual float GetF(std::size_t index) const;

                /**
                * Gets the value as double
                *@param index - the value index
                *@return the value, 0.0 if not found or on error
                */
                virtual double GetD(std::size_t index) const;

                /**
                * Gets the value count
                *@return the value count
                */
                virtual std::size_t GetCount() const;

                /**
                * Gets the values array pointer
                *@return the values array pointer
                */
                const std::vector<double>* GetPtr() const;

            private:
                std::vector<double> m_Values;
                std::size_t         m_Capacity;

                /**
                * Gets and caches all the values
                */
                void GetValues() const;
        };

        /**
        * FBX node
        */
        class IFBXNode : public IFBXItem
        {
            public:
                IFBXNode();
                virtual ~IFBXNode();

                /**
                * Adds a property to a node
                *@param pProp - property to add
                *@note The property will be deleted internally, do not delete it from outside
                */
                virtual void AddProp(IFBXProperty* pProp);

                /**
                * Gets a property contained in the node
                *@param index - property index
                *@return property, nullptr if not found or on error
                */
                virtual IFBXProperty* GetProp(std::size_t index) const;

                /**
                * Gets the property count
                *@return the property count
                */
                virtual std::size_t GetPropCount() const;

                /**
                * Adds a child to a node
                *@param pChild - child to add
                *@note The child will be deleted internally, do not delete it from outside
                */
                virtual void AddChild(IFBXNode* pChild);

                /**
                * Gets a child contained in the node
                *@param index - child index
                *@return child, nullptr if not found or on error
                */
                virtual IFBXNode* GetChild(std::size_t index) const;

                /**
                * Gets the child count
                *@return the child count
                */
                virtual std::size_t GetChildCount() const;

            private:
                IFBXItems m_Props;
                IFBXItems m_Children;
        };

        /**
        * FBX nodes
        */
        typedef std::vector<IFBXNode*> IFBXNodes;

        /**
        * Called when a texture should be loaded
        *@param textureName - texture name to load
        *@param is32bit - if true, the image should be opened in 32 bit BGRA format
        *@return the loaded texture
        *@note The loaded texture will be deleted internally, and should no longer be deleted from outside
        */
        typedef Texture* (*ITfOnLoadTexture)(const std::string& textureName, bool is32bit);

        FBXModel();
        virtual ~FBXModel();

        /**
        * Clears the model
        */
        virtual void Clear();

        /**
        * Opens a FBX file
        *@param fileName - FBX file to open
        *@return true on success, otherwise false
        */
        virtual bool Open(const std::string& fileName);

        /**
        * Reads a FBX data
        *@param data - FBX data to open
        *@return true on success, otherwise false
        */
        virtual bool Read(const std::string& data);

        virtual Model* GetModel() const;

        /**
        * Sets the OnLoadTexture callback
        *@param fOnLoadTexture - callback function handle
        */
        void Set_OnLoadTexture(ITfOnLoadTexture fOnLoadTexture);

    private:
        /**
        * Unique IDs to items dictionary
        */
        typedef std::map<std::string, IFBXItem*> IItemDictionary;

        /**
        * FBX helper functions
        */
        class IFBXHelper
        {
            public:
                /**
                * Trims all unused chars before and after a string
                *@param str - string to trim
                @return trimmed string
                */
                static std::string Trim(const std::string& str);

                /**
                * Forces all the chars between a and z to be in lower case
                *@param str - string to lower
                *@return lowered string
                */
                static std::string ToLower(const std::string& str);

                /**
                * Splits a string by delimiter
                *@param str - string to split
                *@param delimiter - delimiter
                *@param[out] result - vector containing split strings
                */
                static void Split(const std::string&              str,
                                  const std::string&              delimiter,
                                        std::vector<std::string>& result);

                /**
                * Converts a string to a boolean value
                *@param str - string to convert
                *@return converted boolean value
                */
                static bool StrToBool(const std::string& str);
        };

        /**
        * Connection types
        */
        enum class IEConnectionType
        {
            IE_CT_ObjObj,  // object to object (OO) connection
            IE_CT_ObjProp, // object to property (OP) connection
            IE_CT_PropObj, // property to object (PO) connection
            IE_CT_PropProp // property to property (PP) connection
        };

        /**
        * Node types
        */
        enum class IENodeType
        {
            IE_NT_Model,
            IE_NT_Geometry,
            IE_NT_Material,
            IE_NT_Texture,
            IE_NT_Video,
            IE_NT_Deformer,
            IE_NT_NodeAttribute,
            IE_NT_AnimationCurve,
            IE_NT_AnimationCurveNode
        };

        struct IFBXLink;

        /**
        * FBX link list
        */
        typedef std::vector<IFBXLink*> IFBXLinks;

        /**
        * FBX link
        */
        struct IFBXLink
        {
            IENodeType  m_NodeType;
            IFBXNode*   m_pNode;
            IFBXLink*   m_pParent;
            IFBXLinks   m_Children;
            std::string m_PropDesc;

            IFBXLink();
            ~IFBXLink();
        };

        /**
         * Mesh template, contains the source data to use to generate the model meshes vertex buffers
         */
        struct IMeshTemplate
        {
            const std::vector<double>* m_pVertices;
            const std::vector<double>* m_pIndices;
            const std::vector<double>* m_pNormals;
            const std::vector<double>* m_pNormalIndices;
            const std::vector<double>* m_pUVs;
            const std::vector<double>* m_pUVIndices;

            IMeshTemplate();
            ~IMeshTemplate();

            /**
            * Checks if mesh template is valid
            *@return true if mesh template is valid, otherwise false
            */
            bool IsValid() const;
        };

        typedef std::vector<IMeshTemplate*> IMeshTemplates;

        /**
        * Bone dictionary
        */
        typedef std::map<IFBXNode*, Model::IBone*> IBoneDictionary;

        /**
        * Used properties set
        */
        typedef std::set<IFBXProperty*> IUsedProps;

        IFBXNodes        m_Nodes;
        IFBXLinks        m_Links;
        IMeshTemplates   m_Templates;
        IUsedProps       m_UsedProps;
        IItemDictionary  m_ItemDict;
        IBoneDictionary  m_BoneDict;
        Model*           m_pModel;
        std::string      m_Data;
        ITfOnLoadTexture m_fOnLoadTexture;

        /**
        * Clears a dataset
        *@param dataset - dataset to clear
        */
        void ClearDataset(IFBXDataset& dataset) const;

        /**
        * Skips a comment line
        *@param data - FBX data to read from
        *@param[in, out] index - index where comment starts, index after the comment line on function ends
        */
        void SkipComment(const std::string& data, std::size_t& index) const;

        /**
        * Reads a word in the FBX data
        *@param data - data to read in
        *@param length - data length
        *@param[in, out] index - index to start from, index after last read word on function ends
        *@return FBX data containing the word
        */
        IFBXData* ReadWord(const std::string& data, std::size_t length, std::size_t& index) const;

        /**
        * Sets a child node in a parent node
        *@param name - node name
        *@param[in, out] pParent - parent node, new parent node on function ends
        *@param nodes - root node list
        *@param dataset - dataset containing the node data
        *@param itemDict - item dictionary
        */
        void SetNode(const std::string&     name,
                           IFBXNode*&       pParent,
                           IFBXNodes&       nodes,
                           IFBXDataset&     dataset,
                           IItemDictionary& itemDict) const;

        /**
        * Sets a property in a parent node
        *@param name - property name
        *@param pParent - parent node
        *@param dataset - dataset containing the property data
        *@param start - property data start index in the dataset
        *@param end - property data end index in the dataset
        *@return true on success, otherwise false
        */
        bool SetProperty(const std::string& name,
                               IFBXNode*    pParent,
                         const IFBXDataset& dataset,
                               std::size_t  start,
                               std::size_t  end) const;

        /**
        * Performs the model links
        *@return true on success, otherwise false
        */
        bool PerformLinks();

        /**
        * Performs the link between 2 FBX objects
        *@param pParentLink - parent link in which the link will be added
        *@param id - parent link identifier
        //REM *@param isSkeleton - if true, the data contains the skeleton model
        *@param pConnections - connection list containing all the links to perform
        *@return true on success, otherwise false
        */
        bool PerformLink(IFBXLink* pParentLink, std::size_t id, IFBXNode* pConnections);

        /**
        * Gets the link data
        *@param pProp - property containing the link
        *@param[out] type - connection type
        *@param[out] srcID - source identifier (i.e. object id which belongs to destination object)
        *@param[out] dstID - destination identifier (i.e. object which will contain the source object)
        *@param[out] propDesc - property description (optional, empty if destination isn't a property)
        *@return true on success, otherwise false
        */
        bool GetLinkData(IFBXProperty*     pProp,
                         IEConnectionType& type,
                         std::size_t&      srcID,
                         std::size_t&      dstID,
                         std::string&      propDesc) const;

        /**
        * Logs the links hierarchy
        *@param[out] log - log content
        */
        #ifdef _DEBUG
            void LogLinks(std::string& log) const;
        #endif

        /**
        * Logs a link and its children
        *@param pLink - link to log
        *@param tab - tabulation
        *@param[out] log - log content
        */
        #ifdef _DEBUG
            void LogLink(IFBXLink* pLink, unsigned tab, std::string& log) const;
        #endif

        /**
        * Builds the model
        *@return true on success, otherwise false
        */
        // REM NOTE weights point the vertices
        bool BuildModel();

        /**
        * Builds the skeleton
        *@param pLink - link containing the bone data
        *@param pBone - if true, link contains the skeleton root node
        *@param boneDict - bone dictionary
        *@return true on success, otherwise false
        */
        bool BuildSkeleton(IFBXLink* pLink, Model::IBone* pBone, bool isRoot, IBoneDictionary& boneDict) const;

        /**
        * Populates a bone
        *@param pNode - node containing the bone data
        *@param pBone - if true, link contains the skeleton root node
        *@param boneDict - bone dictionary
        *@return true on success, otherwise false
        */
        bool PopulateBone(IFBXNode* pNode, Model::IBone* pBone) const;

        bool PopulateVertexBuffer(const IMeshTemplate* pMeshTemplate, VertexBuffer* pModelVB) const;

        void __TEMP(std::string& log) const; //REM
};
