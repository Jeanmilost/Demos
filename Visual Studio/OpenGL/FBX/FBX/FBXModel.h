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
#include "Vertex.h"
#include "Model.h"

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
                virtual Vector3F Get() const;

            private:
                Vector3F m_Value;
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

            private:
                std::vector<int>    m_IntValues;
                std::vector<double> m_DoubleValues;
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

        //REM virtual IFBXItems Find(const std::string& key) const;

        virtual Model* GetModel() const;

    private:
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
            IE_CT_Unknown = 0,
            IE_CT_ObjObj,      // object to object (OO) connection
            IE_CT_ObjProp,     // object to property (OP) connection
            IE_CT_PropObj,     // property to object (PO) connection
            IE_CT_PropProp     // property to property (PP) connection
        };

        // C: "OO",43642560,43804256
        // OO: Object (source) to Object (destination)
        // the semantics of the relationship are almost always meaningful if you make the following "translations":
        // - Replace "destination" by "container" or "contains"
        // - Replace "source" by "belongs to" or "is a member of" or "member"
        // meaning: Object (source) belongs to Object (destination) which contains it
        // https://download.autodesk.com/us/fbx/20112/fbx_sdk_help/index.html?url=WS73099cc142f487551fea285e1221e4f9ff8-7fda.htm,topicNumber=d0e6388
        struct IFBXConnection
        {
            IEConnectionType m_Type;
            std::size_t      m_SourceID;
            std::size_t      m_DestinationID;

            IFBXConnection();
        };

        struct IGeometryCacheItem
        {
            IFBXNode*   m_pVertices;
            IFBXNode*   m_pIndices;
            std::size_t m_GeometryID;
            std::size_t m_ModelID;
            std::size_t m_MaterialID;
            std::size_t m_DeformerID;

            IGeometryCacheItem();
        };

        typedef std::pair<IFBXNode*, IGeometryCacheItem*> IGeometryCachePair;
        typedef std::vector<IGeometryCachePair>           IGeometryCache;

        IFBXNodes       m_Nodes;
        IGeometryCache  m_GeometryCache;
        IItemDictionary m_ItemDict;
        std::string     m_Data;

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
};

/*REM
{

PasFBXLoader

Copyright (C) 2018 by Benjamin 'BeRo' Rosseaux ( benjamin@rosseaux.de )

License:

Permission is hereby granted, free of charge, to any person (the "Person") obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software, including the rights to use, copy, modify, merge, publish, distribute the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

1. The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

2. Under no circumstances shall the Person be permitted, allowed or authorized to commercially exploit the Software, otherwise the Person must negotiate an individual license with the original author of the original Software, which then permits commercial use.

3. Changes made to the original Software shall be labeled, demarcated or otherwise identified and attributed to the Person. Additionally altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

}
unit PasFBXLoader;
{$ifdef fpc}
 {$mode delphi}
{$else}
 {$warn duplicate_ctor_dtor off}
{$endif}
{$m+}

interface

uses SysUtils,Classes,Math,StrUtils,PasDblStrUtils,Generics.Collections,TypInfo,Variants;

type EFBX=class(Exception);

     TFBXLoader=class;

     PFBXScalar=^TFBXScalar;
     TFBXScalar=Double;

     PFBXSizeInt=^TFBXSizeInt;
     TFBXSizeInt={$ifdef fpc}SizeInt{$else}NativeInt{$endif};

     PFBXPtrInt=^TFBXPtrInt;
     TFBXPtrInt={$ifdef fpc}PtrInt{$else}NativeInt{$endif};

     PFBXPtrUInt=^TFBXPtrUInt;
     TFBXPtrUInt={$ifdef fpc}PtrUInt{$else}NativeUInt{$endif};

     TFBXBytes=array of UInt8;

     TFBXString=RawByteString;

     TFBXBooleanArray=array of Boolean;

     TFBXInt8Array=array of Int8;

     TFBXInt16Array=array of Int16;

     TFBXUInt32Array=array of UInt32;

     TFBXInt32Array=array of Int32;

     TFBXInt64Array=array of Int64;

     TFBXFloat32Array=array of Single;

     TFBXFloat64Array=array of Double;

     TFBXInt64List=class(TList<Int64>);

     TFBXFloat64List=class(TList<Double>);

     PFBXVector2=^TFBXVector2;
     TFBXVector2=record
      public
       constructor Create(const pX:TFBXScalar); overload;
       constructor Create(const pX,pY:TFBXScalar); overload;
       class operator Implicit(const a:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Explicit(const a:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Equal(const a,b:TFBXVector2):boolean; {$ifdef caninline}inline;{$endif}
       class operator NotEqual(const a,b:TFBXVector2):boolean; {$ifdef caninline}inline;{$endif}
       class operator Inc(const a:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Dec(const a:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Add(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Add(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Add(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Subtract(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Subtract(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Subtract(const a:TFBXScalar;const b:TFBXVector2): TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Multiply(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Multiply(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Multiply(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Divide(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Divide(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Divide(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator IntDivide(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator IntDivide(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator IntDivide(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a,b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Negative(const a:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       class operator Positive(const a:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
      private
      private
       function GetComponent(const pIndex:Int32):TFBXScalar; {$ifdef caninline}inline;{$endif}
       procedure SetComponent(const pIndex:Int32;const pValue:TFBXScalar); {$ifdef caninline}inline;{$endif}
      public
       function Perpendicular:TFBXVector2; {$ifdef caninline}inline;{$endif}
       function Length:TFBXScalar; {$ifdef caninline}inline;{$endif}
       function SquaredLength:TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Normalize:TFBXVector2; {$ifdef caninline}inline;{$endif}
       function DistanceTo(const b:TFBXVector2):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Dot(const b:TFBXVector2):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Cross(const b:TFBXVector2):TFBXVector2; {$ifdef caninline}inline;{$endif}
       function Lerp(const b:TFBXVector2;const t:TFBXScalar):TFBXVector2; {$ifdef caninline}inline;{$endif}
       function Angle(const b,c:TFBXVector2):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Rotate(const Angle:TFBXScalar):TFBXVector2; overload; {$ifdef caninline}inline;{$endif}
       function Rotate(const Center:TFBXVector2;const Angle:TFBXScalar):TFBXVector2; overload; {$ifdef caninline}inline;{$endif}
       property Components[const pIndex:Int32]:TFBXScalar read GetComponent write SetComponent; default;
       case UInt8 of
        0:(RawComponents:array[0..1] of TFBXScalar);
        1:(x,y:TFBXScalar);
        2:(u,v:TFBXScalar);
        3:(s,t:TFBXScalar);
        4:(r,g:TFBXScalar);
     end;

     PFBXVector3=^TFBXVector3;
     TFBXVector3=record
      public
       constructor Create(const pX:TFBXScalar); overload;
       constructor Create(const pX,pY,pZ:TFBXScalar); overload;
       constructor Create(const pXY:TFBXVector2;const pZ:TFBXScalar=0.0); overload;
       class operator Implicit(const a:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Explicit(const a:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Equal(const a,b:TFBXVector3):boolean; {$ifdef caninline}inline;{$endif}
       class operator NotEqual(const a,b:TFBXVector3):boolean; {$ifdef caninline}inline;{$endif}
       class operator Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Add(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Subtract(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Subtract(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Multiply(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Divide(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Divide(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator IntDivide(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator IntDivide(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a,b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
       class operator Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Positive(const a:TFBXVector3):TFBXVector3; {$ifdef caninline}inline;{$endif}
      private
      private
       function GetComponent(const pIndex:Int32):TFBXScalar; {$ifdef caninline}inline;{$endif}
       procedure SetComponent(const pIndex:Int32;const pValue:TFBXScalar); {$ifdef caninline}inline;{$endif}
      public
       function Flip:TFBXVector3; {$ifdef caninline}inline;{$endif}
       function Perpendicular:TFBXVector3; {$ifdef caninline}inline;{$endif}
       function OneUnitOrthogonalVector:TFBXVector3;
       function Length:TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function SquaredLength:TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Normalize:TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function DistanceTo({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Abs:TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Dot({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function AngleTo(const b:TFBXVector3):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Cross({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Lerp(const b:TFBXVector3;const t:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       function Angle(const b,c:TFBXVector3):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function RotateX(const Angle:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       function RotateY(const Angle:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       function RotateZ(const Angle:TFBXScalar):TFBXVector3; {$ifdef caninline}inline;{$endif}
       function ProjectToBounds(const MinVector,MaxVector:TFBXVector3):TFBXScalar;
       property Components[const pIndex:Int32]:TFBXScalar read GetComponent write SetComponent; default;
       case UInt8 of
        0:(RawComponents:array[0..2] of TFBXScalar);
        1:(x,y,z:TFBXScalar);
        2:(r,g,b:TFBXScalar);
        3:(s,t,p:TFBXScalar);
        4:(Pitch,Yaw,Roll:TFBXScalar);
        6:(Vector2:TFBXVector2);
     end;

     PFBXVector4=^TFBXVector4;
     TFBXVector4=record
      public
       constructor Create(const pX:TFBXScalar); overload;
       constructor Create(const pX,pY,pZ,pW:TFBXScalar); overload;
       constructor Create(const pXY:TFBXVector2;const pZ:TFBXScalar=0.0;const pW:TFBXScalar=1.0); overload;
       constructor Create(const pXYZ:TFBXVector3;const pW:TFBXScalar=1.0); overload;
       class operator Implicit(const a:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Explicit(const a:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Equal(const a,b:TFBXVector4):boolean; {$ifdef caninline}inline;{$endif}
       class operator NotEqual(const a,b:TFBXVector4):boolean; {$ifdef caninline}inline;{$endif}
       class operator Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Add(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Subtract(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Subtract(const a:TFBXScalar;const b:TFBXVector4): TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Multiply(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Divide(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Divide(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator IntDivide(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator IntDivide(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a,b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Modulus(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
       class operator Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Positive(const a:TFBXVector4):TFBXVector4; {$ifdef caninline}inline;{$endif}
      private
      private
       function GetComponent(const pIndex:Int32):TFBXScalar; {$ifdef caninline}inline;{$endif}
       procedure SetComponent(const pIndex:Int32;const pValue:TFBXScalar); {$ifdef caninline}inline;{$endif}
      public
       function Flip:TFBXVector4; {$ifdef caninline}inline;{$endif}
       function Perpendicular:TFBXVector4; {$ifdef caninline}inline;{$endif}
       function Length:TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function SquaredLength:TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Normalize:TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function DistanceTo({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Abs:TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Dot({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function AngleTo(const b:TFBXVector4):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function Cross({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Lerp(const b:TFBXVector4;const t:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       function Angle(const b,c:TFBXVector4):TFBXScalar; {$ifdef caninline}inline;{$endif}
       function RotateX(const Angle:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       function RotateY(const Angle:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       function RotateZ(const Angle:TFBXScalar):TFBXVector4; {$ifdef caninline}inline;{$endif}
       function Rotate(const Angle:TFBXScalar;const Axis:TFBXVector3):TFBXVector4; {$ifdef caninline}inline;{$endif}
       function ProjectToBounds(const MinVector,MaxVector:TFBXVector4):TFBXScalar;
      public
       property Components[const pIndex:Int32]:TFBXScalar read GetComponent write SetComponent; default;
       case UInt8 of
        0:(RawComponents:array[0..3] of TFBXScalar);
        1:(x,y,z,w:TFBXScalar);
        2:(r,g,b,a:TFBXScalar);
        3:(s,t,p,q:TFBXScalar);
        5:(Vector2:TFBXVector2);
        6:(Vector3:TFBXVector3);
     end;

     PFBXColor=^TFBXColor;
     TFBXColor=record
      private
       function GetComponent(const pIndex:Int32):Double; inline;
       procedure SetComponent(const pIndex:Int32;const pValue:Double); inline;
      public
       constructor Create(const pFrom:TFBXColor); overload;
       constructor Create(const pRed,pGreen,pBlue:Double;const pAlpha:Double=1.0); overload;
       constructor Create(const pArray:array of Double); overload;
       function ToString:TFBXString;
       property Components[const pIndex:Int32]:Double read GetComponent write SetComponent; default;
       case Int32 of
        0:(
         Red:Double;
         Green:Double;
         Blue:Double;
         Alpha:Double;
        );
        1:(
         RawComponents:array[0..3] of Double;
        );
        2:(
         Vector2:TFBXVector2;
        );
        3:(
         Vector3:TFBXVector3;
        );
        4:(
         Vector4:TFBXVector4;
        );
     end;

     PFBXMatrix4x4=^TFBXMatrix4x4;
     TFBXMatrix4x4=record
      public
//     constructor Create; overload;
       constructor Create(const pX:TFBXScalar); overload;
       constructor Create(const pXX,pXY,pXZ,pXW,pYX,pYY,pYZ,pYW,pZX,pZY,pZZ,pZW,pWX,pWY,pWZ,pWW:TFBXScalar); overload;
       constructor Create(const pX,pY,pZ,pW:TFBXVector4); overload;
       constructor CreateRotateX(const Angle:TFBXScalar);
       constructor CreateRotateY(const Angle:TFBXScalar);
       constructor CreateRotateZ(const Angle:TFBXScalar);
       constructor CreateRotate(const Angle:TFBXScalar;const Axis:TFBXVector3);
       constructor CreateRotation(const pMatrix:TFBXMatrix4x4); overload;
       constructor CreateScale(const sx,sy,sz:TFBXScalar); overload;
       constructor CreateScale(const pScale:TFBXVector3); overload;
       constructor CreateScale(const sx,sy,sz,sw:TFBXScalar); overload;
       constructor CreateScale(const pScale:TFBXVector4); overload;
       constructor CreateTranslation(const tx,ty,tz:TFBXScalar); overload;
       constructor CreateTranslation(const pTranslation:TFBXVector3); overload;
       constructor CreateTranslation(const tx,ty,tz,tw:TFBXScalar); overload;
       constructor CreateTranslation(const pTranslation:TFBXVector4); overload;
       constructor CreateTranslated(const pMatrix:TFBXMatrix4x4;pTranslation:TFBXVector3); overload;
       constructor CreateTranslated(const pMatrix:TFBXMatrix4x4;pTranslation:TFBXVector4); overload;
       constructor CreateFromToRotation(const FromDirection,ToDirection:TFBXVector3);
       constructor CreateConstruct(const pForwards,pUp:TFBXVector3);
       constructor CreateOuterProduct(const u,v:TFBXVector3);
       constructor CreateFrustum(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreateOrtho(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreateOrthoLH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreateOrthoRH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreateOrthoOffCenterLH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreateOrthoOffCenterRH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
       constructor CreatePerspective(const fovy,Aspect,zNear,zFar:TFBXScalar);
       constructor CreateLookAt(const Eye,Center,Up:TFBXVector3);
       constructor CreateFill(const Eye,RightVector,UpVector,ForwardVector:TFBXVector3);
       constructor CreateConstructX(const xAxis:TFBXVector3);
       constructor CreateConstructY(const yAxis:TFBXVector3);
       constructor CreateConstructZ(const zAxis:TFBXVector3);
       class operator Implicit({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Explicit({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Equal({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):boolean; {$ifdef caninline}inline;{$endif}
       class operator NotEqual({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):boolean; {$ifdef caninline}inline;{$endif}
       class operator Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Add({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Subtract({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Subtract({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4): TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXVector3; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXVector4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       class operator Divide({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Divide({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       class operator IntDivide({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator IntDivide({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Modulus({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       class operator Modulus({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       class operator Modulus({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       class operator Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       class operator Positive(const a:TFBXMatrix4x4):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
      private
       function GetComponent(const pIndexA,pIndexB:Int32):TFBXScalar; {$ifdef caninline}inline;{$endif}
       procedure SetComponent(const pIndexA,pIndexB:Int32;const pValue:TFBXScalar); {$ifdef caninline}inline;{$endif}
       function GetColumn(const pIndex:Int32):TFBXVector4; {$ifdef caninline}inline;{$endif}
       procedure SetColumn(const pIndex:Int32;const pValue:TFBXVector4); {$ifdef caninline}inline;{$endif}
       function GetRow(const pIndex:Int32):TFBXVector4; {$ifdef caninline}inline;{$endif}
       procedure SetRow(const pIndex:Int32;const pValue:TFBXVector4); {$ifdef caninline}inline;{$endif}
      public
       function Determinant:TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function SimpleInverse:TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function Inverse:TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function Transpose:TFBXMatrix4x4; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
       function EulerAngles:TFBXVector3; {$ifdef caninline}inline;{$endif}
       function Normalize:TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function OrthoNormalize:TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function RobustOrthoNormalize(const Tolerance:TFBXScalar=1e-3):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function ToRotation:TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function SimpleLerp(const b:TFBXMatrix4x4;const t:TFBXScalar):TFBXMatrix4x4; {$ifdef caninline}inline;{$endif}
       function MulInverse({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; overload; {$ifdef caninline}inline;{$endif}
       function MulInverse({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; overload; {$ifdef caninline}inline;{$endif}
       function MulInverted({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; overload; {$ifdef caninline}inline;{$endif}
       function MulInverted({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; overload; {$ifdef caninline}inline;{$endif}
       function MulBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; overload; {$ifdef caninline}inline;{$endif}
       function MulBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; overload; {$ifdef caninline}inline;{$endif}
       function MulTransposedBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; overload; {$ifdef caninline}inline;{$endif}
       function MulTransposedBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; overload; {$ifdef caninline}inline;{$endif}
       function MulHomogen({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3; overload; {$ifdef caninline}inline;{$endif}
       function MulHomogen({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4; overload; {$ifdef caninline}inline;{$endif}
       property Components[const pIndexA,pIndexB:Int32]:TFBXScalar read GetComponent write SetComponent; default;
       property Columns[const pIndex:Int32]:TFBXVector4 read GetColumn write SetColumn;
       property Rows[const pIndex:Int32]:TFBXVector4 read GetRow write SetRow;
       case Int32 of
        0:(RawComponents:array[0..3,0..3] of TFBXScalar);
        1:(LinearRawComponents:array[0..15] of TFBXScalar);
        2:(m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33:TFBXScalar);
        3:(Tangent,Bitangent,Normal,Translation:TFBXVector4);
        4:(Right,Up,Forwards,Offset:TFBXVector4);
     end;

     PFBXTime=^TFBXTime;
     TFBXTime=Int64;

     PFBXTimeSpan=^TFBXTimeSpan;

     TFBXTimeSpan=record
      private
       function GetComponent(const pIndex:Int32):TFBXTime; inline;
       procedure SetComponent(const pIndex:Int32;const pValue:TFBXTime); inline;
      public
       constructor Create(const pFrom:TFBXTimeSpan); overload;
       constructor Create(const pStartTime,pEndTime:TFBXTime); overload;
       constructor Create(const pArray:array of TFBXTime); overload;
       function ToString:TFBXString;
       property Components[const pIndex:Int32]:TFBXTime read GetComponent write SetComponent; default;
       case Int32 of
        0:(
         StartTime:TFBXTime;
         EndTime:TFBXTime;
        );
        1:(
         RawComponents:array[0..1] of TFBXTime;
        );
     end;

     TFBXBaseObject=class(TPersistent)
      private
      public
       constructor Create; reintroduce; virtual;
       destructor Destroy; override;
     end;

     TFBXVector2Property=class(TFBXBaseObject)
      private
       fVector:TFBXVector2;
       function GetX:Double; inline;
       procedure SetX(const pValue:Double); inline;
       function GetY:Double; inline;
       procedure SetY(const pValue:Double); inline;
      public
       constructor Create; reintroduce; overload;
       constructor Create(const pFrom:TFBXVector2); reintroduce; overload;
       constructor Create(const pX,pY:Double); reintroduce; overload;
       constructor Create(const pArray:array of Double); reintroduce; overload;
       destructor Destroy; override;
       property Vector:TFBXVector2 read fVector write fVector;
      published
       property x:Double read GetX write SetX;
       property y:Double read GetY write SetY;
     end;

     TFBXVector3Property=class(TFBXBaseObject)
      private
       fVector:TFBXVector3;
       function GetX:Double; inline;
       procedure SetX(const pValue:Double); inline;
       function GetY:Double; inline;
       procedure SetY(const pValue:Double); inline;
       function GetZ:Double; inline;
       procedure SetZ(const pValue:Double); inline;
      public
       constructor Create; reintroduce; overload;
       constructor Create(const pFrom:TFBXVector3); reintroduce; overload;
       constructor Create(const pX,pY,pZ:Double); reintroduce; overload;
       constructor Create(const pArray:array of Double); reintroduce; overload;
       destructor Destroy; override;
       property Vector:TFBXVector3 read fVector write fVector;
      published
       property x:Double read GetX write SetX;
       property y:Double read GetY write SetY;
       property z:Double read GetZ write SetZ;
     end;

     TFBXVector4Property=class(TFBXBaseObject)
      private
       fVector:TFBXVector4;
       function GetX:Double; inline;
       procedure SetX(const pValue:Double); inline;
       function GetY:Double; inline;
       procedure SetY(const pValue:Double); inline;
       function GetZ:Double; inline;
       procedure SetZ(const pValue:Double); inline;
       function GetW:Double; inline;
       procedure SetW(const pValue:Double); inline;
      public
       constructor Create; reintroduce; overload;
       constructor Create(const pFrom:TFBXVector4); reintroduce; overload;
       constructor Create(const pX,pY,pZ,pW:Double); reintroduce; overload;
       constructor Create(const pArray:array of Double); reintroduce; overload;
       destructor Destroy; override;
       property Vector:TFBXVector4 read fVector write fVector;
      published
       property x:Double read GetX write SetX;
       property y:Double read GetY write SetY;
       property z:Double read GetZ write SetZ;
       property w:Double read GetW write SetW;
     end;

     TFBXColorProperty=class(TFBXBaseObject)
      private
       fColor:TFBXColor;
       function GetRed:Double; inline;
       procedure SetRed(const pValue:Double); inline;
       function GetGreen:Double; inline;
       procedure SetGreen(const pValue:Double); inline;
       function GetBlue:Double; inline;
       procedure SetBlue(const pValue:Double); inline;
       function GetAlpha:Double; inline;
       procedure SetAlpha(const pValue:Double); inline;
      public
       constructor Create; reintroduce; overload;
       constructor Create(const pFrom:TFBXColor); reintroduce; overload;
       constructor Create(const pRed,pGreen,pBlue:Double;const pAlpha:Double=1.0); reintroduce; overload;
       constructor Create(const pArray:array of Double); reintroduce; overload;
       destructor Destroy; override;
       property Color:TFBXColor read fColor write fColor;
      published
       property Red:Double read GetRed write SetRed;
       property Green:Double read GetGreen write SetGreen;
       property Blue:Double read GetBlue write SetBlue;
       property Alpha:Double read GetAlpha write SetAlpha;
       property r:Double read GetRed write SetRed;
       property g:Double read GetGreen write SetGreen;
       property b:Double read GetBlue write SetBlue;
       property a:Double read GetAlpha write SetAlpha;
     end;

     TFBXObject=class;

     TFBXProperty=class(TFBXBaseObject)
      private
       fBaseObject:TObject;
       fBaseName:TFBXString;
       fBasePropInfo:PPropInfo;
       fValue:Variant;
       fConnectedFrom:TFBXObject;
      public
       constructor Create(const pBaseObject:TObject=nil;const pBaseName:TFBXString='';const pBasePropInfo:PPropInfo=nil); reintroduce; overload;
       constructor Create(const pValue:Variant;const pBaseObject:TObject=nil;const pBaseName:TFBXString='';const pBasePropInfo:PPropInfo=nil); reintroduce; overload;
       destructor Destroy; override;
       function GetValue:Variant;
       procedure SetValue(const pValue:Variant);
       property Value:Variant read GetValue write SetValue;
       property ConnectedFrom:TFBXObject read fConnectedFrom write fConnectedFrom;
     end;

     TFBXPropertyList=class(TObjectList<TFBXProperty>);

     TFBXPropertyNameMap=class(TDictionary<TFBXString,TFBXProperty>);

     TFBXElement=class;

     TFBXNode=class;

     TFBXNodeAttribute=class;

     TFBXObjects=array of TFBXObject;

     TFBXObjectList=class(TObjectList<TFBXObject>);

     TFBXNodeAttributeList=class(TObjectList<TFBXNodeAttribute>);

     TFBXObject=class(TFBXBaseObject)
      private
       fLoader:TFBXLoader;
       fElement:TFBXElement;
       fID:Int64;
       fName:TFBXString;
       fType_:TFBXString;
       fProperties:TFBXPropertyList;
       fPropertyNameMap:TFBXPropertyNameMap;
       fConnectedFrom:TFBXObjectList;
       fNodeAttributes:TFBXNodeAttributeList;
       fReference:TFBXString;
      protected
       fConnectedTo:TFBXObjectList;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); reintroduce; virtual;
       destructor Destroy; override;
       procedure AfterConstruction; override;
       procedure BeforeDestruction; override;
       function GetParentNode:TFBXNode;
       function FindConnectionsByType(const pType_:TFBXString):TFBXObjects;
       procedure ConnectTo(const pObject:TFBXObject); virtual;
       procedure ConnectToProperty(const pObject:TFBXObject;const pPropertyName:TFBXString);
       function AddProperty(const pPropertyName:TFBXString):TFBXProperty; overload;
       function AddProperty(const pPropertyName:TFBXString;const pValue:Variant):TFBXProperty; overload;
       procedure SetProperty(const pPropertyName:TFBXString;const pValue:Variant);
       function GetProperty(const pPropertyName:TFBXString):Variant;
      public
       property Loader:TFBXLoader read fLoader;
       property Element:TFBXElement read fElement;
       property ID:Int64 read fID write fID;
       property Name:TFBXString read fName write fName;
       property Type_:TFBXString read fType_ write fType_;
       property Properties:TFBXPropertyList read fProperties;
       property PropertyByName:TFBXPropertyNameMap read fPropertyNameMap;
       property ConnectedFrom:TFBXObjectList read fConnectedFrom;
       property ConnectedTo:TFBXObjectList read fConnectedTo;
       property NodeAttributes:TFBXNodeAttributeList read fNodeAttributes;
       property Reference:TFBXString read fReference write fReference;
      published
     end;

     TFBXNodeList=class(TObjectList<TFBXNode>);

     TFBXNode=class(TFBXObject)
      private
       fParent:TFBXNode;
       fChildren:TFBXNodeList;
       fLclTranslation:TFBXVector3Property;
       fLclRotation:TFBXVector3Property;
       fLclScaling:TFBXVector3Property;
       fVisibility:Boolean;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       procedure ConnectTo(const pObject:TFBXObject); override;
      published
       property Parent:TFBXNode read fParent write fParent;
       property Children:TFBXNodeList read fChildren write fChildren;
       property LclTranslation:TFBXVector3Property read fLclTranslation write fLclTranslation;
       property LclRotation:TFBXVector3Property read fLclRotation write fLclRotation;
       property LclScaling:TFBXVector3Property read fLclScaling write fLclScaling;
       property Visibility:Boolean read fVisibility write fVisibility;
     end;

     TFBXNodeAttribute=class(TFBXObject)
     end;

     TFBXNull=class(TFBXNodeAttribute)
     end;

     TFBXElementProperty=class;

     TFBXElementList=class(TObjectList<TFBXElement>);

     TFBXElementNameMap=class(TDictionary<TFBXString,TFBXElement>);

     TFBXElementPropertyList=class(TObjectList<TFBXElementProperty>);

     TFBXElementPropertyNameMap=class(TDictionary<TFBXString,TFBXElementProperty>);

     TFBXElement=class(TFBXBaseObject)
      private
       fID:TFBXString;
       fChildren:TFBXElementList;
       fChildrenNameMap:TFBXElementNameMap;
       fProperties:TFBXElementPropertyList;
      public
       constructor Create(const pID:TFBXString); reintroduce; virtual;
       destructor Destroy; override;
       function AddChildren(const pElement:TFBXElement):Int32;
       function AddProperty(const pProperty:TFBXElementProperty):Int32;
       property ID:TFBXString read fID;
       property Children:TFBXElementList read fChildren;
       property ChildrenByName:TFBXElementNameMap read fChildrenNameMap;
       property Properties:TFBXElementPropertyList read fProperties;
     end;

     TFBXElementProperty=class(TFBXBaseObject)
      private
      public
       constructor Create; override;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; virtual;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; virtual;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; virtual;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; virtual;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; virtual;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; virtual;
     end;

     TFBXElementPropertyBoolean=class(TFBXElementProperty)
      private
       fValue:Boolean;
      public
       constructor Create(const pValue:Boolean); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
      published
       property Value:Boolean read fValue;
     end;

     TFBXElementPropertyInteger=class(TFBXElementProperty)
      private
       fValue:Int64;
      public
       constructor Create(const pValue:Int64); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
      published
       property Value:Int64 read fValue;
     end;

     TFBXElementPropertyFloat=class(TFBXElementProperty)
      private
       fValue:Double;
      public
       constructor Create(const pValue:Double); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
      published
       property Value:Double read fValue;
     end;

     TFBXElementPropertyBytes=class(TFBXElementProperty)
      private
       fValue:TFBXBytes;
      public
       constructor Create(const pValue:TFBXBytes); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
       property Value:TFBXBytes read fValue;
     end;

     TFBXElementPropertyString=class(TFBXElementProperty)
      private
       fValue:TFBXString;
      public
       constructor Create(const pValue:TFBXString); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
      published
       property Value:TFBXString read fValue;
     end;

     TFBXElementPropertyArrayDataType=
      (
       fepadtBoolean,
       fepadtInt8,
       fepadtInt16,
       fepadtInt32,
       fepadtInt64,
       fepadtFloat32,
       fepadtFloat64
      );

     TFBXElementPropertyArray=class(TFBXElementProperty)
      private
       const DataTypeSizes:array[fepadtBoolean..fepadtFloat64] of TFBXSizeInt=(1,
                                                                               1,
                                                                               2,
                                                                               4,
                                                                               8,
                                                                               4,
                                                                               8);
      private
       fData:TFBXBytes;
       fDataType:TFBXElementPropertyArrayDataType;
       fDataTypeSize:TFBXSizeInt;
       fDataCount:TFBXSizeInt;
      public
       constructor Create(const pData:pointer;const pDataCount:TFBXSizeInt;const pDataType:TFBXElementPropertyArrayDataType); reintroduce;
       constructor CreateFrom(const pStream:TStream;const pDataType:TFBXElementPropertyArrayDataType); reintroduce;
       destructor Destroy; override;
       function GetArrayLength:TFBXSizeInt; override;
       function GetVariantValue(const pIndex:TFBXSizeInt=0):Variant; override;
       function GetString(const pIndex:TFBXSizeInt=0):TFBXString; override;
       function GetBoolean(const pIndex:TFBXSizeInt=0):Boolean; override;
       function GetInteger(const pIndex:TFBXSizeInt=0):Int64; override;
       function GetFloat(const pIndex:TFBXSizeInt=0):Double; override;
     end;

     EFBXParser=class(EFBX);

     TFBXParser=class
      private
       fStream:TStream;
       fVersion:UInt32;
      public
       constructor Create(const pStream:TStream); reintroduce; virtual;
       destructor Destroy; override;
       function SceneName:TFBXString; virtual;
       function GetName(const pRawName:TFBXString):TFBXString; virtual;
       function ConstructName(const pNames:array of TFBXString):TFBXString; virtual;
       function NextElement:TFBXElement; virtual;
       function Parse:TFBXElement; virtual;
      published
       property Version:UInt32 read fVersion;
     end;

     EFBXASCIIParser=class(EFBXParser);

     TFBXASCIIParserTokenKind=
      (
       faptkNone,
       faptkEOF,
       faptkString,
       faptkComma,
       faptkLeftBrace,
       faptkRightBrace,
       faptkColon,
       faptkStar,
       faptkInt64,
       faptkFloat64,
       faptkAlphaNumberic
      );

     PFBXASCIIParserToken=^TFBXASCIIParserToken;
     TFBXASCIIParserToken=record
      StringValue:TFBXString;
      case Kind:TFBXASCIIParserTokenKind of
       faptkInt64:(
        Int64Value:Int64;
       );
       faptkFloat64:(
        Float64Value:Double;
       );
     end;

     TFBXASCIIParser=class(TFBXParser)
      private
       type TFileSignature=array[0..4] of AnsiChar;
       const FILE_SIGNATURE:TFileSignature=';'#32'FBX';
             AlphaNumberic=['a'..'z','A'..'Z','0'..'9','|','_','-','+','.','*'];
      private
       fCurrentToken:TFBXASCIIParserToken;
       function SkipWhiteSpace:boolean;
       procedure NextToken;
      public
       constructor Create(const pStream:TStream); override;
       destructor Destroy; override;
       function SceneName:TFBXString; override;
       function GetName(const pRawName:TFBXString):TFBXString; override;
       function ConstructName(const pNames:array of TFBXString):TFBXString; override;
       function NextElement:TFBXElement; override;
       function Parse:TFBXElement; override;
     end;

     EFBXBinaryParser=class(EFBXParser);

     TFBXBinaryParser=class(TFBXParser)
      private
       type TFileSignature=array[0..20] of AnsiChar;
            TNameSeparator=array[0..1] of AnsiChar;
       const FILE_SIGNATURE:TFileSignature='Kaydara FBX Binary'#$20#$20#$00;
             TYPE_BOOL=67; // 'C'
             TYPE_BYTE=66; // 'B'
             TYPE_INT16=89; // 'Y'
             TYPE_INT32=73; // 'I'
             TYPE_INT64=76; // 'L'
             TYPE_FLOAT32=70; // 'F'
             TYPE_FLOAT64=68; // 'D'
             TYPE_ARRAY_BOOL=99; // 'c'
             TYPE_ARRAY_BYTE=98; // 'b'
             TYPE_ARRAY_INT16=121; // 'y'
             TYPE_ARRAY_INT32=105; // 'i'
             TYPE_ARRAY_INT64=108; // 'l'
             TYPE_ARRAY_FLOAT32=102; // 'f'
             TYPE_ARRAY_FLOAT64=100; // 'd'
             TYPE_BYTES=82; // 'R'
             TYPE_STRING=83; // 'S'
             NAME_SEPARATOR:TNameSeparator=#$00#$01;
      private
       function ReadInt8:Int8;
       function ReadInt16:Int16;
       function ReadInt32:Int32;
       function ReadInt64:Int64;
       function ReadUInt8:UInt8;
       function ReadUInt16:UInt16;
       function ReadUInt32:UInt32;
       function ReadUInt64:UInt64;
       function ReadFloat32:single;
       function ReadFloat64:double;
       function ReadString(const pLength:Int32):TFBXString;
      public
       constructor Create(const pStream:TStream); override;
       destructor Destroy; override;
       function SceneName:TFBXString; override;
       function GetName(const pRawName:TFBXString):TFBXString; override;
       function ConstructName(const pNames:array of TFBXString):TFBXString; override;
       function NextElement:TFBXElement; override;
       function Parse:TFBXElement; override;
     end;

     PFBXTimeMode=^TFBXTimeMode;
     TFBXTimeMode=
      (
       ftmDEFAULT=0,
       ftmFPS_120=1,
       ftmFPS_100=2,
       ftmFPS_60=3,
       ftmFPS_50=4,
       ftmFPS_48=5,
       ftmFPS_30=6,
       ftmFPS_30_DROP=7,
       ftmNTSC_DROP_FRAME=8,
       ftmNTSC_FULL_FRAME=9,
       ftmPAL=10,
       ftmFPS_24=11,
       ftmFPS_1000=12,
       ftmFILM_FULL_FRAME=13,
       ftmCUSTOM=14,
       ftmFPS_96=15,
       ftmFPS_72=16,
       ftmFPS_59_DOT_94=17
      );

     TFBXObjectNameMap=class(TDictionary<TFBXString,TFBXObject>);

     TFBXCamera=class;

     TFBXLight=class;

     TFBXGeometry=class;

     TFBXMesh=class;

     TFBXSkeleton=class;

     TFBXMaterial=class;

     TFBXAnimationStack=class;

     TFBXDeformer=class;

     TFBXTexture=class;

     TFBXPose=class;

     TFBXVideo=class;

     TFBXTake=class;

     TFBXPropertyNameRemap=class(TDictionary<TFBXString,TFBXString>);

     TFBXCameraList=class(TObjectList<TFBXCamera>);

     TFBXLightList=class(TObjectList<TFBXLight>);

     TFBXMeshList=class(TObjectList<TFBXMesh>);

     TFBXSkeletonList=class(TObjectList<TFBXSkeleton>);

     TFBXMaterialList=class(TObjectList<TFBXMaterial>);

     TFBXAnimationStackList=class(TObjectList<TFBXAnimationStack>);

     TFBXDeformerList=class(TObjectList<TFBXDeformer>);

     TFBXTextureList=class(TObjectList<TFBXTexture>);

     TFBXPoseList=class(TObjectList<TFBXPose>);

     TFBXVideoList=class(TObjectList<TFBXVideo>);

     TFBXTakeList=class(TObjectList<TFBXTake>);

     TFBXHeader=class(TFBXObject);

     TFBXSceneInfo=class(TFBXObject);

     TFBXScene=class(TFBXObject)
      private
       fHeader:TFBXHeader;
       fSceneInfo:TFBXSceneInfo;
       fAllObjects:TFBXObjectNameMap;
       fCameras:TFBXCameraList;
       fLights:TFBXLightList;
       fMeshes:TFBXMeshList;
       fSkeletons:TFBXSkeletonList;
       fMaterials:TFBXMaterialList;
       fAnimationStackList:TFBXAnimationStackList;
       fDeformers:TFBXDeformerList;
       fTextures:TFBXTextureList;
       fPoses:TFBXPoseList;
       fVideos:TFBXVideoList;
       fTakes:TFBXTakeList;
       fCurrentTake:TFBXTake;
       fRootNodes:TFBXObjectList;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       property Header:TFBXHeader read fHeader;
       property SceneInfo:TFBXSceneInfo read fSceneInfo;
       property AllObjects:TFBXObjectNameMap read fAllObjects;
       property Cameras:TFBXCameraList read fCameras;
       property Lights:TFBXLightList read fLights;
       property Meshes:TFBXMeshList read fMeshes;
       property Skeletons:TFBXSkeletonList read fSkeletons;
       property Materials:TFBXMaterialList read fMaterials;
       property AnimationStackList:TFBXAnimationStackList read fAnimationStackList;
       property Deformers:TFBXDeformerList read fDeformers;
       property Textures:TFBXTextureList read fTextures;
       property Poses:TFBXPoseList read fPoses;
       property Videos:TFBXVideoList read fVideos;
       property Takes:TFBXTakeList read fTakes;
       property CurrentTake:TFBXTake read fCurrentTake;
       property RootNodes:TFBXObjectList read fRootNodes;
     end;

     TFBXGlobalSettings=class(TFBXObject)
      private
       fUpAxis:Int32;
       fUpAxisSign:Int32;
       fFrontAxis:Int32;
       fFrontAxisSign:Int32;
       fCoordAxis:Int32;
       fCoordAxisSign:Int32;
       fOriginalUpAxis:Int32;
       fOriginalUpAxisSign:Int32;
       fUnitScaleFactor:Double;
       fOriginalUnitScaleFactor:Double;
       fAmbientColor:TFBXColor;
       fDefaultCamera:TFBXString;
       fTimeMode:TFBXTimeMode;
       fTimeProtocol:Int32;
       fSnapOnFrameMode:Int32;
       fTimeSpan:TFBXTimeSpan;
       fCustomFrameRate:Double;
       function GetTimeSpanStart:TFBXTime; inline;
       procedure SetTimeSpanStart(const pValue:TFBXTime); inline;
       function GetTimeSpanStop:TFBXTime; inline;
       procedure SetTimeSpanStop(const pValue:TFBXTime); inline;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       property AmbientColor:TFBXColor read fAmbientColor write fAmbientColor;
       property TimeSpan:TFBXTimeSpan read fTimeSpan write fTimeSpan;
      published
       property UpAxis:Int32 read fUpAxis write fUpAxis;
       property UpAxisSign:Int32 read fUpAxisSign write fUpAxisSign;
       property FrontAxis:Int32 read fFrontAxis write fFrontAxis;
       property FrontAxisSign:Int32 read fFrontAxisSign write fFrontAxisSign;
       property CoordAxis:Int32 read fCoordAxis write fCoordAxis;
       property CoordAxisSign:Int32 read fCoordAxisSign write fCoordAxisSign;
       property OriginalUpAxis:Int32 read fOriginalUpAxis write fOriginalUpAxis;
       property OriginalUpAxisSign:Int32 read fOriginalUpAxisSign write fOriginalUpAxisSign;
       property UnitScaleFactor:Double read fUnitScaleFactor write fUnitScaleFactor;
       property OriginalUnitScaleFactor:Double read fOriginalUnitScaleFactor write fOriginalUnitScaleFactor;
       property DefaultCamera:TFBXString read fDefaultCamera write fDefaultCamera;
       property TimeMode:TFBXTimeMode read fTimeMode write fTimeMode;
       property TimeProtocol:Int32 read fTimeProtocol write fTimeProtocol;
       property SnapOnFrameMode:Int32 read fSnapOnFrameMode write fSnapOnFrameMode;
       property TimeSpanStart:TFBXTime read GetTimeSpanStart write SetTimeSpanStart;
       property TimeSpanStop:TFBXTime read GetTimeSpanStop write SetTimeSpanStop;
       property CustomFrameRate:Double read fCustomFrameRate write fCustomFrameRate;
     end;

     TFBXCamera=class(TFBXObject)
      private
       fPosition:TFBXVector3Property;
       fLookAt:TFBXVector3Property;
       fCameraOrthoZoom:Double;
       fRoll:Double;
       fFieldOfView:Double;
       fFrameColor:TFBXColorProperty;
       fNearPlane:Double;
       fFarPlane:Double;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property Position:TFBXVector3Property read fPosition write fPosition;
       property LookAt:TFBXVector3Property read fLookAt write fLookAt;
       property CameraOrthoZoom:Double read fCameraOrthoZoom write fCameraOrthoZoom;
       property Roll:Double read fRoll write fRoll;
       property FieldOfView:Double read fFieldOfView write fFieldOfView;
       property FrameColor:TFBXColorProperty read fFrameColor write fFrameColor;
       property NearPlane:Double read fNearPlane write fNearPlane;
       property FarPlane:Double read fFarPlane write fFarPlane;
     end;

     TFBXLight=class(TFBXObject)
      public
       const SPOT=0;
             POINT=1;
             DIRECTIONAL=2;
             NO_DECAY=0;
             LINEAR_DECAY=1;
             QUADRATIC_DECAY=2;
             CUBIC_DECAY=3;
      private
       fColor:TFBXColorProperty;
       fIntensity:Double;
       fConeAngle:Double;
       fDecay:Int32;
       fLightType:Int32;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property Color:TFBXColorProperty read fColor write fColor;
       property Intensity:Double read fIntensity write fIntensity;
       property ConeAngle:Double read fConeAngle write fConeAngle;
       property Decay:Int32 read fDecay write fDecay;
       property LightType:Int32 read fLightType write fLightType;
     end;

     TFBXGeometry=class(TFBXNodeAttribute);

     PFBXMappingMode=^TFBXMappingMode;
     TFBXMappingMode=
      (
       fmmNone,
       fmmByVertex,
       fmmByPolygonVertex,
       fmmByPolygon,
       fmmByEdge,
       fmmAllSame
      );

     PFBXReferenceMode=^TFBXReferenceMode;
     TFBXReferenceMode=
      (
       frmDirect,
       frmIndex,
       frmIndexToDirect
      );

     TFBXLayerElement<TDataType>=class(TFBXBaseObject)
      public
       type TFBXLayerElementIntegerList=class(TList<Int64>);
            TFBXLayerElementDataList=class(TList<TDataType>);
      private
       fMappingMode:TFBXMappingMode;
       fReferenceMode:TFBXReferenceMode;
       fIndexArray:TFBXLayerElementIntegerList;
       fByPolygonVertexIndexArray:TFBXLayerElementIntegerList;
       fData:TFBXLayerElementDataList;
       function GetItem(const pIndex:TFBXSizeInt):TDataType; inline;
       procedure SetItem(const pIndex:TFBXSizeInt;const pItem:TDataType); inline;
      public
       constructor Create; override;
       destructor Destroy; override;
       procedure Finish(const pMesh:TFBXMesh);
       property Items[const pIndex:TFBXSizeInt]:TDataType read GetItem write SetItem; default;
      published
       property MappingMode:TFBXMappingMode read fMappingMode;
       property ReferenceMode:TFBXReferenceMode read fReferenceMode;
       property IndexArray:TFBXLayerElementIntegerList read fIndexArray;
       property ByPolygonVertexIndexArray:TFBXLayerElementIntegerList read fByPolygonVertexIndexArray;
       property Data:TFBXLayerElementDataList read fData;
     end;

     TFBXLayerElementVector2=class(TFBXLayerElement<TFBXVector2>);

     TFBXLayerElementVector3=class(TFBXLayerElement<TFBXVector3>);

     TFBXLayerElementVector4=class(TFBXLayerElement<TFBXVector4>);

     TFBXLayerElementColor=class(TFBXLayerElement<TFBXColor>);

     TFBXLayerElementInteger=class(TFBXLayerElement<Int64>);

     TFBXLayer=class(TFBXBaseObject)
      private
       fNormals:TFBXLayerElementVector3;
       fTangents:TFBXLayerElementVector3;
       fBitangents:TFBXLayerElementVector3;
       fUVs:TFBXLayerElementVector2;
       fColors:TFBXLayerElementColor;
       fMaterials:TFBXLayerElementInteger;
      public
       constructor Create; override;
       destructor Destroy; override;
      published
       property Normals:TFBXLayerElementVector3 read fNormals;
       property Tangents:TFBXLayerElementVector3 read fTangents;
       property Bitangents:TFBXLayerElementVector3 read fBitangents;
       property UVs:TFBXLayerElementVector2 read fUVs;
       property Colors:TFBXLayerElementColor read fColors;
       property Materials:TFBXLayerElementInteger read fMaterials write fMaterials;
     end;

     TFBXLayers=class(TObjectList<TFBXLayer>);

     TFBXCluster=class;

     TFBXMeshVertices=class(TList<TFBXVector3>);

     TFBXMeshIndices=class(TList<Int64>);

     TFBXMeshPolygons=class(TObjectList<TFBXMeshIndices>);

     PFBXMeshEdge=^TFBXMeshEdge;
     TFBXMeshEdge=array[0..1] of Int64;

     TFBXMeshEdges=class(TList<TFBXMeshEdge>);

     PFBXMeshClusterMapItem=^TFBXMeshClusterMapItem;
     TFBXMeshClusterMapItem=record
      Cluster:TFBXCluster;
      Weight:Double;
     end;

     TFBXMeshClusterMapItemList=class(TList<TFBXMeshClusterMapItem>);

     TFBXMeshClusterMap=class(TObjectList<TFBXMeshClusterMapItemList>);

     PFBXMeshTriangleVertex=^TFBXMeshTriangleVertex;
     TFBXMeshTriangleVertex=record
      Position:TFBXVector3;
      Normal:TFBXVector3;
      Tangent:TFBXVector3;
      Bitangent:TFBXVector3;
      UV:TFBXVector2;
      Color:TFBXColor;
      Material:Int32;
     end;

     TFBXMeshTriangleVertexList=class(TList<TFBXMeshTriangleVertex>);

     TFBXMesh=class(TFBXGeometry)
      private
       fVertices:TFBXMeshVertices;
       fPolygons:TFBXMeshPolygons;
       fEdges:TFBXMeshEdges;
       fLayers:TFBXLayers;
       fClusterMap:TFBXMeshClusterMap;
       fTriangleVertices:TFBXMeshTriangleVertexList;
       fTriangleIndices:TFBXInt64List;
       fMaterials:TFBXMaterialList;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       procedure ConnectTo(const pObject:TFBXObject); override;
       procedure Finish;
      published
       property Vertices:TFBXMeshVertices read fVertices;
       property Polygons:TFBXMeshPolygons read fPolygons;
       property Edges:TFBXMeshEdges read fEdges;
       property Layers:TFBXLayers read fLayers;
       property ClusterMap:TFBXMeshClusterMap read fClusterMap;
       property TriangleVertices:TFBXMeshTriangleVertexList read fTriangleVertices;
       property TriangleIndices:TFBXInt64List read fTriangleIndices;
       property Materials:TFBXMaterialList read fMaterials;
     end;

     TFBXSkeleton=class(TFBXNode)
      public
       const ROOT=0;
             LIMB=1;
             LIMB_NODE=2;
             EFFECTOR=3;
      private
       fSkeletonType:Int32;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property SkeletonType:Int32 read fSkeletonType;
     end;

     TFBXSkeletonRoot=class(TFBXSkeleton);

     TFBXSkeletonLimb=class(TFBXSkeleton);

     TFBXSkeletonLimbNode=class(TFBXSkeleton);

     TFBXSkeletonEffector=class(TFBXSkeleton);

     TFBXMaterial=class(TFBXObject)
      private
       fShadingModel:TFBXString;
       fMultiLayer:Boolean;
       fAmbientColor:TFBXColorProperty;
       fDiffuseColor:TFBXColorProperty;
       fTransparencyFactor:Double;
       fEmissive:TFBXColorProperty;
       fAmbient:TFBXColorProperty;
       fDiffuse:TFBXColorProperty;
       fOpacity:Double;
       fSpecular:TFBXColorProperty;
       fSpecularFactor:Double;
       fShininess:Double;
       fShininessExponent:Double;
       fReflection:TFBXColorProperty;
       fReflectionFactor:Double;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       procedure ConnectTo(const pObject:TFBXObject); override;
      published
       property ShadingModel:TFBXString read fShadingModel write fShadingModel;
       property MultiLayer:Boolean read fMultiLayer write fMultiLayer;
       property AmbientColor:TFBXColorProperty read fAmbientColor write fAmbientColor;
       property DiffuseColor:TFBXColorProperty read fDiffuseColor write fDiffuseColor;
       property TransparencyFactor:Double read fTransparencyFactor write fTransparencyFactor;
       property Emissive:TFBXColorProperty read fEmissive write fEmissive;
       property Ambient:TFBXColorProperty read fAmbient write fAmbient;
       property Diffuse:TFBXColorProperty read fDiffuse write fDiffuse;
       property Opacity:Double read fOpacity write fOpacity;
       property Specular:TFBXColorProperty read fSpecular write fSpecular;
       property SpecularFactor:Double read fSpecularFactor write fSpecularFactor;
       property Shininess:Double read fShininess write fShininess;
       property ShininessExponent:Double read fShininessExponent write fShininessExponent;
       property Reflection:TFBXColorProperty read fReflection write fReflection;
       property ReflectionFactor:Double read fReflectionFactor write fReflectionFactor;
     end;

     TFBXAnimationStack=class(TFBXObject)
      private
       fDescription:TFBXString;
       fLocalStart:TFBXTime;
       fLocalStop:TFBXTime;
       fReferenceStart:TFBXTime;
       fReferenceStop:TFBXTime;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property Description:TFBXString read fDescription write fDescription;
       property LocalStart:TFBXTime read fLocalStart write fLocalStart;
       property LocalStop:TFBXTime read fLocalStop write fLocalStop;
       property ReferenceStart:TFBXTime read fReferenceStart write fReferenceStart;
       property ReferenceStop:TFBXTime read fReferenceStop write fReferenceStop;
     end;

     TFBXAnimationLayer=class(TFBXObject)
      public
       const BLEND_ADDITIVE=0;
             BLEND_OVERRIDE=1;
             BLEND_OVERRIDE_PASSTHROUGH=2;
             ROTATION_BY_LAYER=0;
             ROTATION_BY_CHANNEL=1;
             SCALE_MULTIPLY=0;
             SCALE_ADDITIVE=1;
      private
       fWeight:Double;
       fMute:Boolean;
       fSolo:Boolean;
       fLock:Boolean;
       fColor:TFBXColorProperty;
       fBlendMode:Int32;
       fRotationAccumulationMode:Int32;
       fScaleAccumulationMode:Int32;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property Weight:Double read fWeight write fWeight;
       property Mute:Boolean read fMute write fMute;
       property Solo:Boolean read fSolo write fSolo;
       property Lock:Boolean read fLock write fLock;
       property Color:TFBXColorProperty read fColor write fColor;
       property BlendMode:Int32 read fBlendMode write fBlendMode;
       property RotationAccumulationMode:Int32 read fRotationAccumulationMode write fRotationAccumulationMode;
       property ScaleAccumulationMode:Int32 read fScaleAccumulationMode write fScaleAccumulationMode;
     end;

     TFBXAnimationCurveNode=class(TFBXObject)
      private
       fX:Double;
       fY:Double;
       fZ:Double;
      published
       property x:Double read fX write fX;
       property y:Double read fY write fY;
       property z:Double read fZ write fZ;
     end;

     TFBXDeformer=class(TFBXObject)
     end;

     TFBXSkinDeformer=class(TFBXDeformer)
      public
       const RIGID=0;
             LINEAR=1;
             DUAL_QUATERNION=2;
             BLEND=3;
      private
       fLink_DeformAcuracy:Int32;
       fSkinningType:Int32;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property Link_DeformAcuracy:Int32 read fLink_DeformAcuracy write fLink_DeformAcuracy;
       property SkinningType:Int32 read fSkinningType write fSkinningType;
       property Clusters:TFBXObjectList read fConnectedTo;
     end;

     TFBXCluster=class(TFBXDeformer)
      public
       const NORMALIZE=0;
             ADDITIVE=1;
             TOTAL_ONE=2;
      private
       fIndexes:TFBXInt64Array;
       fWeights:TFBXFloat64Array;
       fTransform:TFBXMatrix4x4;
       fTransformLink:TFBXMatrix4x4;
       fLinkMode:Int32;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       function GetLink:TFBXNode;
       property Transform:TFBXMatrix4x4 read fTransform write fTransform;
       property TransformLink:TFBXMatrix4x4 read fTransformLink write fTransformLink;
      published
       property Indexes:TFBXInt64Array read fIndexes write fIndexes;
       property Weights:TFBXFloat64Array read fWeights write fWeights;
       property LinkMode:Int32 read fLinkMode write fLinkMode;
     end;

     TFBXTexture=class(TFBXNode)
      private
       fFileName:TFBXString;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property FileName:TFBXString read fFileName write fFileName;
     end;

     TFBXFolder=class(TFBXObject);

     TFBXConstraint=class(TFBXObject);

     PFBXAnimationKeyDataFloats=^TFBXAnimationKeyDataFloats;
     TFBXAnimationKeyDataFloats=array[0..5] of Single;

     TFBXAnimationKey=class
      public
       const DEFAULT_WEIGHT=1.0/3.0;
             MIN_WEIGHT=0.000099999997;
             MAX_WEIGHT=0.99;
             DEFAULT_VELOCITY=0;
             TANGENT_AUTO=$00000100;
             TANGENT_TCB=$00000200;
             TANGENT_USER=$00000400;
             TANGENT_GENERIC_BREAK=$00000800;
             TANGENT_BREAK=TANGENT_GENERIC_BREAK or TANGENT_USER;
             TANGENT_AUTO_BREAK=TANGENT_GENERIC_BREAK or TANGENT_AUTO;
             TANGENT_GENERIC_CLAMP=$00001000;
             TANGENT_GENERIC_TIME_INDEPENDENT=$00002000;
             TANGENT_GENERIC_CLAMP_PROGRESSIVE=$00004000 or TANGENT_GENERIC_TIME_INDEPENDENT;
             TANGENT_MASK=$00007f00;
             INTERPOLATION_CONSTANT=$00000002;
             INTERPOLATION_LINEAR=$00000004;
             INTERPOLATION_CUBIC=$00000008;
             INTERPOLATION_MASK=$0000000e;
             WEIGHTED_NONE=$00000000;
             WEIGHTED_RIGHT=$01000000;
             WEIGHTED_NEXT_LEFT=$02000000;
             WEIGHTED_ALL=WEIGHTED_RIGHT or WEIGHTED_NEXT_LEFT;
             WEIGHT_MASK=$03000000;
             CONSTANT_STANDARD=$00000000;
             CONSTANT_NEXT=$00000100;
             CONSTANT_MASK=$00000100;
             VELOCITY_NONE=$00000000;
             VELOCITY_RIGHT=$10000000;
             VELOCITY_NEXT_LEFT=$20000000;
             VELOCITY_ALL=VELOCITY_RIGHT or VELOCITY_NEXT_LEFT;
             VELOCITY_MASK=$30000000;
             VISIBILITY_NONE=$00000000;
             VISIBILITY_SHOW_LEFT=$00100000;
             VISIBILITY_SHOW_RIGHT=$00200000;
             VISIBILITY_SHOW_BOTH=VISIBILITY_SHOW_LEFT or VISIBILITY_SHOW_RIGHT;
             VISIBILITY_MASK=$00300000;
             RightSlopeIndex=0;
             NextLeftSlopeIndex=1;
             WeightsIndex=2;
             RightWeightIndex=2;
             NextLeftWeightIndex=3;
             VelocityIndex=4;
             RightVelocityIndex=4;
             NextLeftVelocityIndex=5;
             TCBTensionIndex=0;
             TCBContinuityIndex=1;
             TCBBiasIndex=2;
             DefaultDataFloats:TFBXAnimationKeyDataFloats=
              (
               0,                // RightSlope, TCBTension
               0,                // NextLeftSlope, TCBContinuity
               DEFAULT_WEIGHT,   // RightWeight, TCBBias
               DEFAULT_WEIGHT,   // NextLeftWeight
               DEFAULT_VELOCITY, // RightVelocity
               DEFAULT_VELOCITY  // NextLeftVelocity
              );
      private
       fTime:TFBXTime;
       fValue:Double;
       fTangentMode:Int32;
       fInterpolation:Int32;
       fWeight:Int32;
       fConstant:Int32;
       fVelocity:Int32;
       fVisibility:Int32;
       fDataFloats:TFBXAnimationKeyDataFloats;
      public
       constructor Create; reintroduce;
       destructor Destroy; override;
       property DataFloats:TFBXAnimationKeyDataFloats read fDataFloats write fDataFloats;
      published
       property Time:TFBXTime read fTime write fTime;
       property Value:Double read fValue write fValue;
       property TangentMode:Int32 read fTangentMode write fTangentMode;
       property Interpolation:Int32 read fInterpolation write fInterpolation;
       property Weight:Int32 read fWeight write fWeight;
       property Constant:Int32 read fConstant write fConstant;
       property Velocity:Int32 read fVelocity write fVelocity;
       property Visibility:Int32 read fVisibility write fVisibility;
     end;

     TFBXAnimationKeyList=class(TObjectList<TFBXAnimationKey>);

     TFBXAnimationCurve=class(TFBXNode)
      private
       fDefaultValue:Double;
       fAnimationKeys:TFBXAnimationKeyList;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       constructor CreateOldFBX6000(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
       destructor Destroy; override;
      published
       property DefaultValue:Double read fDefaultValue write fDefaultValue;
       property AnimationKeys:TFBXAnimationKeyList read fAnimationKeys;
     end;

     TFBXPoseNodeMatrixMap=class(TDictionary<TFBXNode,TFBXMatrix4x4>);

     TFBXPose=class(TFBXObject)
      private
       fPoseType:TFBXString;
       fNodeMatrixMap:TFBXPoseNodeMatrixMap;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       function GetMatrix(const pNode:TFBXNode):TFBXMatrix4x4;
      published
       property PoseType:TFBXString read fPoseType;
       property NodeMatrixMap:TFBXPoseNodeMatrixMap read fNodeMatrixMap;
     end;

     TFBXVideo=class(TFBXObject)
      private
       fFileName:TFBXString;
       fUseMipMap:Boolean;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
      published
       property FileName:TFBXString read fFileName write fFileName;
       property UseMipMap:Boolean read fUseMipMap write fUseMipMap;
     end;

     TFBXTake=class(TFBXObject)
      private
       fFileName:TFBXString;
       fLocalTimeSpan:TFBXTimeSpan;
       fReferenceTimeSpan:TFBXTimeSpan;
      public
       constructor Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString); override;
       destructor Destroy; override;
       property LocalTimeSpan:TFBXTimeSpan read fLocalTimeSpan write fLocalTimeSpan;
       property ReferenceTimeSpan:TFBXTimeSpan read fReferenceTimeSpan write fReferenceTimeSpan;
      published
       property FileName:TFBXString read fFileName write fFileName;
     end;

     TFBXTimeUtils=class
      public
       const UnitsPerSecond=Int64(46186158000);
             InverseUnitsPerSecond=1.0/Int64(46186158000);
             Zero=Int64(0);
             Infinite=Int64($7fffffffffffffff);
             FramesPerSecondValues:array[ftmDEFAULT..ftmFPS_59_DOT_94] of Double=(
                                                                                  60.0,
                                                                                  120.0,
                                                                                  100.0,
                                                                                  60.0,
                                                                                  50.0,
                                                                                  48.0,
                                                                                  30.0,
                                                                                  29.97,
                                                                                  30.0,
                                                                                  29.97,
                                                                                  50.0,
                                                                                  24.0,
                                                                                  1000.0,
                                                                                  24.0,
                                                                                  1.0,
                                                                                  96.0,
                                                                                  72.0,
                                                                                  59.94
                                                                                 );
      private
       fGlobalSettings:TFBXGlobalSettings;
      public
       constructor Create(const pGlobalSettings:TFBXGlobalSettings); reintroduce;
       destructor Destroy; override;
       function TimeToFrame(const pTime:TFBXTime;const pTimeMode:TFBXTimeMode=ftmDEFAULT):Double;
       function FrameToSeconds(const pFrame:Double;const pTimeMode:TFBXTimeMode=ftmDEFAULT):Double;
       function TimeToSeconds(const pTime:TFBXTime):Double;
     end;

     TFBXAllocatedList=class(TObjectList<TObject>);

     TFBXLoader=class
      private
       fFileVersion:Int32;
       fAllocatedList:TFBXAllocatedList;
       fScene:TFBXScene;
       fGlobalSettings:TFBXGlobalSettings;
       fTimeUtils:TFBXTimeUtils;
       fRootElement:TFBXElement;
      public
       constructor Create; reintroduce;
       destructor Destroy; override;
       procedure LoadFromStream(const pStream:TStream);
       procedure LoadFromFile(const pFileName:UTF8String);
      published
       property FileVersion:Int32 read fFileVersion;
       property Scene:TFBXScene read fScene;
       property GlobalSettings:TFBXGlobalSettings read fGlobalSettings;
       property TimeUtils:TFBXTimeUtils read fTimeUtils;
     end;

var PropertyNameRemap:TFBXPropertyNameRemap=nil;

const FBXMatrix4x4Identity:TFBXMatrix4x4=(RawComponents:((1.0,0.0,0.0,0.0),(0.0,1.0,0.0,0.0),(0.0,0.0,1.0,0.0),(0.0,0.0,0.0,1.0)););

function Modulus(x,y:TFBXScalar):TFBXScalar; {$ifdef caninline}inline;{$endif}

function DoInflate(InData:pointer;InLen:UInt32;var DestData:pointer;var DestLen:UInt32;ParseHeader:boolean):boolean;

function StreamReadInt8(const pStream:TStream):Int8;
function StreamReadInt16(const pStream:TStream):Int16;
function StreamReadInt32(const pStream:TStream):Int32;
function StreamReadInt64(const pStream:TStream):Int64;
function StreamReadUInt8(const pStream:TStream):UInt8;
function StreamReadUInt16(const pStream:TStream):UInt16;
function StreamReadUInt32(const pStream:TStream):UInt32;
function StreamReadUInt64(const pStream:TStream):UInt64;
function StreamReadFloat32(const pStream:TStream):single;
function StreamReadFloat64(const pStream:TStream):double;

implementation

function Modulus(x,y:TFBXScalar):TFBXScalar; {$ifdef caninline}inline;{$endif}
begin
 result:=(abs(x)-(abs(y)*(floor(abs(x)/abs(y)))))*sign(x);
end;

function DoInflate(InData:pointer;InLen:UInt32;var DestData:pointer;var DestLen:UInt32;ParseHeader:boolean):boolean;
const CLCIndex:array[0..18] of UInt8=(16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15);
type pword=^UInt16;
     PTree=^TTree;
     TTree=packed record
      Table:array[0..15] of UInt16;
      Translation:array[0..287] of UInt16;
     end;
     PBuffer=^TBuffer;
     TBuffer=array[0..65535] of UInt8;
     PLengths=^TLengths;
     TLengths=array[0..288+32-1] of UInt8;
     POffsets=^TOffsets;
     TOffsets=array[0..15] of UInt16;
     PBits=^TBits;
     TBits=array[0..29] of UInt8;
     PBase=^TBase;
     TBase=array[0..29] of UInt16;
var Tag,BitCount,DestSize:UInt32;
    SymbolLengthTree,DistanceTree,FixedSymbolLengthTree,FixedDistanceTree:PTree;
    LengthBits,DistanceBits:PBits;
    LengthBase,DistanceBase:PBase;
    Source,SourceEnd:pansichar;
    Dest:pansichar;
 procedure IncSize(length:UInt32);
 var j:UInt32;
 begin
  if (DestLen+length)>=DestSize then begin
   if DestSize=0 then begin
    DestSize:=1;
   end;
   while (DestLen+length)>=DestSize do begin
    inc(DestSize,DestSize);
   end;
   j:=TFBXPtrUInt(Dest)-TFBXPtrUInt(DestData);
   ReAllocMem(DestData,DestSize);
   TFBXPtrUInt(Dest):=TFBXPtrUInt(DestData)+j;
  end;
 end;
 function adler32(data:pointer;length:UInt32):UInt32;
 const BASE=65521;
       NMAX=5552;
 var buf:pansichar;
     s1,s2,k,i:UInt32;
 begin
  s1:=1;
  s2:=0;
  buf:=data;
  while length>0 do begin
   if length<NMAX then begin
    k:=length;
   end else begin
    k:=NMAX;
   end;
   dec(length,k);
   for i:=1 to k do begin
    inc(s1,UInt8(buf^));
    inc(s2,s1);
    inc(buf);
   end;
   s1:=s1 mod BASE;
   s2:=s2 mod BASE;
  end;
  result:=(s2 shl 16) or s1;
 end;
 procedure BuildBitsBase(Bits:pansichar;Base:pword;Delta,First:Int32);
 var i,Sum:Int32;
 begin
  for i:=0 to Delta-1 do begin
   Bits[i]:=ansichar(#0);
  end;
  for i:=0 to (30-Delta)-1 do begin
   Bits[i+Delta]:=ansichar(UInt8(i div Delta));
  end;
  Sum:=First;
  for i:=0 to 29 do begin
   Base^:=Sum;
   inc(Base);
   inc(Sum,1 shl UInt8(Bits[i]));
  end;
 end;
 procedure BuildFixedTrees(var lt,dt:TTree);
 var i:Int32;
 begin
  for i:=0 to 6 do begin
   lt.Table[i]:=0;
  end;
  lt.Table[7]:=24;
  lt.Table[8]:=152;
  lt.Table[9]:=112;
  for i:=0 to 23 do begin
   lt.Translation[i]:=256+i;
  end;
  for i:=0 to 143 do begin
   lt.Translation[24+i]:=i;
  end;
  for i:=0 to 7 do begin
   lt.Translation[168+i]:=280+i;
  end;
  for i:=0 to 111 do begin
   lt.Translation[176+i]:=144+i;
  end;
  for i:=0 to 4 do begin
   dt.Table[i]:=0;
  end;
  dt.Table[5]:=32;
  for i:=0 to 31 do begin
   dt.Translation[i]:=i;
  end;
 end;
 procedure BuildTree(var t:TTree;Lengths:pansichar;Num:Int32);
 var Offsets:POffsets;
     i:Int32;
     Sum:UInt32;
 begin
  New(Offsets);
  try
   for i:=0 to 15 do begin
    t.Table[i]:=0;
   end;
   for i:=0 to Num-1 do begin
    inc(t.Table[UInt8(Lengths[i])]);
   end;
   t.Table[0]:=0;
   Sum:=0;
   for i:=0 to 15 do begin
    Offsets^[i]:=Sum;
    inc(Sum,t.Table[i]);
   end;
   for i:=0 to Num-1 do begin
    if lengths[i]<>ansichar(#0) then begin
     t.Translation[Offsets^[UInt8(lengths[i])]]:=i;
     inc(Offsets^[UInt8(lengths[i])]);
    end;
   end;
  finally
   Dispose(Offsets);
  end;
 end;
 function GetBit:UInt32;
 begin
  if BitCount=0 then begin
   Tag:=UInt8(Source^);
   inc(Source);
   BitCount:=7;
  end else begin
   dec(BitCount);
  end;
  result:=Tag and 1;
  Tag:=Tag shr 1;
 end;
 function ReadBits(Num,Base:UInt32):UInt32;
 var Limit,Mask:UInt32;
 begin
  result:=0;
  if Num<>0 then begin
   Limit:=1 shl Num;
   Mask:=1;
   while Mask<Limit do begin
    if GetBit<>0 then begin
     inc(result,Mask);
    end;
    Mask:=Mask shl 1;
   end;
  end;
  inc(result,Base);
 end;
 function DecodeSymbol(var t:TTree):UInt32;
 var Sum,c,l:Int32;
 begin
  Sum:=0;
  c:=0;
  l:=0;
  repeat
   c:=(c*2)+Int32(GetBit);
   inc(l);
   inc(Sum,t.Table[l]);
   dec(c,t.Table[l]);
  until not (c>=0);
  result:=t.Translation[Sum+c];
 end;
 procedure DecodeTrees(var lt,dt:TTree);
 var CodeTree:PTree;
     Lengths:PLengths;
     hlit,hdist,hclen,i,num,length,clen,Symbol,Prev:UInt32;
 begin
  New(CodeTree);
  New(Lengths);
  try
   FillChar(CodeTree^,sizeof(TTree),ansichar(#0));
   FillChar(Lengths^,sizeof(TLengths),ansichar(#0));
   hlit:=ReadBits(5,257);
   hdist:=ReadBits(5,1);
   hclen:=ReadBits(4,4);
   for i:=0 to 18 do begin
    lengths^[i]:=0;
   end;
   for i:=1 to hclen do begin
    clen:=ReadBits(3,0);
    lengths^[CLCIndex[i-1]]:=clen;
   end;
   BuildTree(CodeTree^,pansichar(pointer(@lengths^[0])),19);
   num:=0;
   while num<(hlit+hdist) do begin
    Symbol:=DecodeSymbol(CodeTree^);
    case Symbol of
     16:begin
      prev:=lengths^[num-1];
      length:=ReadBits(2,3);
      while length>0 do begin
       lengths^[num]:=prev;
       inc(num);
       dec(length);
      end;
     end;
     17:begin
      length:=ReadBits(3,3);
      while length>0 do begin
       lengths^[num]:=0;
       inc(num);
       dec(length);
      end;
     end;
     18:begin
      length:=ReadBits(7,11);
      while length>0 do begin
       lengths^[num]:=0;
       inc(num);
       dec(length);
      end;
     end;
     else begin
      lengths^[num]:=Symbol;
      inc(num);
     end;
    end;
   end;
   BuildTree(lt,pansichar(pointer(@lengths^[0])),hlit);
   BuildTree(dt,pansichar(pointer(@lengths^[hlit])),hdist);
  finally
   Dispose(CodeTree);
   Dispose(Lengths);
  end;
 end;
 function InflateBlockData(var lt,dt:TTree):boolean;
 var Symbol:UInt32;
     Length,Distance,Offset,i:Int32;
 begin
  result:=false;
  while (Source<SourceEnd) or (BitCount>0) do begin
   Symbol:=DecodeSymbol(lt);
   if Symbol=256 then begin
    result:=true;
    break;
   end;
   if Symbol<256 then begin
    IncSize(1);
    Dest^:=ansichar(UInt8(Symbol));
    inc(Dest);
    inc(DestLen);
   end else begin
    dec(Symbol,257);
    Length:=ReadBits(LengthBits^[Symbol],LengthBase^[Symbol]);
    Distance:=DecodeSymbol(dt);
    Offset:=ReadBits(DistanceBits^[Distance],DistanceBase^[Distance]);
    IncSize(length);
    for i:=0 to length-1 do begin
     Dest[i]:=Dest[i-Offset];
    end;
    inc(Dest,Length);
    inc(DestLen,Length);
   end;
  end;
 end;
 function InflateUncompressedBlock:boolean;
 var length,invlength:UInt32;
 begin
  result:=false;
  length:=(UInt8(source[1]) shl 8) or UInt8(source[0]);
  invlength:=(UInt8(source[3]) shl 8) or UInt8(source[2]);
  if length<>((not invlength) and $ffff) then begin
   exit;
  end;
  IncSize(length);
  inc(Source,4);
  if Length>0 then begin
   Move(Source^,Dest^,Length);
   inc(Source,Length);
   inc(Dest,Length);
  end;
  BitCount:=0;
  inc(DestLen,Length);
  result:=true;
 end;
 function InflateFixedBlock:boolean;
 begin
  result:=InflateBlockData(FixedSymbolLengthTree^,FixedDistanceTree^);
 end;
 function InflateDynamicBlock:boolean;
 begin
  DecodeTrees(SymbolLengthTree^,DistanceTree^);
  result:=InflateBlockData(SymbolLengthTree^,DistanceTree^);
 end;
 function Uncompress:boolean;
 var Final,r:boolean;
     BlockType:UInt32;
 begin
  result:=false;
  BitCount:=0;
  Final:=false;
  while not Final do begin
   Final:=GetBit<>0;
   BlockType:=ReadBits(2,0);
   case BlockType of
    0:begin
     r:=InflateUncompressedBlock;
    end;
    1:begin
     r:=InflateFixedBlock;
    end;
    2:begin
     r:=InflateDynamicBlock;
    end;
    else begin
     r:=false;
    end;
   end;
   if not r then begin
    exit;
   end;
  end;
  result:=true;
 end;
 function UncompressZLIB:boolean;
 var cmf,flg:UInt8;
     a32:UInt32;
 begin
  result:=false;
  Source:=InData;
  cmf:=UInt8(Source[0]);
  flg:=UInt8(Source[1]);
  if ((((cmf shl 8)+flg) mod 31)<>0) or ((cmf and $f)<>8) or ((cmf shr 4)>7) or ((flg and $20)<>0) then begin
   exit;
  end;
  a32:=(UInt8(Source[InLen-4]) shl 24) or (UInt8(Source[InLen-3]) shl 16) or (UInt8(Source[InLen-2]) shl 8) or (UInt8(Source[InLen-1]) shl 0);
  inc(Source,2);
  dec(InLen,6);
  SourceEnd:=@Source[InLen];
  result:=Uncompress;
  if not result then begin
   exit;
  end;
  result:=adler32(DestData,DestLen)=a32;
 end;
 function UncompressDirect:boolean;
 begin
  Source:=InData;
  SourceEnd:=@Source[InLen];
  result:=Uncompress;
 end;
begin
 DestData:=nil;
 LengthBits:=nil;
 DistanceBits:=nil;
 LengthBase:=nil;
 DistanceBase:=nil;
 SymbolLengthTree:=nil;
 DistanceTree:=nil;
 FixedSymbolLengthTree:=nil;
 FixedDistanceTree:=nil;
 try
  New(LengthBits);
  New(DistanceBits);
  New(LengthBase);
  New(DistanceBase);
  New(SymbolLengthTree);
  New(DistanceTree);
  New(FixedSymbolLengthTree);
  New(FixedDistanceTree);
  try
   begin
    FillChar(LengthBits^,sizeof(TBits),ansichar(#0));
    FillChar(DistanceBits^,sizeof(TBits),ansichar(#0));
    FillChar(LengthBase^,sizeof(TBase),ansichar(#0));
    FillChar(DistanceBase^,sizeof(TBase),ansichar(#0));
    FillChar(SymbolLengthTree^,sizeof(TTree),ansichar(#0));
    FillChar(DistanceTree^,sizeof(TTree),ansichar(#0));
    FillChar(FixedSymbolLengthTree^,sizeof(TTree),ansichar(#0));
    FillChar(FixedDistanceTree^,sizeof(TTree),ansichar(#0));
   end;
   begin
    BuildFixedTrees(FixedSymbolLengthTree^,FixedDistanceTree^);
    BuildBitsBase(pansichar(pointer(@LengthBits^[0])),pword(pointer(@LengthBase^[0])),4,3);
    BuildBitsBase(pansichar(pointer(@DistanceBits^[0])),pword(pointer(@DistanceBase^[0])),2,1);
    LengthBits^[28]:=0;
    LengthBase^[28]:=258;
   end;
   begin
    GetMem(DestData,4096);
    DestSize:=4096;
    Dest:=DestData;
    DestLen:=0;
    if ParseHeader then begin
     result:=UncompressZLIB;
    end else begin
     result:=UncompressDirect;
    end;
    if result then begin
     ReAllocMem(DestData,DestLen);
    end else if assigned(DestData) then begin
     FreeMem(DestData);
     DestData:=nil;
    end;
   end;
  finally
   if assigned(LengthBits) then begin
    Dispose(LengthBits);
   end;
   if assigned(DistanceBits) then begin
    Dispose(DistanceBits);
   end;
   if assigned(LengthBase) then begin
    Dispose(LengthBase);
   end;
   if assigned(DistanceBase) then begin
    Dispose(DistanceBase);
   end;
   if assigned(SymbolLengthTree) then begin
    Dispose(SymbolLengthTree);
   end;
   if assigned(DistanceTree) then begin
    Dispose(DistanceTree);
   end;
   if assigned(FixedSymbolLengthTree) then begin
    Dispose(FixedSymbolLengthTree);
   end;
   if assigned(FixedDistanceTree) then begin
    Dispose(FixedDistanceTree);
   end;
  end;
 except
  result:=false;
 end;
end;

constructor TFBXVector2.Create(const pX:TFBXScalar);
begin
 x:=pX;
 y:=pX;
end;

constructor TFBXVector2.Create(const pX,pY:TFBXScalar);
begin
 x:=pX;
 y:=pY;
end;

class operator TFBXVector2.Implicit(const a:TFBXScalar):TFBXVector2;
begin
 result.x:=a;
 result.y:=a;
end;

class operator TFBXVector2.Explicit(const a:TFBXScalar):TFBXVector2;
begin
 result.x:=a;
 result.y:=a;
end;

class operator TFBXVector2.Equal(const a,b:TFBXVector2):boolean;
begin
 result:=SameValue(a.x,b.x) and SameValue(a.y,b.y);
end;

class operator TFBXVector2.NotEqual(const a,b:TFBXVector2):boolean;
begin
 result:=(not SameValue(a.x,b.x)) or (not SameValue(a.y,b.y));
end;

class operator TFBXVector2.Inc(const a:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x+1.0;
 result.y:=a.y+1.0;
end;

class operator TFBXVector2.Dec(const a:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x-1.0;
 result.y:=a.y-1.0;
end;

class operator TFBXVector2.Add(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x+b.x;
 result.y:=a.y+b.y;
end;

class operator TFBXVector2.Add(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=a.x+b;
 result.y:=a.y+b;
end;

class operator TFBXVector2.Add(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2;
begin
 result.x:=a+b.x;
 result.y:=a+b.y;
end;

class operator TFBXVector2.Subtract(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x-b.x;
 result.y:=a.y-b.y;
end;

class operator TFBXVector2.Subtract(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=a.x-b;
 result.y:=a.y-b;
end;

class operator TFBXVector2.Subtract(const a:TFBXScalar;const b:TFBXVector2): TFBXVector2;
begin
 result.x:=a-b.x;
 result.y:=a-b.y;
end;

class operator TFBXVector2.Multiply(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x*b.x;
 result.y:=a.y*b.y;
end;

class operator TFBXVector2.Multiply(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=a.x*b;
 result.y:=a.y*b;
end;

class operator TFBXVector2.Multiply(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2;
begin
 result.x:=a*b.x;
 result.y:=a*b.y;
end;

class operator TFBXVector2.Divide(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
end;

class operator TFBXVector2.Divide(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
end;

class operator TFBXVector2.Divide(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
end;

class operator TFBXVector2.IntDivide(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
end;

class operator TFBXVector2.IntDivide(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
end;

class operator TFBXVector2.IntDivide(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
end;

class operator TFBXVector2.Modulus(const a,b:TFBXVector2):TFBXVector2;
begin
 result.x:=Modulus(a.x,b.x);
 result.y:=Modulus(a.y,b.y);
end;

class operator TFBXVector2.Modulus(const a:TFBXVector2;const b:TFBXScalar):TFBXVector2;
begin
 result.x:=Modulus(a.x,b);
 result.y:=Modulus(a.y,b);
end;

class operator TFBXVector2.Modulus(const a:TFBXScalar;const b:TFBXVector2):TFBXVector2;
begin
 result.x:=Modulus(a,b.x);
 result.y:=Modulus(a,b.y);
end;

class operator TFBXVector2.Negative(const a:TFBXVector2):TFBXVector2;
begin
 result.x:=-a.x;
 result.y:=-a.y;
end;

class operator TFBXVector2.Positive(const a:TFBXVector2):TFBXVector2;
begin
 result:=a;
end;

function TFBXVector2.GetComponent(const pIndex:Int32):TFBXScalar;
begin
 result:=RawComponents[pIndex];
end;

procedure TFBXVector2.SetComponent(const pIndex:Int32;const pValue:TFBXScalar);
begin
 RawComponents[pIndex]:=pValue;
end;

function TFBXVector2.Perpendicular:TFBXVector2;
begin
 result.x:=-y;
 result.y:=x;
end;

function TFBXVector2.Length:TFBXScalar;
begin
 result:=sqrt(sqr(x)+sqr(y));
end;

function TFBXVector2.SquaredLength:TFBXScalar;
begin
 result:=sqr(x)+sqr(y);
end;

function TFBXVector2.Normalize:TFBXVector2;
var Factor:TFBXScalar;
begin
 Factor:=sqrt(sqr(x)+sqr(y));
 if Factor<>0.0 then begin
  Factor:=1.0/Factor;
  result.x:=x*Factor;
  result.y:=y*Factor;
 end else begin
  result.x:=0.0;
  result.y:=0.0;
 end;
end;

function TFBXVector2.DistanceTo(const b:TFBXVector2):TFBXScalar;
begin
 result:=sqrt(sqr(x-b.x)+sqr(y-b.y));
end;

function TFBXVector2.Dot(const b:TFBXVector2):TFBXScalar;
begin
 result:=(x*b.x)+(y*b.y);
end;

function TFBXVector2.Cross(const b:TFBXVector2):TFBXVector2;
begin
 result.x:=(y*b.x)-(x*b.y);
 result.y:=(x*b.y)-(y*b.x);
end;

function TFBXVector2.Lerp(const b:TFBXVector2;const t:TFBXScalar):TFBXVector2;
var InvT:TFBXScalar;
begin
 if t<=0.0 then begin
  result:=self;
 end else if t>=1.0 then begin
  result:=b;
 end else begin
  InvT:=1.0-t;
  result.x:=(x*InvT)+(b.x*t);
  result.y:=(y*InvT)+(b.y*t);
 end;
end;

function TFBXVector2.Angle(const b,c:TFBXVector2):TFBXScalar;
var DeltaAB,DeltaCB:TFBXVector2;
    LengthAB,LengthCB:TFBXScalar;
begin
 DeltaAB:=self-b;
 DeltaCB:=c-b;
 LengthAB:=DeltaAB.Length;
 LengthCB:=DeltaCB.Length;
 if (LengthAB=0.0) or (LengthCB=0.0) then begin
  result:=0.0;
 end else begin
  result:=ArcCos(DeltaAB.Dot(DeltaCB)/(LengthAB*LengthCB));
 end;
end;

function TFBXVector2.Rotate(const Angle:TFBXScalar):TFBXVector2;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(x*Cosinus)-(y*Sinus);
 result.y:=(y*Cosinus)+(x*Sinus);
end;

function TFBXVector2.Rotate(const Center:TFBXVector2;const Angle:TFBXScalar):TFBXVector2;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(((x-Center.x)*Cosinus)-((y-Center.y)*Sinus))+Center.x;
 result.y:=(((y-Center.y)*Cosinus)+((x-Center.x)*Sinus))+Center.y;
end;

constructor TFBXVector3.Create(const pX:TFBXScalar);
begin
 x:=pX;
 y:=pX;
 z:=pX;
end;

constructor TFBXVector3.Create(const pX,pY,pZ:TFBXScalar);
begin
 x:=pX;
 y:=pY;
 z:=pZ;
end;

constructor TFBXVector3.Create(const pXY:TFBXVector2;const pZ:TFBXScalar=0.0);
begin
 x:=pXY.x;
 y:=pXY.y;
 z:=pZ;
end;

class operator TFBXVector3.Implicit(const a:TFBXScalar):TFBXVector3;
begin
 result.x:=a;
 result.y:=a;
 result.z:=a;
end;

class operator TFBXVector3.Explicit(const a:TFBXScalar):TFBXVector3;
begin
 result.x:=a;
 result.y:=a;
 result.z:=a;
end;

class operator TFBXVector3.Equal(const a,b:TFBXVector3):boolean;
begin
 result:=SameValue(a.x,b.x) and SameValue(a.y,b.y) and SameValue(a.z,b.z);
end;

class operator TFBXVector3.NotEqual(const a,b:TFBXVector3):boolean;
begin
 result:=(not SameValue(a.x,b.x)) or (not SameValue(a.y,b.y)) or (not SameValue(a.z,b.z));
end;

class operator TFBXVector3.Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x+1.0;
 result.y:=a.y+1.0;
 result.z:=a.z+1.0;
end;

class operator TFBXVector3.Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x-1.0;
 result.y:=a.y-1.0;
 result.z:=a.z-1.0;
end;

class operator TFBXVector3.Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x+b.x;
 result.y:=a.y+b.y;
 result.z:=a.z+b.z;
end;

class operator TFBXVector3.Add(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=a.x+b;
 result.y:=a.y+b;
 result.z:=a.z+b;
end;

class operator TFBXVector3.Add(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3;
begin
 result.x:=a+b.x;
 result.y:=a+b.y;
 result.z:=a+b.z;
end;

class operator TFBXVector3.Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x-b.x;
 result.y:=a.y-b.y;
 result.z:=a.z-b.z;
end;

class operator TFBXVector3.Subtract(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=a.x-b;
 result.y:=a.y-b;
 result.z:=a.z-b;
end;

class operator TFBXVector3.Subtract(const a:TFBXScalar;const b:TFBXVector3): TFBXVector3;
begin
 result.x:=a-b.x;
 result.y:=a-b.y;
 result.z:=a-b.z;
end;

class operator TFBXVector3.Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x*b.x;
 result.y:=a.y*b.y;
 result.z:=a.z*b.z;
end;

class operator TFBXVector3.Multiply(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=a.x*b;
 result.y:=a.y*b;
 result.z:=a.z*b;
end;

class operator TFBXVector3.Multiply(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3;
begin
 result.x:=a*b.x;
 result.y:=a*b.y;
 result.z:=a*b.z;
end;

class operator TFBXVector3.Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
 result.z:=a.z/b.z;
end;

class operator TFBXVector3.Divide(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
 result.z:=a.z/b;
end;

class operator TFBXVector3.Divide(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
 result.z:=a/b.z;
end;

class operator TFBXVector3.IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
 result.z:=a.z/b.z;
end;

class operator TFBXVector3.IntDivide(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
 result.z:=a.z/b;
end;

class operator TFBXVector3.IntDivide(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
 result.z:=a/b.z;
end;

class operator TFBXVector3.Modulus(const a,b:TFBXVector3):TFBXVector3;
begin
 result.x:=Modulus(a.x,b.x);
 result.y:=Modulus(a.y,b.y);
 result.z:=Modulus(a.z,b.z);
end;

class operator TFBXVector3.Modulus(const a:TFBXVector3;const b:TFBXScalar):TFBXVector3;
begin
 result.x:=Modulus(a.x,b);
 result.y:=Modulus(a.y,b);
 result.z:=Modulus(a.z,b);
end;

class operator TFBXVector3.Modulus(const a:TFBXScalar;const b:TFBXVector3):TFBXVector3;
begin
 result.x:=Modulus(a,b.x);
 result.y:=Modulus(a,b.y);
 result.z:=Modulus(a,b.z);
end;

class operator TFBXVector3.Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
begin
 result.x:=-a.x;
 result.y:=-a.y;
 result.z:=-a.z;
end;

class operator TFBXVector3.Positive(const a:TFBXVector3):TFBXVector3;
begin
 result:=a;
end;

function TFBXVector3.GetComponent(const pIndex:Int32):TFBXScalar;
begin
 result:=RawComponents[pIndex];
end;

procedure TFBXVector3.SetComponent(const pIndex:Int32;const pValue:TFBXScalar);
begin
 RawComponents[pIndex]:=pValue;
end;

function TFBXVector3.Flip:TFBXVector3;
begin
 result.x:=x;
 result.y:=z;
 result.z:=-y;
end;

function TFBXVector3.Perpendicular:TFBXVector3;
var v,p:TFBXVector3;
begin
 v:=p.Normalize;
 p.x:=System.abs(v.x);
 p.y:=System.abs(v.y);
 p.z:=System.abs(v.z);
 if (p.x<=p.y) and (p.x<=p.z) then begin
  p.x:=1.0;
  p.y:=0.0;
  p.z:=0.0;
 end else if (p.y<=p.x) and (p.y<=p.z) then begin
  p.x:=0.0;
  p.y:=1.0;
  p.z:=0.0;
 end else begin
  p.x:=0.0;
  p.y:=0.0;
  p.z:=1.0;
 end;
 result:=p-(v*v.Dot(p));
end;

function TFBXVector3.OneUnitOrthogonalVector:TFBXVector3;
var MinimumAxis:Int32;
    l:TFBXScalar;
begin
 if System.abs(x)<System.abs(y) then begin
  if System.abs(x)<System.abs(z) then begin
   MinimumAxis:=0;
  end else begin
   MinimumAxis:=2;
  end;
 end else begin
  if System.abs(y)<System.abs(z) then begin
   MinimumAxis:=1;
  end else begin
   MinimumAxis:=2;
  end;
 end;
 case MinimumAxis of
  0:begin
   l:=sqrt(sqr(y)+sqr(z));
   result.x:=0.0;
   result.y:=-(z/l);
   result.z:=y/l;
  end;
  1:begin
   l:=sqrt(sqr(x)+sqr(z));
   result.x:=-(z/l);
   result.y:=0.0;
   result.z:=x/l;
  end;
  else begin
   l:=sqrt(sqr(x)+sqr(y));
   result.x:=-(y/l);
   result.y:=x/l;
   result.z:=0.0;
  end;
 end;
end;

function TFBXVector3.Length:TFBXScalar;
begin
 result:=sqrt(sqr(x)+sqr(y)+sqr(z));
end;

function TFBXVector3.SquaredLength:TFBXScalar;
begin
 result:=sqr(x)+sqr(y)+sqr(z);
end;

function TFBXVector3.Normalize:TFBXVector3;
var Factor:TFBXScalar;
begin
 Factor:=sqrt(sqr(x)+sqr(y)+sqr(z));
 if Factor<>0.0 then begin
  Factor:=1.0/Factor;
  result.x:=x*Factor;
  result.y:=y*Factor;
  result.z:=z*Factor;
 end else begin
  result.x:=0.0;
  result.y:=0.0;
  result.z:=0.0;
 end;
end;

function TFBXVector3.DistanceTo({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXScalar;
begin
 result:=sqrt(sqr(x-b.x)+sqr(y-b.y)+sqr(z-b.z));
end;

function TFBXVector3.Abs:TFBXVector3;
begin
 result.x:=System.Abs(x);
 result.y:=System.Abs(y);
 result.z:=System.Abs(z);
end;

function TFBXVector3.Dot({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXScalar;
begin
 result:=(x*b.x)+(y*b.y)+(z*b.z);
end;

function TFBXVector3.AngleTo(const b:TFBXVector3):TFBXScalar;
var d:single;
begin
 d:=sqrt(SquaredLength*b.SquaredLength);
 if d<>0.0 then begin
  result:=Dot(b)/d;
 end else begin
  result:=0.0;
 end
end;

function TFBXVector3.Cross({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXVector3;
begin
 result.x:=(y*b.z)-(z*b.y);
 result.y:=(z*b.x)-(x*b.z);
 result.z:=(x*b.y)-(y*b.x);
end;

function TFBXVector3.Lerp(const b:TFBXVector3;const t:TFBXScalar):TFBXVector3;
var InvT:TFBXScalar;
begin
 if t<=0.0 then begin
  result:=self;
 end else if t>=1.0 then begin
  result:=b;
 end else begin
  InvT:=1.0-t;
  result:=(self*InvT)+(b*t);
 end;
end;

function TFBXVector3.Angle(const b,c:TFBXVector3):TFBXScalar;
var DeltaAB,DeltaCB:TFBXVector3;
    LengthAB,LengthCB:TFBXScalar;
begin
 DeltaAB:=self-b;
 DeltaCB:=c-b;
 LengthAB:=DeltaAB.Length;
 LengthCB:=DeltaCB.Length;
 if (LengthAB=0.0) or (LengthCB=0.0) then begin
  result:=0.0;
 end else begin
  result:=ArcCos(DeltaAB.Dot(DeltaCB)/(LengthAB*LengthCB));
 end;
end;

function TFBXVector3.RotateX(const Angle:TFBXScalar):TFBXVector3;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=x;
 result.y:=(y*Cosinus)-(z*Sinus);
 result.z:=(y*Sinus)+(z*Cosinus);
end;

function TFBXVector3.RotateY(const Angle:TFBXScalar):TFBXVector3;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(z*Sinus)+(x*Cosinus);
 result.y:=y;
 result.z:=(z*Cosinus)-(x*Sinus);
end;

function TFBXVector3.RotateZ(const Angle:TFBXScalar):TFBXVector3;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(x*Cosinus)-(y*Sinus);
 result.y:=(x*Sinus)+(y*Cosinus);
 result.z:=z;
end;

function TFBXVector3.ProjectToBounds(const MinVector,MaxVector:TFBXVector3):TFBXScalar;
begin
 if x<0.0 then begin
  result:=x*MaxVector.x;
 end else begin
  result:=x*MinVector.x;
 end;
 if y<0.0 then begin
  result:=result+(y*MaxVector.y);
 end else begin
  result:=result+(y*MinVector.y);
 end;
 if z<0.0 then begin
  result:=result+(z*MaxVector.z);
 end else begin
  result:=result+(z*MinVector.z);
 end;
end;

constructor TFBXVector4.Create(const pX:TFBXScalar);
begin
 x:=pX;
 y:=pX;
 z:=pX;
 w:=pX;
end;

constructor TFBXVector4.Create(const pX,pY,pZ,pW:TFBXScalar);
begin
 x:=pX;
 y:=pY;
 z:=pZ;
 w:=pW;
end;

constructor TFBXVector4.Create(const pXY:TFBXVector2;const pZ:TFBXScalar=0.0;const pW:TFBXScalar=1.0);
begin
 x:=pXY.x;
 y:=pXY.y;
 z:=pZ;
 w:=pW;
end;

constructor TFBXVector4.Create(const pXYZ:TFBXVector3;const pW:TFBXScalar=1.0);
begin
 x:=pXYZ.x;
 y:=pXYZ.y;
 z:=pXYZ.z;
 w:=pW;
end;

class operator TFBXVector4.Implicit(const a:TFBXScalar):TFBXVector4;
begin
 result.x:=a;
 result.y:=a;
 result.z:=a;
 result.w:=a;
end;

class operator TFBXVector4.Explicit(const a:TFBXScalar):TFBXVector4;
begin
 result.x:=a;
 result.y:=a;
 result.z:=a;
 result.w:=a;
end;

class operator TFBXVector4.Equal(const a,b:TFBXVector4):boolean;
begin
 result:=SameValue(a.x,b.x) and SameValue(a.y,b.y) and SameValue(a.z,b.z) and SameValue(a.w,b.w);
end;

class operator TFBXVector4.NotEqual(const a,b:TFBXVector4):boolean;
begin
 result:=(not SameValue(a.x,b.x)) or (not SameValue(a.y,b.y)) or (not SameValue(a.z,b.z)) or (not SameValue(a.w,b.w));
end;

class operator TFBXVector4.Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x+1.0;
 result.y:=a.y+1.0;
 result.z:=a.z+1.0;
 result.w:=a.w+1.0;
end;

class operator TFBXVector4.Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x-1.0;
 result.y:=a.y-1.0;
 result.z:=a.z-1.0;
 result.w:=a.w-1.0;
end;

class operator TFBXVector4.Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x+b.x;
 result.y:=a.y+b.y;
 result.z:=a.z+b.z;
 result.w:=a.w+b.w;
end;

class operator TFBXVector4.Add(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=a.x+b;
 result.y:=a.y+b;
 result.z:=a.z+b;
 result.w:=a.w+b;
end;

class operator TFBXVector4.Add(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4;
begin
 result.x:=a+b.x;
 result.y:=a+b.y;
 result.z:=a+b.z;
 result.w:=a+b.w;
end;

class operator TFBXVector4.Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x-b.x;
 result.y:=a.y-b.y;
 result.z:=a.z-b.z;
 result.w:=a.w-b.w;
end;

class operator TFBXVector4.Subtract(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=a.x-b;
 result.y:=a.y-b;
 result.z:=a.z-b;
 result.w:=a.w-b;
end;

class operator TFBXVector4.Subtract(const a:TFBXScalar;const b:TFBXVector4): TFBXVector4;
begin
 result.x:=a-b.x;
 result.y:=a-b.y;
 result.z:=a-b.z;
 result.w:=a-b.w;
end;

class operator TFBXVector4.Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x*b.x;
 result.y:=a.y*b.y;
 result.z:=a.z*b.z;
 result.w:=a.w*b.w;
end;

class operator TFBXVector4.Multiply(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=a.x*b;
 result.y:=a.y*b;
 result.z:=a.z*b;
 result.w:=a.w*b;
end;

class operator TFBXVector4.Multiply(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4;
begin
 result.x:=a*b.x;
 result.y:=a*b.y;
 result.z:=a*b.z;
 result.w:=a*b.w;
end;

class operator TFBXVector4.Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
 result.z:=a.z/b.z;
 result.w:=a.w/b.w;
end;

class operator TFBXVector4.Divide(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
 result.z:=a.z/b;
 result.w:=a.w/b;
end;

class operator TFBXVector4.Divide(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
 result.z:=a/b.z;
 result.w:=a/b.z;
end;

class operator TFBXVector4.IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=a.x/b.x;
 result.y:=a.y/b.y;
 result.z:=a.z/b.z;
 result.w:=a.w/b.w;
end;

class operator TFBXVector4.IntDivide(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=a.x/b;
 result.y:=a.y/b;
 result.z:=a.z/b;
 result.w:=a.w/b;
end;

class operator TFBXVector4.IntDivide(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4;
begin
 result.x:=a/b.x;
 result.y:=a/b.y;
 result.z:=a/b.z;
 result.w:=a/b.w;
end;

class operator TFBXVector4.Modulus(const a,b:TFBXVector4):TFBXVector4;
begin
 result.x:=Modulus(a.x,b.x);
 result.y:=Modulus(a.y,b.y);
 result.z:=Modulus(a.z,b.z);
 result.w:=Modulus(a.w,b.w);
end;

class operator TFBXVector4.Modulus(const a:TFBXVector4;const b:TFBXScalar):TFBXVector4;
begin
 result.x:=Modulus(a.x,b);
 result.y:=Modulus(a.y,b);
 result.z:=Modulus(a.z,b);
 result.w:=Modulus(a.w,b);
end;

class operator TFBXVector4.Modulus(const a:TFBXScalar;const b:TFBXVector4):TFBXVector4;
begin
 result.x:=Modulus(a,b.x);
 result.y:=Modulus(a,b.y);
 result.z:=Modulus(a,b.z);
 result.w:=Modulus(a,b.w);
end;

class operator TFBXVector4.Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result.x:=-a.x;
 result.y:=-a.y;
 result.z:=-a.z;
 result.w:=-a.w;
end;

class operator TFBXVector4.Positive(const a:TFBXVector4):TFBXVector4;
begin
 result:=a;
end;

function TFBXVector4.GetComponent(const pIndex:Int32):TFBXScalar;
begin
 result:=RawComponents[pIndex];
end;

procedure TFBXVector4.SetComponent(const pIndex:Int32;const pValue:TFBXScalar);
begin
 RawComponents[pIndex]:=pValue;
end;

function TFBXVector4.Flip:TFBXVector4;
begin
 result.x:=x;
 result.y:=z;
 result.z:=-y;
 result.w:=w;
end;

function TFBXVector4.Perpendicular:TFBXVector4;
var v,p:TFBXVector4;
begin
 v:=p.Normalize;
 p.x:=System.abs(v.x);
 p.y:=System.abs(v.y);
 p.z:=System.abs(v.z);
 p.w:=System.abs(v.w);
 if (p.x<=p.y) and (p.x<=p.z) and (p.x<=p.w) then begin
  p.x:=1.0;
  p.y:=0.0;
  p.z:=0.0;
  p.w:=0.0;
 end else if (p.y<=p.x) and (p.y<=p.z) and (p.y<=p.w) then begin
  p.x:=0.0;
  p.y:=1.0;
  p.z:=0.0;
  p.w:=0.0;
 end else if (p.z<=p.x) and (p.z<=p.y) and (p.z<=p.w) then begin
  p.x:=0.0;
  p.y:=0.0;
  p.z:=0.0;
  p.w:=1.0;
 end else begin
  p.x:=0.0;
  p.y:=0.0;
  p.z:=1.0;
  p.w:=0.0;
 end;
 result:=p-(v*v.Dot(p));
end;

function TFBXVector4.Length:TFBXScalar;
begin
 result:=sqrt(sqr(x)+sqr(y)+sqr(z)+sqr(w));
end;

function TFBXVector4.SquaredLength:TFBXScalar;
begin
 result:=sqr(x)+sqr(y)+sqr(z)+sqr(w);
end;

function TFBXVector4.Normalize:TFBXVector4;
var Factor:TFBXScalar;
begin
 Factor:=sqrt(sqr(x)+sqr(y)+sqr(z)+sqr(w));
 if Factor<>0.0 then begin
  Factor:=1.0/Factor;
  result.x:=x*Factor;
  result.y:=y*Factor;
  result.z:=z*Factor;
  result.w:=w*Factor;
 end else begin
  result.x:=0.0;
  result.y:=0.0;
  result.z:=0.0;
  result.w:=0.0;
 end;
end;

function TFBXVector4.DistanceTo({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXScalar;
begin
 result:=sqrt(sqr(x-b.x)+sqr(y-b.y)+sqr(z-b.z)+sqr(w-b.w));
end;

function TFBXVector4.Abs:TFBXVector4;
begin
 result.x:=System.Abs(x);
 result.y:=System.Abs(y);
 result.z:=System.Abs(z);
 result.w:=System.Abs(w);
end;

function TFBXVector4.Dot({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXScalar; {$if not (defined(cpu386) or defined(cpux64))}{$ifdef caninline}inline;{$endif}{$ifend}
begin
 result:=(x*b.x)+(y*b.y)+(z*b.z)+(w*b.w);
end;

function TFBXVector4.AngleTo(const b:TFBXVector4):TFBXScalar;
var d:single;
begin
 d:=sqrt(SquaredLength*b.SquaredLength);
 if d<>0.0 then begin
  result:=Dot(b)/d;
 end else begin
  result:=0.0;
 end
end;

function TFBXVector4.Cross({$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXVector4;
begin
 result.x:=(y*b.z)-(z*b.y);
 result.y:=(z*b.x)-(x*b.z);
 result.z:=(x*b.y)-(y*b.x);
 result.w:=1.0;
end;

function TFBXVector4.Lerp(const b:TFBXVector4;const t:TFBXScalar):TFBXVector4;
var InvT:TFBXScalar;
begin
 if t<=0.0 then begin
  result:=self;
 end else if t>=1.0 then begin
  result:=b;
 end else begin
  InvT:=1.0-t;
  result.x:=(x*InvT)+(b.x*t);
  result.y:=(y*InvT)+(b.y*t);
  result.z:=(z*InvT)+(b.z*t);
  result.w:=(w*InvT)+(b.w*t);
 end;
end;

function TFBXVector4.Angle(const b,c:TFBXVector4):TFBXScalar;
var DeltaAB,DeltaCB:TFBXVector4;
    LengthAB,LengthCB:TFBXScalar;
begin
 DeltaAB:=self-b;
 DeltaCB:=c-b;
 LengthAB:=DeltaAB.Length;
 LengthCB:=DeltaCB.Length;
 if (LengthAB=0.0) or (LengthCB=0.0) then begin
  result:=0.0;
 end else begin
  result:=ArcCos(DeltaAB.Dot(DeltaCB)/(LengthAB*LengthCB));
 end;
end;

function TFBXVector4.RotateX(const Angle:TFBXScalar):TFBXVector4;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=x;
 result.y:=(y*Cosinus)-(z*Sinus);
 result.z:=(y*Sinus)+(z*Cosinus);
 result.w:=w;
end;

function TFBXVector4.RotateY(const Angle:TFBXScalar):TFBXVector4;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(z*Sinus)+(x*Cosinus);
 result.y:=y;
 result.z:=(z*Cosinus)-(x*Sinus);
 result.w:=w;
end;

function TFBXVector4.RotateZ(const Angle:TFBXScalar):TFBXVector4;
var Sinus,Cosinus:TFBXScalar;
begin
 Sinus:=0.0;
 Cosinus:=0.0;
 SinCos(Angle,Sinus,Cosinus);
 result.x:=(x*Cosinus)-(y*Sinus);
 result.y:=(x*Sinus)+(y*Cosinus);
 result.z:=z;
 result.w:=w;
end;

function TFBXVector4.Rotate(const Angle:TFBXScalar;const Axis:TFBXVector3):TFBXVector4;
begin
 result:=TFBXMatrix4x4.CreateRotate(Angle,Axis)*self;
end;

function TFBXVector4.ProjectToBounds(const MinVector,MaxVector:TFBXVector4):TFBXScalar;
begin
 if x<0.0 then begin
  result:=x*MaxVector.x;
 end else begin
  result:=x*MinVector.x;
 end;
 if y<0.0 then begin
  result:=result+(y*MaxVector.y);
 end else begin
  result:=result+(y*MinVector.y);
 end;
 if z<0.0 then begin
  result:=result+(z*MaxVector.z);
 end else begin
  result:=result+(z*MinVector.z);
 end;
 if w<0.0 then begin
  result:=result+(w*MaxVector.w);
 end else begin
  result:=result+(w*MinVector.w);
 end;
end;

constructor TFBXColor.Create(const pFrom:TFBXColor);
begin
 self:=pFrom;
end;

constructor TFBXColor.Create(const pRed,pGreen,pBlue:Double;const pAlpha:Double=1.0);
begin
 Red:=pRed;
 Green:=pGreen;
 Blue:=pBlue;
 Alpha:=pAlpha;
end;

constructor TFBXColor.Create(const pArray:array of Double);
begin
 Red:=pArray[0];
 Green:=pArray[1];
 Blue:=pArray[2];
 Alpha:=pArray[3];
end;

function TFBXColor.GetComponent(const pIndex:Int32):Double;
begin
 result:=RawComponents[pIndex];
end;

procedure TFBXColor.SetComponent(const pIndex:Int32;const pValue:Double);
begin
 RawComponents[pIndex]:=pValue;
end;

function TFBXColor.ToString:TFBXString;
begin
 result:='{{R:{'+ConvertDoubleToString(Red,omStandard,-1)+'} G:{'+ConvertDoubleToString(Green,omStandard,-1)+'} B:{'+ConvertDoubleToString(Blue,omStandard,-1)+'} A:{'+ConvertDoubleToString(Alpha,omStandard,-1)+'}}}';
end;

constructor TFBXMatrix4x4.Create(const pX:TFBXScalar);
begin
 RawComponents[0,0]:=pX;
 RawComponents[0,1]:=pX;
 RawComponents[0,2]:=pX;
 RawComponents[0,3]:=pX;
 RawComponents[1,0]:=pX;
 RawComponents[1,1]:=pX;
 RawComponents[1,2]:=pX;
 RawComponents[1,3]:=pX;
 RawComponents[2,0]:=pX;
 RawComponents[2,1]:=pX;
 RawComponents[2,2]:=pX;
 RawComponents[2,3]:=pX;
 RawComponents[3,0]:=pX;
 RawComponents[3,1]:=pX;
 RawComponents[3,2]:=pX;
 RawComponents[3,3]:=pX;
end;

constructor TFBXMatrix4x4.Create(const pXX,pXY,pXZ,pXW,pYX,pYY,pYZ,pYW,pZX,pZY,pZZ,pZW,pWX,pWY,pWZ,pWW:TFBXScalar);
begin
 RawComponents[0,0]:=pXX;
 RawComponents[0,1]:=pXY;
 RawComponents[0,2]:=pXZ;
 RawComponents[0,3]:=pXW;
 RawComponents[1,0]:=pYX;
 RawComponents[1,1]:=pYY;
 RawComponents[1,2]:=pYZ;
 RawComponents[1,3]:=pYW;
 RawComponents[2,0]:=pZX;
 RawComponents[2,1]:=pZY;
 RawComponents[2,2]:=pZZ;
 RawComponents[2,3]:=pZW;
 RawComponents[3,0]:=pWX;
 RawComponents[3,1]:=pWY;
 RawComponents[3,2]:=pWZ;
 RawComponents[3,3]:=pWW;
end;

constructor TFBXMatrix4x4.Create(const pX,pY,pZ,pW:TFBXVector4);
begin
 RawComponents[0,0]:=pX.x;
 RawComponents[0,1]:=pX.y;
 RawComponents[0,2]:=pX.z;
 RawComponents[0,3]:=pX.w;
 RawComponents[1,0]:=pY.x;
 RawComponents[1,1]:=pY.y;
 RawComponents[1,2]:=pY.z;
 RawComponents[1,3]:=pY.w;
 RawComponents[2,0]:=pZ.x;
 RawComponents[2,1]:=pZ.y;
 RawComponents[2,2]:=pZ.z;
 RawComponents[2,3]:=pZ.w;
 RawComponents[3,0]:=pW.x;
 RawComponents[3,1]:=pW.y;
 RawComponents[3,2]:=pW.z;
 RawComponents[3,3]:=pW.w;
end;

constructor TFBXMatrix4x4.CreateRotateX(const Angle:TFBXScalar);
begin
 RawComponents[0,0]:=1.0;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 SinCos(Angle,RawComponents[1,2],RawComponents[1,1]);
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=-RawComponents[1,2];
 RawComponents[2,2]:=RawComponents[1,1];
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateRotateY(const Angle:TFBXScalar);
begin
 SinCos(Angle,RawComponents[2,0],RawComponents[0,0]);
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=-RawComponents[2,0];
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=1.0;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=RawComponents[0,0];
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateRotateZ(const Angle:TFBXScalar);
begin
 SinCos(Angle,RawComponents[0,1],RawComponents[0,0]);
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=-RawComponents[0,1];
 RawComponents[1,1]:=RawComponents[0,0];
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateRotate(const Angle:TFBXScalar;const Axis:TFBXVector3);
var SinusAngle,CosinusAngle:TFBXScalar;
begin
 SinCos(Angle,SinusAngle,CosinusAngle);
 RawComponents[0,0]:=CosinusAngle+((1.0-CosinusAngle)*sqr(Axis.x));
 RawComponents[1,0]:=((1.0-CosinusAngle)*Axis.x*Axis.y)-(Axis.z*SinusAngle);
 RawComponents[2,0]:=((1.0-CosinusAngle)*Axis.x*Axis.z)+(Axis.y*SinusAngle);
 RawComponents[0,3]:=0.0;
 RawComponents[0,1]:=((1.0-CosinusAngle)*Axis.x*Axis.z)+(Axis.z*SinusAngle);
 RawComponents[1,1]:=CosinusAngle+((1.0-CosinusAngle)*sqr(Axis.y));
 RawComponents[2,1]:=((1.0-CosinusAngle)*Axis.y*Axis.z)-(Axis.x*SinusAngle);
 RawComponents[1,3]:=0.0;
 RawComponents[0,2]:=((1.0-CosinusAngle)*Axis.x*Axis.z)-(Axis.y*SinusAngle);
 RawComponents[1,2]:=((1.0-CosinusAngle)*Axis.y*Axis.z)+(Axis.x*SinusAngle);
 RawComponents[2,2]:=CosinusAngle+((1.0-CosinusAngle)*sqr(Axis.z));
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateRotation(const pMatrix:TFBXMatrix4x4);
begin
 RawComponents[0,0]:=pMatrix.RawComponents[0,0];
 RawComponents[0,1]:=pMatrix.RawComponents[0,1];
 RawComponents[0,2]:=pMatrix.RawComponents[0,2];
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=pMatrix.RawComponents[1,0];
 RawComponents[1,1]:=pMatrix.RawComponents[1,1];
 RawComponents[1,2]:=pMatrix.RawComponents[1,2];
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=pMatrix.RawComponents[2,0];
 RawComponents[2,1]:=pMatrix.RawComponents[2,1];
 RawComponents[2,2]:=pMatrix.RawComponents[2,2];
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateScale(const sx,sy,sz:TFBXScalar);
begin
 RawComponents[0,0]:=sx;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=sy;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=sz;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateScale(const pScale:TFBXVector3);
begin
 RawComponents[0,0]:=pScale.x;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=pScale.y;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=pScale.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateScale(const sx,sy,sz,sw:TFBXScalar);
begin
 RawComponents[0,0]:=sx;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=sy;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=sz;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=sw;
end;

constructor TFBXMatrix4x4.CreateScale(const pScale:TFBXVector4);
begin
 RawComponents[0,0]:=pScale.x;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=pScale.y;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=pScale.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=pScale.w;
end;

constructor TFBXMatrix4x4.CreateTranslation(const tx,ty,tz:TFBXScalar);
begin
 RawComponents[0,0]:=1.0;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=1.0;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=tx;
 RawComponents[3,1]:=ty;
 RawComponents[3,2]:=tz;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateTranslation(const pTranslation:TFBXVector3);
begin
 RawComponents[0,0]:=1.0;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=1.0;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=pTranslation.x;
 RawComponents[3,1]:=pTranslation.y;
 RawComponents[3,2]:=pTranslation.z;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateTranslation(const tx,ty,tz,tw:TFBXScalar);
begin
 RawComponents[0,0]:=1.0;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=1.0;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=tx;
 RawComponents[3,1]:=ty;
 RawComponents[3,2]:=tz;
 RawComponents[3,3]:=tw;
end;

constructor TFBXMatrix4x4.CreateTranslation(const pTranslation:TFBXVector4);
begin
 RawComponents[0,0]:=1.0;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=1.0;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=pTranslation.x;
 RawComponents[3,1]:=pTranslation.y;
 RawComponents[3,2]:=pTranslation.z;
 RawComponents[3,3]:=pTranslation.w;
end;

constructor TFBXMatrix4x4.CreateTranslated(const pMatrix:TFBXMatrix4x4;pTranslation:TFBXVector3);
begin
 RawComponents[0]:=pMatrix.RawComponents[0];
 RawComponents[1]:=pMatrix.RawComponents[1];
 RawComponents[2]:=pMatrix.RawComponents[2];
 RawComponents[3,0]:=(pMatrix.RawComponents[0,0]*pTranslation.x)+(pMatrix.RawComponents[1,0]*pTranslation.y)+(pMatrix.RawComponents[2,0]*pTranslation.z)+pMatrix.RawComponents[3,0];
 RawComponents[3,1]:=(pMatrix.RawComponents[0,1]*pTranslation.x)+(pMatrix.RawComponents[1,1]*pTranslation.y)+(pMatrix.RawComponents[2,1]*pTranslation.z)+pMatrix.RawComponents[3,1];
 RawComponents[3,2]:=(pMatrix.RawComponents[0,2]*pTranslation.x)+(pMatrix.RawComponents[1,2]*pTranslation.y)+(pMatrix.RawComponents[2,2]*pTranslation.z)+pMatrix.RawComponents[3,2];
 RawComponents[3,3]:=(pMatrix.RawComponents[0,3]*pTranslation.x)+(pMatrix.RawComponents[1,3]*pTranslation.y)+(pMatrix.RawComponents[2,3]*pTranslation.z)+pMatrix.RawComponents[3,3];
end;

constructor TFBXMatrix4x4.CreateTranslated(const pMatrix:TFBXMatrix4x4;pTranslation:TFBXVector4);
begin
 RawComponents[0]:=pMatrix.RawComponents[0];
 RawComponents[1]:=pMatrix.RawComponents[1];
 RawComponents[2]:=pMatrix.RawComponents[2];
 RawComponents[3,0]:=(pMatrix.RawComponents[0,0]*pTranslation.x)+(pMatrix.RawComponents[1,0]*pTranslation.y)+(pMatrix.RawComponents[2,0]*pTranslation.z)+(pMatrix.RawComponents[3,0]*pTranslation.w);
 RawComponents[3,1]:=(pMatrix.RawComponents[0,1]*pTranslation.x)+(pMatrix.RawComponents[1,1]*pTranslation.y)+(pMatrix.RawComponents[2,1]*pTranslation.z)+(pMatrix.RawComponents[3,1]*pTranslation.w);
 RawComponents[3,2]:=(pMatrix.RawComponents[0,2]*pTranslation.x)+(pMatrix.RawComponents[1,2]*pTranslation.y)+(pMatrix.RawComponents[2,2]*pTranslation.z)+(pMatrix.RawComponents[3,2]*pTranslation.w);
 RawComponents[3,3]:=(pMatrix.RawComponents[0,3]*pTranslation.x)+(pMatrix.RawComponents[1,3]*pTranslation.y)+(pMatrix.RawComponents[2,3]*pTranslation.z)+(pMatrix.RawComponents[3,3]*pTranslation.w);
end;

constructor TFBXMatrix4x4.CreateFromToRotation(const FromDirection,ToDirection:TFBXVector3);
const EPSILON=1e-8;
var e,h,hvx,hvz,hvxy,hvxz,hvyz:TFBXScalar;
    x,u,v,c:TFBXVector3;
begin
 e:=FromDirection.Dot(ToDirection);
 if abs(e)>(1.0-EPSILON) then begin
  x:=FromDirection.Abs;
  if x.x<x.y then begin
   if x.x<x.z then begin
    x.x:=1.0;
    x.y:=0.0;
    x.z:=0.0;
   end else begin
    x.x:=0.0;
    x.y:=0.0;
    x.z:=1.0;
   end;
  end else begin
   if x.y<x.z then begin
    x.x:=0.0;
    x.y:=1.0;
    x.z:=0.0;
   end else begin
    x.x:=0.0;
    x.y:=0.0;
    x.z:=1.0;
   end;
  end;
  u:=x-FromDirection;
  v:=x-ToDirection;
  c.x:=2.0/(sqr(u.x)+sqr(u.y)+sqr(u.z));
  c.y:=2.0/(sqr(v.x)+sqr(v.y)+sqr(v.z));
  c.z:=c.x*c.y*((u.x*v.x)+(u.y*v.y)+(u.z*v.z));
  RawComponents[0,0]:=1.0+((c.z*(v.x*u.x))-((c.y*(v.x*v.x))+(c.x*(u.x*u.x))));
  RawComponents[0,1]:=(c.z*(v.x*u.y))-((c.y*(v.x*v.y))+(c.x*(u.x*u.y)));
  RawComponents[0,2]:=(c.z*(v.x*u.z))-((c.y*(v.x*v.z))+(c.x*(u.x*u.z)));
  RawComponents[0,3]:=0.0;
  RawComponents[1,0]:=(c.z*(v.y*u.x))-((c.y*(v.y*v.x))+(c.x*(u.y*u.x)));
  RawComponents[1,1]:=1.0+((c.z*(v.y*u.y))-((c.y*(v.y*v.y))+(c.x*(u.y*u.y))));
  RawComponents[1,2]:=(c.z*(v.y*u.z))-((c.y*(v.y*v.z))+(c.x*(u.y*u.z)));
  RawComponents[1,3]:=0.0;
  RawComponents[2,0]:=(c.z*(v.z*u.x))-((c.y*(v.z*v.x))+(c.x*(u.z*u.x)));
  RawComponents[2,1]:=(c.z*(v.z*u.y))-((c.y*(v.z*v.y))+(c.x*(u.z*u.y)));
  RawComponents[2,2]:=1.0+((c.z*(v.z*u.z))-((c.y*(v.z*v.z))+(c.x*(u.z*u.z))));
  RawComponents[2,3]:=0.0;
  RawComponents[3,0]:=0.0;
  RawComponents[3,1]:=0.0;
  RawComponents[3,2]:=0.0;
  RawComponents[3,3]:=1.0;
 end else begin
  v:=FromDirection.Cross(ToDirection);
  h:=1.0/(1.0+e);
  hvx:=h*v.x;
  hvz:=h*v.z;
  hvxy:=hvx*v.y;
  hvxz:=hvx*v.z;
  hvyz:=hvz*v.y;
  RawComponents[0,0]:=e+(hvx*v.x);
  RawComponents[0,1]:=hvxy-v.z;
  RawComponents[0,2]:=hvxz+v.y;
  RawComponents[0,3]:=0.0;
  RawComponents[1,0]:=hvxy+v.z;
  RawComponents[1,1]:=e+(h*sqr(v.y));
  RawComponents[1,2]:=hvyz-v.x;
  RawComponents[1,3]:=0.0;
  RawComponents[2,0]:=hvxz-v.y;
  RawComponents[2,1]:=hvyz+v.x;
  RawComponents[2,2]:=e+(hvz*v.z);
  RawComponents[2,3]:=0.0;
  RawComponents[3,0]:=0.0;
  RawComponents[3,1]:=0.0;
  RawComponents[3,2]:=0.0;
  RawComponents[3,3]:=1.0;
 end;
end;

constructor TFBXMatrix4x4.CreateConstruct(const pForwards,pUp:TFBXVector3);
var RightVector,UpVector,ForwardVector:TFBXVector3;
begin
 ForwardVector:=(-pForwards).Normalize;
 RightVector:=pUp.Cross(ForwardVector).Normalize;
 UpVector:=ForwardVector.Cross(RightVector).Normalize;
 RawComponents[0,0]:=RightVector.x;
 RawComponents[0,1]:=RightVector.y;
 RawComponents[0,2]:=RightVector.z;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=UpVector.x;
 RawComponents[1,1]:=UpVector.y;
 RawComponents[1,2]:=UpVector.z;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=ForwardVector.x;
 RawComponents[2,1]:=ForwardVector.y;
 RawComponents[2,2]:=ForwardVector.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateOuterProduct(const u,v:TFBXVector3);
begin
 RawComponents[0,0]:=u.x*v.x;
 RawComponents[0,1]:=u.x*v.y;
 RawComponents[0,2]:=u.x*v.z;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=u.y*v.x;
 RawComponents[1,1]:=u.y*v.y;
 RawComponents[1,2]:=u.y*v.z;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=u.z*v.x;
 RawComponents[2,1]:=u.z*v.y;
 RawComponents[2,2]:=u.z*v.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateFrustum(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=(zNear*2.0)/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=(zNear*2.0)/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=(Right+Left)/rml;
 RawComponents[2,1]:=(Top+Bottom)/tmb;
 RawComponents[2,2]:=(-(zFar+zNear))/fmn;
 RawComponents[2,3]:=-1.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=(-((zFar*zNear)*2.0))/fmn;
 RawComponents[3,3]:=0.0;
end;

constructor TFBXMatrix4x4.CreateOrtho(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=2.0/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=2.0/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=(-2.0)/fmn;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=(-(Right+Left))/rml;
 RawComponents[3,1]:=(-(Top+Bottom))/tmb;
 RawComponents[3,2]:=(-(zFar+zNear))/fmn;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateOrthoLH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=2.0/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=2.0/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0/fmn;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0;
 RawComponents[3,1]:=0;
 RawComponents[3,2]:=(-zNear)/fmn;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateOrthoRH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=2.0/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=2.0/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0/fmn;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0;
 RawComponents[3,1]:=0;
 RawComponents[3,2]:=zNear/fmn;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateOrthoOffCenterLH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=2.0/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=2.0/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=1.0/fmn;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=(Right+Left)/rml;
 RawComponents[3,1]:=(Top+Bottom)/tmb;
 RawComponents[3,2]:=zNear/fmn;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateOrthoOffCenterRH(const Left,Right,Bottom,Top,zNear,zFar:TFBXScalar);
var rml,tmb,fmn:TFBXScalar;
begin
 rml:=Right-Left;
 tmb:=Top-Bottom;
 fmn:=zFar-zNear;
 RawComponents[0,0]:=2.0/rml;
 RawComponents[0,1]:=0.0;
 RawComponents[0,2]:=0.0;
 RawComponents[0,3]:=0.0;
 RawComponents[1,0]:=0.0;
 RawComponents[1,1]:=2.0/tmb;
 RawComponents[1,2]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,0]:=0.0;
 RawComponents[2,1]:=0.0;
 RawComponents[2,2]:=(-2.0)/fmn;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=(-(Right+Left))/rml;
 RawComponents[3,1]:=(-(Top+Bottom))/tmb;
 RawComponents[3,2]:=(-(zFar+zNear))/fmn;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreatePerspective(const fovy,Aspect,zNear,zFar:TFBXScalar);
const DEG2RAD=180.0/pi;
var Sine,Cotangent,ZDelta,Radians:TFBXScalar;
begin
 Radians:=(fovy*0.5)*DEG2RAD;
 ZDelta:=zFar-zNear;
 Sine:=sin(Radians);
 if not ((ZDelta=0) or (Sine=0) or (aspect=0)) then begin
  Cotangent:=cos(Radians)/Sine;
  RawComponents:=FBXMatrix4x4Identity.RawComponents;
  RawComponents[0,0]:=Cotangent/aspect;
  RawComponents[1,1]:=Cotangent;
  RawComponents[2,2]:=(-(zFar+zNear))/ZDelta;
  RawComponents[2,3]:=-1-0;
  RawComponents[3,2]:=(-(2.0*zNear*zFar))/ZDelta;
  RawComponents[3,3]:=0.0;
 end;
end;

constructor TFBXMatrix4x4.CreateLookAt(const Eye,Center,Up:TFBXVector3);
var RightVector,UpVector,ForwardVector:TFBXVector3;
begin
 ForwardVector:=(Eye-Center).Normalize;
 RightVector:=(Up.Cross(ForwardVector)).Normalize;
 UpVector:=(ForwardVector.Cross(RightVector)).Normalize;
 RawComponents[0,0]:=RightVector.x;
 RawComponents[1,0]:=RightVector.y;
 RawComponents[2,0]:=RightVector.z;
 RawComponents[3,0]:=-((RightVector.x*Eye.x)+(RightVector.y*Eye.y)+(RightVector.z*Eye.z));
 RawComponents[0,1]:=UpVector.x;
 RawComponents[1,1]:=UpVector.y;
 RawComponents[2,1]:=UpVector.z;
 RawComponents[3,1]:=-((UpVector.x*Eye.x)+(UpVector.y*Eye.y)+(UpVector.z*Eye.z));
 RawComponents[0,2]:=ForwardVector.x;
 RawComponents[1,2]:=ForwardVector.y;
 RawComponents[2,2]:=ForwardVector.z;
 RawComponents[3,2]:=-((ForwardVector.x*Eye.x)+(ForwardVector.y*Eye.y)+(ForwardVector.z*Eye.z));
 RawComponents[0,3]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateFill(const Eye,RightVector,UpVector,ForwardVector:TFBXVector3);
begin
 RawComponents[0,0]:=RightVector.x;
 RawComponents[1,0]:=RightVector.y;
 RawComponents[2,0]:=RightVector.z;
 RawComponents[3,0]:=-((RightVector.x*Eye.x)+(RightVector.y*Eye.y)+(RightVector.z*Eye.z));
 RawComponents[0,1]:=UpVector.x;
 RawComponents[1,1]:=UpVector.y;
 RawComponents[2,1]:=UpVector.z;
 RawComponents[3,1]:=-((UpVector.x*Eye.x)+(UpVector.y*Eye.y)+(UpVector.z*Eye.z));
 RawComponents[0,2]:=ForwardVector.x;
 RawComponents[1,2]:=ForwardVector.y;
 RawComponents[2,2]:=ForwardVector.z;
 RawComponents[3,2]:=-((ForwardVector.x*Eye.x)+(ForwardVector.y*Eye.y)+(ForwardVector.z*Eye.z));
 RawComponents[0,3]:=0.0;
 RawComponents[1,3]:=0.0;
 RawComponents[2,3]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateConstructX(const xAxis:TFBXVector3);
var a,b,c:TFBXVector3;
begin
 a:=xAxis.Normalize;
 RawComponents[0,0]:=a.x;
 RawComponents[0,1]:=a.y;
 RawComponents[0,2]:=a.z;
 RawComponents[0,3]:=0.0;
//b:=TFBXVector3.Create(0.0,0.0,1.0).Cross(a).Normalize;
 b:=a.Perpendicular.Normalize;
 RawComponents[1,0]:=b.x;
 RawComponents[1,1]:=b.y;
 RawComponents[1,2]:=b.z;
 RawComponents[1,3]:=0.0;
 c:=b.Cross(a).Normalize;
 RawComponents[2,0]:=c.x;
 RawComponents[2,1]:=c.y;
 RawComponents[2,2]:=c.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateConstructY(const yAxis:TFBXVector3);
var a,b,c:TFBXVector3;
begin
 a:=yAxis.Normalize;
 RawComponents[1,0]:=a.x;
 RawComponents[1,1]:=a.y;
 RawComponents[1,2]:=a.z;
 RawComponents[1,3]:=0.0;
 b:=a.Perpendicular.Normalize;
 RawComponents[0,0]:=b.x;
 RawComponents[0,1]:=b.y;
 RawComponents[0,2]:=b.z;
 RawComponents[0,3]:=0.0;
 c:=b.Cross(a).Normalize;
 RawComponents[2,0]:=c.x;
 RawComponents[2,1]:=c.y;
 RawComponents[2,2]:=c.z;
 RawComponents[2,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

constructor TFBXMatrix4x4.CreateConstructZ(const zAxis:TFBXVector3);
var a,b,c:TFBXVector3;
begin
 a:=zAxis.Normalize;
 RawComponents[2,0]:=a.x;
 RawComponents[2,1]:=a.y;
 RawComponents[2,2]:=a.z;
 RawComponents[2,3]:=0.0;
//b:=TFBXVector3.Create(0.0,1.0,0.0).Cross(a).Normalize;
 b:=a.Perpendicular.Normalize;
 RawComponents[1,0]:=b.x;
 RawComponents[1,1]:=b.y;
 RawComponents[1,2]:=b.z;
 RawComponents[1,3]:=0.0;
 c:=b.Cross(a).Normalize;
 RawComponents[0,0]:=c.x;
 RawComponents[0,1]:=c.y;
 RawComponents[0,2]:=c.z;
 RawComponents[0,3]:=0.0;
 RawComponents[3,0]:=0.0;
 RawComponents[3,1]:=0.0;
 RawComponents[3,2]:=0.0;
 RawComponents[3,3]:=1.0;
end;

class operator TFBXMatrix4x4.Implicit({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a;
 result.RawComponents[0,1]:=a;
 result.RawComponents[0,2]:=a;
 result.RawComponents[0,3]:=a;
 result.RawComponents[1,0]:=a;
 result.RawComponents[1,1]:=a;
 result.RawComponents[1,2]:=a;
 result.RawComponents[1,3]:=a;
 result.RawComponents[2,0]:=a;
 result.RawComponents[2,1]:=a;
 result.RawComponents[2,2]:=a;
 result.RawComponents[2,3]:=a;
 result.RawComponents[3,0]:=a;
 result.RawComponents[3,1]:=a;
 result.RawComponents[3,2]:=a;
 result.RawComponents[3,3]:=a;
end;

class operator TFBXMatrix4x4.Explicit({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a;
 result.RawComponents[0,1]:=a;
 result.RawComponents[0,2]:=a;
 result.RawComponents[0,3]:=a;
 result.RawComponents[1,0]:=a;
 result.RawComponents[1,1]:=a;
 result.RawComponents[1,2]:=a;
 result.RawComponents[1,3]:=a;
 result.RawComponents[2,0]:=a;
 result.RawComponents[2,1]:=a;
 result.RawComponents[2,2]:=a;
 result.RawComponents[2,3]:=a;
 result.RawComponents[3,0]:=a;
 result.RawComponents[3,1]:=a;
 result.RawComponents[3,2]:=a;
 result.RawComponents[3,3]:=a;
end;

class operator TFBXMatrix4x4.Equal({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):boolean;
begin
 result:=SameValue(a.RawComponents[0,0],b.RawComponents[0,0]) and
         SameValue(a.RawComponents[0,1],b.RawComponents[0,1]) and
         SameValue(a.RawComponents[0,2],b.RawComponents[0,2]) and
         SameValue(a.RawComponents[0,3],b.RawComponents[0,3]) and
         SameValue(a.RawComponents[1,0],b.RawComponents[1,0]) and
         SameValue(a.RawComponents[1,1],b.RawComponents[1,1]) and
         SameValue(a.RawComponents[1,2],b.RawComponents[1,2]) and
         SameValue(a.RawComponents[1,3],b.RawComponents[1,3]) and
         SameValue(a.RawComponents[2,0],b.RawComponents[2,0]) and
         SameValue(a.RawComponents[2,1],b.RawComponents[2,1]) and
         SameValue(a.RawComponents[2,2],b.RawComponents[2,2]) and
         SameValue(a.RawComponents[2,3],b.RawComponents[2,3]) and
         SameValue(a.RawComponents[3,0],b.RawComponents[3,0]) and
         SameValue(a.RawComponents[3,1],b.RawComponents[3,1]) and
         SameValue(a.RawComponents[3,2],b.RawComponents[3,2]) and
         SameValue(a.RawComponents[3,3],b.RawComponents[3,3]);
end;

class operator TFBXMatrix4x4.NotEqual({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):boolean;
begin
 result:=(not SameValue(a.RawComponents[0,0],b.RawComponents[0,0])) or
         (not SameValue(a.RawComponents[0,1],b.RawComponents[0,1])) or
         (not SameValue(a.RawComponents[0,2],b.RawComponents[0,2])) or
         (not SameValue(a.RawComponents[0,3],b.RawComponents[0,3])) or
         (not SameValue(a.RawComponents[1,0],b.RawComponents[1,0])) or
         (not SameValue(a.RawComponents[1,1],b.RawComponents[1,1])) or
         (not SameValue(a.RawComponents[1,2],b.RawComponents[1,2])) or
         (not SameValue(a.RawComponents[1,3],b.RawComponents[1,3])) or
         (not SameValue(a.RawComponents[2,0],b.RawComponents[2,0])) or
         (not SameValue(a.RawComponents[2,1],b.RawComponents[2,1])) or
         (not SameValue(a.RawComponents[2,2],b.RawComponents[2,2])) or
         (not SameValue(a.RawComponents[2,3],b.RawComponents[2,3])) or
         (not SameValue(a.RawComponents[3,0],b.RawComponents[3,0])) or
         (not SameValue(a.RawComponents[3,1],b.RawComponents[3,1])) or
         (not SameValue(a.RawComponents[3,2],b.RawComponents[3,2])) or
         (not SameValue(a.RawComponents[3,3],b.RawComponents[3,3]));
end;

class operator TFBXMatrix4x4.Inc({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]+1.0;
 result.RawComponents[0,1]:=a.RawComponents[0,1]+1.0;
 result.RawComponents[0,2]:=a.RawComponents[0,2]+1.0;
 result.RawComponents[0,3]:=a.RawComponents[0,3]+1.0;
 result.RawComponents[1,0]:=a.RawComponents[1,0]+1.0;
 result.RawComponents[1,1]:=a.RawComponents[1,1]+1.0;
 result.RawComponents[1,2]:=a.RawComponents[1,2]+1.0;
 result.RawComponents[1,3]:=a.RawComponents[1,3]+1.0;
 result.RawComponents[2,0]:=a.RawComponents[2,0]+1.0;
 result.RawComponents[2,1]:=a.RawComponents[2,1]+1.0;
 result.RawComponents[2,2]:=a.RawComponents[2,2]+1.0;
 result.RawComponents[2,3]:=a.RawComponents[2,3]+1.0;
 result.RawComponents[3,0]:=a.RawComponents[3,0]+1.0;
 result.RawComponents[3,1]:=a.RawComponents[3,1]+1.0;
 result.RawComponents[3,2]:=a.RawComponents[3,2]+1.0;
 result.RawComponents[3,3]:=a.RawComponents[3,3]+1.0;
end;

class operator TFBXMatrix4x4.Dec({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]-1.0;
 result.RawComponents[0,1]:=a.RawComponents[0,1]-1.0;
 result.RawComponents[0,2]:=a.RawComponents[0,2]-1.0;
 result.RawComponents[0,3]:=a.RawComponents[0,3]-1.0;
 result.RawComponents[1,0]:=a.RawComponents[1,0]-1.0;
 result.RawComponents[1,1]:=a.RawComponents[1,1]-1.0;
 result.RawComponents[1,2]:=a.RawComponents[1,2]-1.0;
 result.RawComponents[1,3]:=a.RawComponents[1,3]-1.0;
 result.RawComponents[2,0]:=a.RawComponents[2,0]-1.0;
 result.RawComponents[2,1]:=a.RawComponents[2,1]-1.0;
 result.RawComponents[2,2]:=a.RawComponents[2,2]-1.0;
 result.RawComponents[2,3]:=a.RawComponents[2,3]-1.0;
 result.RawComponents[3,0]:=a.RawComponents[3,0]-1.0;
 result.RawComponents[3,1]:=a.RawComponents[3,1]-1.0;
 result.RawComponents[3,2]:=a.RawComponents[3,2]-1.0;
 result.RawComponents[3,3]:=a.RawComponents[3,3]-1.0;
end;

class operator TFBXMatrix4x4.Add({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]+b.RawComponents[0,0];
 result.RawComponents[0,1]:=a.RawComponents[0,1]+b.RawComponents[0,1];
 result.RawComponents[0,2]:=a.RawComponents[0,2]+b.RawComponents[0,2];
 result.RawComponents[0,3]:=a.RawComponents[0,3]+b.RawComponents[0,3];
 result.RawComponents[1,0]:=a.RawComponents[1,0]+b.RawComponents[1,0];
 result.RawComponents[1,1]:=a.RawComponents[1,1]+b.RawComponents[1,1];
 result.RawComponents[1,2]:=a.RawComponents[1,2]+b.RawComponents[1,2];
 result.RawComponents[1,3]:=a.RawComponents[1,3]+b.RawComponents[1,3];
 result.RawComponents[2,0]:=a.RawComponents[2,0]+b.RawComponents[2,0];
 result.RawComponents[2,1]:=a.RawComponents[2,1]+b.RawComponents[2,1];
 result.RawComponents[2,2]:=a.RawComponents[2,2]+b.RawComponents[2,2];
 result.RawComponents[2,3]:=a.RawComponents[2,3]+b.RawComponents[2,3];
 result.RawComponents[3,0]:=a.RawComponents[3,0]+b.RawComponents[3,0];
 result.RawComponents[3,1]:=a.RawComponents[3,1]+b.RawComponents[3,1];
 result.RawComponents[3,2]:=a.RawComponents[3,2]+b.RawComponents[3,2];
 result.RawComponents[3,3]:=a.RawComponents[3,3]+b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Add({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]+b;
 result.RawComponents[0,1]:=a.RawComponents[0,1]+b;
 result.RawComponents[0,2]:=a.RawComponents[0,2]+b;
 result.RawComponents[0,3]:=a.RawComponents[0,3]+b;
 result.RawComponents[1,0]:=a.RawComponents[1,0]+b;
 result.RawComponents[1,1]:=a.RawComponents[1,1]+b;
 result.RawComponents[1,2]:=a.RawComponents[1,2]+b;
 result.RawComponents[1,3]:=a.RawComponents[1,3]+b;
 result.RawComponents[2,0]:=a.RawComponents[2,0]+b;
 result.RawComponents[2,1]:=a.RawComponents[2,1]+b;
 result.RawComponents[2,2]:=a.RawComponents[2,2]+b;
 result.RawComponents[2,3]:=a.RawComponents[2,3]+b;
 result.RawComponents[3,0]:=a.RawComponents[3,0]+b;
 result.RawComponents[3,1]:=a.RawComponents[3,1]+b;
 result.RawComponents[3,2]:=a.RawComponents[3,2]+b;
 result.RawComponents[3,3]:=a.RawComponents[3,3]+b;
end;

class operator TFBXMatrix4x4.Add({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a+b.RawComponents[0,0];
 result.RawComponents[0,1]:=a+b.RawComponents[0,1];
 result.RawComponents[0,2]:=a+b.RawComponents[0,2];
 result.RawComponents[0,3]:=a+b.RawComponents[0,3];
 result.RawComponents[1,0]:=a+b.RawComponents[1,0];
 result.RawComponents[1,1]:=a+b.RawComponents[1,1];
 result.RawComponents[1,2]:=a+b.RawComponents[1,2];
 result.RawComponents[1,3]:=a+b.RawComponents[1,3];
 result.RawComponents[2,0]:=a+b.RawComponents[2,0];
 result.RawComponents[2,1]:=a+b.RawComponents[2,1];
 result.RawComponents[2,2]:=a+b.RawComponents[2,2];
 result.RawComponents[2,3]:=a+b.RawComponents[2,3];
 result.RawComponents[3,0]:=a+b.RawComponents[3,0];
 result.RawComponents[3,1]:=a+b.RawComponents[3,1];
 result.RawComponents[3,2]:=a+b.RawComponents[3,2];
 result.RawComponents[3,3]:=a+b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Subtract({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]-b.RawComponents[0,0];
 result.RawComponents[0,1]:=a.RawComponents[0,1]-b.RawComponents[0,1];
 result.RawComponents[0,2]:=a.RawComponents[0,2]-b.RawComponents[0,2];
 result.RawComponents[0,3]:=a.RawComponents[0,3]-b.RawComponents[0,3];
 result.RawComponents[1,0]:=a.RawComponents[1,0]-b.RawComponents[1,0];
 result.RawComponents[1,1]:=a.RawComponents[1,1]-b.RawComponents[1,1];
 result.RawComponents[1,2]:=a.RawComponents[1,2]-b.RawComponents[1,2];
 result.RawComponents[1,3]:=a.RawComponents[1,3]-b.RawComponents[1,3];
 result.RawComponents[2,0]:=a.RawComponents[2,0]-b.RawComponents[2,0];
 result.RawComponents[2,1]:=a.RawComponents[2,1]-b.RawComponents[2,1];
 result.RawComponents[2,2]:=a.RawComponents[2,2]-b.RawComponents[2,2];
 result.RawComponents[2,3]:=a.RawComponents[2,3]-b.RawComponents[2,3];
 result.RawComponents[3,0]:=a.RawComponents[3,0]-b.RawComponents[3,0];
 result.RawComponents[3,1]:=a.RawComponents[3,1]-b.RawComponents[3,1];
 result.RawComponents[3,2]:=a.RawComponents[3,2]-b.RawComponents[3,2];
 result.RawComponents[3,3]:=a.RawComponents[3,3]-b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Subtract({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]-b;
 result.RawComponents[0,1]:=a.RawComponents[0,1]-b;
 result.RawComponents[0,2]:=a.RawComponents[0,2]-b;
 result.RawComponents[0,3]:=a.RawComponents[0,3]-b;
 result.RawComponents[1,0]:=a.RawComponents[1,0]-b;
 result.RawComponents[1,1]:=a.RawComponents[1,1]-b;
 result.RawComponents[1,2]:=a.RawComponents[1,2]-b;
 result.RawComponents[1,3]:=a.RawComponents[1,3]-b;
 result.RawComponents[2,0]:=a.RawComponents[2,0]-b;
 result.RawComponents[2,1]:=a.RawComponents[2,1]-b;
 result.RawComponents[2,2]:=a.RawComponents[2,2]-b;
 result.RawComponents[2,3]:=a.RawComponents[2,3]-b;
 result.RawComponents[3,0]:=a.RawComponents[3,0]-b;
 result.RawComponents[3,1]:=a.RawComponents[3,1]-b;
 result.RawComponents[3,2]:=a.RawComponents[3,2]-b;
 result.RawComponents[3,3]:=a.RawComponents[3,3]-b;
end;

class operator TFBXMatrix4x4.Subtract({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4): TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a-b.RawComponents[0,0];
 result.RawComponents[0,1]:=a-b.RawComponents[0,1];
 result.RawComponents[0,2]:=a-b.RawComponents[0,2];
 result.RawComponents[0,3]:=a-b.RawComponents[0,3];
 result.RawComponents[1,0]:=a-b.RawComponents[1,0];
 result.RawComponents[1,1]:=a-b.RawComponents[1,1];
 result.RawComponents[1,2]:=a-b.RawComponents[1,2];
 result.RawComponents[1,3]:=a-b.RawComponents[1,3];
 result.RawComponents[2,0]:=a-b.RawComponents[2,0];
 result.RawComponents[2,1]:=a-b.RawComponents[2,1];
 result.RawComponents[2,2]:=a-b.RawComponents[2,2];
 result.RawComponents[2,3]:=a-b.RawComponents[2,3];
 result.RawComponents[3,0]:=a-b.RawComponents[3,0];
 result.RawComponents[3,1]:=a-b.RawComponents[3,1];
 result.RawComponents[3,2]:=a-b.RawComponents[3,2];
 result.RawComponents[3,3]:=a-b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=(a.RawComponents[0,0]*b.RawComponents[0,0])+(a.RawComponents[0,1]*b.RawComponents[1,0])+(a.RawComponents[0,2]*b.RawComponents[2,0])+(a.RawComponents[0,3]*b.RawComponents[3,0]);
 result.RawComponents[0,1]:=(a.RawComponents[0,0]*b.RawComponents[0,1])+(a.RawComponents[0,1]*b.RawComponents[1,1])+(a.RawComponents[0,2]*b.RawComponents[2,1])+(a.RawComponents[0,3]*b.RawComponents[3,1]);
 result.RawComponents[0,2]:=(a.RawComponents[0,0]*b.RawComponents[0,2])+(a.RawComponents[0,1]*b.RawComponents[1,2])+(a.RawComponents[0,2]*b.RawComponents[2,2])+(a.RawComponents[0,3]*b.RawComponents[3,2]);
 result.RawComponents[0,3]:=(a.RawComponents[0,0]*b.RawComponents[0,3])+(a.RawComponents[0,1]*b.RawComponents[1,3])+(a.RawComponents[0,2]*b.RawComponents[2,3])+(a.RawComponents[0,3]*b.RawComponents[3,3]);
 result.RawComponents[1,0]:=(a.RawComponents[1,0]*b.RawComponents[0,0])+(a.RawComponents[1,1]*b.RawComponents[1,0])+(a.RawComponents[1,2]*b.RawComponents[2,0])+(a.RawComponents[1,3]*b.RawComponents[3,0]);
 result.RawComponents[1,1]:=(a.RawComponents[1,0]*b.RawComponents[0,1])+(a.RawComponents[1,1]*b.RawComponents[1,1])+(a.RawComponents[1,2]*b.RawComponents[2,1])+(a.RawComponents[1,3]*b.RawComponents[3,1]);
 result.RawComponents[1,2]:=(a.RawComponents[1,0]*b.RawComponents[0,2])+(a.RawComponents[1,1]*b.RawComponents[1,2])+(a.RawComponents[1,2]*b.RawComponents[2,2])+(a.RawComponents[1,3]*b.RawComponents[3,2]);
 result.RawComponents[1,3]:=(a.RawComponents[1,0]*b.RawComponents[0,3])+(a.RawComponents[1,1]*b.RawComponents[1,3])+(a.RawComponents[1,2]*b.RawComponents[2,3])+(a.RawComponents[1,3]*b.RawComponents[3,3]);
 result.RawComponents[2,0]:=(a.RawComponents[2,0]*b.RawComponents[0,0])+(a.RawComponents[2,1]*b.RawComponents[1,0])+(a.RawComponents[2,2]*b.RawComponents[2,0])+(a.RawComponents[2,3]*b.RawComponents[3,0]);
 result.RawComponents[2,1]:=(a.RawComponents[2,0]*b.RawComponents[0,1])+(a.RawComponents[2,1]*b.RawComponents[1,1])+(a.RawComponents[2,2]*b.RawComponents[2,1])+(a.RawComponents[2,3]*b.RawComponents[3,1]);
 result.RawComponents[2,2]:=(a.RawComponents[2,0]*b.RawComponents[0,2])+(a.RawComponents[2,1]*b.RawComponents[1,2])+(a.RawComponents[2,2]*b.RawComponents[2,2])+(a.RawComponents[2,3]*b.RawComponents[3,2]);
 result.RawComponents[2,3]:=(a.RawComponents[2,0]*b.RawComponents[0,3])+(a.RawComponents[2,1]*b.RawComponents[1,3])+(a.RawComponents[2,2]*b.RawComponents[2,3])+(a.RawComponents[2,3]*b.RawComponents[3,3]);
 result.RawComponents[3,0]:=(a.RawComponents[3,0]*b.RawComponents[0,0])+(a.RawComponents[3,1]*b.RawComponents[1,0])+(a.RawComponents[3,2]*b.RawComponents[2,0])+(a.RawComponents[3,3]*b.RawComponents[3,0]);
 result.RawComponents[3,1]:=(a.RawComponents[3,0]*b.RawComponents[0,1])+(a.RawComponents[3,1]*b.RawComponents[1,1])+(a.RawComponents[3,2]*b.RawComponents[2,1])+(a.RawComponents[3,3]*b.RawComponents[3,1]);
 result.RawComponents[3,2]:=(a.RawComponents[3,0]*b.RawComponents[0,2])+(a.RawComponents[3,1]*b.RawComponents[1,2])+(a.RawComponents[3,2]*b.RawComponents[2,2])+(a.RawComponents[3,3]*b.RawComponents[3,2]);
 result.RawComponents[3,3]:=(a.RawComponents[3,0]*b.RawComponents[0,3])+(a.RawComponents[3,1]*b.RawComponents[1,3])+(a.RawComponents[3,2]*b.RawComponents[2,3])+(a.RawComponents[3,3]*b.RawComponents[3,3]);
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]*b;
 result.RawComponents[0,1]:=a.RawComponents[0,1]*b;
 result.RawComponents[0,2]:=a.RawComponents[0,2]*b;
 result.RawComponents[0,3]:=a.RawComponents[0,3]*b;
 result.RawComponents[1,0]:=a.RawComponents[1,0]*b;
 result.RawComponents[1,1]:=a.RawComponents[1,1]*b;
 result.RawComponents[1,2]:=a.RawComponents[1,2]*b;
 result.RawComponents[1,3]:=a.RawComponents[1,3]*b;
 result.RawComponents[2,0]:=a.RawComponents[2,0]*b;
 result.RawComponents[2,1]:=a.RawComponents[2,1]*b;
 result.RawComponents[2,2]:=a.RawComponents[2,2]*b;
 result.RawComponents[2,3]:=a.RawComponents[2,3]*b;
 result.RawComponents[3,0]:=a.RawComponents[3,0]*b;
 result.RawComponents[3,1]:=a.RawComponents[3,1]*b;
 result.RawComponents[3,2]:=a.RawComponents[3,2]*b;
 result.RawComponents[3,3]:=a.RawComponents[3,3]*b;
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a*b.RawComponents[0,0];
 result.RawComponents[0,1]:=a*b.RawComponents[0,1];
 result.RawComponents[0,2]:=a*b.RawComponents[0,2];
 result.RawComponents[0,3]:=a*b.RawComponents[0,3];
 result.RawComponents[1,0]:=a*b.RawComponents[1,0];
 result.RawComponents[1,1]:=a*b.RawComponents[1,1];
 result.RawComponents[1,2]:=a*b.RawComponents[1,2];
 result.RawComponents[1,3]:=a*b.RawComponents[1,3];
 result.RawComponents[2,0]:=a*b.RawComponents[2,0];
 result.RawComponents[2,1]:=a*b.RawComponents[2,1];
 result.RawComponents[2,2]:=a*b.RawComponents[2,2];
 result.RawComponents[2,3]:=a*b.RawComponents[2,3];
 result.RawComponents[3,0]:=a*b.RawComponents[3,0];
 result.RawComponents[3,1]:=a*b.RawComponents[3,1];
 result.RawComponents[3,2]:=a*b.RawComponents[3,2];
 result.RawComponents[3,3]:=a*b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXVector3):TFBXVector3;
begin
 result.x:=(a.RawComponents[0,0]*b.x)+(a.RawComponents[1,0]*b.y)+(a.RawComponents[2,0]*b.z)+a.RawComponents[3,0];
 result.y:=(a.RawComponents[0,1]*b.x)+(a.RawComponents[1,1]*b.y)+(a.RawComponents[2,1]*b.z)+a.RawComponents[3,1];
 result.z:=(a.RawComponents[0,2]*b.x)+(a.RawComponents[1,2]*b.y)+(a.RawComponents[2,2]*b.z)+a.RawComponents[3,2];
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXVector3;
begin
 result.x:=(a.x*b.RawComponents[0,0])+(a.y*b.RawComponents[0,1])+(a.z*b.RawComponents[0,2])+b.RawComponents[0,3];
 result.y:=(a.x*b.RawComponents[1,0])+(a.y*b.RawComponents[1,1])+(a.z*b.RawComponents[1,2])+b.RawComponents[1,3];
 result.z:=(a.x*b.RawComponents[2,0])+(a.y*b.RawComponents[2,1])+(a.z*b.RawComponents[2,2])+b.RawComponents[2,3];
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXVector4):TFBXVector4;
begin
 result.x:=(a.RawComponents[0,0]*b.x)+(a.RawComponents[1,0]*b.y)+(a.RawComponents[2,0]*b.z)+(a.RawComponents[3,0]*b.w);
 result.y:=(a.RawComponents[0,1]*b.x)+(a.RawComponents[1,1]*b.y)+(a.RawComponents[2,1]*b.z)+(a.RawComponents[3,1]*b.w);
 result.z:=(a.RawComponents[0,2]*b.x)+(a.RawComponents[1,2]*b.y)+(a.RawComponents[2,2]*b.z)+(a.RawComponents[3,2]*b.w);
 result.w:=(a.RawComponents[0,3]*b.x)+(a.RawComponents[1,3]*b.y)+(a.RawComponents[2,3]*b.z)+(a.RawComponents[3,3]*b.w);
end;

class operator TFBXMatrix4x4.Multiply({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXVector4;
begin
 result.x:=(a.x*b.RawComponents[0,0])+(a.y*b.RawComponents[0,1])+(a.z*b.RawComponents[0,2])+(a.w*b.RawComponents[0,3]);
 result.y:=(a.x*b.RawComponents[1,0])+(a.y*b.RawComponents[1,1])+(a.z*b.RawComponents[1,2])+(a.w*b.RawComponents[1,3]);
 result.z:=(a.x*b.RawComponents[2,0])+(a.y*b.RawComponents[2,1])+(a.z*b.RawComponents[2,2])+(a.w*b.RawComponents[2,3]);
 result.w:=(a.x*b.RawComponents[3,0])+(a.y*b.RawComponents[3,1])+(a.z*b.RawComponents[3,2])+(a.w*b.RawComponents[3,3]);
end;

class operator TFBXMatrix4x4.Divide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result:=a*b.Inverse;
end;

class operator TFBXMatrix4x4.Divide({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]/b;
 result.RawComponents[0,1]:=a.RawComponents[0,1]/b;
 result.RawComponents[0,2]:=a.RawComponents[0,2]/b;
 result.RawComponents[0,3]:=a.RawComponents[0,3]/b;
 result.RawComponents[1,0]:=a.RawComponents[1,0]/b;
 result.RawComponents[1,1]:=a.RawComponents[1,1]/b;
 result.RawComponents[1,2]:=a.RawComponents[1,2]/b;
 result.RawComponents[1,3]:=a.RawComponents[1,3]/b;
 result.RawComponents[2,0]:=a.RawComponents[2,0]/b;
 result.RawComponents[2,1]:=a.RawComponents[2,1]/b;
 result.RawComponents[2,2]:=a.RawComponents[2,2]/b;
 result.RawComponents[2,3]:=a.RawComponents[2,3]/b;
 result.RawComponents[3,0]:=a.RawComponents[3,0]/b;
 result.RawComponents[3,1]:=a.RawComponents[3,1]/b;
 result.RawComponents[3,2]:=a.RawComponents[3,2]/b;
 result.RawComponents[3,3]:=a.RawComponents[3,3]/b;
end;

class operator TFBXMatrix4x4.Divide({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a/b.RawComponents[0,0];
 result.RawComponents[0,1]:=a/b.RawComponents[0,1];
 result.RawComponents[0,2]:=a/b.RawComponents[0,2];
 result.RawComponents[0,3]:=a/b.RawComponents[0,3];
 result.RawComponents[1,0]:=a/b.RawComponents[1,0];
 result.RawComponents[1,1]:=a/b.RawComponents[1,1];
 result.RawComponents[1,2]:=a/b.RawComponents[1,2];
 result.RawComponents[1,3]:=a/b.RawComponents[1,3];
 result.RawComponents[2,0]:=a/b.RawComponents[2,0];
 result.RawComponents[2,1]:=a/b.RawComponents[2,1];
 result.RawComponents[2,2]:=a/b.RawComponents[2,2];
 result.RawComponents[2,3]:=a/b.RawComponents[2,3];
 result.RawComponents[3,0]:=a/b.RawComponents[3,0];
 result.RawComponents[3,1]:=a/b.RawComponents[3,1];
 result.RawComponents[3,2]:=a/b.RawComponents[3,2];
 result.RawComponents[3,3]:=a/b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.IntDivide({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result:=a*b.Inverse;
end;

class operator TFBXMatrix4x4.IntDivide({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a.RawComponents[0,0]/b;
 result.RawComponents[0,1]:=a.RawComponents[0,1]/b;
 result.RawComponents[0,2]:=a.RawComponents[0,2]/b;
 result.RawComponents[0,3]:=a.RawComponents[0,3]/b;
 result.RawComponents[1,0]:=a.RawComponents[1,0]/b;
 result.RawComponents[1,1]:=a.RawComponents[1,1]/b;
 result.RawComponents[1,2]:=a.RawComponents[1,2]/b;
 result.RawComponents[1,3]:=a.RawComponents[1,3]/b;
 result.RawComponents[2,0]:=a.RawComponents[2,0]/b;
 result.RawComponents[2,1]:=a.RawComponents[2,1]/b;
 result.RawComponents[2,2]:=a.RawComponents[2,2]/b;
 result.RawComponents[2,3]:=a.RawComponents[2,3]/b;
 result.RawComponents[3,0]:=a.RawComponents[3,0]/b;
 result.RawComponents[3,1]:=a.RawComponents[3,1]/b;
 result.RawComponents[3,2]:=a.RawComponents[3,2]/b;
 result.RawComponents[3,3]:=a.RawComponents[3,3]/b;
end;

class operator TFBXMatrix4x4.IntDivide({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=a/b.RawComponents[0,0];
 result.RawComponents[0,1]:=a/b.RawComponents[0,1];
 result.RawComponents[0,2]:=a/b.RawComponents[0,2];
 result.RawComponents[0,3]:=a/b.RawComponents[0,3];
 result.RawComponents[1,0]:=a/b.RawComponents[1,0];
 result.RawComponents[1,1]:=a/b.RawComponents[1,1];
 result.RawComponents[1,2]:=a/b.RawComponents[1,2];
 result.RawComponents[1,3]:=a/b.RawComponents[1,3];
 result.RawComponents[2,0]:=a/b.RawComponents[2,0];
 result.RawComponents[2,1]:=a/b.RawComponents[2,1];
 result.RawComponents[2,2]:=a/b.RawComponents[2,2];
 result.RawComponents[2,3]:=a/b.RawComponents[2,3];
 result.RawComponents[3,0]:=a/b.RawComponents[3,0];
 result.RawComponents[3,1]:=a/b.RawComponents[3,1];
 result.RawComponents[3,2]:=a/b.RawComponents[3,2];
 result.RawComponents[3,3]:=a/b.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Modulus({$ifdef fpc}constref{$else}const{$endif} a,b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=Modulus(a.RawComponents[0,0],b.RawComponents[0,0]);
 result.RawComponents[0,1]:=Modulus(a.RawComponents[0,1],b.RawComponents[0,1]);
 result.RawComponents[0,2]:=Modulus(a.RawComponents[0,2],b.RawComponents[0,2]);
 result.RawComponents[0,3]:=Modulus(a.RawComponents[0,3],b.RawComponents[0,3]);
 result.RawComponents[1,0]:=Modulus(a.RawComponents[1,0],b.RawComponents[1,0]);
 result.RawComponents[1,1]:=Modulus(a.RawComponents[1,1],b.RawComponents[1,1]);
 result.RawComponents[1,2]:=Modulus(a.RawComponents[1,2],b.RawComponents[1,2]);
 result.RawComponents[1,3]:=Modulus(a.RawComponents[1,3],b.RawComponents[1,3]);
 result.RawComponents[2,0]:=Modulus(a.RawComponents[2,0],b.RawComponents[2,0]);
 result.RawComponents[2,1]:=Modulus(a.RawComponents[2,1],b.RawComponents[2,1]);
 result.RawComponents[2,2]:=Modulus(a.RawComponents[2,2],b.RawComponents[2,2]);
 result.RawComponents[2,3]:=Modulus(a.RawComponents[2,3],b.RawComponents[2,3]);
 result.RawComponents[3,0]:=Modulus(a.RawComponents[3,0],b.RawComponents[3,0]);
 result.RawComponents[3,1]:=Modulus(a.RawComponents[3,1],b.RawComponents[3,1]);
 result.RawComponents[3,2]:=Modulus(a.RawComponents[3,2],b.RawComponents[3,2]);
 result.RawComponents[3,3]:=Modulus(a.RawComponents[3,3],b.RawComponents[3,3]);
end;

class operator TFBXMatrix4x4.Modulus({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4;{$ifdef fpc}constref{$else}const{$endif} b:TFBXScalar):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=Modulus(a.RawComponents[0,0],b);
 result.RawComponents[0,1]:=Modulus(a.RawComponents[0,1],b);
 result.RawComponents[0,2]:=Modulus(a.RawComponents[0,2],b);
 result.RawComponents[0,3]:=Modulus(a.RawComponents[0,3],b);
 result.RawComponents[1,0]:=Modulus(a.RawComponents[1,0],b);
 result.RawComponents[1,1]:=Modulus(a.RawComponents[1,1],b);
 result.RawComponents[1,2]:=Modulus(a.RawComponents[1,2],b);
 result.RawComponents[1,3]:=Modulus(a.RawComponents[1,3],b);
 result.RawComponents[2,0]:=Modulus(a.RawComponents[2,0],b);
 result.RawComponents[2,1]:=Modulus(a.RawComponents[2,1],b);
 result.RawComponents[2,2]:=Modulus(a.RawComponents[2,2],b);
 result.RawComponents[2,3]:=Modulus(a.RawComponents[2,3],b);
 result.RawComponents[3,0]:=Modulus(a.RawComponents[3,0],b);
 result.RawComponents[3,1]:=Modulus(a.RawComponents[3,1],b);
 result.RawComponents[3,2]:=Modulus(a.RawComponents[3,2],b);
 result.RawComponents[3,3]:=Modulus(a.RawComponents[3,3],b);
end;

class operator TFBXMatrix4x4.Modulus({$ifdef fpc}constref{$else}const{$endif} a:TFBXScalar;{$ifdef fpc}constref{$else}const{$endif} b:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=Modulus(a,b.RawComponents[0,0]);
 result.RawComponents[0,1]:=Modulus(a,b.RawComponents[0,1]);
 result.RawComponents[0,2]:=Modulus(a,b.RawComponents[0,2]);
 result.RawComponents[0,3]:=Modulus(a,b.RawComponents[0,3]);
 result.RawComponents[1,0]:=Modulus(a,b.RawComponents[1,0]);
 result.RawComponents[1,1]:=Modulus(a,b.RawComponents[1,1]);
 result.RawComponents[1,2]:=Modulus(a,b.RawComponents[1,2]);
 result.RawComponents[1,3]:=Modulus(a,b.RawComponents[1,3]);
 result.RawComponents[2,0]:=Modulus(a,b.RawComponents[2,0]);
 result.RawComponents[2,1]:=Modulus(a,b.RawComponents[2,1]);
 result.RawComponents[2,2]:=Modulus(a,b.RawComponents[2,2]);
 result.RawComponents[2,3]:=Modulus(a,b.RawComponents[2,3]);
 result.RawComponents[3,0]:=Modulus(a,b.RawComponents[3,0]);
 result.RawComponents[3,1]:=Modulus(a,b.RawComponents[3,1]);
 result.RawComponents[3,2]:=Modulus(a,b.RawComponents[3,2]);
 result.RawComponents[3,3]:=Modulus(a,b.RawComponents[3,3]);
end;

class operator TFBXMatrix4x4.Negative({$ifdef fpc}constref{$else}const{$endif} a:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=-a.RawComponents[0,0];
 result.RawComponents[0,1]:=-a.RawComponents[0,1];
 result.RawComponents[0,2]:=-a.RawComponents[0,2];
 result.RawComponents[0,3]:=-a.RawComponents[0,3];
 result.RawComponents[1,0]:=-a.RawComponents[1,0];
 result.RawComponents[1,1]:=-a.RawComponents[1,1];
 result.RawComponents[1,2]:=-a.RawComponents[1,2];
 result.RawComponents[1,3]:=-a.RawComponents[1,3];
 result.RawComponents[2,0]:=-a.RawComponents[2,0];
 result.RawComponents[2,1]:=-a.RawComponents[2,1];
 result.RawComponents[2,2]:=-a.RawComponents[2,2];
 result.RawComponents[2,3]:=-a.RawComponents[2,3];
 result.RawComponents[3,0]:=-a.RawComponents[3,0];
 result.RawComponents[3,1]:=-a.RawComponents[3,1];
 result.RawComponents[3,2]:=-a.RawComponents[3,2];
 result.RawComponents[3,3]:=-a.RawComponents[3,3];
end;

class operator TFBXMatrix4x4.Positive(const a:TFBXMatrix4x4):TFBXMatrix4x4;
begin
 result:=a;
end;

function TFBXMatrix4x4.GetComponent(const pIndexA,pIndexB:Int32):TFBXScalar;
begin
 result:=RawComponents[pIndexA,pIndexB];
end;

procedure TFBXMatrix4x4.SetComponent(const pIndexA,pIndexB:Int32;const pValue:TFBXScalar);
begin
 RawComponents[pIndexA,pIndexB]:=pValue;
end;

function TFBXMatrix4x4.GetColumn(const pIndex:Int32):TFBXVector4;
begin
 result.x:=RawComponents[pIndex,0];
 result.y:=RawComponents[pIndex,1];
 result.z:=RawComponents[pIndex,2];
 result.w:=RawComponents[pIndex,3];
end;

procedure TFBXMatrix4x4.SetColumn(const pIndex:Int32;const pValue:TFBXVector4);
begin
 RawComponents[pIndex,0]:=pValue.x;
 RawComponents[pIndex,1]:=pValue.y;
 RawComponents[pIndex,2]:=pValue.z;
 RawComponents[pIndex,3]:=pValue.w;
end;

function TFBXMatrix4x4.GetRow(const pIndex:Int32):TFBXVector4;
begin
 result.x:=RawComponents[0,pIndex];
 result.y:=RawComponents[1,pIndex];
 result.z:=RawComponents[2,pIndex];
 result.w:=RawComponents[3,pIndex];
end;

procedure TFBXMatrix4x4.SetRow(const pIndex:Int32;const pValue:TFBXVector4);
begin
 RawComponents[0,pIndex]:=pValue.x;
 RawComponents[1,pIndex]:=pValue.y;
 RawComponents[2,pIndex]:=pValue.z;
 RawComponents[3,pIndex]:=pValue.w;
end;

function TFBXMatrix4x4.Determinant:TFBXScalar;
begin
 result:=(RawComponents[0,0]*((((RawComponents[1,1]*RawComponents[2,2]*RawComponents[3,3])-(RawComponents[1,1]*RawComponents[2,3]*RawComponents[3,2]))-(RawComponents[2,1]*RawComponents[1,2]*RawComponents[3,3])+(RawComponents[2,1]*RawComponents[1,3]*RawComponents[3,2])+(RawComponents[3,1]*RawComponents[1,2]*RawComponents[2,3]))-(RawComponents[3,1]*RawComponents[1,3]*RawComponents[2,2])))+
         (RawComponents[0,1]*(((((-(RawComponents[1,0]*RawComponents[2,2]*RawComponents[3,3]))+(RawComponents[1,0]*RawComponents[2,3]*RawComponents[3,2])+(RawComponents[2,0]*RawComponents[1,2]*RawComponents[3,3]))-(RawComponents[2,0]*RawComponents[1,3]*RawComponents[3,2]))-(RawComponents[3,0]*RawComponents[1,2]*RawComponents[2,3]))+(RawComponents[3,0]*RawComponents[1,3]*RawComponents[2,2])))+
         (RawComponents[0,2]*(((((RawComponents[1,0]*RawComponents[2,1]*RawComponents[3,3])-(RawComponents[1,0]*RawComponents[2,3]*RawComponents[3,1]))-(RawComponents[2,0]*RawComponents[1,1]*RawComponents[3,3]))+(RawComponents[2,0]*RawComponents[1,3]*RawComponents[3,1])+(RawComponents[3,0]*RawComponents[1,1]*RawComponents[2,3]))-(RawComponents[3,0]*RawComponents[1,3]*RawComponents[2,1])))+
         (RawComponents[0,3]*(((((-(RawComponents[1,0]*RawComponents[2,1]*RawComponents[3,2]))+(RawComponents[1,0]*RawComponents[2,2]*RawComponents[3,1])+(RawComponents[2,0]*RawComponents[1,1]*RawComponents[3,2]))-(RawComponents[2,0]*RawComponents[1,2]*RawComponents[3,1]))-(RawComponents[3,0]*RawComponents[1,1]*RawComponents[2,2]))+(RawComponents[3,0]*RawComponents[1,2]*RawComponents[2,1])));
end;

function TFBXMatrix4x4.SimpleInverse:TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=RawComponents[0,0];
 result.RawComponents[0,1]:=RawComponents[1,0];
 result.RawComponents[0,2]:=RawComponents[2,0];
 result.RawComponents[0,3]:=RawComponents[0,3];
 result.RawComponents[1,0]:=RawComponents[0,1];
 result.RawComponents[1,1]:=RawComponents[1,1];
 result.RawComponents[1,2]:=RawComponents[2,1];
 result.RawComponents[1,3]:=RawComponents[1,3];
 result.RawComponents[2,0]:=RawComponents[0,2];
 result.RawComponents[2,1]:=RawComponents[1,2];
 result.RawComponents[2,2]:=RawComponents[2,2];
 result.RawComponents[2,3]:=RawComponents[2,3];
 result.RawComponents[3,0]:=-PFBXVector3(pointer(@RawComponents[3,0]))^.Dot(TFBXVector3.Create(RawComponents[0,0],RawComponents[0,1],RawComponents[0,2]));
 result.RawComponents[3,1]:=-PFBXVector3(pointer(@RawComponents[3,0]))^.Dot(TFBXVector3.Create(RawComponents[1,0],RawComponents[1,1],RawComponents[1,2]));
 result.RawComponents[3,2]:=-PFBXVector3(pointer(@RawComponents[3,0]))^.Dot(TFBXVector3.Create(RawComponents[2,0],RawComponents[2,1],RawComponents[2,2]));
 result.RawComponents[3,3]:=RawComponents[3,3];
end;

function TFBXMatrix4x4.Inverse:TFBXMatrix4x4;
var t0,t4,t8,t12,d:TFBXScalar;
begin
 t0:=(((RawComponents[1,1]*RawComponents[2,2]*RawComponents[3,3])-(RawComponents[1,1]*RawComponents[2,3]*RawComponents[3,2]))-(RawComponents[2,1]*RawComponents[1,2]*RawComponents[3,3])+(RawComponents[2,1]*RawComponents[1,3]*RawComponents[3,2])+(RawComponents[3,1]*RawComponents[1,2]*RawComponents[2,3]))-(RawComponents[3,1]*RawComponents[1,3]*RawComponents[2,2]);
 t4:=((((-(RawComponents[1,0]*RawComponents[2,2]*RawComponents[3,3]))+(RawComponents[1,0]*RawComponents[2,3]*RawComponents[3,2])+(RawComponents[2,0]*RawComponents[1,2]*RawComponents[3,3]))-(RawComponents[2,0]*RawComponents[1,3]*RawComponents[3,2]))-(RawComponents[3,0]*RawComponents[1,2]*RawComponents[2,3]))+(RawComponents[3,0]*RawComponents[1,3]*RawComponents[2,2]);
 t8:=((((RawComponents[1,0]*RawComponents[2,1]*RawComponents[3,3])-(RawComponents[1,0]*RawComponents[2,3]*RawComponents[3,1]))-(RawComponents[2,0]*RawComponents[1,1]*RawComponents[3,3]))+(RawComponents[2,0]*RawComponents[1,3]*RawComponents[3,1])+(RawComponents[3,0]*RawComponents[1,1]*RawComponents[2,3]))-(RawComponents[3,0]*RawComponents[1,3]*RawComponents[2,1]);
 t12:=((((-(RawComponents[1,0]*RawComponents[2,1]*RawComponents[3,2]))+(RawComponents[1,0]*RawComponents[2,2]*RawComponents[3,1])+(RawComponents[2,0]*RawComponents[1,1]*RawComponents[3,2]))-(RawComponents[2,0]*RawComponents[1,2]*RawComponents[3,1]))-(RawComponents[3,0]*RawComponents[1,1]*RawComponents[2,2]))+(RawComponents[3,0]*RawComponents[1,2]*RawComponents[2,1]);
 d:=(RawComponents[0,0]*t0)+(RawComponents[0,1]*t4)+(RawComponents[0,2]*t8)+(RawComponents[0,3]*t12);
 if d<>0.0 then begin
  d:=1.0/d;
  result.RawComponents[0,0]:=t0*d;
  result.RawComponents[0,1]:=(((((-(RawComponents[0,1]*RawComponents[2,2]*RawComponents[3,3]))+(RawComponents[0,1]*RawComponents[2,3]*RawComponents[3,2])+(RawComponents[2,1]*RawComponents[0,2]*RawComponents[3,3]))-(RawComponents[2,1]*RawComponents[0,3]*RawComponents[3,2]))-(RawComponents[3,1]*RawComponents[0,2]*RawComponents[2,3]))+(RawComponents[3,1]*RawComponents[0,3]*RawComponents[2,2]))*d;
  result.RawComponents[0,2]:=(((((RawComponents[0,1]*RawComponents[1,2]*RawComponents[3,3])-(RawComponents[0,1]*RawComponents[1,3]*RawComponents[3,2]))-(RawComponents[1,1]*RawComponents[0,2]*RawComponents[3,3]))+(RawComponents[1,1]*RawComponents[0,3]*RawComponents[3,2])+(RawComponents[3,1]*RawComponents[0,2]*RawComponents[1,3]))-(RawComponents[3,1]*RawComponents[0,3]*RawComponents[1,2]))*d;
  result.RawComponents[0,3]:=(((((-(RawComponents[0,1]*RawComponents[1,2]*RawComponents[2,3]))+(RawComponents[0,1]*RawComponents[1,3]*RawComponents[2,2])+(RawComponents[1,1]*RawComponents[0,2]*RawComponents[2,3]))-(RawComponents[1,1]*RawComponents[0,3]*RawComponents[2,2]))-(RawComponents[2,1]*RawComponents[0,2]*RawComponents[1,3]))+(RawComponents[2,1]*RawComponents[0,3]*RawComponents[1,2]))*d;
  result.RawComponents[1,0]:=t4*d;
  result.RawComponents[1,1]:=((((RawComponents[0,0]*RawComponents[2,2]*RawComponents[3,3])-(RawComponents[0,0]*RawComponents[2,3]*RawComponents[3,2]))-(RawComponents[2,0]*RawComponents[0,2]*RawComponents[3,3])+(RawComponents[2,0]*RawComponents[0,3]*RawComponents[3,2])+(RawComponents[3,0]*RawComponents[0,2]*RawComponents[2,3]))-(RawComponents[3,0]*RawComponents[0,3]*RawComponents[2,2]))*d;
  result.RawComponents[1,2]:=(((((-(RawComponents[0,0]*RawComponents[1,2]*RawComponents[3,3]))+(RawComponents[0,0]*RawComponents[1,3]*RawComponents[3,2])+(RawComponents[1,0]*RawComponents[0,2]*RawComponents[3,3]))-(RawComponents[1,0]*RawComponents[0,3]*RawComponents[3,2]))-(RawComponents[3,0]*RawComponents[0,2]*RawComponents[1,3]))+(RawComponents[3,0]*RawComponents[0,3]*RawComponents[1,2]))*d;
  result.RawComponents[1,3]:=(((((RawComponents[0,0]*RawComponents[1,2]*RawComponents[2,3])-(RawComponents[0,0]*RawComponents[1,3]*RawComponents[2,2]))-(RawComponents[1,0]*RawComponents[0,2]*RawComponents[2,3]))+(RawComponents[1,0]*RawComponents[0,3]*RawComponents[2,2])+(RawComponents[2,0]*RawComponents[0,2]*RawComponents[1,3]))-(RawComponents[2,0]*RawComponents[0,3]*RawComponents[1,2]))*d;
  result.RawComponents[2,0]:=t8*d;
  result.RawComponents[2,1]:=(((((-(RawComponents[0,0]*RawComponents[2,1]*RawComponents[3,3]))+(RawComponents[0,0]*RawComponents[2,3]*RawComponents[3,1])+(RawComponents[2,0]*RawComponents[0,1]*RawComponents[3,3]))-(RawComponents[2,0]*RawComponents[0,3]*RawComponents[3,1]))-(RawComponents[3,0]*RawComponents[0,1]*RawComponents[2,3]))+(RawComponents[3,0]*RawComponents[0,3]*RawComponents[2,1]))*d;
  result.RawComponents[2,2]:=(((((RawComponents[0,0]*RawComponents[1,1]*RawComponents[3,3])-(RawComponents[0,0]*RawComponents[1,3]*RawComponents[3,1]))-(RawComponents[1,0]*RawComponents[0,1]*RawComponents[3,3]))+(RawComponents[1,0]*RawComponents[0,3]*RawComponents[3,1])+(RawComponents[3,0]*RawComponents[0,1]*RawComponents[1,3]))-(RawComponents[3,0]*RawComponents[0,3]*RawComponents[1,1]))*d;
  result.RawComponents[2,3]:=(((((-(RawComponents[0,0]*RawComponents[1,1]*RawComponents[2,3]))+(RawComponents[0,0]*RawComponents[1,3]*RawComponents[2,1])+(RawComponents[1,0]*RawComponents[0,1]*RawComponents[2,3]))-(RawComponents[1,0]*RawComponents[0,3]*RawComponents[2,1]))-(RawComponents[2,0]*RawComponents[0,1]*RawComponents[1,3]))+(RawComponents[2,0]*RawComponents[0,3]*RawComponents[1,1]))*d;
  result.RawComponents[3,0]:=t12*d;
  result.RawComponents[3,1]:=(((((RawComponents[0,0]*RawComponents[2,1]*RawComponents[3,2])-(RawComponents[0,0]*RawComponents[2,2]*RawComponents[3,1]))-(RawComponents[2,0]*RawComponents[0,1]*RawComponents[3,2]))+(RawComponents[2,0]*RawComponents[0,2]*RawComponents[3,1])+(RawComponents[3,0]*RawComponents[0,1]*RawComponents[2,2]))-(RawComponents[3,0]*RawComponents[0,2]*RawComponents[2,1]))*d;
  result.RawComponents[3,2]:=(((((-(RawComponents[0,0]*RawComponents[1,1]*RawComponents[3,2]))+(RawComponents[0,0]*RawComponents[1,2]*RawComponents[3,1])+(RawComponents[1,0]*RawComponents[0,1]*RawComponents[3,2]))-(RawComponents[1,0]*RawComponents[0,2]*RawComponents[3,1]))-(RawComponents[3,0]*RawComponents[0,1]*RawComponents[1,2]))+(RawComponents[3,0]*RawComponents[0,2]*RawComponents[1,1]))*d;
  result.RawComponents[3,3]:=(((((RawComponents[0,0]*RawComponents[1,1]*RawComponents[2,2])-(RawComponents[0,0]*RawComponents[1,2]*RawComponents[2,1]))-(RawComponents[1,0]*RawComponents[0,1]*RawComponents[2,2]))+(RawComponents[1,0]*RawComponents[0,2]*RawComponents[2,1])+(RawComponents[2,0]*RawComponents[0,1]*RawComponents[1,2]))-(RawComponents[2,0]*RawComponents[0,2]*RawComponents[1,1]))*d;
 end;
end;

function TFBXMatrix4x4.Transpose:TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=RawComponents[0,0];
 result.RawComponents[0,1]:=RawComponents[1,0];
 result.RawComponents[0,2]:=RawComponents[2,0];
 result.RawComponents[0,3]:=RawComponents[3,0];
 result.RawComponents[1,0]:=RawComponents[0,1];
 result.RawComponents[1,1]:=RawComponents[1,1];
 result.RawComponents[1,2]:=RawComponents[2,1];
 result.RawComponents[1,3]:=RawComponents[3,1];
 result.RawComponents[2,0]:=RawComponents[0,2];
 result.RawComponents[2,1]:=RawComponents[1,2];
 result.RawComponents[2,2]:=RawComponents[2,2];
 result.RawComponents[2,3]:=RawComponents[3,2];
 result.RawComponents[3,0]:=RawComponents[0,3];
 result.RawComponents[3,1]:=RawComponents[1,3];
 result.RawComponents[3,2]:=RawComponents[2,3];
 result.RawComponents[3,3]:=RawComponents[3,3];
end;

function TFBXMatrix4x4.EulerAngles:TFBXVector3;
const EPSILON=1e-8;
var v0,v1:TFBXVector3;
begin
 if abs((-1.0)-RawComponents[0,2])<EPSILON then begin
  result.x:=0.0;
  result.y:=pi*0.5;
  result.z:=ArcTan2(RawComponents[1,0],RawComponents[2,0]);
 end else if abs(1.0-RawComponents[0,2])<EPSILON then begin
  result.x:=0.0;
  result.y:=-(pi*0.5);
  result.z:=ArcTan2(-RawComponents[1,0],-RawComponents[2,0]);
 end else begin
  v0.x:=-ArcSin(RawComponents[0,2]);
  v1.x:=pi-v0.x;
  v0.y:=ArcTan2(RawComponents[1,2]/cos(v0.x),RawComponents[2,2]/cos(v0.x));
  v1.y:=ArcTan2(RawComponents[1,2]/cos(v1.x),RawComponents[2,2]/cos(v1.x));
  v0.z:=ArcTan2(RawComponents[0,1]/cos(v0.x),RawComponents[0,0]/cos(v0.x));
  v1.z:=ArcTan2(RawComponents[0,1]/cos(v1.x),RawComponents[0,0]/cos(v1.x));
  if v0.SquaredLength<v1.SquaredLength then begin
   result:=v0;
  end else begin
   result:=v1;
  end;
 end;
end;

function TFBXMatrix4x4.Normalize:TFBXMatrix4x4;
begin
 result.Right.Vector3:=Right.Vector3.Normalize;
 result.RawComponents[0,3]:=RawComponents[0,3];
 result.Up.Vector3:=Up.Vector3.Normalize;
 result.RawComponents[1,3]:=RawComponents[1,3];
 result.Forwards.Vector3:=Forwards.Vector3.Normalize;
 result.RawComponents[2,3]:=RawComponents[2,3];
 result.Translation:=Translation;
end;

function TFBXMatrix4x4.OrthoNormalize:TFBXMatrix4x4;
var Backup:TFBXVector3;
begin
 Backup.x:=RawComponents[0,3];
 Backup.y:=RawComponents[1,3];
 Backup.z:=RawComponents[2,3];
 Normal.Vector3:=Normal.Vector3.Normalize;
 Tangent.Vector3:=(Tangent.Vector3-(Normal.Vector3*Tangent.Vector3.Dot(Normal.Vector3))).Normalize;
 Bitangent.Vector3:=Normal.Vector3.Cross(Tangent.Vector3).Normalize;
 Bitangent.Vector3:=Bitangent.Vector3-(Normal.Vector3*Bitangent.Vector3.Dot(Normal.Vector3));
 Bitangent.Vector3:=(Bitangent.Vector3-(Tangent.Vector3*Bitangent.Vector3.Dot(Tangent.Vector3))).Normalize;
 Tangent.Vector3:=Bitangent.Vector3.Cross(Normal.Vector3).Normalize;
 Normal.Vector3:=Tangent.Vector3.Cross(Bitangent.Vector3).Normalize;
 result.RawComponents:=RawComponents;
 result.RawComponents[0,3]:=Backup.x;
 result.RawComponents[1,3]:=Backup.y;
 result.RawComponents[2,3]:=Backup.z;
end;

function TFBXMatrix4x4.RobustOrthoNormalize(const Tolerance:TFBXScalar=1e-3):TFBXMatrix4x4;
var Backup,Bisector,Axis:TFBXVector3;
begin
 Backup.x:=RawComponents[0,3];
 Backup.y:=RawComponents[1,3];
 Backup.z:=RawComponents[2,3];
 begin
  if Normal.Vector3.Length<Tolerance then begin
   // Degenerate case, compute new normal
   Normal.Vector3:=Tangent.Vector3.Cross(Bitangent.Vector3);
   if Normal.Vector3.Length<Tolerance then begin
    Tangent.Vector3:=TFBXVector3.Create(1.0,0.0,0.0);
    Bitangent.Vector3:=TFBXVector3.Create(0.0,1.0,0.0);
    Normal.Vector3:=TFBXVector3.Create(0.0,0.0,1.0);
    RawComponents[0,3]:=Backup.x;
    RawComponents[1,3]:=Backup.y;
    RawComponents[2,3]:=Backup.z;
    exit;
   end;
  end;
  Normal.Vector3:=Normal.Vector3.Normalize;
 end;
 begin
  // Project tangent and bitangent onto the normal orthogonal plane
  Tangent.Vector3:=Tangent.Vector3-(Normal.Vector3*Tangent.Vector3.Dot(Normal.Vector3));
  Bitangent.Vector3:=Bitangent.Vector3-(Normal.Vector3*Bitangent.Vector3.Dot(Normal.Vector3));
 end;
 begin
  // Check for several degenerate cases
  if Tangent.Vector3.Length<Tolerance then begin
   if Bitangent.Vector3.Length<Tolerance then begin
    Tangent.Vector3:=Normal.Vector3.Normalize;
    if (Tangent.x<=Tangent.y) and (Tangent.x<=Tangent.z) then begin
     Tangent.Vector3:=TFBXVector3.Create(1.0,0.0,0.0);
    end else if (Tangent.y<=Tangent.x) and (Tangent.y<=Tangent.z) then begin
     Tangent.Vector3:=TFBXVector3.Create(0.0,1.0,0.0);
    end else begin
     Tangent.Vector3:=TFBXVector3.Create(0.0,0.0,1.0);
    end;
    Tangent.Vector3:=Tangent.Vector3-(Normal.Vector3*Tangent.Vector3.Dot(Normal.Vector3));
    Bitangent.Vector3:=Normal.Vector3.Cross(Tangent.Vector3).Normalize;
   end else begin
    Tangent.Vector3:=Bitangent.Vector3.Cross(Normal.Vector3).Normalize;
   end;
  end else begin
   Tangent.Vector3:=Tangent.Vector3.Normalize;
   if Bitangent.Vector3.Length<Tolerance then begin
    Bitangent.Vector3:=Normal.Vector3.Cross(Tangent.Vector3).Normalize;
   end else begin
    Bitangent.Vector3:=Bitangent.Vector3.Normalize;
    Bisector:=Tangent.Vector3+Bitangent.Vector3;
    if Bisector.Length<Tolerance then begin
     Bisector:=Tangent.Vector3;
    end else begin
     Bisector:=Bisector.Normalize;
    end;
    Axis:=Bisector.Cross(Normal.Vector3).Normalize;
    if Axis.Dot(Tangent.Vector3)>0.0 then begin
     Tangent.Vector3:=(Bisector+Axis).Normalize;
     Bitangent.Vector3:=(Bisector-Axis).Normalize;
    end else begin
     Tangent.Vector3:=(Bisector-Axis).Normalize;
     Bitangent.Vector3:=(Bisector+Axis).Normalize;
    end;
   end;
  end;
 end;
 Bitangent.Vector3:=Normal.Vector3.Cross(Tangent.Vector3).Normalize;
 Tangent.Vector3:=Bitangent.Vector3.Cross(Normal.Vector3).Normalize;
 Normal.Vector3:=Tangent.Vector3.Cross(Bitangent.Vector3).Normalize;
 result.RawComponents:=RawComponents;
 result.RawComponents[0,3]:=Backup.x;
 result.RawComponents[1,3]:=Backup.y;
 result.RawComponents[2,3]:=Backup.z;
end;

function TFBXMatrix4x4.ToRotation:TFBXMatrix4x4;
begin
 result.RawComponents[0,0]:=RawComponents[0,0];
 result.RawComponents[0,1]:=RawComponents[0,1];
 result.RawComponents[0,2]:=RawComponents[0,2];
 result.RawComponents[0,3]:=0.0;
 result.RawComponents[1,0]:=RawComponents[1,0];
 result.RawComponents[1,1]:=RawComponents[1,1];
 result.RawComponents[1,2]:=RawComponents[1,2];
 result.RawComponents[1,3]:=0.0;
 result.RawComponents[2,0]:=RawComponents[2,0];
 result.RawComponents[2,1]:=RawComponents[2,1];
 result.RawComponents[2,2]:=RawComponents[2,2];
 result.RawComponents[2,3]:=0.0;
 result.RawComponents[3,0]:=0.0;
 result.RawComponents[3,1]:=0.0;
 result.RawComponents[3,2]:=0.0;
 result.RawComponents[3,3]:=1.0;
end;

function TFBXMatrix4x4.SimpleLerp(const b:TFBXMatrix4x4;const t:TFBXScalar):TFBXMatrix4x4;
begin
 if t<=0.0 then begin
  result:=self;
 end else if t>=1.0 then begin
  result:=b;
 end else begin
  result:=(self*(1.0-t))+(b*t);
 end;
end;

function TFBXMatrix4x4.MulInverse({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
var d:TFBXScalar;
begin
 d:=((RawComponents[0,0]*((RawComponents[1,1]*RawComponents[2,2])-(RawComponents[2,1]*RawComponents[1,2])))-
     (RawComponents[0,1]*((RawComponents[1,0]*RawComponents[2,2])-(RawComponents[2,0]*RawComponents[1,2]))))+
     (RawComponents[0,2]*((RawComponents[1,0]*RawComponents[2,1])-(RawComponents[2,0]*RawComponents[1,1])));
 if d<>0.0 then begin
  d:=1.0/d;
 end;
 result.x:=((a.x*((RawComponents[1,1]*RawComponents[2,2])-(RawComponents[1,2]*RawComponents[2,1])))+(a.y*((RawComponents[1,2]*RawComponents[2,0])-(RawComponents[1,0]*RawComponents[2,2])))+(a.z*((RawComponents[1,0]*RawComponents[2,1])-(RawComponents[1,1]*RawComponents[2,0]))))*d;
 result.y:=((RawComponents[0,0]*((a.y*RawComponents[2,2])-(a.z*RawComponents[2,1])))+(RawComponents[0,1]*((a.z*RawComponents[2,0])-(a.x*RawComponents[2,2])))+(RawComponents[0,2]*((a.x*RawComponents[2,1])-(a.y*RawComponents[2,0]))))*d;
 result.z:=((RawComponents[0,0]*((RawComponents[1,1]*a.z)-(RawComponents[1,2]*a.y)))+(RawComponents[0,1]*((RawComponents[1,2]*a.x)-(RawComponents[1,0]*a.z)))+(RawComponents[0,2]*((RawComponents[1,0]*a.y)-(RawComponents[1,1]*a.x))))*d;
end;

function TFBXMatrix4x4.MulInverse({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
var d:TFBXScalar;
begin
 d:=((RawComponents[0,0]*((RawComponents[1,1]*RawComponents[2,2])-(RawComponents[2,1]*RawComponents[1,2])))-
     (RawComponents[0,1]*((RawComponents[1,0]*RawComponents[2,2])-(RawComponents[2,0]*RawComponents[1,2]))))+
     (RawComponents[0,2]*((RawComponents[1,0]*RawComponents[2,1])-(RawComponents[2,0]*RawComponents[1,1])));
 if d<>0.0 then begin
  d:=1.0/d;
 end;
 result.x:=((a.x*((RawComponents[1,1]*RawComponents[2,2])-(RawComponents[1,2]*RawComponents[2,1])))+(a.y*((RawComponents[1,2]*RawComponents[2,0])-(RawComponents[1,0]*RawComponents[2,2])))+(a.z*((RawComponents[1,0]*RawComponents[2,1])-(RawComponents[1,1]*RawComponents[2,0]))))*d;
 result.y:=((RawComponents[0,0]*((a.y*RawComponents[2,2])-(a.z*RawComponents[2,1])))+(RawComponents[0,1]*((a.z*RawComponents[2,0])-(a.x*RawComponents[2,2])))+(RawComponents[0,2]*((a.x*RawComponents[2,1])-(a.y*RawComponents[2,0]))))*d;
 result.z:=((RawComponents[0,0]*((RawComponents[1,1]*a.z)-(RawComponents[1,2]*a.y)))+(RawComponents[0,1]*((RawComponents[1,2]*a.x)-(RawComponents[1,0]*a.z)))+(RawComponents[0,2]*((RawComponents[1,0]*a.y)-(RawComponents[1,1]*a.x))))*d;
 result.w:=a.w;
end;

function TFBXMatrix4x4.MulInverted({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
var p:TFBXVector3;
begin
 p.x:=a.x-RawComponents[3,0];
 p.y:=a.y-RawComponents[3,1];
 p.z:=a.z-RawComponents[3,2];
 result.x:=(RawComponents[0,0]*p.x)+(RawComponents[0,1]*p.y)+(RawComponents[0,2]*p.z);
 result.y:=(RawComponents[1,0]*p.x)+(RawComponents[1,1]*p.y)+(RawComponents[1,2]*p.z);
 result.z:=(RawComponents[2,0]*p.x)+(RawComponents[2,1]*p.y)+(RawComponents[2,2]*p.z);
end;

function TFBXMatrix4x4.MulInverted({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
var p:TFBXVector3;
begin
 p.x:=a.x-RawComponents[3,0];
 p.y:=a.y-RawComponents[3,1];
 p.z:=a.z-RawComponents[3,2];
 result.x:=(RawComponents[0,0]*p.x)+(RawComponents[0,1]*p.y)+(RawComponents[0,2]*p.z);
 result.y:=(RawComponents[1,0]*p.x)+(RawComponents[1,1]*p.y)+(RawComponents[1,2]*p.z);
 result.z:=(RawComponents[2,0]*p.x)+(RawComponents[2,1]*p.y)+(RawComponents[2,2]*p.z);
 result.w:=a.w;
end;

function TFBXMatrix4x4.MulBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
begin
 result.x:=(RawComponents[0,0]*a.x)+(RawComponents[1,0]*a.y)+(RawComponents[2,0]*a.z);
 result.y:=(RawComponents[0,1]*a.x)+(RawComponents[1,1]*a.y)+(RawComponents[2,1]*a.z);
 result.z:=(RawComponents[0,2]*a.x)+(RawComponents[1,2]*a.y)+(RawComponents[2,2]*a.z);
end;

function TFBXMatrix4x4.MulBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result.x:=(RawComponents[0,0]*a.x)+(RawComponents[1,0]*a.y)+(RawComponents[2,0]*a.z);
 result.y:=(RawComponents[0,1]*a.x)+(RawComponents[1,1]*a.y)+(RawComponents[2,1]*a.z);
 result.z:=(RawComponents[0,2]*a.x)+(RawComponents[1,2]*a.y)+(RawComponents[2,2]*a.z);
 result.w:=a.w;
end;

function TFBXMatrix4x4.MulTransposedBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
begin
 result.x:=(RawComponents[0,0]*a.x)+(RawComponents[0,1]*a.y)+(RawComponents[0,2]*a.z);
 result.y:=(RawComponents[1,0]*a.x)+(RawComponents[1,1]*a.y)+(RawComponents[1,2]*a.z);
 result.z:=(RawComponents[2,0]*a.x)+(RawComponents[2,1]*a.y)+(RawComponents[2,2]*a.z);
end;

function TFBXMatrix4x4.MulTransposedBasis({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result.x:=(RawComponents[0,0]*a.x)+(RawComponents[0,1]*a.y)+(RawComponents[0,2]*a.z);
 result.y:=(RawComponents[1,0]*a.x)+(RawComponents[1,1]*a.y)+(RawComponents[1,2]*a.z);
 result.z:=(RawComponents[2,0]*a.x)+(RawComponents[2,1]*a.y)+(RawComponents[2,2]*a.z);
 result.w:=a.w;
end;

function TFBXMatrix4x4.MulHomogen({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector3):TFBXVector3;
var Temporary:TFBXVector4;
begin
 Temporary:=self*TFBXVector4.Create(a,1.0);
 Temporary:=Temporary/Temporary.w;
 result:=Temporary.Vector3;
end;

function TFBXMatrix4x4.MulHomogen({$ifdef fpc}constref{$else}const{$endif} a:TFBXVector4):TFBXVector4;
begin
 result:=self*a;
 result:=result/result.w;
end;

constructor TFBXTimeSpan.Create(const pFrom:TFBXTimeSpan);
begin
 self:=pFrom;
end;

constructor TFBXTimeSpan.Create(const pStartTime,pEndTime:TFBXTime);
begin
 StartTime:=pStartTime;
 EndTime:=pEndTime;
end;

constructor TFBXTimeSpan.Create(const pArray:array of TFBXTime);
begin
 StartTime:=pArray[0];
 EndTime:=pArray[1];
end;

function TFBXTimeSpan.GetComponent(const pIndex:Int32):TFBXTime;
begin
 result:=RawComponents[pIndex];
end;

procedure TFBXTimeSpan.SetComponent(const pIndex:Int32;const pValue:TFBXTime);
begin
 RawComponents[pIndex]:=pValue;
end;

function TFBXTimeSpan.ToString:TFBXString;
begin
 result:=TFBXString('{{StartTime:{'+IntToStr(StartTime)+'} EndTime:{'+IntToStr(EndTime)+'}}}');
end;

constructor TFBXVector2Property.Create;
begin
 inherited Create;
 fVector.x:=0.0;
 fVector.y:=0.0;
end;

constructor TFBXVector2Property.Create(const pFrom:TFBXVector2);
begin
 inherited Create;
 fVector:=pFrom;
end;

constructor TFBXVector2Property.Create(const pX,pY:Double);
begin
 inherited Create;
 fVector.x:=pX;
 fVector.y:=pY;
end;

constructor TFBXVector2Property.Create(const pArray:array of Double);
begin
 inherited Create;
 fVector.x:=pArray[0];
 fVector.y:=pArray[1];
end;

destructor TFBXVector2Property.Destroy;
begin
 inherited Destroy;
end;

function TFBXVector2Property.GetX:Double;
begin
 result:=fVector.x;
end;

procedure TFBXVector2Property.SetX(const pValue:Double);
begin
 fVector.x:=pValue;
end;

function TFBXVector2Property.GetY:Double;
begin
 result:=fVector.y;
end;

procedure TFBXVector2Property.SetY(const pValue:Double);
begin
 fVector.y:=pValue;
end;

constructor TFBXVector3Property.Create;
begin
 inherited Create;
 fVector.x:=0.0;
 fVector.y:=0.0;
 fVector.z:=0.0;
end;

constructor TFBXVector3Property.Create(const pFrom:TFBXVector3);
begin
 inherited Create;
 fVector:=pFrom;
end;

constructor TFBXVector3Property.Create(const pX,pY,pZ:Double);
begin
 inherited Create;
 fVector.x:=pX;
 fVector.y:=pY;
 fVector.z:=pZ;
end;

constructor TFBXVector3Property.Create(const pArray:array of Double);
begin
 inherited Create;
 fVector.x:=pArray[0];
 fVector.y:=pArray[1];
 fVector.z:=pArray[2];
end;

destructor TFBXVector3Property.Destroy;
begin
 inherited Destroy;
end;

function TFBXVector3Property.GetX:Double;
begin
 result:=fVector.x;
end;

procedure TFBXVector3Property.SetX(const pValue:Double);
begin
 fVector.x:=pValue;
end;

function TFBXVector3Property.GetY:Double;
begin
 result:=fVector.y;
end;

procedure TFBXVector3Property.SetY(const pValue:Double);
begin
 fVector.y:=pValue;
end;

function TFBXVector3Property.GetZ:Double;
begin
 result:=fVector.z;
end;

procedure TFBXVector3Property.SetZ(const pValue:Double);
begin
 fVector.z:=pValue;
end;

constructor TFBXVector4Property.Create;
begin
 inherited Create;
 fVector.x:=0.0;
 fVector.y:=0.0;
 fVector.z:=0.0;
 fVector.w:=0.0;
end;

constructor TFBXVector4Property.Create(const pFrom:TFBXVector4);
begin
 inherited Create;
 fVector:=pFrom;
end;

constructor TFBXVector4Property.Create(const pX,pY,pZ,pW:Double);
begin
 inherited Create;
 fVector.x:=pX;
 fVector.y:=pY;
 fVector.z:=pZ;
 fVector.w:=pw;
end;

constructor TFBXVector4Property.Create(const pArray:array of Double);
begin
 inherited Create;
 fVector.x:=pArray[0];
 fVector.y:=pArray[1];
 fVector.z:=pArray[2];
 fVector.w:=pArray[3];
end;

destructor TFBXVector4Property.Destroy;
begin
 inherited Destroy;
end;

function TFBXVector4Property.GetX:Double;
begin
 result:=fVector.x;
end;

procedure TFBXVector4Property.SetX(const pValue:Double);
begin
 fVector.x:=pValue;
end;

function TFBXVector4Property.GetY:Double;
begin
 result:=fVector.y;
end;

procedure TFBXVector4Property.SetY(const pValue:Double);
begin
 fVector.y:=pValue;
end;

function TFBXVector4Property.GetZ:Double;
begin
 result:=fVector.z;
end;

procedure TFBXVector4Property.SetZ(const pValue:Double);
begin
 fVector.z:=pValue;
end;

function TFBXVector4Property.GetW:Double;
begin
 result:=fVector.w;
end;

procedure TFBXVector4Property.SetW(const pValue:Double);
begin
 fVector.w:=pValue;
end;

constructor TFBXColorProperty.Create;
begin
 inherited Create;
 fColor.Red:=0.0;
 fColor.Green:=0.0;
 fColor.Blue:=0.0;
 fColor.Alpha:=0.0;
end;

constructor TFBXColorProperty.Create(const pFrom:TFBXColor);
begin
 inherited Create;
 fColor:=pFrom;
end;

constructor TFBXColorProperty.Create(const pRed,pGreen,pBlue:Double;const pAlpha:Double=1.0);
begin
 inherited Create;
 fColor.Red:=pRed;
 fColor.Green:=pGreen;
 fColor.Blue:=pBlue;
 fColor.Alpha:=pAlpha;
end;

constructor TFBXColorProperty.Create(const pArray:array of Double);
begin
 inherited Create;
 fColor.Red:=pArray[0];
 fColor.Green:=pArray[1];
 fColor.Blue:=pArray[2];
 fColor.Alpha:=pArray[3];
end;

destructor TFBXColorProperty.Destroy;
begin
 inherited Destroy;
end;

function TFBXColorProperty.GetRed:Double;
begin
 result:=fColor.Red;
end;

procedure TFBXColorProperty.SetRed(const pValue:Double);
begin
 fColor.Red:=pValue;
end;

function TFBXColorProperty.GetGreen:Double;
begin
 result:=fColor.Green;
end;

procedure TFBXColorProperty.SetGreen(const pValue:Double);
begin
 fColor.Green:=pValue;
end;

function TFBXColorProperty.GetBlue:Double;
begin
 result:=fColor.Blue;
end;

procedure TFBXColorProperty.SetBlue(const pValue:Double);
begin
 fColor.Blue:=pValue;
end;

function TFBXColorProperty.GetAlpha:Double;
begin
 result:=fColor.Alpha;
end;

procedure TFBXColorProperty.SetAlpha(const pValue:Double);
begin
 fColor.Alpha:=pValue;
end;

function StreamReadInt8(const pStream:TStream):Int8;
begin
 result:=0;
 pStream.ReadBuffer(result,SizeOf(Int8));
end;

function StreamReadInt16(const pStream:TStream):Int16;
type TBytes=array[0..1] of UInt8;
var Bytes:TBytes;
    Temporary:UInt16;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt16(Bytes[0]) shl 0) or (UInt16(Bytes[1]) shl 8);
 result:=Int16(pointer(@Temporary)^);
end;

function StreamReadInt32(const pStream:TStream):Int32;
type TBytes=array[0..3] of UInt8;
var Bytes:TBytes;
    Temporary:UInt32;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt32(Bytes[0]) shl 0) or (UInt32(Bytes[1]) shl 8) or (UInt32(Bytes[2]) shl 16) or (UInt32(Bytes[3]) shl 24);
 result:=Int32(pointer(@Temporary)^);
end;

function StreamReadInt64(const pStream:TStream):Int64;
type TBytes=array[0..7] of UInt8;
var Bytes:TBytes;
    Temporary:UInt64;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt64(Bytes[0]) shl 0) or (UInt64(Bytes[1]) shl 8) or (UInt64(Bytes[2]) shl 16) or (UInt64(Bytes[3]) shl 24) or (UInt64(Bytes[4]) shl 32) or (UInt64(Bytes[5]) shl 40) or (UInt64(Bytes[6]) shl 48) or (UInt64(Bytes[7]) shl 56);
 result:=Int64(pointer(@Temporary)^);
end;

function StreamReadUInt8(const pStream:TStream):UInt8;
begin
 result:=0;
 pStream.ReadBuffer(result,SizeOf(UInt8));
end;

function StreamReadUInt16(const pStream:TStream):UInt16;
type TBytes=array[0..1] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt16(Bytes[0]) shl 0) or (UInt16(Bytes[1]) shl 8);
end;

function StreamReadUInt32(const pStream:TStream):UInt32;
type TBytes=array[0..3] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt32(Bytes[0]) shl 0) or (UInt32(Bytes[1]) shl 8) or (UInt32(Bytes[2]) shl 16) or (UInt32(Bytes[3]) shl 24);
end;

function StreamReadUInt64(const pStream:TStream):UInt64;
type TBytes=array[0..7] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 pStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt64(Bytes[0]) shl 0) or (UInt64(Bytes[1]) shl 8) or (UInt64(Bytes[2]) shl 16) or (UInt64(Bytes[3]) shl 24) or (UInt64(Bytes[4]) shl 32) or (UInt64(Bytes[5]) shl 40) or (UInt64(Bytes[6]) shl 48) or (UInt64(Bytes[7]) shl 56);
end;

function StreamReadFloat32(const pStream:TStream):single;
begin
 UInt32(pointer(@result)^):=StreamReadUInt32(pStream);
end;

function StreamReadFloat64(const pStream:TStream):double;
begin
 UInt64(pointer(@result)^):=StreamReadUInt64(pStream);
end;

constructor TFBXBaseObject.Create;
begin
 inherited Create;
end;

destructor TFBXBaseObject.Destroy;
begin
 inherited Destroy;
end;

constructor TFBXElement.Create(const pID:TFBXString);
begin
 inherited Create;
 fID:=pID;
 fChildren:=TFBXElementList.Create(true);
 fChildrenNameMap:=TFBXElementNameMap.Create;
 fProperties:=TFBXElementPropertyList.Create(true);
end;

destructor TFBXElement.Destroy;
begin
 fProperties.Free;
 fChildrenNameMap.Free;
 fChildren.Free;
 inherited Destroy;
end;

function TFBXElement.AddChildren(const pElement:TFBXElement):Int32;
begin
 result:=fChildren.Add(pElement);
 if not fChildrenNameMap.ContainsKey(pElement.fID) then begin
  fChildrenNameMap.Add(pElement.fID,pElement);
 end;
end;

function TFBXElement.AddProperty(const pProperty:TFBXElementProperty):Int32;
begin
 result:=fProperties.Add(pProperty);
end;

constructor TFBXElementProperty.Create;
begin
 inherited Create;
end;

destructor TFBXElementProperty.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementProperty.GetArrayLength:TFBXSizeInt;
begin
 result:=1;
end;

function TFBXElementProperty.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=0;
end;

function TFBXElementProperty.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=TFBXString(String(GetVariantValue));
end;

function TFBXElementProperty.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=GetVariantValue;
end;

function TFBXElementProperty.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=GetVariantValue;
end;

function TFBXElementProperty.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=GetVariantValue;
end;

constructor TFBXElementPropertyBoolean.Create(const pValue:Boolean);
begin
 inherited Create;
 fValue:=pValue;
end;

destructor TFBXElementPropertyBoolean.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementPropertyBoolean.GetArrayLength:TFBXSizeInt;
begin
 result:=1;
end;

function TFBXElementPropertyBoolean.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyBoolean.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 if fValue then begin
  result:='true';
 end else begin
  result:='false';
 end;
end;

function TFBXElementPropertyBoolean.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyBoolean.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=ord(fValue) and 1;
end;

function TFBXElementPropertyBoolean.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=ord(fValue) and 1;
end;

constructor TFBXElementPropertyInteger.Create(const pValue:Int64);
begin
 inherited Create;
 fValue:=pValue;
end;

destructor TFBXElementPropertyInteger.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementPropertyInteger.GetArrayLength:TFBXSizeInt;
begin
 result:=1;
end;

function TFBXElementPropertyInteger.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyInteger.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=TFBXString(IntToStr(fValue));
end;

function TFBXElementPropertyInteger.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue<>0;
end;

function TFBXElementPropertyInteger.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyInteger.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

constructor TFBXElementPropertyFloat.Create(const pValue:Double);
begin
 inherited Create;
 fValue:=pValue;
end;

destructor TFBXElementPropertyFloat.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementPropertyFloat.GetArrayLength:TFBXSizeInt;
begin
 result:=1;
end;

function TFBXElementPropertyFloat.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyFloat.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
var f:Double;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 f:=fValue;
 result:=ConvertDoubleToString(f,omStandard,-1);
end;

function TFBXElementPropertyFloat.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue<>0.0;
end;

function TFBXElementPropertyFloat.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=trunc(fValue);
end;

function TFBXElementPropertyFloat.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

constructor TFBXElementPropertyBytes.Create(const pValue:TFBXBytes);
begin
 inherited Create;
 fValue:=pValue;
end;

destructor TFBXElementPropertyBytes.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementPropertyBytes.GetArrayLength:TFBXSizeInt;
begin
 result:=length(fValue);
end;

function TFBXElementPropertyBytes.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if (pIndex<0) or (pIndex>=length(fValue)) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(length(fValue)));
 end;
 result:=fValue[pIndex];
end;

function TFBXElementPropertyBytes.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if (pIndex<0) or (pIndex>=length(fValue)) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(length(fValue)));
 end;
 result:=TFBXString(IntToStr(fValue[pIndex]));
end;

function TFBXElementPropertyBytes.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if (pIndex<0) or (pIndex>=length(fValue)) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(length(fValue)));
 end;
 result:=fValue[pIndex]<>0.0;
end;

function TFBXElementPropertyBytes.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if (pIndex<0) or (pIndex>=length(fValue)) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(length(fValue)));
 end;
 result:=fValue[pIndex];
end;

function TFBXElementPropertyBytes.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if (pIndex<0) or (pIndex>=length(fValue)) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(length(fValue)));
 end;
 result:=fValue[pIndex];
end;

constructor TFBXElementPropertyString.Create(const pValue:TFBXString);
begin
 inherited Create;
 fValue:=pValue;
end;

destructor TFBXElementPropertyString.Destroy;
begin
 inherited Destroy;
end;

function TFBXElementPropertyString.GetArrayLength:TFBXSizeInt;
begin
 result:=1;
end;

function TFBXElementPropertyString.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyString.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=fValue;
end;

function TFBXElementPropertyString.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=(fValue<>'false') and (fValue<>'0');
end;

function TFBXElementPropertyString.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 result:=StrToIntDef(String(fValue),0);
end;

function TFBXElementPropertyString.GetFloat(const pIndex:TFBXSizeInt=0):Double;
var OK:TPasDblStrUtilsBoolean;
begin
 if pIndex<>0 then begin
  raise ERangeError.Create('Array index must be zero');
 end;
 OK:=false;
 result:=ConvertStringToDouble(TPasDblStrUtilsString(fValue),rmNearest,@OK,-1);
 if not OK then begin
  result:=0.0;
 end;
end;

constructor TFBXElementPropertyArray.Create(const pData:pointer;const pDataCount:TFBXSizeInt;const pDataType:TFBXElementPropertyArrayDataType);
begin
 inherited Create;

 fData:=nil;
 fDataType:=pDataType;
 fDataTypeSize:=DataTypeSizes[fDataType];
 fDataCount:=pDataCount;

 if fDataCount>0 then begin
  SetLength(fData,fDataCount*fDataTypeSize);
  Move(pData^,fData[0],fDataCount*fDataTypeSize);
 end;

end;

constructor TFBXElementPropertyArray.CreateFrom(const pStream:TStream;const pDataType:TFBXElementPropertyArrayDataType);
const UNCOMPRESSED=0;
      ZLIB_COMPRESSED=1;
var DataStream:TStream;
    Encoding,CompressedLength,Index,OutSize:UInt32;
    OutData:pointer;
begin

 inherited Create;

 fData:=nil;
 fDataType:=pDataType;
 fDataTypeSize:=DataTypeSizes[fDataType];

 fDataCount:=StreamReadUInt32(pStream);
 Encoding:=StreamReadUInt32(pStream);
 CompressedLength:=StreamReadUInt32(pStream);

 DataStream:=TMemoryStream.Create;
 try
  DataStream.CopyFrom(pStream,CompressedLength);
  DataStream.Seek(0,soBeginning);

  case Encoding of
   UNCOMPRESSED:begin
   end;
   ZLIB_COMPRESSED:begin
    OutData:=nil;
    OutSize:=0;
    try
     if DoInflate(TMemoryStream(DataStream).Memory,TMemoryStream(DataStream).Size,OutData,OutSize,true) then begin
{     DataStream.Free;
      DataStream:=TMemoryStream.Create;}
      TMemoryStream(DataStream).Clear;
      TMemoryStream(DataStream).Seek(0,soBeginning);
      TMemoryStream(DataStream).Write(OutData^,OutSize);
      TMemoryStream(DataStream).Seek(0,soBeginning);
     end else begin
      raise EFBXBinaryParser.Create('Corrupt binary FBX file');
     end;
    finally
     if assigned(OutData) then begin
      FreeMem(OutData);
      OutData:=nil;
     end;
    end;
   end;
  end;

  if Int64(fDataCount*fDataTypeSize)<>DataStream.Size then begin
   raise EFBXBinaryParser.Create('Corrupt binary FBX file');
  end;

  SetLength(fData,fDataCount*fDataTypeSize);

  for Index:=1 to fDataCount do begin
   case fDataType of
    fepadtBoolean:begin
     Boolean(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadUInt8(DataStream)<>0;
    end;
    fepadtInt8:begin
     Int8(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadInt8(DataStream);
    end;
    fepadtInt16:begin
     Int16(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadInt16(DataStream);
    end;
    fepadtInt32:begin
     Int32(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadInt32(DataStream);
    end;
    fepadtInt64:begin
     Int64(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadInt64(DataStream);
    end;
    fepadtFloat32:begin
     Single(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadFloat32(DataStream);
    end;
    fepadtFloat64:begin
     Double(pointer(@fData[TFBXSizeInt(Index-1)*fDataTypeSize])^):=StreamReadFloat64(DataStream);
    end;
   end;
  end;

 finally
  DataStream.Free;
 end;

end;

destructor TFBXElementPropertyArray.Destroy;
begin
 fData:=nil;
 inherited Destroy;
end;

function TFBXElementPropertyArray.GetArrayLength:TFBXSizeInt;
begin
 result:=fDataCount;
end;

function TFBXElementPropertyArray.GetVariantValue(const pIndex:TFBXSizeInt=0):Variant;
begin
 if (pIndex<0) or (pIndex>=fDataCount) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(fDataCount));
 end;
 case fDataType of
  fepadtBoolean:begin
   result:=Boolean(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt8:begin
   result:=Int8(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt16:begin
   result:=Int16(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt32:begin
   result:=Int32(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt64:begin
   result:=Int64(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtFloat32:begin
   result:=Single(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtFloat64:begin
   result:=Double(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  else begin
   Assert(false,'Unsupported data type');
   result:=0.0;
  end;
 end;
end;

function TFBXElementPropertyArray.GetString(const pIndex:TFBXSizeInt=0):TFBXString;
begin
 if (pIndex<0) or (pIndex>=fDataCount) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(fDataCount));
 end;
 case fDataType of
  fepadtBoolean:begin
   if Boolean(pointer(@fData[pIndex*fDataTypeSize])^) then begin
    result:='true';
   end else begin
    result:='false';
   end;
  end;
  fepadtInt8:begin
   result:=TFBXString(IntToStr(Int8(pointer(@fData[pIndex*fDataTypeSize])^)));
  end;
  fepadtInt16:begin
   result:=TFBXString(IntToStr(Int16(pointer(@fData[pIndex*fDataTypeSize])^)));
  end;
  fepadtInt32:begin
   result:=TFBXString(IntToStr(Int32(pointer(@fData[pIndex*fDataTypeSize])^)));
  end;
  fepadtInt64:begin
   result:=TFBXString(IntToStr(Int64(pointer(@fData[pIndex*fDataTypeSize])^)));
  end;
  fepadtFloat32:begin
   result:=TFBXString(ConvertDoubleToString(Single(pointer(@fData[pIndex*fDataTypeSize])^),omStandard,-1));
  end;
  fepadtFloat64:begin
   result:=TFBXString(ConvertDoubleToString(Double(pointer(@fData[pIndex*fDataTypeSize])^),omStandard,-1));
  end;
  else begin
   Assert(false,'Unsupported data type');
   result:='';
  end;
 end;
end;

function TFBXElementPropertyArray.GetBoolean(const pIndex:TFBXSizeInt=0):Boolean;
begin
 if (pIndex<0) or (pIndex>=fDataCount) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(fDataCount));
 end;
 case fDataType of
  fepadtBoolean:begin
   result:=Boolean(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt8:begin
   result:=Int8(pointer(@fData[pIndex*fDataTypeSize])^)<>0;
  end;
  fepadtInt16:begin
   result:=Int16(pointer(@fData[pIndex*fDataTypeSize])^)<>0;
  end;
  fepadtInt32:begin
   result:=Int32(pointer(@fData[pIndex*fDataTypeSize])^)<>0;
  end;
  fepadtInt64:begin
   result:=Int64(pointer(@fData[pIndex*fDataTypeSize])^)<>0;
  end;
  fepadtFloat32:begin
   result:=Single(pointer(@fData[pIndex*fDataTypeSize])^)<>0.0;
  end;
  fepadtFloat64:begin
   result:=Double(pointer(@fData[pIndex*fDataTypeSize])^)<>0.0;
  end;
  else begin
   Assert(false,'Unsupported data type');
   result:=false;
  end;
 end;
end;

function TFBXElementPropertyArray.GetInteger(const pIndex:TFBXSizeInt=0):Int64;
begin
 if (pIndex<0) or (pIndex>=fDataCount) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(fDataCount));
 end;
 case fDataType of
  fepadtBoolean:begin
   result:=ord(Boolean(pointer(@fData[pIndex*fDataTypeSize])^)) and 1;
  end;
  fepadtInt8:begin
   result:=Int8(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt16:begin
   result:=Int16(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt32:begin
   result:=Int32(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt64:begin
   result:=Int64(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtFloat32:begin
   result:=trunc(Single(pointer(@fData[pIndex*fDataTypeSize])^));
  end;
  fepadtFloat64:begin
   result:=trunc(Double(pointer(@fData[pIndex*fDataTypeSize])^));
  end;
  else begin
   Assert(false,'Unsupported data type');
   result:=0;
  end;
 end;
end;

function TFBXElementPropertyArray.GetFloat(const pIndex:TFBXSizeInt=0):Double;
begin
 if (pIndex<0) or (pIndex>=fDataCount) then begin
  raise ERangeError.Create('Array index must be greater than or equal zero and less than '+IntToStr(fDataCount));
 end;
 case fDataType of
  fepadtBoolean:begin
   result:=ord(Boolean(pointer(@fData[pIndex*fDataTypeSize])^)) and 1;
  end;
  fepadtInt8:begin
   result:=Int8(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt16:begin
   result:=Int16(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt32:begin
   result:=Int32(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtInt64:begin
   result:=Int64(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtFloat32:begin
   result:=Single(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  fepadtFloat64:begin
   result:=Double(pointer(@fData[pIndex*fDataTypeSize])^);
  end;
  else begin
   Assert(false,'Unsupported data type');
   result:=0.0;
  end;
 end;
end;

constructor TFBXParser.Create(const pStream:TStream);
begin
 inherited Create;
 fStream:=pStream;
end;

destructor TFBXParser.Destroy;
begin
 inherited Destroy;
end;

function TFBXParser.SceneName:TFBXString;
begin
 result:='';
end;

function TFBXParser.GetName(const pRawName:TFBXString):TFBXString;
begin
 result:=pRawName;
end;

function TFBXParser.ConstructName(const pNames:array of TFBXString):TFBXString;
begin
 result:='';
end;

function TFBXParser.NextElement:TFBXElement;
begin
 result:=nil;
end;

function TFBXParser.Parse:TFBXElement;
begin
 result:=nil;
end;

constructor TFBXASCIIParser.Create(const pStream:TStream);
var FileSignature:TFileSignature;
begin
 inherited Create(pStream);
 FileSignature[0]:=#0;
 fStream.ReadBuffer(FileSignature,SizeOf(TFileSignature));
 if FileSignature<>FILE_SIGNATURE then begin
  raise EFBXBinaryParser.Create('Not a valid ASCII FBX file');
 end;
 fStream.Seek(-SizeOf(TFileSignature),soCurrent);
end;

destructor TFBXASCIIParser.Destroy;
begin
 inherited Destroy;
end;

function TFBXASCIIParser.SceneName:TFBXString;
begin
 result:='Model::Scene';
end;

function TFBXASCIIParser.GetName(const pRawName:TFBXString):TFBXString;
var Position:TFBXSizeInt;
begin
 for Position:=length(pRawName)-1 downto 1 do begin
  if (pRawName[Position]=':') and (pRawName[Position+1]=':') then begin
   result:=Copy(pRawName,Position+2,Length(pRawName)-(Position+1));
   exit;
  end;
 end;
 result:=pRawName;
end;

function TFBXASCIIParser.ConstructName(const pNames:array of TFBXString):TFBXString;
var Index:Int32;
begin
 result:='';
 for Index:=0 to length(pNames)-1 do begin
  if Index>0 then begin
   result:=result+'::';
  end;
  result:=result+pNames[Index];
 end;
end;

function TFBXASCIIParser.SkipWhiteSpace:boolean;
var CurrentChar:AnsiChar;
begin
 result:=false;
 CurrentChar:=#0;
 while fStream.Position<fStream.Size do begin
  fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
  case CurrentChar of
   #8,#9,#32:begin
    // Do nothing
   end;
   #10:begin
    result:=true;
    if fStream.Position<fStream.Size then begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     if CurrentChar<>#13 then begin
      fStream.Seek(-SizeOf(AnsiChar),soCurrent);
     end;
    end;
   end;
   #13:begin
    result:=true;
    if fStream.Position<fStream.Size then begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     if CurrentChar<>#10 then begin
      fStream.Seek(-SizeOf(AnsiChar),soCurrent);
     end;
    end;
   end;
   ';':begin
    // Skip comment line
    result:=true;
    while fStream.Position<fStream.Size do begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     case CurrentChar of
      #10:begin
       if fStream.Position<fStream.Size then begin
        fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
        if CurrentChar<>#13 then begin
         fStream.Seek(-SizeOf(AnsiChar),soCurrent);
        end;
       end;
       break;
      end;
      #13:begin
       if fStream.Position<fStream.Size then begin
        fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
        if CurrentChar<>#10 then begin
         fStream.Seek(-SizeOf(AnsiChar),soCurrent);
        end;
       end;
       break;
      end;
     end;
    end;
   end;
   else begin
    fStream.Seek(-SizeOf(AnsiChar),soCurrent);
    break;
   end;
  end;
 end;
end;

procedure TFBXASCIIParser.NextToken;
var Len:Int64;
    CurrentChar:AnsiChar;
    OK:TPasDblStrUtilsBoolean;
begin

 fCurrentToken.StringValue:='';
 fCurrentToken.Kind:=faptkNone;

 SkipWhiteSpace;

 if fStream.Position>=fStream.Size then begin

  fCurrentToken.Kind:=faptkEOF;

 end else begin

  CurrentChar:=#0;

  fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));

  case CurrentChar of
   '"':begin
    fCurrentToken.Kind:=faptkString;
    while fStream.Position<fStream.Size do begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     if CurrentChar='"' then begin
      break;
     end else begin
      fCurrentToken.StringValue:=fCurrentToken.StringValue+CurrentChar;
     end;
    end;
   end;
   ',':begin
    fCurrentToken.Kind:=faptkComma;
   end;
   '{':begin
    fCurrentToken.Kind:=faptkLeftBrace;
   end;
   '}':begin
    fCurrentToken.Kind:=faptkRightBrace;
   end;
   ':':begin
    fCurrentToken.Kind:=faptkColon;
   end;
   '*':begin
    fCurrentToken.Kind:=faptkStar;
   end;
   '0'..'9','.','+','-':begin
    Len:=1;
    while fStream.Position<fStream.Size do begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     if CurrentChar in ['0'..'9','.','+','-','a'..'f','A'..'F','x','X'] then begin
      inc(Len);
     end else begin
      fStream.Seek(-SizeOf(AnsiChar),soCurrent);
      break;
     end;
    end;
    fStream.Seek(-Len,soCurrent);
    SetLength(fCurrentToken.StringValue,Len*SizeOf(AnsiChar));
    fStream.ReadBuffer(fCurrentToken.StringValue[1],Len*SizeOf(AnsiChar));
    fCurrentToken.Int64Value:=StrToInt64Def(String(fCurrentToken.StringValue),-1);
    if TFBXString(IntToStr(fCurrentToken.Int64Value))=fCurrentToken.StringValue then begin
     fCurrentToken.Kind:=faptkInt64;
    end else begin
     fCurrentToken.Kind:=faptkFloat64;
     OK:=false;
     fCurrentToken.Float64Value:=ConvertStringToDouble(fCurrentToken.StringValue,rmNearest,@OK,-1);
     if not OK then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;
    end;
   end;
   'a'..'z','A'..'Z','|','_':begin
    fCurrentToken.Kind:=faptkAlphaNumberic;
    Len:=1;
    while fStream.Position<fStream.Size do begin
     fStream.ReadBuffer(CurrentChar,SizeOf(AnsiChar));
     if CurrentChar in AlphaNumberic then begin
      inc(Len);
     end else begin
      fStream.Seek(-SizeOf(AnsiChar),soCurrent);
      break;
     end;
    end;
    fStream.Seek(-Len,soCurrent);
    SetLength(fCurrentToken.StringValue,Len*SizeOf(AnsiChar));
    fStream.ReadBuffer(fCurrentToken.StringValue[1],Len*SizeOf(AnsiChar));
   end;
   else begin
    fCurrentToken.Kind:=faptkNone;
   end;
  end;

 end;

end;

function TFBXASCIIParser.NextElement:TFBXElement;
type TProperties=array of TFBXASCIIParserToken;
     //PNumberDataType=^TNumberDataType;
     TNumberDataType=
      (
       ndtInt64,
       ndtFloat64
      );
     PNumber=^TNumber;
     TNumber=record
      case DataType:TNumberDataType of
       ndtInt64:(
        Int64Value:Int64;
       );
       ndtFloat64:(
        Float64Value:Double;
       );
     end;
     TNumbers=array of TNumber;
var Current,Element:TFBXElement;
    Properties:TProperties;
    Numbers:TNumbers;
    Number:PNumber;
    Int64Array:TFBXInt64Array;
    Float64Array:TFBXFloat64Array;
    CountProperties,CountNumbers,Index:Int32;
    HasFloat:boolean;
    OldToken:TFBXASCIIParserToken;
    OldPosition:Int64;
 procedure FlushProperties;
 var Index:Int32;
     Token:PFBXASCIIParserToken;
 begin
  try
   for Index:=0 to CountProperties-1 do begin
    Token:=@Properties[Index];
    case Token^.Kind of
     faptkInt64:begin
      Current.AddProperty(TFBXElementPropertyInteger.Create(Token^.Int64Value));
     end;
     faptkFloat64:begin
      Current.AddProperty(TFBXElementPropertyFloat.Create(Token^.Float64Value));
     end;
     faptkString:begin
      Current.AddProperty(TFBXElementPropertyString.Create(Token^.StringValue));
     end;
     faptkAlphaNumberic:begin
      Current.AddProperty(TFBXElementPropertyString.Create(Token^.StringValue));
     end;
     else begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;
    end;
   end;
  finally
   CountProperties:=0;
  end;
 end;
begin

 if fCurrentToken.Kind<>faptkAlphaNumberic then begin
  raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
 end;

 result:=TFBXElement.Create(fCurrentToken.StringValue);

 Current:=result;

 NextToken;

 if fCurrentToken.Kind<>faptkColon then begin
  raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
 end;

 NextToken;

 Properties:=nil;
 CountProperties:=0;

 try

  repeat

   case fCurrentToken.Kind of
    faptkEOF:begin
     break;
    end;
    faptkLeftBrace:begin
     FlushProperties;
     NextToken;
     repeat
      case fCurrentToken.Kind of
       faptkEOF:begin
        raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
       end;
       faptkRightBrace:begin
        NextToken;
        break;
       end;
       else begin
        Element:=NextElement;
        if assigned(Element) then begin
         result.AddChildren(Element);
        end else begin
         raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
        end;
       end;
      end;
     until false;
     break;
    end;
    faptkStar:begin

     FlushProperties;

     NextToken;

     if not (fCurrentToken.Kind in [faptkInt64,faptkFloat64]) then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;

     NextToken;

     if fCurrentToken.Kind<>faptkLeftBrace then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;

     NextToken;

     if fCurrentToken.Kind<>faptkAlphaNumberic then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;

     NextToken;

     if fCurrentToken.Kind<>faptkColon then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;

     NextToken;

     Numbers:=nil;
     try

      HasFloat:=false;

      CountNumbers:=0;
      try

       repeat
        case fCurrentToken.Kind of
         faptkRightBrace,faptkEOF:begin
          break;
         end;
         faptkInt64:begin
          Index:=CountNumbers;
          inc(CountNumbers);
          if length(Numbers)<CountNumbers then begin
           SetLength(Numbers,CountNumbers*2);
          end;
          Number:=@Numbers[Index];
          Number^.DataType:=ndtInt64;
          Number^.Int64Value:=fCurrentToken.Int64Value;
          NextToken;
          if fCurrentToken.Kind=faptkComma then begin
           NextToken;
          end else begin
           break;
          end;
         end;
         faptkFloat64:begin
          Index:=CountNumbers;
          inc(CountNumbers);
          if length(Numbers)<CountNumbers then begin
           SetLength(Numbers,CountNumbers*2);
          end;
          Number:=@Numbers[Index];
          Number^.DataType:=ndtFloat64;
          Number^.Float64Value:=fCurrentToken.Float64Value;
          HasFloat:=true;
          NextToken;
          if fCurrentToken.Kind=faptkComma then begin
           NextToken;
          end else begin
           break;
          end;
         end;
         else begin
          break;
         end;
        end;
       until false;

      finally
       SetLength(Numbers,CountNumbers);
      end;

      if HasFloat then begin
       Float64Array:=nil;
       try
        SetLength(Float64Array,CountNumbers);
        for Index:=0 to CountNumbers-1 do begin
         Number:=@Numbers[Index];
         case Number^.DataType of
          ndtInt64:begin
           Float64Array[Index]:=Number^.Int64Value;
          end;
          ndtFloat64:begin
           Float64Array[Index]:=Number^.Float64Value;
          end;
          else begin
           raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
          end;
         end;
        end;
        Current.AddProperty(TFBXElementPropertyArray.Create(@Float64Array[0],length(Float64Array),fepadtFloat64));
       finally
        Float64Array:=nil;
       end;
      end else begin
       Int64Array:=nil;
       try
        SetLength(Int64Array,CountNumbers);
        for Index:=0 to CountNumbers-1 do begin
         Number:=@Numbers[Index];
         case Number^.DataType of
          ndtInt64:begin
           Int64Array[Index]:=Number^.Int64Value;
          end;
          ndtFloat64:begin
           Int64Array[Index]:=trunc(Number^.Float64Value);
          end;
          else begin
           raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
          end;
         end;
        end;
        Current.AddProperty(TFBXElementPropertyArray.Create(@Int64Array[0],length(Int64Array),fepadtInt64));
       finally
        Int64Array:=nil;
       end;
      end;

     finally
      Numbers:=nil;
     end;

     if fCurrentToken.Kind<>faptkRightBrace then begin
      raise EFBXASCIIParser.Create('Invalid or corrupt ASCII FBX stream');
     end;

     NextToken;

     break;

    end;
    faptkString,faptkInt64,faptkFloat64:begin
     Index:=CountProperties;
     inc(CountProperties);
     if length(Properties)<CountProperties then begin
      SetLength(Properties,CountProperties*2);
     end;
     Properties[Index]:=fCurrentToken;
     NextToken;
     case fCurrentToken.Kind of
      faptkComma:begin
       NextToken;
      end;
      faptkLeftBrace:begin
      end;
      faptkRightBrace,faptkEOF:begin
       break;
      end;
      else begin
       break;
      end;
     end;
    end;
    faptkAlphaNumberic:begin
     OldToken:=fCurrentToken;
     OldPosition:=fStream.Position;
     NextToken;
     if fCurrentToken.Kind=faptkColon then begin
      fStream.Seek(OldPosition,soBeginning);
      fCurrentToken:=OldToken;
      break;
     end;
     Index:=CountProperties;
     inc(CountProperties);
     if length(Properties)<CountProperties then begin
      SetLength(Properties,CountProperties*2);
     end;
     Properties[Index]:=OldToken;
     case fCurrentToken.Kind of
      faptkComma:begin
       NextToken;
      end;
      faptkLeftBrace:begin
      end;
      faptkRightBrace,faptkEOF:begin
       break;
      end;
      else begin
       break;
      end;
     end;
    end;
   end;
  until false;

  FlushProperties;

 finally
  Properties:=nil;
 end;

end;

function TFBXASCIIParser.Parse:TFBXElement;
var Element:TFBXElement;
begin
 result:=TFBXElement.Create('');
 if assigned(result) then begin
  try
   NextToken;
   while fCurrentToken.Kind<>faptkEOF do begin
    Element:=NextElement;
    if assigned(Element) then begin
     result.AddChildren(Element);
    end else begin
     break;
    end;
   end;
  except
   FreeAndNil(result);
   raise;
  end;
 end;
end;

constructor TFBXBinaryParser.Create(const pStream:TStream);
var FileSignature:TFileSignature;
begin
 inherited Create(pStream);
 FileSignature[0]:=#0;
 fStream.ReadBuffer(FileSignature,SizeOf(TFileSignature));
 if FileSignature<>FILE_SIGNATURE then begin
  raise EFBXBinaryParser.Create('Not a valid binary FBX file');
 end;
 ReadUInt16;
 fVersion:=ReadUInt32;
end;

destructor TFBXBinaryParser.Destroy;
begin
 inherited Destroy;
end;

function TFBXBinaryParser.SceneName:TFBXString;
begin
 result:='Scene'#0#1'Model';
end;

function TFBXBinaryParser.GetName(const pRawName:TFBXString):TFBXString;
var Position:TFBXSizeInt;
begin
 Position:=Pos(#0,String(pRawName));
 if Position>0 then begin
  result:=Copy(pRawName,1,Position-1);
 end else begin
  result:=pRawName;
 end;
end;

function TFBXBinaryParser.ConstructName(const pNames:array of TFBXString):TFBXString;
var Index:Int32;
begin
 result:='';
 for Index:=0 to length(pNames)-1 do begin
  if Index>0 then begin
   result:=TFBXString(#0#1)+result;
  end;
  result:=pNames[Index]+result;
 end;
end;

function TFBXBinaryParser.ReadInt8:Int8;
begin
 result:=0;
 fStream.ReadBuffer(result,SizeOf(Int8));
end;

function TFBXBinaryParser.ReadInt16:Int16;
type TBytes=array[0..1] of UInt8;
var Bytes:TBytes;
    Temporary:UInt16;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt16(Bytes[0]) shl 0) or (UInt16(Bytes[1]) shl 8);
 result:=Int16(pointer(@Temporary)^);
end;

function TFBXBinaryParser.ReadInt32:Int32;
type TBytes=array[0..3] of UInt8;
var Bytes:TBytes;
    Temporary:UInt32;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt32(Bytes[0]) shl 0) or (UInt32(Bytes[1]) shl 8) or (UInt32(Bytes[2]) shl 16) or (UInt32(Bytes[3]) shl 24);
 result:=Int32(pointer(@Temporary)^);
end;

function TFBXBinaryParser.ReadInt64:Int64;
type TBytes=array[0..7] of UInt8;
var Bytes:TBytes;
    Temporary:UInt64;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 Temporary:=(UInt64(Bytes[0]) shl 0) or (UInt64(Bytes[1]) shl 8) or (UInt64(Bytes[2]) shl 16) or (UInt64(Bytes[3]) shl 24) or (UInt64(Bytes[4]) shl 32) or (UInt64(Bytes[5]) shl 40) or (UInt64(Bytes[6]) shl 48) or (UInt64(Bytes[7]) shl 56);
 result:=Int64(pointer(@Temporary)^);
end;

function TFBXBinaryParser.ReadUInt8:UInt8;
begin
 result:=0;
 fStream.ReadBuffer(result,SizeOf(UInt8));
end;

function TFBXBinaryParser.ReadUInt16:UInt16;
type TBytes=array[0..1] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt16(Bytes[0]) shl 0) or (UInt16(Bytes[1]) shl 8);
end;

function TFBXBinaryParser.ReadUInt32:UInt32;
type TBytes=array[0..3] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt32(Bytes[0]) shl 0) or (UInt32(Bytes[1]) shl 8) or (UInt32(Bytes[2]) shl 16) or (UInt32(Bytes[3]) shl 24);
end;

function TFBXBinaryParser.ReadUInt64:UInt64;
type TBytes=array[0..7] of UInt8;
var Bytes:TBytes;
begin
 Bytes[0]:=0;
 fStream.ReadBuffer(Bytes,SizeOf(TBytes));
 result:=(UInt64(Bytes[0]) shl 0) or (UInt64(Bytes[1]) shl 8) or (UInt64(Bytes[2]) shl 16) or (UInt64(Bytes[3]) shl 24) or (UInt64(Bytes[4]) shl 32) or (UInt64(Bytes[5]) shl 40) or (UInt64(Bytes[6]) shl 48) or (UInt64(Bytes[7]) shl 56);
end;

function TFBXBinaryParser.ReadFloat32:single;
begin
 UInt32(pointer(@result)^):=ReadUInt32;
end;

function TFBXBinaryParser.ReadFloat64:double;
begin
 UInt64(pointer(@result)^):=ReadUInt64;
end;

function TFBXBinaryParser.ReadString(const pLength:Int32):TFBXString;
begin
 result:='';
 if pLength>0 then begin
  SetLength(result,pLength);
  fStream.ReadBuffer(result[1],pLength);
 end;
end;

function TFBXBinaryParser.NextElement:TFBXElement;
const BLOCK_SENTINEL_LENGTH=13;
var EndOffset,CountProperties,PropertiesLength,NameLength,
    PropertyIndex,PropertyDataType:UInt32;
    Bytes:TFBXBytes;
    Element:TFBXElement;
begin

 EndOffset:=ReadUInt32;
 CountProperties:=ReadUInt32;
 PropertiesLength:=ReadUInt32;
 NameLength:=ReadUInt8;

 if EndOffset=0 then begin
  // Behind a object record, there is 13 zero bytes, which should then match up with the EndOffset.
  result:=nil;
  exit;
 end;

 result:=TFBXElement.Create(ReadString(NameLength));

 if (CountProperties>0) and (PropertiesLength>0) then begin
  for PropertyIndex:=1 to CountProperties do begin
   PropertyDataType:=ReadUInt8;
   case PropertyDataType of
    TYPE_BOOL:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyBoolean.Create(ReadUInt8<>0));
    end;
    TYPE_BYTE:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyInteger.Create(ReadInt8));
    end;
    TYPE_INT16:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyInteger.Create(ReadInt16));
    end;
    TYPE_INT32:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyInteger.Create(ReadInt32));
    end;
    TYPE_INT64:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyInteger.Create(ReadInt64));
    end;
    TYPE_FLOAT32:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyFloat.Create(ReadFloat32));
    end;
    TYPE_FLOAT64:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyFloat.Create(ReadFloat64));
    end;
    TYPE_BYTES:begin
     Bytes:=nil;
     try
      SetLength(Bytes,ReadUInt32);
      if length(Bytes)>0 then begin
       fStream.ReadBuffer(Bytes[0],length(Bytes));
      end;
      TFBXElement(result).AddProperty(TFBXElementPropertyBytes.Create(Bytes));
     finally
      Bytes:=nil;
     end;
    end;
    TYPE_STRING:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyString.Create(ReadString(ReadUInt32)));
    end;
    TYPE_ARRAY_FLOAT32:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtFloat32));
    end;
    TYPE_ARRAY_FLOAT64:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtFloat64));
    end;
    TYPE_ARRAY_INT16:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtInt16));
    end;
    TYPE_ARRAY_INT32:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtInt32));
    end;
    TYPE_ARRAY_INT64:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtInt64));
    end;
    TYPE_ARRAY_BYTE:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtInt8));
    end;
    TYPE_ARRAY_BOOL:begin
     TFBXElement(result).AddProperty(TFBXElementPropertyArray.CreateFrom(fStream,fepadtBoolean));
    end;
    else begin
     //
    end;
   end;
  end;
 end;

 if fStream.Position<EndOffset then begin
  while (fStream.Position+(BLOCK_SENTINEL_LENGTH-1))<EndOffset do begin
   Element:=NextElement;
   if assigned(Element) then begin
    TFBXElement(result).AddChildren(Element);
   end;
  end;
 end;

 if fStream.Position<>EndOffset then begin
  raise EFBXBinaryParser.Create('Corrupt binary FBX file');
 end;

end;

function TFBXBinaryParser.Parse:TFBXElement;
var Element:TFBXElement;
begin
 result:=TFBXElement.Create('');
 if assigned(result) then begin
  try
   while fStream.Position<fStream.Size do begin
    Element:=NextElement;
    if assigned(Element) then begin
     result.AddChildren(Element);
    end else begin
     break;
    end;
   end;
  except
   FreeAndNil(result);
   raise
  end;
 end;
end;

constructor TFBXTimeUtils.Create(const pGlobalSettings:TFBXGlobalSettings);
begin
 inherited Create;
 fGlobalSettings:=pGlobalSettings;
end;

destructor TFBXTimeUtils.Destroy;
begin
 inherited Destroy;
end;

function TFBXTimeUtils.TimeToFrame(const pTime:TFBXTime;const pTimeMode:TFBXTimeMode=ftmDEFAULT):Double;
var FramesPerSecond:Double;
begin
 if (pTimeMode=ftmCUSTOM) or
    ((pTimeMode=ftmDEFAULT) and (fGlobalSettings.fTimeMode=ftmCUSTOM)) then begin
  FramesPerSecond:=Max(1,fGlobalSettings.fCustomFrameRate);
 end else if pTimeMode=ftmDEFAULT then begin
  FramesPerSecond:=FramesPerSecondValues[fGlobalSettings.fTimeMode];
 end else begin
  FramesPerSecond:=FramesPerSecondValues[pTimeMode];
 end;
 result:=pTime*(FramesPerSecond/TFBXTimeUtils.UnitsPerSecond);
end;

function TFBXTimeUtils.FrameToSeconds(const pFrame:Double;const pTimeMode:TFBXTimeMode=ftmDEFAULT):Double;
var FramesPerSecond:Double;
begin
 if (pTimeMode=ftmCUSTOM) or
    ((pTimeMode=ftmDEFAULT) and (fGlobalSettings.fTimeMode=ftmCUSTOM)) then begin
  FramesPerSecond:=Max(1,fGlobalSettings.fCustomFrameRate);
 end else if pTimeMode=ftmDEFAULT then begin
  FramesPerSecond:=FramesPerSecondValues[fGlobalSettings.fTimeMode];
 end else begin
  FramesPerSecond:=FramesPerSecondValues[pTimeMode];
 end;
 result:=pFrame/FramesPerSecond;
end;

function TFBXTimeUtils.TimeToSeconds(const pTime:TFBXTime):Double;
begin
 result:=pTime/TFBXTimeUtils.UnitsPerSecond;
end;

constructor TFBXProperty.Create(const pBaseObject:TObject;const pBaseName:TFBXString;const pBasePropInfo:PPropInfo);
begin
 inherited Create;
 fBaseObject:=pBaseObject;
 if not PropertyNameRemap.TryGetValue(pBaseName,fBaseName) then begin
  fBaseName:=pBaseName;
 end;
 fBasePropInfo:=pBasePropInfo;
 if assigned(fBaseObject) and not assigned(fBasePropInfo) then begin
  fBasePropInfo:=GetPropInfo(fBaseObject,String(fBaseName));
 end;
end;

constructor TFBXProperty.Create(const pValue:Variant;const pBaseObject:TObject;const pBaseName:TFBXString;const pBasePropInfo:PPropInfo);
begin
 Create(pBaseObject,pBaseName,pBasePropInfo);
 if not VarIsEmpty(pValue) then begin
  if assigned(fBasePropInfo) then begin
   SetVariantProp(fBaseObject,fBasePropInfo,pValue);
  end else begin
   fValue:=pValue;
  end;
 end;
end;

destructor TFBXProperty.Destroy;
begin
 inherited Destroy;
end;

function TFBXProperty.GetValue:Variant;
begin
 if assigned(fBasePropInfo) then begin
  result:=GetVariantProp(fBaseObject,fBasePropInfo);
 end else begin
  result:=fValue;
 end;
end;

procedure TFBXProperty.SetValue(const pValue:Variant);
begin
 if assigned(fBasePropInfo) then begin
  SetVariantProp(fBaseObject,fBasePropInfo,pValue);
 end else begin
  fValue:=pValue;
 end;
end;

constructor TFBXObject.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
begin
 inherited Create;
 fLoader:=pLoader;
 fElement:=pElement;
 fID:=pID;
 fName:=pName;
 fType_:=pType_;
 fProperties:=TFBXPropertyList.Create(true);
 fPropertyNameMap:=TFBXPropertyNameMap.Create;
 fConnectedFrom:=TFBXObjectList.Create(false);
 fConnectedTo:=TFBXObjectList.Create(false);
 fNodeAttributes:=TFBXNodeAttributeList.Create(false);
 fReference:='';
end;

destructor TFBXObject.Destroy;
begin
 FreeAndNil(fProperties);
 FreeAndNil(fPropertyNameMap);
 FreeAndNil(fConnectedFrom);
 FreeAndNil(fConnectedTo);
 FreeAndNil(fNodeAttributes);
 inherited Destroy;
end;

procedure TFBXObject.AfterConstruction;
begin
 inherited AfterConstruction;
 fLoader.fAllocatedList.Add(self);
end;

procedure TFBXObject.BeforeDestruction;
var Index:TFBXSizeInt;
begin
 Index:=fLoader.fAllocatedList.IndexOf(self);
 if Index>=0 then begin
  fLoader.fAllocatedList.Extract(self);
 end;
 inherited BeforeDestruction;
end;

function TFBXObject.GetParentNode:TFBXNode;
var CurrentObject:TFBXObject;
begin
 if self is TFBXNode then begin
  result:=TFBXNode(self);
 end else begin
  for CurrentObject in fConnectedFrom do begin
   if CurrentObject is TFBXNode then begin
    result:=TFBXNode(CurrentObject);
    exit;
   end;
  end;
  for CurrentObject in fConnectedFrom do begin
   result:=CurrentObject.GetParentNode;
   if assigned(result) then begin
    exit;
   end;
  end;
  result:=nil;
 end;
end;

function TFBXObject.FindConnectionsByType(const pType_:TFBXString):TFBXObjects;
var CurrentObject:TFBXObject;
    Count:Int32;
begin
 Count:=0;
 for CurrentObject in fConnectedTo do begin
  if CurrentObject.fType_=pType_ then begin
   inc(Count);
  end;
 end;
 result:=nil;
 SetLength(result,Count);
 Count:=0;
 for CurrentObject in fConnectedTo do begin
  if CurrentObject.fType_=pType_ then begin
   result[Count]:=CurrentObject;
   inc(Count);
  end;
 end;
end;

procedure TFBXObject.ConnectTo(const pObject:TFBXObject);
begin
 fConnectedTo.Add(pObject);
 pObject.fConnectedFrom.Add(self);
end;

procedure TFBXObject.ConnectToProperty(const pObject:TFBXObject;const pPropertyName:TFBXString);
var PropertyName:TFBXString;
begin
 if not PropertyNameRemap.TryGetValue(pPropertyName,PropertyName) then begin
  PropertyName:=pPropertyName;
 end;
 AddProperty(pPropertyName).ConnectedFrom:=pObject;
end;

function TFBXObject.AddProperty(const pPropertyName:TFBXString):TFBXProperty;
var PropertyName:TFBXString;
begin
 if not PropertyNameRemap.TryGetValue(pPropertyName,PropertyName) then begin
  PropertyName:=pPropertyName;
 end;
 if not fPropertyNameMap.TryGetValue(PropertyName,result) then begin
  result:=TFBXProperty.Create(self,PropertyName,nil);
  fPropertyNameMap.AddOrSetValue(PropertyName,result);
 end;
end;

function TFBXObject.AddProperty(const pPropertyName:TFBXString;const pValue:Variant):TFBXProperty;
var PropertyName:TFBXString;
begin
 if not PropertyNameRemap.TryGetValue(pPropertyName,PropertyName) then begin
  PropertyName:=pPropertyName;
 end;
 if not fPropertyNameMap.TryGetValue(PropertyName,result) then begin
  result:=TFBXProperty.Create(pValue,self,PropertyName,nil);
  fPropertyNameMap.AddOrSetValue(PropertyName,result);
 end;
end;

procedure TFBXObject.SetProperty(const pPropertyName:TFBXString;const pValue:Variant);
var p:TFBXProperty;
    PropertyName:TFBXString;
begin
 if not PropertyNameRemap.TryGetValue(pPropertyName,PropertyName) then begin
  PropertyName:=pPropertyName;
 end;
 if fPropertyNameMap.TryGetValue(PropertyName,p) then begin
  p.SetValue(pValue);
 end;
end;

function TFBXObject.GetProperty(const pPropertyName:TFBXString):Variant;
var p:TFBXProperty;
    PropertyName:TFBXString;
begin
 if not PropertyNameRemap.TryGetValue(pPropertyName,PropertyName) then begin
  PropertyName:=pPropertyName;
 end;
 if fPropertyNameMap.TryGetValue(PropertyName,p) then begin
  result:=p.GetValue;
 end else begin
  VarClear(result);
 end;
end;

constructor TFBXNode.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fParent:=nil;

 fChildren:=TFBXNodeList.Create(false);

 fLclTranslation:=TFBXVector3Property.Create(0.0,0.0,0.0);

 fLclRotation:=TFBXVector3Property.Create(0.0,0.0,0.0);

 fLclScaling:=TFBXVector3Property.Create(1.0,1.0,1.0);

 fVisibility:=true;

end;

destructor TFBXNode.Destroy;
begin
 FreeAndNil(fChildren);
 FreeAndNil(fLclTranslation);
 FreeAndNil(fLclRotation);
 FreeAndNil(fLclScaling);
 inherited Destroy;
end;

procedure TFBXNode.ConnectTo(const pObject:TFBXObject);
begin
 if fType_='Transform' then begin
  if (fConnectedTo.Count>0) and (fConnectedTo[0] is TFBXMesh) then begin
   if assigned(pObject) and (pObject is TFBXMaterial) then begin
    TFBXMesh(fConnectedTo[0]).fMaterials.Add(TFBXMaterial(pObject));
   end;
  end;
 end;
 if assigned(pObject) and (pObject is TFBXNode) then begin
  fChildren.Add(TFBXNode(pObject));
  TFBXNode(pObject).fParent:=self;
 end else begin
  inherited ConnectTo(pObject);
 end;
end;

constructor TFBXScene.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);
 fAllObjects:=TFBXObjectNameMap.Create;
 fHeader:=TFBXHeader.Create(pLoader,nil,0,'','');
 fSceneInfo:=nil;
 fCameras:=TFBXCameraList.Create(false);
 fLights:=TFBXLightList.Create(false);
 fMeshes:=TFBXMeshList.Create(false);
 fSkeletons:=TFBXSkeletonList.Create(false);
 fMaterials:=TFBXMaterialList.Create(false);
 fAnimationStackList:=TFBXAnimationStackList.Create(false);
 fDeformers:=TFBXDeformerList.Create(false);
 fTextures:=TFBXTextureList.Create(false);
 fPoses:=TFBXPoseList.Create(false);
 fVideos:=TFBXVideoList.Create(false);
 fTakes:=TFBXTakeList.Create(false);
 fCurrentTake:=nil;
 fRootNodes:=TFBXObjectList.Create(false);
end;

destructor TFBXScene.Destroy;
begin
 fCurrentTake:=nil;
 FreeAndNil(fAllObjects);
 FreeAndNil(fHeader);
 FreeAndNil(fSceneInfo);
 FreeAndNil(fCameras);
 FreeAndNil(fLights);
 FreeAndNil(fMeshes);
 FreeAndNil(fSkeletons);
 FreeAndNil(fMaterials);
 FreeAndNil(fAnimationStackList);
 FreeAndNil(fDeformers);
 FreeAndNil(fTextures);
 FreeAndNil(fPoses);
 FreeAndNil(fVideos);
 FreeAndNil(fTakes);
 FreeAndNil(fRootNodes);
 inherited Destroy;
end;

constructor TFBXGlobalSettings.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);
 fUpAxis:=1;
 fUpAxisSign:=1;
 fFrontAxis:=2;
 fFrontAxisSign:=1;
 fCoordAxis:=0;
 fCoordAxisSign:=1;
 fOriginalUpAxis:=0;
 fOriginalUpAxisSign:=1;
 fUnitScaleFactor:=1.0;
 fOriginalUnitScaleFactor:=1.0;
 fAmbientColor:=TFBXColor.Create(0.0,0.0,0.0,0.0);
 fDefaultCamera:='';
 fTimeMode:=ftmDEFAULT;
 fTimeProtocol:=0;
 fSnapOnFrameMode:=0;
 fTimeSpan:=TFBXTimeSpan.Create(0,0);
 fCustomFrameRate:=-1.0;
end;

destructor TFBXGlobalSettings.Destroy;
begin
 inherited Destroy;
end;

function TFBXGlobalSettings.GetTimeSpanStart:TFBXTime;
begin
 result:=fTimeSpan.StartTime;
end;

procedure TFBXGlobalSettings.SetTimeSpanStart(const pValue:TFBXTime);
begin
 fTimeSpan.StartTime:=pValue;
end;

function TFBXGlobalSettings.GetTimeSpanStop:TFBXTime;
begin
 result:=fTimeSpan.EndTime;
end;

procedure TFBXGlobalSettings.SetTimeSpanStop(const pValue:TFBXTime);
begin
 fTimeSpan.EndTime:=pValue;
end;

constructor TFBXCamera.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,SubSubElement:TFBXElement;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fPosition:=TFBXVector3Property.Create(0.0,0.0,0.0);
 fLookAt:=TFBXVector3Property.Create(0.0,0.0,0.0);
 fCameraOrthoZoom:=1.0;
 fRoll:=0.0;
 fFieldOfView:=0.0;
 fFrameColor:=TFBXColorProperty.Create(0.0,0.0,0.0,1.0);
 fNearPlane:=1.0;
 fFarPlane:=10000.0;

 for SubElement in pElement.Children do begin
  if (SubElement.ID='CameraOrthoZoom') and
     (SubElement.Properties.Count>0) then begin
   fCameraOrthoZoom:=SubElement.Properties[0].GetFloat;
  end else if (SubElement.ID='LookAt') and
              (SubElement.Properties.Count>2) then begin
   fLookAt.x:=SubElement.Properties[0].GetFloat;
   fLookAt.y:=SubElement.Properties[1].GetFloat;
   fLookAt.z:=SubElement.Properties[2].GetFloat;
  end else if (SubElement.ID='Position') and
              (SubElement.Properties.Count>2) then begin
   fPosition.x:=SubElement.Properties[0].GetFloat;
   fPosition.y:=SubElement.Properties[1].GetFloat;
   fPosition.z:=SubElement.Properties[2].GetFloat;
  end else if SubElement.ID='Properties60' then begin
   for SubSubElement in SubElement.Children do begin
    if ((SubSubElement.ID='P') or (SubSubElement.ID='Property')) and
       (SubSubElement.Properties.Count>0) then begin
     if (SubSubElement.Properties[0].GetString='Roll') and (3<SubSubElement.Properties.Count) then begin
      fRoll:=SubSubElement.Properties[3].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='FieldOfView') and (3<SubSubElement.Properties.Count) then begin
      fFieldOfView:=SubSubElement.Properties[3].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='FrameColor') and (5<SubSubElement.Properties.Count) then begin
      fFrameColor.Red:=SubSubElement.Properties[3].GetFloat;
      fFrameColor.Green:=SubSubElement.Properties[4].GetFloat;
      fFrameColor.Blue:=SubSubElement.Properties[5].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='NearPlane') and (3<SubSubElement.Properties.Count) then begin
      fNearPlane:=SubSubElement.Properties[3].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='FarPlane') and (3<SubSubElement.Properties.Count) then begin
      fFarPlane:=SubSubElement.Properties[3].GetFloat;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXCamera.Destroy;
begin
 FreeAndNil(fPosition);
 FreeAndNil(fLookAt);
 FreeAndNil(fFrameColor);
 inherited Destroy;
end;

constructor TFBXLight.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,SubSubElement:TFBXElement;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fColor:=TFBXColorProperty.Create(1.0,1.0,1.0,1.0);
 fIntensity:=1.0;
 fConeAngle:=1.0;
 fDecay:=NO_DECAY;
 fLightType:=DIRECTIONAL;

 for SubElement in pElement.Children do begin
  if SubElement.ID='Properties60' then begin
   for SubSubElement in SubElement.Children do begin
    if ((SubSubElement.ID='P') or (SubSubElement.ID='Property')) and
       (SubSubElement.Properties.Count>0) then begin
     if (SubSubElement.Properties[0].GetString='Color') and (5<SubSubElement.Properties.Count) then begin
      fColor.Red:=SubSubElement.Properties[3].GetFloat;
      fColor.Green:=SubSubElement.Properties[4].GetFloat;
      fColor.Blue:=SubSubElement.Properties[5].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='Intensity') and (3<SubSubElement.Properties.Count) then begin
      fIntensity:=SubSubElement.Properties[3].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='Cone angle') and (3<SubSubElement.Properties.Count) then begin
      fConeAngle:=SubSubElement.Properties[3].GetFloat;
     end else if (SubSubElement.Properties[0].GetString='LightType') and (3<SubSubElement.Properties.Count) then begin
      fLightType:=SubSubElement.Properties[3].GetInteger;
     end else if (SubSubElement.Properties[0].GetString='Decay') and (3<SubSubElement.Properties.Count) then begin
      fDecay:=SubSubElement.Properties[3].GetInteger;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXLight.Destroy;
begin
 FreeAndNil(fColor);
 inherited Destroy;
end;

constructor TFBXLayerElement<TDataType>.Create;
begin
 inherited Create;
 fMappingMode:=TFBXMappingMode.fmmNone;
 fReferenceMode:=TFBXReferenceMode.frmDirect;
 fIndexArray:=TFBXLayerElementIntegerList.Create;
 fByPolygonVertexIndexArray:=TFBXLayerElementIntegerList.Create;
 fData:=TFBXLayerElementDataList.Create;
end;

destructor TFBXLayerElement<TDataType>.Destroy;
begin
 FreeAndNil(fIndexArray);
 FreeAndNil(fByPolygonVertexIndexArray);
 FreeAndNil(fData);
 inherited Destroy;
end;

procedure TFBXLayerElement<TDataType>.Finish(const pMesh:TFBXMesh);
type TEdgeMap=TDictionary<TFBXMeshEdge,Int64>;
var Index,SubIndex:Int32;
    DataIndex:Int64;
    EdgeMap:TEdgeMap;
    Edge:TFBXMeshEdge;
begin
 fByPolygonVertexIndexArray.Clear;
 case fMappingMode of
  TFBXMappingMode.fmmByVertex:begin
   for Index:=0 to pMesh.fPolygons.Count-1 do begin
    for SubIndex:=0 to pMesh.fPolygons[Index].Count-1 do begin
     DataIndex:=pMesh.fPolygons[Index][SubIndex];
     if (fReferenceMode=TFBXReferenceMode.frmDirect) or (DataIndex>=fIndexArray.Count) then begin
      fByPolygonVertexIndexArray.Add(DataIndex);
     end else begin
      fByPolygonVertexIndexArray.Add(fIndexArray[DataIndex]);
     end;
    end;
   end;
  end;
  TFBXMappingMode.fmmByPolygon:begin
   DataIndex:=0;
   for Index:=0 to pMesh.fPolygons.Count-1 do begin
    for SubIndex:=0 to pMesh.fPolygons[Index].Count-1 do begin
     if (fReferenceMode=TFBXReferenceMode.frmDirect) or (DataIndex>=fIndexArray.Count) then begin
      fByPolygonVertexIndexArray.Add(DataIndex);
     end else begin
      fByPolygonVertexIndexArray.Add(fIndexArray[DataIndex]);
     end;
    end;
    inc(DataIndex);
   end;
  end;
  TFBXMappingMode.fmmByPolygonVertex:begin
   DataIndex:=0;
   for Index:=0 to pMesh.fPolygons.Count-1 do begin
    for SubIndex:=0 to pMesh.fPolygons[Index].Count-1 do begin
     if (fReferenceMode=TFBXReferenceMode.frmDirect) or (DataIndex>=fIndexArray.Count) then begin
      fByPolygonVertexIndexArray.Add(DataIndex);
     end else begin
      fByPolygonVertexIndexArray.Add(fIndexArray[DataIndex]);
     end;
     inc(DataIndex);
    end;
   end;
  end;
  TFBXMappingMode.fmmByEdge:begin
   EdgeMap:=TEdgeMap.Create;
   try
    for Index:=0 to pMesh.fEdges.Count-1 do begin
     EdgeMap.AddOrSetValue(pMesh.fEdges[Index],Index);
    end;
    for Index:=0 to pMesh.fPolygons.Count-1 do begin
     for SubIndex:=0 to pMesh.fPolygons[Index].Count-1 do begin
      Edge[0]:=pMesh.fPolygons[Index][SubIndex];
      Edge[1]:=pMesh.fPolygons[Index][(SubIndex+1) mod pMesh.fPolygons[Index].Count];
      if not EdgeMap.TryGetValue(Edge,DataIndex) then begin
       Edge[1]:=pMesh.fPolygons[Index][SubIndex];
       Edge[0]:=pMesh.fPolygons[Index][(SubIndex+1) mod pMesh.fPolygons[Index].Count];
       if not EdgeMap.TryGetValue(Edge,DataIndex) then begin
        DataIndex:=0;
       end;
      end;
      if (fReferenceMode=TFBXReferenceMode.frmDirect) or (DataIndex>=fIndexArray.Count) then begin
       fByPolygonVertexIndexArray.Add(DataIndex);
      end else begin
       fByPolygonVertexIndexArray.Add(fIndexArray[DataIndex]);
      end;
      inc(DataIndex);
     end;
    end;
   finally
    EdgeMap.Free;
   end;
  end;
  TFBXMappingMode.fmmAllSame:begin
   for Index:=0 to pMesh.fPolygons.Count-1 do begin
    for SubIndex:=0 to pMesh.fPolygons[Index].Count-1 do begin
     if (fReferenceMode=TFBXReferenceMode.frmDirect) or (fIndexArray.Count=0) then begin
      fByPolygonVertexIndexArray.Add(0);
     end else begin
      fByPolygonVertexIndexArray.Add(fIndexArray[0]);
     end;
    end;
   end;
  end;
  else begin
   raise EFBX.Create('Unknown mapping mode');
  end;
 end;
end;

function TFBXLayerElement<TDataType>.GetItem(const pIndex:TFBXSizeInt):TDataType;
begin
 result:=fData[pIndex];
end;

procedure TFBXLayerElement<TDataType>.SetItem(const pIndex:TFBXSizeInt;const pItem:TDataType);
begin
 fData[pIndex]:=pItem;
end;

constructor TFBXLayer.Create;
begin
 inherited Create;
 fNormals:=TFBXLayerElementVector3.Create;
 fTangents:=TFBXLayerElementVector3.Create;
 fBitangents:=TFBXLayerElementVector3.Create;
 fUVs:=TFBXLayerElementVector2.Create;
 fColors:=TFBXLayerElementColor.Create;
 fMaterials:=TFBXLayerElementInteger.Create;
end;

destructor TFBXLayer.Destroy;
begin
 FreeAndNil(fNormals);
 FreeAndNil(fTangents);
 FreeAndNil(fBitangents);
 FreeAndNil(fUVs);
 FreeAndNil(fColors);
 FreeAndNil(fMaterials);
 inherited Destroy;
end;

constructor TFBXMesh.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
 function GetLayer(const pLayerIndex:Int32):TFBXLayer;
 begin
  while fLayers.Count<=pLayerIndex do begin
   fLayers.Add(TFBXLayer.Create);
  end;
  result:=fLayers[pLayerIndex];
 end;
 function StringToMappingMode(pValue:TFBXString):TFBXMappingMode;
 begin
  pValue:=TFBXString(LowerCase(String(pValue)));
  if (pValue='byvertex') or
     (pValue='byvertice') or
     (pValue='byvertices') or
     (pValue='bycontrolpoint') then begin
   result:=TFBXMappingMode.fmmByVertex;
  end else if pValue='bypolygonvertex' then begin
   result:=TFBXMappingMode.fmmByPolygonVertex;
  end else if pValue='bypolygon' then begin
   result:=TFBXMappingMode.fmmByPolygon;
  end else if pValue='byedge' then begin
   result:=TFBXMappingMode.fmmByEdge;
  end else if pValue='allsame' then begin
   result:=TFBXMappingMode.fmmAllSame;
  end else begin
   result:=TFBXMappingMode.fmmNone;
  end;
 end;
 function StringToReferenceMode(pValue:TFBXString):TFBXReferenceMode;
 begin
  pValue:=TFBXString(LowerCase(String(pValue)));
  if pValue='direct' then begin
   result:=TFBXReferenceMode.frmDirect;
  end else if pValue='index' then begin
   result:=TFBXReferenceMode.frmIndex;
  end else if pValue='indextodirect' then begin
   result:=TFBXReferenceMode.frmIndexToDirect;
  end else begin
   result:=TFBXReferenceMode.frmDirect;
  end;
 end;
var SubElement,SubSubElement,ArrayElement:TFBXElement;
    Index,LayerIndex:Int32;
    Value:Int64;
    IntList:TFBXInt64List;
    FloatList:TFBXFloat64List;
    Polygon:TFBXMeshIndices;
    Edge:TFBXMeshEdge;
    Layer:TFBXLayer;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fVertices:=TFBXMeshVertices.Create;

 fPolygons:=TFBXMeshPolygons.Create(true);

 fEdges:=TFBXMeshEdges.Create;

 fLayers:=TFBXLayers.Create(true);

 fClusterMap:=TFBXMeshClusterMap.Create(true);

 fTriangleVertices:=TFBXMeshTriangleVertexList.Create;

 fTriangleIndices:=TFBXInt64List.Create;

 fMaterials:=TFBXMaterialList.Create(false);

 if assigned(pElement) then begin
  for SubElement in pElement.Children do begin
   if SubElement.ID='Vertices' then begin
    FloatList:=TFBXFloat64List.Create;
    try
     if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
      FloatList.Capacity:=SubElement.Properties[0].GetArrayLength;
      for Index:=1 to SubElement.Properties[0].GetArrayLength do begin
       FloatList.Add(SubElement.Properties[0].GetFloat(Index-1));
      end;
     end else begin
      if SubElement.Children.Count=1 then begin
       ArrayElement:=SubElement.Children[0];
      end else begin
       ArrayElement:=SubElement;
      end;
      if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
       FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
       for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
        FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
       end;
      end else begin
       FloatList.Capacity:=ArrayElement.Properties.Count;
       for Index:=1 to ArrayElement.Properties.Count do begin
        FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
       end;
      end;
     end;
     Index:=0;
     while (Index+2)<FloatList.Count do begin
      fVertices.Add(TFBXVector3.Create(FloatList[Index+0],FloatList[Index+1],FloatList[Index+2]));
      inc(Index,3);
     end;
    finally
     FloatList.Free;
    end;
   end else if SubElement.ID='PolygonVertexIndex' then begin
    IntList:=TFBXInt64List.Create;
    try
     if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
      IntList.Capacity:=SubElement.Properties[0].GetArrayLength;
      for Index:=1 to SubElement.Properties[0].GetArrayLength do begin
       IntList.Add(SubElement.Properties[0].GetInteger(Index-1));
      end;
     end else begin
      if SubElement.Children.Count=1 then begin
       ArrayElement:=SubElement.Children[0];
      end else begin
       ArrayElement:=SubElement;
      end;
      if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
       IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
       for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
        IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
       end;
      end else begin
       IntList.Capacity:=ArrayElement.Properties.Count;
       for Index:=1 to ArrayElement.Properties.Count do begin
        IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
       end;
      end;
     end;
     Polygon:=nil;
     for Value in IntList do begin
      if not assigned(Polygon) then begin
       Polygon:=TFBXMeshIndices.Create;
       fPolygons.Add(Polygon);
      end;
      if Value<0 then begin
       Polygon.Add(not Value);
       Polygon:=nil;
      end else begin
       Polygon.Add(Value);
      end;
     end;
    finally
     IntList.Free;
    end;
   end else if SubElement.ID='Edges' then begin
    IntList:=TFBXInt64List.Create;
    try
     if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
      IntList.Capacity:=SubElement.Properties[0].GetArrayLength;
      for Index:=1 to SubElement.Properties[0].GetArrayLength do begin
       IntList.Add(SubElement.Properties[0].GetInteger(Index-1));
      end;
     end else begin
      if SubElement.Children.Count=1 then begin
       ArrayElement:=SubElement.Children[0];
      end else begin
       ArrayElement:=SubElement;
      end;
      if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
       IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
       for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
        IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
       end;
      end else begin
       IntList.Capacity:=ArrayElement.Properties.Count;
       for Index:=1 to ArrayElement.Properties.Count do begin
        IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
       end;
      end;
     end;
     Index:=0;
     while (Index+1)<IntList.Count do begin
      Edge[0]:=IntList[Index+0];
      Edge[1]:=IntList[Index+1];
      fEdges.Add(Edge);
      inc(Index,2);
     end;
    finally
     IntList.Free;
    end;
   end else if (SubElement.ID='LayerElementNormal') or (SubElement.ID='LayerElementNormals') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Normals.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Normals.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='Normal') or (SubSubElement.ID='Normals') then begin
        FloatList:=TFBXFloat64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          FloatList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           FloatList.Add(SubSubElement.Properties[0].GetFloat(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
           end;
          end else begin
           FloatList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
           end;
          end;
         end;
         Index:=0;
         while (Index+2)<FloatList.Count do begin
          Layer.Normals.fData.Add(TFBXVector3.Create(FloatList[Index+0],FloatList[Index+1],FloatList[Index+2]));
          inc(Index,3);
         end;
        finally
         FloatList.Free;
        end;
       end else if (SubSubElement.ID='NormalIndex') or (SubSubElement.ID='NormalsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Normals.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.Normals.Finish(self);
     end;
    end;
   end else if (SubElement.ID='LayerElementTangent') or (SubElement.ID='LayerElementTangents') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Tangents.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Tangents.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='Tangent') or (SubSubElement.ID='Tangents') then begin
        FloatList:=TFBXFloat64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          FloatList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           FloatList.Add(SubSubElement.Properties[0].GetFloat(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
           end;
          end else begin
           FloatList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
           end;
          end;
         end;
         Index:=0;
         while (Index+2)<FloatList.Count do begin
          Layer.Tangents.fData.Add(TFBXVector3.Create(FloatList[Index+0],FloatList[Index+1],FloatList[Index+2]));
          inc(Index,3);
         end;
        finally
         FloatList.Free;
        end;
       end else if (SubSubElement.ID='TangentIndex') or (SubSubElement.ID='TangentsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Tangents.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.Tangents.Finish(self);
     end;
    end;
   end else if (SubElement.ID='LayerElementBinormal') or (SubElement.ID='LayerElementBinormals') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Bitangents.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Bitangents.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='Binormal') or (SubSubElement.ID='Binormals') then begin
        FloatList:=TFBXFloat64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          FloatList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           FloatList.Add(SubSubElement.Properties[0].GetFloat(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
           end;
          end else begin
           FloatList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
           end;
          end;
         end;
         Index:=0;
         while (Index+2)<FloatList.Count do begin
          Layer.Bitangents.fData.Add(TFBXVector3.Create(FloatList[Index+0],FloatList[Index+1],FloatList[Index+2]));
          inc(Index,3);
         end;
        finally
         FloatList.Free;
        end;
       end else if (SubSubElement.ID='BinormalIndex') or (SubSubElement.ID='BinormalsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Bitangents.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.Bitangents.Finish(self);
     end;
    end;
   end else if (SubElement.ID='LayerElementColor') or (SubElement.ID='LayerElementColors') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Colors.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Colors.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='Color') or (SubSubElement.ID='Colors') then begin
        FloatList:=TFBXFloat64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          FloatList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           FloatList.Add(SubSubElement.Properties[0].GetFloat(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
           end;
          end else begin
           FloatList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
           end;
          end;
         end;
         Index:=0;
         while (Index+3)<FloatList.Count do begin
          Layer.Colors.fData.Add(TFBXColor.Create(FloatList[Index+0],FloatList[Index+1],FloatList[Index+2],FloatList[Index+3]));
          inc(Index,4);
         end;
        finally
         FloatList.Free;
        end;
       end else if (SubSubElement.ID='ColorIndex') or (SubSubElement.ID='ColorsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Colors.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.Colors.Finish(self);
     end;
    end;
   end else if (SubElement.ID='LayerElementUV') or (SubElement.ID='LayerElementUVs') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.UVs.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.UVs.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='UV') or (SubSubElement.ID='UVs') then begin
        FloatList:=TFBXFloat64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          FloatList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           FloatList.Add(SubSubElement.Properties[0].GetFloat(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           FloatList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            FloatList.Add(ArrayElement.Properties[0].GetFloat(Index-1));
           end;
          end else begin
           FloatList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            FloatList.Add(ArrayElement.Properties[Index-1].GetFloat);
           end;
          end;
         end;
         Index:=0;
         while (Index+1)<FloatList.Count do begin
          Layer.UVs.fData.Add(TFBXVector2.Create(FloatList[Index+0],FloatList[Index+1]));
          inc(Index,2);
         end;
        finally
         FloatList.Free;
        end;
       end else if (SubSubElement.ID='UVIndex') or (SubSubElement.ID='UVsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.UVs.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.UVs.Finish(self);
     end;
    end;
   end else if (SubElement.ID='LayerElementMaterial') or (SubElement.ID='LayerElementMaterials') then begin
    LayerIndex:=SubElement.Properties[0].GetInteger;
    if LayerIndex>=0 then begin
     Layer:=GetLayer(LayerIndex);
     if assigned(Layer) then begin
      for SubSubElement in SubElement.Children do begin
       if SubSubElement.ID='MappingInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Materials.fMappingMode:=StringToMappingMode(SubSubElement.Properties[0].GetString);
        end;
       end else if SubSubElement.ID='ReferenceInformationType' then begin
        if SubSubElement.Properties.Count>0 then begin
         Layer.Materials.fReferenceMode:=StringToReferenceMode(SubSubElement.Properties[0].GetString);
        end;
       end else if (SubSubElement.ID='Material') or (SubSubElement.ID='Materials') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Materials.fData.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end else if (SubSubElement.ID='MaterialIndex') or (SubSubElement.ID='MaterialsIndex') then begin
        IntList:=TFBXInt64List.Create;
        try
         if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
          IntList.Capacity:=SubSubElement.Properties[0].GetArrayLength;
          for Index:=1 to SubSubElement.Properties[0].GetArrayLength do begin
           IntList.Add(SubSubElement.Properties[0].GetInteger(Index-1));
          end;
         end else begin
          if SubSubElement.Children.Count=1 then begin
           ArrayElement:=SubSubElement.Children[0];
          end else begin
           ArrayElement:=SubSubElement;
          end;
          if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
           IntList.Capacity:=ArrayElement.Properties[0].GetArrayLength;
           for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
            IntList.Add(ArrayElement.Properties[0].GetInteger(Index-1));
           end;
          end else begin
           IntList.Capacity:=ArrayElement.Properties.Count;
           for Index:=1 to ArrayElement.Properties.Count do begin
            IntList.Add(ArrayElement.Properties[Index-1].GetInteger);
           end;
          end;
         end;
         for Value in IntList do begin
          Layer.Materials.fIndexArray.Add(Value);
         end;
        finally
         IntList.Free;
        end;
       end;
      end;
      Layer.Materials.Finish(self);
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXMesh.Destroy;
begin
 FreeAndNil(fVertices);
 FreeAndNil(fPolygons);
 FreeAndNil(fEdges);
 FreeAndNil(fLayers);
 FreeAndNil(fClusterMap);
 FreeAndNil(fTriangleVertices);
 FreeAndNil(fTriangleIndices);
 FreeAndNil(fMaterials);
 inherited Destroy;
end;

procedure TFBXMesh.ConnectTo(const pObject:TFBXObject);
begin
 if assigned(pObject) and (pObject is TFBXMaterial) then begin
  fMaterials.Add(TFBXMaterial(pObject));
 end;
 inherited ConnectTo(pObject);
end;

procedure TFBXMesh.Finish;
type TTriangleVertexMap=TDictionary<TFBXMeshTriangleVertex,Int64>;
var Index,SubIndex,WorkIndex,PolygonVertexIndex:Int32;
    VertexIndex,TriangleVertexIndex,ClusterIndex:Int64;
    Skins,Clusters:TFBXObjects;
    Skin,Cluster:TFBXObject;
    ClusterMapItem:TFBXMeshClusterMapItem;
    Polygon:TFBXMeshIndices;
    TriangleVertex:TFBXMeshTriangleVertex;
    TriangleVertexMap:TTriangleVertexMap;
    Layer:TFBXLayer;
    HasBlendShapes:boolean;
 function FillTriangleVertex(const pVertexIndex,pPolygonVertexIndex:Int64):TFBXMeshTriangleVertex;
 begin
  FillChar(result,SizeOf(TFBXMeshTriangleVertex),#0);
  result.Position:=fVertices[pVertexIndex];
  if assigned(Layer) then begin
   if pPolygonVertexIndex<Layer.fNormals.fByPolygonVertexIndexArray.Count then begin
    result.Normal:=Layer.fNormals.fData[Layer.fNormals.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
   if pPolygonVertexIndex<Layer.fTangents.fByPolygonVertexIndexArray.Count then begin
    result.Tangent:=Layer.fTangents.fData[Layer.fTangents.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
   if pPolygonVertexIndex<Layer.fBitangents.fByPolygonVertexIndexArray.Count then begin
    result.Bitangent:=Layer.fBitangents.fData[Layer.fBitangents.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
   if pPolygonVertexIndex<Layer.fUVs.fByPolygonVertexIndexArray.Count then begin
    result.UV:=Layer.fUVs.fData[Layer.fUVs.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
   if pPolygonVertexIndex<Layer.fColors.fByPolygonVertexIndexArray.Count then begin
    result.Color:=Layer.fColors.fData[Layer.fColors.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
   if pPolygonVertexIndex<Layer.fMaterials.fByPolygonVertexIndexArray.Count then begin
    result.Material:=Layer.fMaterials.fData[Layer.fMaterials.fByPolygonVertexIndexArray[pPolygonVertexIndex]];
   end;
  end;
 end;
begin

 fClusterMap.Clear;
 for Index:=0 to fVertices.Count-1 do begin
  fClusterMap.Add(TFBXMeshClusterMapItemList.Create);
 end;

 Skins:=FindConnectionsByType('Skin');
 try
  for Skin in Skins do begin
   Clusters:=Skin.FindConnectionsByType('Cluster');
   for Cluster in Clusters do begin
    if (length(TFBXCluster(Cluster).fIndexes)>0) and (length(TFBXCluster(Cluster).fWeights)>0) then begin
     ClusterMapItem.Cluster:=TFBXCluster(Cluster);
     for Index:=0 to length(TFBXCluster(Cluster).fIndexes)-1 do begin
      ClusterIndex:=TFBXCluster(Cluster).fIndexes[Index];
      ClusterMapItem.Weight:=TFBXCluster(Cluster).fWeights[Index];
      fClusterMap[ClusterIndex].Add(ClusterMapItem);
     end;
    end;
   end;
  end;
 finally
  Skins:=nil;
 end;

 if fLayers.Count>0 then begin
  Layer:=fLayers[0];
 end else begin
  Layer:=nil;
 end;

 HasBlendShapes:=false;

 if HasBlendShapes then begin
  PolygonVertexIndex:=0;
  for Polygon in fPolygons do begin
   for Index:=0 to Polygon.Count-1 do begin
    TriangleVertexIndex:=fTriangleVertices.Add(FillTriangleVertex(Polygon[WorkIndex],PolygonVertexIndex+Index));
   end;
   inc(PolygonVertexIndex,Polygon.Count);
  end;
 end else begin
  TriangleVertexMap:=TTriangleVertexMap.Create;
  try
   PolygonVertexIndex:=0;
   for Polygon in fPolygons do begin
    for Index:=0 to Polygon.Count-3 do begin
     for SubIndex:=0 to 2 do begin
      case SubIndex of
       1:begin
        WorkIndex:=Index+1;
       end;
       2:begin
        WorkIndex:=Index+2;
       end;
       else begin
        WorkIndex:=0;
       end;
      end;
      TriangleVertex:=FillTriangleVertex(Polygon[WorkIndex],PolygonVertexIndex+WorkIndex);
      if not TriangleVertexMap.TryGetValue(TriangleVertex,TriangleVertexIndex) then begin
       TriangleVertexIndex:=fTriangleVertices.Add(TriangleVertex);
       TriangleVertexMap.AddOrSetValue(TriangleVertex,TriangleVertexIndex);
      end;
      fTriangleIndices.Add(TriangleVertexIndex);
     end;
    end;
    inc(PolygonVertexIndex,Polygon.Count);
   end;
  finally
   TriangleVertexMap.Free;
  end;
 end;

end;

constructor TFBXSkeleton.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);
 if pType_='Root' then begin
  fSkeletonType:=ROOT;
 end else if pType_='Limb' then begin
  fSkeletonType:=LIMB;
 end else if pType_='LimbNode' then begin
  fSkeletonType:=LIMB_NODE;
 end else if pType_='Effector' then begin
  fSkeletonType:=EFFECTOR;
 end else begin
  fSkeletonType:=LIMB;
 end;
end;

destructor TFBXSkeleton.Destroy;
begin
 inherited Destroy;
end;

constructor TFBXMaterial.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,SubSubElement:TFBXElement;
    PropertyName:TFBXString;
    ValuePropertyIndex:Int32;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fShadingModel:='lambert';
 fMultiLayer:=false;
 fAmbientColor:=TFBXColorProperty.Create(0.0,0.0,0.0,1.0);
 fDiffuseColor:=TFBXColorProperty.Create(1.0,1.0,1.0,1.0);
 fTransparencyFactor:=1.0;
 fEmissive:=TFBXColorProperty.Create(0.0,0.0,0.0,1.0);
 fAmbient:=TFBXColorProperty.Create(0.0,0.0,0.0,1.0);
 fDiffuse:=TFBXColorProperty.Create(1.0,1.0,1.0,1.0);
 fOpacity:=1.0;
 fSpecular:=TFBXColorProperty.Create(1.0,1.0,1.0,1.0);
 fSpecularFactor:=0.0;
 fShininess:=1.0;
 fShininessExponent:=1.0;
 fReflection:=TFBXColorProperty.Create(1.0,1.0,1.0,1.0);
 fReflectionFactor:=0.0;

 for SubElement in pElement.Children do begin
  if SubElement.ID='ShadingModel' then begin
   fShadingModel:=SubElement.Properties[0].GetString;
  end else if (SubElement.ID='Properties60') or
              (SubElement.ID='Properties70') then begin
   if SubElement.ID='Properties60' then begin
    ValuePropertyIndex:=3;
   end else begin
    ValuePropertyIndex:=4;
   end;
   for SubSubElement in SubElement.Children do begin
    if (SubSubElement.ID='P') or (SubSubElement.ID='Property') then begin
     PropertyName:=SubSubElement.Properties[0].GetString;
     if PropertyName='ShadingModel' then begin
      fShadingModel:=SubSubElement.Properties[ValuePropertyIndex].GetString;
     end else if PropertyName='MultiLayer' then begin
      fMultiLayer:=SubSubElement.Properties[ValuePropertyIndex].GetBoolean;
     end else if PropertyName='AmbientColor' then begin
      fAmbientColor.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fAmbientColor.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fAmbientColor.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='DiffuseColor' then begin
      fDiffuseColor.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fDiffuseColor.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fDiffuseColor.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='TransparencyFactor' then begin
      fTransparencyFactor:=SubSubElement.Properties[ValuePropertyIndex].GetFloat;
     end else if PropertyName='Emissive' then begin
      fEmissive.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fEmissive.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fEmissive.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='Ambient' then begin
      fAmbient.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fAmbient.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fAmbient.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='Diffuse' then begin
      fDiffuse.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fDiffuse.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fDiffuse.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='Opacity' then begin
      fOpacity:=SubSubElement.Properties[ValuePropertyIndex].GetFloat;
     end else if PropertyName='Specular' then begin
      fSpecular.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fSpecular.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fSpecular.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='Shininess' then begin
      fShininess:=SubSubElement.Properties[ValuePropertyIndex].GetFloat;
     end else if PropertyName='ShininessExponent' then begin
      fShininessExponent:=SubSubElement.Properties[ValuePropertyIndex].GetFloat;
     end else if PropertyName='Reflection' then begin
      fReflection.Red:=SubSubElement.Properties[ValuePropertyIndex+0].GetFloat;
      fReflection.Green:=SubSubElement.Properties[ValuePropertyIndex+1].GetFloat;
      fReflection.Blue:=SubSubElement.Properties[ValuePropertyIndex+2].GetFloat;
     end else if PropertyName='ReflectionFactor' then begin
      fReflectionFactor:=SubSubElement.Properties[ValuePropertyIndex].GetFloat;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXMaterial.Destroy;
begin
 FreeAndNil(fAmbientColor);
 FreeAndNil(fDiffuseColor);
 FreeAndNil(fEmissive);
 FreeAndNil(fAmbient);
 FreeAndNil(fDiffuse);
 FreeAndNil(fSpecular);
 FreeAndNil(fReflection);
 inherited Destroy;
end;

procedure TFBXMaterial.ConnectTo(const pObject:TFBXObject);
begin
 if assigned(pObject) and (pObject is TFBXMesh) then begin
  TFBXMesh(pObject).fMaterials.Add(self);
 end;
 inherited ConnectTo(pObject);
end;

constructor TFBXAnimationStack.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,SubSubElement:TFBXElement;
    PropertyName:TFBXString;
    ValuePropertyIndex:Int32;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fDescription:='';
 fLocalStart:=0;
 fLocalStop:=0;
 fReferenceStart:=0;
 fReferenceStop:=0;

 if assigned(pElement) then begin
  for SubElement in pElement.Children do begin
   if (SubElement.ID='Properties60') or
      (SubElement.ID='Properties70') then begin
    if SubElement.ID='Properties60' then begin
     ValuePropertyIndex:=3;
    end else begin
     ValuePropertyIndex:=4;
    end;
    for SubSubElement in SubElement.Children do begin
     if (SubSubElement.ID='P') or (SubSubElement.ID='Property') then begin
      PropertyName:=SubSubElement.Properties[0].GetString;
      if PropertyName='Description' then begin
       fDescription:=SubSubElement.Properties[ValuePropertyIndex].GetString;
      end else if PropertyName='LocalStart' then begin
       fLocalStart:=SubSubElement.Properties[ValuePropertyIndex].GetInteger;
      end else if PropertyName='LocalStop' then begin
       fLocalStop:=SubSubElement.Properties[ValuePropertyIndex].GetInteger;
      end else if PropertyName='ReferenceStart' then begin
       fReferenceStart:=SubSubElement.Properties[ValuePropertyIndex].GetInteger;
      end else if PropertyName='ReferenceStop' then begin
       fReferenceStop:=SubSubElement.Properties[ValuePropertyIndex].GetInteger;
      end;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXAnimationStack.Destroy;
begin
 inherited Destroy;
end;

constructor TFBXAnimationLayer.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fWeight:=100.0;
 fMute:=false;
 fSolo:=false;
 fLock:=false;
 fColor:=TFBXColorProperty.Create(0.8,0.8,0.8,1.0);
 fBlendMode:=BLEND_ADDITIVE;
 fRotationAccumulationMode:=ROTATION_BY_LAYER;
 fScaleAccumulationMode:=SCALE_MULTIPLY;

 if assigned(pElement) then begin
  for SubElement in pElement.Children do begin
   if SubElement.ID='Weight' then begin
    fWeight:=SubElement.Properties[0].GetFloat;
   end else if SubElement.ID='Mute' then begin
    fMute:=SubElement.Properties[0].GetBoolean;
   end else if SubElement.ID='Solo' then begin
    fSolo:=SubElement.Properties[0].GetBoolean;
   end else if SubElement.ID='Lock' then begin
    fLock:=SubElement.Properties[0].GetBoolean;
   end else if SubElement.ID='Color' then begin
    fColor.Red:=SubElement.Properties[0].GetFloat;
    fColor.Green:=SubElement.Properties[1].GetFloat;
    fColor.Blue:=SubElement.Properties[2].GetFloat;
   end else if SubElement.ID='BlendMode' then begin
    fBlendMode:=SubElement.Properties[0].GetInteger;
   end else if SubElement.ID='RotationAccumulationMode' then begin
    fRotationAccumulationMode:=SubElement.Properties[0].GetInteger;
   end else if SubElement.ID='ScaleAccumulationMode' then begin
    fScaleAccumulationMode:=SubElement.Properties[0].GetInteger;
   end;
  end;
 end;

end;

destructor TFBXAnimationLayer.Destroy;
begin
 FreeAndNil(fColor);
 inherited Destroy;
end;

constructor TFBXSkinDeformer.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fLink_DeformAcuracy:=50;
 fSkinningType:=RIGID;

 for SubElement in pElement.Children do begin
  if SubElement.ID='Link_DeformAcuracy' then begin
   fLink_DeformAcuracy:=SubElement.Properties[0].GetInteger;
  end else if SubElement.ID='SkinningType' then begin
   if SubElement.Properties[0].GetString='Rigid' then begin
    fSkinningType:=RIGID;
   end else if SubElement.Properties[0].GetString='Linear' then begin
    fSkinningType:=LINEAR;
   end else if SubElement.Properties[0].GetString='DualQuaternion' then begin
    fSkinningType:=DUAL_QUATERNION;
   end else if SubElement.Properties[0].GetString='Blend' then begin
    fSkinningType:=BLEND;
   end;
  end;
 end;

end;

destructor TFBXSkinDeformer.Destroy;
begin
 inherited Destroy;
end;

constructor TFBXCluster.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,ArrayElement:TFBXElement;
    Index:UInt32;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fIndexes:=nil;
 fWeights:=nil;
 fTransform:=FBXMatrix4x4Identity;;
 fTransformLink:=FBXMatrix4x4Identity;
 fLinkMode:=NORMALIZE;

 for SubElement in pElement.Children do begin
  if SubElement.ID='Indexes' then begin
   if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
    SetLength(fIndexes,SubElement.Properties[0].GetArrayLength);
    for Index:=1 to SubElement.Properties[0].GetArrayLength do begin
     fIndexes[Index-1]:=SubElement.Properties[0].GetInteger(Index-1);
    end;
   end else begin
    if SubElement.Children.Count=1 then begin
     ArrayElement:=SubElement.Children[0];
    end else begin
     ArrayElement:=SubElement;
    end;
    if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
     SetLength(fIndexes,ArrayElement.Properties[0].GetArrayLength);
     for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
      fIndexes[Index-1]:=ArrayElement.Properties[0].GetInteger(Index-1);
     end;
    end else begin
     SetLength(fIndexes,ArrayElement.Properties.Count);
     for Index:=1 to ArrayElement.Properties.Count do begin
      fIndexes[Index-1]:=ArrayElement.Properties[Index-1].GetInteger;
     end;
    end;
   end;
  end else if SubElement.ID='Weights' then begin
   if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
    SetLength(fWeights,SubElement.Properties[0].GetArrayLength);
    for Index:=1 to SubElement.Properties[0].GetArrayLength do begin
     fWeights[Index-1]:=SubElement.Properties[0].GetFloat(Index-1);
    end;
   end else begin
    if SubElement.Children.Count=1 then begin
     ArrayElement:=SubElement.Children[0];
    end else begin
     ArrayElement:=SubElement;
    end;
    if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
     SetLength(fWeights,ArrayElement.Properties[0].GetArrayLength);
     for Index:=1 to ArrayElement.Properties[0].GetArrayLength do begin
      fWeights[Index-1]:=ArrayElement.Properties[0].GetFloat(Index-1);
     end;
    end else begin
     SetLength(fWeights,ArrayElement.Properties.Count);
     for Index:=1 to ArrayElement.Properties.Count do begin
      fWeights[Index-1]:=ArrayElement.Properties[Index-1].GetFloat;
     end;
    end;
   end;
  end else if SubElement.ID='Transform' then begin
   if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
    for Index:=1 to Min(16,SubElement.Properties[0].GetArrayLength) do begin
     fTransform.LinearRawComponents[Index-1]:=SubElement.Properties[0].GetFloat(Index-1);
    end;
   end else begin
    if SubElement.Children.Count=1 then begin
     ArrayElement:=SubElement.Children[0];
    end else begin
     ArrayElement:=SubElement;
    end;
    if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
     for Index:=1 to Min(16,ArrayElement.Properties[0].GetArrayLength) do begin
      fTransform.LinearRawComponents[Index-1]:=ArrayElement.Properties[0].GetFloat(Index-1);
     end;
    end else begin
     for Index:=1 to Min(16,ArrayElement.Properties.Count) do begin
      fTransform.LinearRawComponents[Index-1]:=ArrayElement.Properties[Index-1].GetFloat;
     end;
    end;
   end;
  end else if SubElement.ID='TransformLink' then begin
   if (SubElement.Properties.Count=1) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
    for Index:=1 to Min(16,SubElement.Properties[0].GetArrayLength) do begin
     fTransformLink.LinearRawComponents[Index-1]:=SubElement.Properties[0].GetFloat(Index-1);
    end;
   end else begin
    if SubElement.Children.Count=1 then begin
     ArrayElement:=SubElement.Children[0];
    end else begin
     ArrayElement:=SubElement;
    end;
    if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
     for Index:=1 to Min(16,ArrayElement.Properties[0].GetArrayLength) do begin
      fTransformLink.LinearRawComponents[Index-1]:=ArrayElement.Properties[0].GetFloat(Index-1);
     end;
    end else begin
     for Index:=1 to Min(16,ArrayElement.Properties.Count) do begin
      fTransformLink.LinearRawComponents[Index-1]:=ArrayElement.Properties[Index-1].GetFloat;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXCluster.Destroy;
begin
 fIndexes:=nil;
 fWeights:=nil;
 inherited Destroy;
end;

function TFBXCluster.GetLink:TFBXNode;
begin
 if fConnectedTo.Count>0 then begin
  result:=TFBXNode(fConnectedTo[0]);
 end else begin
  result:=nil;
 end;
end;

constructor TFBXTexture.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
begin
 inherited Create(pLoader,pElement,pID,pName,pType_);

 fFileName:='';

 for SubElement in pElement.Children do begin
  if SubElement.ID='FileName' then begin
   fFileName:=SubElement.Properties[0].GetString;
  end;
 end;

end;

destructor TFBXTexture.Destroy;
begin
 fFileName:='';
 inherited Destroy;
end;

constructor TFBXAnimationKey.Create;
begin
 inherited Create;
 fTime:=0;
 fValue:=0.0;
 fTangentMode:=TANGENT_AUTO;
 fInterpolation:=INTERPOLATION_LINEAR;
 fWeight:=WEIGHTED_NONE;
 fConstant:=CONSTANT_STANDARD;
 fVelocity:=VELOCITY_NONE;
 fVisibility:=VISIBILITY_NONE;
 fDataFloats:=DefaultDataFloats;
end;

destructor TFBXAnimationKey.Destroy;
begin
 inherited Destroy;
end;

constructor TFBXAnimationCurve.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
    Index,OtherIndex,YetOtherIndex,AttrCount,Flags:UInt32;
    Data0,Data1,Data2,Data3:Int64;
    KeyTime,KeyValue,KeyAttrFlags,KeyAttrDataFloat,KeyAttrRefCount:TFBXElementPropertyArray;
    Key:TFBXAnimationKey;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fDefaultValue:=0;

 fAnimationKeys:=TFBXAnimationKeyList.Create(true);

 KeyTime:=nil;
 KeyValue:=nil;
 KeyAttrFlags:=nil;
 KeyAttrDataFloat:=nil;
 KeyAttrRefCount:=nil;

 if assigned(pElement) then begin

  for SubElement in pElement.Children do begin
   if SubElement.ID='Default' then begin
    if SubElement.Properties.Count>0 then begin
     fDefaultValue:=SubElement.Properties[0].GetFloat;
    end;
   end else if SubElement.ID='KeyVer' then begin
    if SubElement.Properties.Count>0 then begin
     Data0:=SubElement.Properties[0].GetInteger;
     if (Data0<>4008) and (Data0<>4009) then begin
      raise EFBX.Create('Not implemented TFBXAnimationCurve KeyVer version '+IntToStr(Data0));
     end;
    end else begin
     raise EFBX.Create('Not implemented feature');
    end;
   end else if SubElement.ID='KeyTime' then begin
    if (SubElement.Properties.Count>0) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
     KeyTime:=TFBXElementPropertyArray(SubElement.Properties[0]);
    end;
   end else if SubElement.ID='KeyValueFloat' then begin
    if (SubElement.Properties.Count>0) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
     KeyValue:=TFBXElementPropertyArray(SubElement.Properties[0]);
    end;
   end else if SubElement.ID='KeyAttrFlags' then begin
    if (SubElement.Properties.Count>0) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
     KeyAttrFlags:=TFBXElementPropertyArray(SubElement.Properties[0]);
    end;
   end else if SubElement.ID='KeyAttrDataFloat' then begin
    if (SubElement.Properties.Count>0) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
     KeyAttrDataFloat:=TFBXElementPropertyArray(SubElement.Properties[0]);
    end;
   end else if SubElement.ID='KeyAttrRefCount' then begin
    if (SubElement.Properties.Count>0) and (SubElement.Properties[0] is TFBXElementPropertyArray) then begin
     KeyAttrRefCount:=TFBXElementPropertyArray(SubElement.Properties[0]);
    end;
   end;
  end;

  if (assigned(KeyTime) and assigned(KeyValue)) and
     (Min(KeyTime.GetArrayLength,KeyValue.GetArrayLength)>0) then begin

   for Index:=1 to Min(KeyTime.GetArrayLength,KeyValue.GetArrayLength) do begin
    Key:=TFBXAnimationKey.Create;
    fAnimationKeys.Add(Key);
    Key.fTime:=KeyTime.GetInteger(Index-1);
    Key.fValue:=KeyTime.GetFloat(Index-1);
   end;

   if assigned(KeyAttrRefCount) and
      assigned(KeyAttrDataFloat) and
      assigned(KeyAttrFlags) then begin

    OtherIndex:=0;
    YetOtherIndex:=0;
    for Index:=1 to KeyAttrRefCount.GetArrayLength do begin
     AttrCount:=KeyAttrRefCount.GetInteger(Index-1);
     Data0:=KeyAttrDataFloat.GetInteger((YetOtherIndex shl 2) or 0);
     Data1:=KeyAttrDataFloat.GetInteger((YetOtherIndex shl 2) or 1);
     Data2:=KeyAttrDataFloat.GetInteger((YetOtherIndex shl 2) or 2);
     Data3:=KeyAttrDataFloat.GetInteger((YetOtherIndex shl 2) or 3);
     Flags:=KeyAttrFlags.GetInteger(YetOtherIndex);
     while AttrCount>0 do begin
      dec(AttrCount);
      if TFBXSizeInt(OtherIndex)<TFBXSizeInt(fAnimationKeys.Count) then begin
       Key:=fAnimationKeys[OtherIndex];
       Key.fTangentMode:=Flags and TFBXAnimationKey.TANGENT_MASK;
       Key.fInterpolation:=Flags and TFBXAnimationKey.INTERPOLATION_MASK;
       Key.fWeight:=Flags and TFBXAnimationKey.WEIGHT_MASK;
       Key.fConstant:=Flags and TFBXAnimationKey.CONSTANT_MASK;
       Key.fVelocity:=Flags and TFBXAnimationKey.VELOCITY_MASK;
       Key.fVisibility:=Flags and TFBXAnimationKey.VISIBILITY_MASK;
       Key.fDataFloats[TFBXAnimationKey.RightWeightIndex]:=(Data2 and $0000ffff)/9999.0;
       Key.fDataFloats[TFBXAnimationKey.NextLeftWeightIndex]:=((Data2 shr 16) and $0000ffff)/9999.0;
      end else begin
       break;
      end;
      inc(OtherIndex);
     end;
     if TFBXSizeInt(OtherIndex)<TFBXSizeInt(fAnimationKeys.Count) then begin
      inc(YetOtherIndex);
     end else begin
      break;
     end;
    end;

   end;

  end;

 end;

end;

constructor TFBXAnimationCurve.CreateOldFBX6000(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,ArrayElement:TFBXElement;
    Index:UInt32;
    Data0:Int64;
    Key:TFBXAnimationKey;
    InterpolationType:TFBXString;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fDefaultValue:=0;

 fAnimationKeys:=TFBXAnimationKeyList.Create(true);

 if assigned(pElement) then begin

  ArrayElement:=nil;

  for SubElement in pElement.Children do begin
   if SubElement.ID='Default' then begin
    if SubElement.Properties.Count>0 then begin
     fDefaultValue:=SubElement.Properties[0].GetFloat;
    end;
   end else if SubElement.ID='KeyVer' then begin
    if SubElement.Properties.Count>0 then begin
     Data0:=SubElement.Properties[0].GetInteger;
     if (Data0<>4005) and (Data0<>4006) and (Data0<>4007) then begin
      raise EFBX.Create('Not implemented TFBXAnimationCurve KeyVer version '+IntToStr(Data0));
     end;
    end else begin
     raise EFBX.Create('Not implemented feature');
    end;
   end else if SubElement.ID='Key' then begin
    ArrayElement:=SubElement;
   end;
  end;

  if assigned(ArrayElement) then begin
   Index:=0;
   while Index<Int64(ArrayElement.fProperties.Count) do begin
    Key:=TFBXAnimationKey.Create;
    fAnimationKeys.Add(Key);

    Key.fTime:=ArrayElement.Properties[Index].GetInteger;
    inc(Index);

    if Index<Int64(ArrayElement.fProperties.Count) then begin
     Key.fValue:=ArrayElement.Properties[Index].GetFloat;
     inc(Index);
    end else begin
     break;
    end;

    if Index<Int64(ArrayElement.fProperties.Count) then begin
     InterpolationType:=ArrayElement.Properties[Index].GetString;
     inc(Index);

     if InterpolationType='C' then begin
      Key.fInterpolation:=TFBXAnimationKey.INTERPOLATION_CONSTANT;
      if ArrayElement.Properties[Index].GetString='s' then begin
       Key.fConstant:=TFBXAnimationKey.CONSTANT_STANDARD;
      end else if ArrayElement.Properties[Index].GetString='n' then begin
       Key.fConstant:=TFBXAnimationKey.CONSTANT_NEXT;
      end;
      inc(Index);
     end else if InterpolationType='L' then begin
      Key.fInterpolation:=TFBXAnimationKey.INTERPOLATION_LINEAR;
     end else if InterpolationType='true' then begin
      Key.fInterpolation:=TFBXAnimationKey.INTERPOLATION_CUBIC;
      Key.fDataFloats[TFBXAnimationKey.RightWeightIndex]:=ArrayElement.Properties[Index+0].GetFloat;
      Key.fDataFloats[TFBXAnimationKey.NextLeftWeightIndex]:=ArrayElement.Properties[Index+1].GetFloat;
      inc(Index,2);
     end else begin
      Key.fInterpolation:=TFBXAnimationKey.INTERPOLATION_CUBIC;
      if ArrayElement.Properties[Index].GetString='s' then begin
       Key.fConstant:=TFBXAnimationKey.CONSTANT_STANDARD;
      end else if ArrayElement.Properties[Index].GetString='n' then begin
       Key.fConstant:=TFBXAnimationKey.CONSTANT_NEXT;
      end;
      Key.fDataFloats[TFBXAnimationKey.RightWeightIndex]:=ArrayElement.Properties[Index+1].GetFloat;
      Key.fDataFloats[TFBXAnimationKey.NextLeftWeightIndex]:=ArrayElement.Properties[Index+2].GetFloat;
      inc(Index,4);
     end;

    end else begin
     break;
    end;

   end;
  end;

 end;

end;

destructor TFBXAnimationCurve.Destroy;
begin
 FreeAndNil(fAnimationKeys);
 inherited Destroy;
end;

constructor TFBXPose.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement,SubSubElement,ArrayElement:TFBXElement;
    Index:Int32;
    Matrix:TFBXMatrix4x4;
    NodeName:TFBXString;
    Node:TFBXObject;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fPoseType:='BindPose';
 fNodeMatrixMap:=TFBXPoseNodeMatrixMap.Create;

 for SubElement in pElement.Children do begin
  if SubElement.ID='Type' then begin
   fPoseType:=SubElement.Properties[0].GetString;
  end else if SubElement.ID='PoseNode' then begin
   Matrix:=FBXMatrix4x4Identity;
   NodeName:=TFBXString(#0#1#2);
   for SubSubElement in SubElement.Children do begin
    if SubSubElement.ID='Node' then begin
     NodeName:=SubSubElement.Properties[0].GetString;
    end else if SubSubElement.ID='Matrix' then begin
     if (SubSubElement.Properties.Count=1) and (SubSubElement.Properties[0] is TFBXElementPropertyArray) then begin
      for Index:=1 to Min(16,SubSubElement.Properties[0].GetArrayLength) do begin
       Matrix.LinearRawComponents[Index-1]:=SubSubElement.Properties[0].GetFloat(Index-1);
      end;
     end else begin
      if SubSubElement.Children.Count=1 then begin
       ArrayElement:=SubSubElement.Children[0];
      end else begin
       ArrayElement:=SubSubElement;
      end;
      if (ArrayElement.Properties.Count=1) and (ArrayElement.Properties[0] is TFBXElementPropertyArray) then begin
       for Index:=1 to Min(16,ArrayElement.Properties[0].GetArrayLength) do begin
        Matrix.LinearRawComponents[Index-1]:=ArrayElement.Properties[0].GetFloat(Index-1);
       end;
      end else begin
       for Index:=1 to Min(16,ArrayElement.Properties.Count) do begin
        Matrix.LinearRawComponents[Index-1]:=ArrayElement.Properties[Index-1].GetFloat;
       end;
      end;
     end;
    end;
   end;
   if length(NodeName)>0 then begin
    if fLoader.fScene.AllObjects.TryGetValue(NodeName,Node) then begin
     if assigned(Node) then begin
      if Node is TFBXNode then begin
       fNodeMatrixMap.AddOrSetValue(TFBXNode(Node),Matrix);
      end else begin
       raise EFBX.Create('Wrong object type of "'+String(NodeName)+'", it must be TFBXNode and not '+Node.ClassName);
      end;
     end;
    end;
   end;
  end;
 end;

end;

destructor TFBXPose.Destroy;
begin
 FreeAndNil(fNodeMatrixMap);
 inherited Destroy;
end;

function TFBXPose.GetMatrix(const pNode:TFBXNode):TFBXMatrix4x4;
begin
 result:=FBXMatrix4x4Identity;
 if not fNodeMatrixMap.TryGetValue(pNode,result) then begin
  result:=FBXMatrix4x4Identity;
 end;
end;

constructor TFBXVideo.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fFileName:='';
 fUseMipMap:=false;

 for SubElement in pElement.Children do begin
  if (SubElement.ID='Filename') or (SubElement.ID='FileName') then begin
   fFileName:=SubElement.Properties[0].GetString;
  end else if SubElement.ID='UseMipMap' then begin
   fUseMipMap:=SubElement.Properties[0].GetBoolean;
  end;
 end;

end;

destructor TFBXVideo.Destroy;
begin
 fFileName:='';
 inherited Destroy;
end;

constructor TFBXTake.Create(const pLoader:TFBXLoader;const pElement:TFBXElement;const pID:Int64;const pName,pType_:TFBXString);
var SubElement:TFBXElement;
begin

 inherited Create(pLoader,pElement,pID,pName,pType_);

 fFileName:='';
 fLocalTimeSpan.StartTime:=0;
 fLocalTimeSpan.EndTime:=0;
 fReferenceTimeSpan.StartTime:=0;
 fReferenceTimeSpan.EndTime:=0;

 for SubElement in pElement.Children do begin
  if (SubElement.ID='Filename') or (SubElement.ID='FileName') then begin
   fFileName:=SubElement.Properties[0].GetString;
  end else if SubElement.ID='LocalTime' then begin
   fLocalTimeSpan.StartTime:=SubElement.Properties[0].GetInteger;
   fLocalTimeSpan.EndTime:=SubElement.Properties[1].GetInteger;
  end else if SubElement.ID='ReferenceTime' then begin
   fReferenceTimeSpan.StartTime:=SubElement.Properties[0].GetInteger;
   fReferenceTimeSpan.EndTime:=SubElement.Properties[1].GetInteger;
  end;
 end;

end;

destructor TFBXTake.Destroy;
begin
 fFileName:='';
 inherited Destroy;
end;

constructor TFBXLoader.Create;
begin
 inherited Create;
 fAllocatedList:=TFBXAllocatedList.Create(true);
 fScene:=nil;
 fGlobalSettings:=nil;
 fTimeUtils:=TFBXTimeUtils.Create(fGlobalSettings);
 fRootElement:=nil;
end;

destructor TFBXLoader.Destroy;
begin
 FreeAndNil(fTimeUtils);
 FreeAndNil(fGlobalSettings);
 FreeAndNil(fScene);
 FreeAndNil(fRootElement);
 while fAllocatedList.Count>0 do begin
  fAllocatedList[fAllocatedList.Count-1].Free;
 end;
 FreeAndNil(fAllocatedList);
 inherited Destroy;
end;

procedure TFBXLoader.LoadFromStream(const pStream:TStream);
var Parser:TFBXParser;
 procedure ProcessFBXHeaderExtension(const pFBXHeaderExtensionElement:TFBXElement);
 var Element,SubElement:TFBXElement;
 begin
  for Element in pFBXHeaderExtensionElement.Children do begin
   if Element.ID='OtherFlags' then begin
    for SubElement in Element.Children do begin
     if SubElement.Properties.Count=1 then begin
      fScene.fHeader.AddProperty(SubElement.ID,SubElement.Properties[0].GetVariantValue);
     end;
    end;
   end else begin
    if Element.Properties.Count=1 then begin
     if Element.ID='FBXVersion' then begin
      fFileVersion:=Element.Properties[0].GetInteger;
     end;
     fScene.fHeader.AddProperty(Element.ID,Element.Properties[0].GetVariantValue);
    end;
   end;
  end;
 end;
 procedure ProcessGlobalSettings(const pGlobalSettingsElement:TFBXElement);
 var Properties,PropertyElement:TFBXElement;
     ValuePropertyIndex:Int32;
     PropertyName:TFBXString;
 begin
  if not assigned(fGlobalSettings) then begin
   fGlobalSettings:=TFBXGlobalSettings.Create(self,pGlobalSettingsElement,0,TFBXString(#0'GlobalSettings'#255),'');
  end;
  if pGlobalSettingsElement.ChildrenByName.TryGetValue('Properties60',Properties) then begin
   ValuePropertyIndex:=3;
  end else if pGlobalSettingsElement.ChildrenByName.TryGetValue('Properties70',Properties) then begin
   ValuePropertyIndex:=4;
  end else begin
   Properties:=nil;
   ValuePropertyIndex:=0;
  end;
  if assigned(Properties) then begin
   for PropertyElement in Properties.Children do begin
    if (PropertyElement.ID='P') and (PropertyElement.fProperties.Count>0) then begin
     PropertyName:=PropertyElement.fProperties[0].GetString;
     if (PropertyName='UpAxis') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fUpAxis:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='UpAxisSign') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fUpAxisSign:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='FrontAxis') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fFrontAxis:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='FrontAxisSign') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fFrontAxisSign:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='CoordAxis') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fCoordAxis:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='CoordAxisSign') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fCoordAxisSign:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='OriginalUpAxis') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fOriginalUpAxis:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='OriginalUpAxisSign') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fOriginalUpAxisSign:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='UnitScaleFactor') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fUnitScaleFactor:=PropertyElement.fProperties[ValuePropertyIndex].GetFloat;
      continue;
     end;
     if (PropertyName='OriginalUnitScaleFactor') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fOriginalUnitScaleFactor:=PropertyElement.fProperties[ValuePropertyIndex].GetFloat;
      continue;
     end;
     if (PropertyName='AmbientColor') and
        ((ValuePropertyIndex+2)<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fAmbientColor.Red:=PropertyElement.fProperties.Items[ValuePropertyIndex+0].GetFloat;
      fGlobalSettings.fAmbientColor.Green:=PropertyElement.fProperties.Items[ValuePropertyIndex+1].GetFloat;
      fGlobalSettings.fAmbientColor.Blue:=PropertyElement.fProperties.Items[ValuePropertyIndex+2].GetFloat;
      continue;
     end;
     if (PropertyName='DefaultCamera') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fDefaultCamera:=PropertyElement.fProperties[ValuePropertyIndex].GetString;
      continue;
     end;
     if (PropertyName='TimeMode') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fTimeMode:=TFBXTimeMode(PropertyElement.fProperties[ValuePropertyIndex].GetInteger);
      continue;
     end;
     if (PropertyName='TimeProtocol') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fTimeProtocol:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='SnapOnFrameMode') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fSnapOnFrameMode:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='TimeSpanStart') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fTimeSpan.StartTime:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='TimeSpanStop') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fTimeSpan.EndTime:=PropertyElement.fProperties[ValuePropertyIndex].GetInteger;
      continue;
     end;
     if (PropertyName='CustomFrameRate') and
        (ValuePropertyIndex<PropertyElement.fProperties.Count) then begin
      fGlobalSettings.fCustomFrameRate:=PropertyElement.fProperties[ValuePropertyIndex].GetFloat;
      continue;
     end;
     if ValuePropertyIndex<PropertyElement.fProperties.Count then begin
      fGlobalSettings.AddProperty(PropertyName,PropertyElement.fProperties[ValuePropertyIndex].GetVariantValue);
     end;
    end;
   end;
  end;
 end;
 procedure ProcessObjects(const pObjectsElement:TFBXElement);
  procedure ProcessModel(const pModelElement:TFBXElement);
  var ID:Int64;
      RawName,Type_,Name:TFBXString;
      StringList:TStringList;
      Node,MeshNode:TFBXNode;
      Camera:TFBXCamera;
      Light:TFBXLight;
      Mesh:TFBXMesh;
      Skeleton:TFBXSkeleton;
  begin
   if pModelElement.fProperties.Count=3 then begin
    ID:=pModelElement.fProperties[0].GetInteger;
    RawName:=pModelElement.fProperties[1].GetString;
    Type_:=pModelElement.fProperties[2].GetString;
   end else begin
    ID:=0;
    RawName:=pModelElement.fProperties[0].GetString;
    Type_:=pModelElement.fProperties[1].GetString;
   end;
   Name:=Parser.GetName(RawName);
   if Type_='Camera' then begin
    Camera:=TFBXCamera.Create(self,pModelElement,ID,Name,Type_);
    if ID<>0 then begin
     fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Camera);
    end;
    fScene.fAllObjects.AddOrSetValue(RawName,Camera);
    fScene.fAllObjects.AddOrSetValue(Name,Camera);
    fScene.fCameras.Add(Camera);
   end else if Type_='Light' then begin
    Light:=TFBXLight.Create(self,pModelElement,ID,Name,Type_);
    if ID<>0 then begin
     fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Light);
    end;
    fScene.fAllObjects.AddOrSetValue(RawName,Light);
    fScene.fAllObjects.AddOrSetValue(Name,Light);
    fScene.fLights.Add(Light);
   end else if Type_='Mesh' then begin
    if ID=0 then begin
     // For older versions of FBX
     MeshNode:=TFBXNode.Create(self,pModelElement,ID,Name,'Transform');
     fScene.fAllObjects.AddOrSetValue(RawName,MeshNode);
     fScene.fAllObjects.AddOrSetValue(Name,MeshNode);
     Mesh:=TFBXMesh.Create(self,pModelElement,ID,Name,'Mesh');
     fScene.fMeshes.Add(Mesh);
     MeshNode.ConnectTo(Mesh);
    end else begin
     // For newer versions of FBX
     Node:=TFBXNode.Create(self,pModelElement,ID,Name,'Transform');
     if ID<>0 then begin
      fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Node);
     end;
     fScene.fAllObjects.AddOrSetValue(RawName,Node);
     fScene.fAllObjects.AddOrSetValue(Name,Node);
    end;
   end else if (Type_='Limb') or (Type_='LimbNode') then begin
    if Type_='Limb' then begin
     Skeleton:=TFBXSkeletonLimb.Create(self,pModelElement,ID,Name,Type_);
    end else begin
     Skeleton:=TFBXSkeletonLimbNode.Create(self,pModelElement,ID,Name,Type_);
    end;
    if ID<>0 then begin
     fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Skeleton);
    end;
    fScene.fAllObjects.AddOrSetValue(RawName,Skeleton);
    fScene.fAllObjects.AddOrSetValue(Name,Skeleton);
    fScene.fSkeletons.Add(Skeleton);
   end else begin
    StringList:=TStringList.Create;
    try
     StringList.Delimiter:=':';
     StringList.StrictDelimiter:=true;
     StringList.DelimitedText:=String(Name);
     if StringList.Count>1 then begin
      Name:=TFBXString(StringList[1]);
      Node:=TFBXNode.Create(self,pModelElement,ID,Name,Type_);
      Node.fReference:=TFBXString(StringList[0]);
      if ID<>0 then begin
       fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Node);
      end;
      fScene.fAllObjects.AddOrSetValue(RawName,Node);
      fScene.fAllObjects.AddOrSetValue(Name,Node);
     end else begin
      Node:=TFBXNode.Create(self,pModelElement,ID,Name,Type_);
      if ID<>0 then begin
       fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Node);
      end;
      fScene.fAllObjects.AddOrSetValue(RawName,Node);
      fScene.fAllObjects.AddOrSetValue(Name,Node);
     end;
    finally
     StringList.Free;
    end;
   end;
  end;
  procedure ProcessGeometry(const pGeometryElement:TFBXElement);
  var ID:Int64;
      Type_:TFBXString;
      Mesh:TFBXMesh;
  begin
   ID:=pGeometryElement.fProperties[0].GetInteger;
   Type_:=pGeometryElement.fProperties[2].GetString;
   if (Type_='Mesh') or (Type_='Shape') then begin
    Mesh:=TFBXMesh.Create(self,pGeometryElement,ID,TFBXString(IntToStr(ID)),'Mesh');
    if ID<>0 then begin
     fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Mesh);
    end;
    fScene.fMeshes.Add(Mesh);
   end;
  end;
  procedure ProcessMaterial(const pMaterialElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      Material:TFBXMaterial;
  begin
   if pMaterialElement.fProperties.Count=3 then begin
    ID:=pMaterialElement.fProperties[0].GetInteger;
    RawName:=pMaterialElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pMaterialElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   Material:=TFBXMaterial.Create(self,pMaterialElement,ID,Name,'Material');
   fScene.AllObjects.AddOrSetValue(RawName,Material);
   fScene.AllObjects.AddOrSetValue(Name,Material);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Material);
   end;
   fScene.fMaterials.Add(Material);
  end;
  procedure ProcessAnimationStack(const pAnimationStackElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      AnimationStack:TFBXAnimationStack;
  begin
   if pAnimationStackElement.fProperties.Count=3 then begin
    ID:=pAnimationStackElement.fProperties[0].GetInteger;
    RawName:=pAnimationStackElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pAnimationStackElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   AnimationStack:=TFBXAnimationStack.Create(self,pAnimationStackElement,ID,Name,'AnimStack');
   fScene.AllObjects.AddOrSetValue(RawName,AnimationStack);
   fScene.AllObjects.AddOrSetValue(Name,AnimationStack);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),AnimationStack);
   end;
   fScene.fAnimationStackList.Add(AnimationStack);
  end;
  procedure ProcessAnimationLayer(const pAnimationLayerElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      AnimationLayer:TFBXAnimationLayer;
  begin
   if pAnimationLayerElement.fProperties.Count=3 then begin
    ID:=pAnimationLayerElement.fProperties[0].GetInteger;
    RawName:=pAnimationLayerElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pAnimationLayerElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   AnimationLayer:=TFBXAnimationLayer.Create(self,pAnimationLayerElement,ID,Name,'AnimLayer');
   fScene.AllObjects.AddOrSetValue(RawName,AnimationLayer);
   fScene.AllObjects.AddOrSetValue(Name,AnimationLayer);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),AnimationLayer);
   end;
  end;
  procedure ProcessAnimationCurveNode(const pAnimationCurveNodeElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      AnimationCurveNode:TFBXAnimationCurveNode;
  begin
   if pAnimationCurveNodeElement.fProperties.Count=3 then begin
    ID:=pAnimationCurveNodeElement.fProperties[0].GetInteger;
    RawName:=pAnimationCurveNodeElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pAnimationCurveNodeElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   AnimationCurveNode:=TFBXAnimationCurveNode.Create(self,pAnimationCurveNodeElement,ID,Name,'AnimCurveNode');
   fScene.AllObjects.AddOrSetValue(RawName,AnimationCurveNode);
   fScene.AllObjects.AddOrSetValue(Name,AnimationCurveNode);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),AnimationCurveNode);
   end;
  end;
  procedure ProcessDeformer(const pDeformerElement:TFBXElement);
  var ID:Int64;
      RawName,Name,Type_:TFBXString;
      Deformer:TFBXDeformer;
  begin
   if pDeformerElement.fProperties.Count=3 then begin
    ID:=pDeformerElement.fProperties[0].GetInteger;
    RawName:=pDeformerElement.fProperties[1].GetString;
    Type_:=pDeformerElement.fProperties[2].GetString;
   end else begin
    ID:=0;
    RawName:=pDeformerElement.fProperties[0].GetString;
    Type_:=pDeformerElement.fProperties[1].GetString;
   end;
   Name:=Parser.GetName(RawName);
   if Type_='Skin' then begin
    Deformer:=TFBXSkinDeformer.Create(self,pDeformerElement,ID,Name,'Skin');
   end else if Type_='Cluster' then begin
    Deformer:=TFBXCluster.Create(self,pDeformerElement,ID,Name,'Cluster');
   end else begin
    Deformer:=nil;
   end;
   if assigned(Deformer) then begin
    fScene.AllObjects.AddOrSetValue(RawName,Deformer);
    fScene.AllObjects.AddOrSetValue(Name,Deformer);
    if ID<>0 then begin
     fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Deformer);
    end;
    fScene.fDeformers.Add(Deformer);
   end;
  end;
  procedure ProcessTexture(const pTextureElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      Texture:TFBXTexture;
  begin
   if pTextureElement.fProperties.Count=3 then begin
    ID:=pTextureElement.fProperties[0].GetInteger;
    RawName:=pTextureElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pTextureElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   Texture:=TFBXTexture.Create(self,pTextureElement,ID,Name,'Texture');
   fScene.AllObjects.AddOrSetValue(RawName,Texture);
   fScene.AllObjects.AddOrSetValue(Name,Texture);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Texture);
   end;
   fScene.fTextures.Add(Texture);
  end;
  procedure ProcessFolder(const pFolderElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      Folder:TFBXFolder;
  begin
   if pFolderElement.fProperties.Count=3 then begin
    ID:=pFolderElement.fProperties[0].GetInteger;
    RawName:=pFolderElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pFolderElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   Folder:=TFBXFolder.Create(self,pFolderElement,ID,Name,'Folder');
   fScene.AllObjects.AddOrSetValue(RawName,Folder);
   fScene.AllObjects.AddOrSetValue(Name,Folder);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Folder);
   end;
  end;
  procedure ProcessConstraint(const pConstraintElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      Constraint:TFBXConstraint;
  begin
   if pConstraintElement.fProperties.Count=3 then begin
    ID:=pConstraintElement.fProperties[0].GetInteger;
    RawName:=pConstraintElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pConstraintElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   Constraint:=TFBXConstraint.Create(self,pConstraintElement,ID,Name,'Constraint');
   fScene.AllObjects.AddOrSetValue(RawName,Constraint);
   fScene.AllObjects.AddOrSetValue(Name,Constraint);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Constraint);
   end;
  end;
  procedure ProcessAnimationCurve(const pAnimationCurveElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      AnimationCurve:TFBXAnimationCurve;
  begin
   if pAnimationCurveElement.fProperties.Count=3 then begin
    ID:=pAnimationCurveElement.fProperties[0].GetInteger;
    RawName:=pAnimationCurveElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pAnimationCurveElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   AnimationCurve:=TFBXAnimationCurve.Create(self,pAnimationCurveElement,ID,Name,'AnimCurve');
   fScene.AllObjects.AddOrSetValue(RawName,AnimationCurve);
   fScene.AllObjects.AddOrSetValue(Name,AnimationCurve);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),AnimationCurve);
   end;
  end;
  procedure ProcessNodeAttribute(const pNodeAttributeElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      NodeAttribute:TFBXNodeAttribute;
  begin
   if pNodeAttributeElement.fProperties.Count=3 then begin
    ID:=pNodeAttributeElement.fProperties[0].GetInteger;
    RawName:=pNodeAttributeElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pNodeAttributeElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   NodeAttribute:=TFBXNodeAttribute.Create(self,pNodeAttributeElement,ID,Name,'NodeAttribute');
   fScene.AllObjects.AddOrSetValue(RawName,NodeAttribute);
   fScene.AllObjects.AddOrSetValue(Name,NodeAttribute);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),NodeAttribute);
   end;
  end;
  procedure ProcessSceneInfo(const pSceneInfoElement:TFBXElement);
  begin
   FreeAndNil(fScene.fSceneInfo);
   fScene.fSceneInfo:=TFBXSceneInfo.Create(self,pSceneInfoElement,0,pSceneInfoElement.ID,'SceneInfo');
  end;
  procedure ProcessPose(const pPoseElement:TFBXElement);
  begin
   fScene.fPoses.Add(TFBXPose.Create(self,pPoseElement,0,pPoseElement.Properties[0].GetString,pPoseElement.Properties[1].GetString));
  end;
  procedure ProcessVideo(const pVideoElement:TFBXElement);
  var ID:Int64;
      RawName,Name:TFBXString;
      Video:TFBXVideo;
  begin
   if pVideoElement.fProperties.Count=3 then begin
    ID:=pVideoElement.fProperties[0].GetInteger;
    RawName:=pVideoElement.fProperties[1].GetString;
   end else begin
    ID:=0;
    RawName:=pVideoElement.fProperties[0].GetString;
   end;
   Name:=Parser.GetName(RawName);
   Video:=TFBXVideo.Create(self,pVideoElement,ID,Name,'Video');
   fScene.AllObjects.AddOrSetValue(RawName,Video);
   fScene.AllObjects.AddOrSetValue(Name,Video);
   if ID<>0 then begin
    fScene.AllObjects.AddOrSetValue(TFBXString(IntToStr(ID)),Video);
   end;
   fScene.fVideos.Add(Video);
  end;
 var Element:TFBXElement;
 begin
  for Element in pObjectsElement.Children do begin
   if Element.ID='Model' then begin
    ProcessModel(Element);
   end else if Element.ID='Geometry' then begin
    ProcessGeometry(Element);
   end else if Element.ID='Material' then begin
    ProcessMaterial(Element);
   end else if Element.ID='AnimationStack' then begin
    ProcessAnimationStack(Element);
   end else if Element.ID='AnimationLayer' then begin
    ProcessAnimationLayer(Element);
   end else if Element.ID='AnimationCurveNode' then begin
    ProcessAnimationCurveNode(Element);
   end else if Element.ID='Deformer' then begin
    ProcessDeformer(Element);
   end else if Element.ID='Texture' then begin
    ProcessTexture(Element);
   end else if Element.ID='Folder' then begin
    ProcessFolder(Element);
   end else if Element.ID='Constraint' then begin
    ProcessConstraint(Element);
   end else if Element.ID='AnimationCurve' then begin
    ProcessAnimationCurve(Element);
   end else if Element.ID='NodeAttribute' then begin
    ProcessNodeAttribute(Element);
   end else if Element.ID='GlobalSettings' then begin
    ProcessGlobalSettings(Element);
   end else if Element.ID='SceneInfo' then begin
    ProcessSceneInfo(Element);
   end else if Element.ID='Pose' then begin
    ProcessPose(Element);
   end else if Element.ID='Video' then begin
    ProcessVideo(Element);
   end;
  end;
 end;
 procedure ProcessConnections(const pConnectionsElement:TFBXElement);
 var Element:TFBXElement;
     Type_,Src,Dst,Attribute:TFBXString;
     SrcObject,DstObject:TFBXObject;
     StringList:TStringList;
     Mesh:TFBXMesh;
 begin
  for Element in pConnectionsElement.Children do begin
   if (Element.ID='C') or (Element.ID='Connect') then begin
    Type_:=Element.Properties[0].GetString;
    if Type_='OO' then begin
     Src:=Element.Properties[1].GetString;
     Dst:=Element.Properties[2].GetString;
     if fScene.fAllObjects.TryGetValue(Src,SrcObject) then begin
      if assigned(SrcObject) then begin
       if (Dst='0') or (Dst=Parser.SceneName) then begin
        fScene.fRootNodes.Add(SrcObject);
       end else if fScene.fAllObjects.TryGetValue(Dst,DstObject) then begin
        if assigned(DstObject) then begin
         DstObject.ConnectTo(SrcObject);
        end;
       end;
      end;
     end;
    end else if Type_='OP' then begin
     Src:=Element.Properties[1].GetString;
     Dst:=Element.Properties[2].GetString;
     Attribute:=Element.Properties[3].GetString;
     if length(Attribute)>0 then begin
      if Pos('|',String(Attribute))>0 then begin
       StringList:=TStringList.Create;
       try
        StringList.Delimiter:='|';
        StringList.StrictDelimiter:=true;
        StringList.DelimitedText:=String(Attribute);
        if StringList.Count>0 then begin
         Attribute:=TFBXString(StringList[StringList.Count-1]);
        end;
       finally
        StringList.Free;
       end;
      end;
      if length(Attribute)>0 then begin
       if fScene.fAllObjects.TryGetValue(Src,SrcObject) then begin
        if assigned(SrcObject) then begin
         if fScene.fAllObjects.TryGetValue(Dst,DstObject) then begin
          if assigned(DstObject) then begin
           DstObject.ConnectToProperty(SrcObject,Attribute);
          end;
         end;
        end;
       end;
      end;
     end;
    end;
   end;
  end;
  begin
   // Older FBX versions are connecting deformers to the transform instead of the mesh. So, that must fix it here
   for Mesh in fScene.fMeshes do begin
    if Mesh.fConnectedFrom.Count>0 then begin
     for SrcObject in Mesh.fConnectedFrom do begin
      if SrcObject is TFBXNode then begin
       for DstObject in SrcObject.fConnectedTo do begin
        if DstObject is TFBXDeformer then begin
         if not Mesh.fConnectedTo.Contains(DstObject) then begin
          Mesh.ConnectTo(DstObject);
         end;
        end;
       end;
      end;
     end;
    end;
   end;
  end;
 end;
 procedure ProcessTakes(const pTakesElement:TFBXElement);
 var CurrentTakeName:TFBXString;
  procedure ProcessTake(const pTakeElement:TFBXElement);
  var Element,SubElement,SubSubElement,SubSubSubElement:TFBXElement;
      TakeName,Name,SubElementKind,SubSubElementKind,SubSubSubElementKind:TFBXString;
      Object_:TFBXObject;
      Take:TFBXTake;
      AnimationStack:TFBXAnimationStack;
      AnimationLayer:TFBXAnimationLayer;
      AnimationCurveNode:TFBXAnimationCurveNode;
      AnimationCurve:TFBXAnimationCurve;
  begin
   TakeName:=pTakeElement.Properties[0].GetString;
   Take:=TFBXTake.Create(self,pTakeElement,0,TakeName,'Take');
   fScene.fTakes.Add(Take);
   if CurrentTakeName=TakeName then begin
    fScene.fCurrentTake:=Take;
   end;
   if fFileVersion<7000 then begin
    begin
     AnimationStack:=TFBXAnimationStack.Create(self,nil,0,Parser.ConstructName([TFBXString('AnimStack'),TakeName]),'AnimStack');
     fScene.AllObjects.AddOrSetValue(TakeName,AnimationStack);
     fScene.AllObjects.AddOrSetValue(AnimationStack.Name,AnimationStack);
     fScene.fAnimationStackList.Add(AnimationStack);
     AnimationStack.fDescription:=TakeName;
     AnimationStack.fLocalStart:=Take.fLocalTimeSpan.StartTime;
     AnimationStack.fLocalStop:=Take.fLocalTimeSpan.EndTime;
     AnimationStack.fReferenceStart:=Take.fReferenceTimeSpan.StartTime;
     AnimationStack.fReferenceStop:=Take.fReferenceTimeSpan.EndTime;
    end;
    begin
     AnimationLayer:=TFBXAnimationLayer.Create(self,nil,0,Parser.ConstructName([TFBXString('AnimLayer'),TakeName]),'AnimLayer');
     fScene.AllObjects.AddOrSetValue(TakeName,AnimationLayer);
     fScene.AllObjects.AddOrSetValue(AnimationLayer.Name,AnimationLayer);
     AnimationLayer.ConnectTo(AnimationStack);
    end;
    for Element in pTakeElement.Children do begin
     if Element.ID='Model' then begin
      Name:=Element.Properties[0].GetString;
      Object_:=Scene.AllObjects[Name];
      if assigned(Object_) then begin
       for SubElement in Element.Children do begin
        if (SubElement.ID='C') or (SubElement.ID='Channel') then begin
         SubElementKind:=SubElement.Properties[0].GetString;
         if SubElementKind='Transform' then begin
          for SubSubElement in SubElement.Children do begin
           if (SubSubElement.ID='C') or (SubSubElement.ID='Channel') then begin
            SubSubElementKind:=SubSubElement.Properties[0].GetString;
            if (SubSubElementKind='T') or
               (SubSubElementKind='R') or
               (SubSubElementKind='S') then begin
             if SubSubElementKind='T' then begin
              AnimationCurveNode:=TFBXAnimationCurveNode.Create(self,nil,0,'T','AnimCurveNode');
              AnimationCurveNode.ConnectTo(AnimationLayer);
              Object_.ConnectToProperty(AnimationCurveNode,'Lcl Translation');
             end else if SubSubElementKind='R' then begin
              AnimationCurveNode:=TFBXAnimationCurveNode.Create(self,nil,0,'R','AnimCurveNode');
              AnimationCurveNode.ConnectTo(AnimationLayer);
              Object_.ConnectToProperty(AnimationCurveNode,'Lcl Rotation');
             end else{if SubSubElementKind='S' then}begin
              AnimationCurveNode:=TFBXAnimationCurveNode.Create(self,nil,0,'S','AnimCurveNode');
              AnimationCurveNode.ConnectTo(AnimationLayer);
              Object_.ConnectToProperty(AnimationCurveNode,'Lcl Scaling');
             end;
             for SubSubSubElement in SubSubElement.Children do begin
              if (SubSubSubElement.ID='C') or (SubSubSubElement.ID='Channel') then begin
               SubSubSubElementKind:=SubSubSubElement.Properties[0].GetString;
               if (SubSubSubElementKind='X') or
                  (SubSubSubElementKind='Y') or
                  (SubSubSubElementKind='Z') then begin
                AnimationCurve:=TFBXAnimationCurve.CreateOldFBX6000(self,SubSubSubElement,0,Parser.ConstructName([TFBXString('AnimCurve'),TakeName,SubElementKind,SubSubElementKind,SubSubSubElementKind]),'AnimCurve');
                AnimationCurveNode.ConnectToProperty(AnimationCurve,SubSubSubElementKind);
               end;
              end;
             end;
            end;
           end;
          end;
         end else if SubElementKind='Visibility' then begin
          AnimationCurveNode:=TFBXAnimationCurveNode.Create(self,nil,0,'Visibility','AnimCurveNode');
          AnimationCurveNode.ConnectTo(AnimationLayer);
          Object_.ConnectToProperty(AnimationCurveNode,'Visibility');
          AnimationCurve:=TFBXAnimationCurve.CreateOldFBX6000(self,SubElement,0,Parser.ConstructName([TFBXString('AnimCurve'),TakeName,SubElementKind]),'AnimCurve');
          AnimationCurveNode.ConnectToProperty(AnimationCurve,SubElementKind);
         end;
        end;
       end;
      end;
     end;
    end;
   end;
  end;
 var Element:TFBXElement;
 begin
  CurrentTakeName:='';
  for Element in pTakesElement.Children do begin
   if Element.ID='Current' then begin
    CurrentTakeName:=Element.Properties[0].GetString;
   end else if Element.ID='Take' then begin
    ProcessTake(Element);
   end;
  end;
 end;
var FirstByte:UInt8;
    ChildElement:TFBXElement;
    Mesh:TFBXMesh;
begin

 FirstByte:=0;

 pStream.ReadBuffer(FirstByte,SizeOf(UInt8));

 pStream.Seek(-SizeOf(UInt8),soCurrent);

 Parser:=nil;
 try

  case FirstByte of
   ord(';'):begin
    Parser:=TFBXASCIIParser.Create(pStream);
   end;
   ord('K'):begin
    Parser:=TFBXBinaryParser.Create(pStream);
   end;
  end;

  if not assigned(Parser) then begin
   raise EFBX.Create('Invalid oder corrupt FBX file');
  end;

  FreeAndNil(fGlobalSettings);

  FreeAndNil(fScene);

  FreeAndNil(fRootElement);

  fRootElement:=Parser.Parse;
  if assigned(fRootElement) then begin
   try
    fScene:=TFBXScene.Create(self,fRootElement,0,TFBXString(#0'Scene'#255),'');
    if fRootElement.ChildrenByName.TryGetValue('FBXHeaderExtension',ChildElement) then begin
     ProcessFBXHeaderExtension(ChildElement);
    end;
    if fRootElement.ChildrenByName.TryGetValue('GlobalSettings',ChildElement) then begin
     ProcessGlobalSettings(ChildElement);
    end;
    if fRootElement.ChildrenByName.TryGetValue('Objects',ChildElement) then begin
     ProcessObjects(ChildElement);
    end;
    if fRootElement.ChildrenByName.TryGetValue('Connections',ChildElement) then begin
     ProcessConnections(ChildElement);
    end;
    if fRootElement.ChildrenByName.TryGetValue('Takes',ChildElement) then begin
     ProcessTakes(ChildElement);
    end;
   finally
   end;
  end;

  for Mesh in fScene.fMeshes do begin
   Mesh.Finish;
  end;

 finally
  Parser.Free;
 end;

end;

procedure TFBXLoader.LoadFromFile(const pFileName:UTF8String);
var fs:TFileStream;
    ms:TMemoryStream;
begin
 fs:=TFileStream.Create(String(pFileName),fmOpenRead or fmShareDenyWrite);
 try
  ms:=TMemoryStream.Create;
  try
   ms.LoadFromStream(fs);
   ms.Seek(0,soBeginning);
   LoadFromStream(ms);
  finally
   ms.Free;
  end;
 finally
  fs.Free;
 end;
end;

initialization
 PropertyNameRemap:=TFBXPropertyNameRemap.Create;
 PropertyNameRemap.AddOrSetValue(TFBXString('Lcl Translation'),TFBXString('LclTranslation'));
 PropertyNameRemap.AddOrSetValue(TFBXString('Lcl Rotation'),TFBXString('LclRotation'));
 PropertyNameRemap.AddOrSetValue(TFBXString('Lcl Scaling'),TFBXString('LclScaling'));
 PropertyNameRemap.AddOrSetValue(TFBXString('X'),TFBXString('x'));
 PropertyNameRemap.AddOrSetValue(TFBXString('Y'),TFBXString('y'));
 PropertyNameRemap.AddOrSetValue(TFBXString('Z'),TFBXString('z'));
finalization
 PropertyNameRemap.Free;
end.
*/