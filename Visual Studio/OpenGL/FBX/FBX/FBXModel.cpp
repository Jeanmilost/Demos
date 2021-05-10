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

#include "FBXModel.h"

// std
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sstream>

//---------------------------------------------------------------------------
// FBXModel::IFBXData
//---------------------------------------------------------------------------
FBXModel::IFBXData::IFBXData(const std::string* pData) :
    m_Type(IEDataType::IE_DT_Unknown),
    m_pData(pData),
    m_Start(0),
    m_End(0),
    m_IsCounter(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXData::IFBXData(IEDataType type, const std::string* pData, std::size_t start, std::size_t end) :
    m_Type(type),
    m_pData(pData),
    m_Start(start),
    m_End(end),
    m_IsCounter(false)
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXData::GetRaw() const
{
    if (!m_pData)
        return "";

    const std::size_t length = m_pData->length();

    if (!length)
        return "";

    const std::size_t start = std::min(m_Start, length - 1);
    const std::size_t end   = std::min(m_End,   length - 1);

    if (start > end)
        return "";

    return m_pData->substr(start, end - start);
}
//---------------------------------------------------------------------------
bool FBXModel::IFBXData::GetBool() const
{
    // get raw value
    const std::string str = GetRaw();

    // found it?
    if (str.empty())
        return false;

    // convert raw data to boolean
    return IFBXHelper::StrToBool(str);
}
//---------------------------------------------------------------------------
int FBXModel::IFBXData::GetInt() const
{
    // contains an integer value?
    if (m_Type != IEDataType::IE_DT_Int)
        return 0;

    // get raw value
    std::string str = GetRaw();

    // found it?
    if (str.empty())
        return 0;

    // is a counter?
    if (str[0] == '*')
    {
        // remove the counter marker from string
        str = str.substr(1, str.length() - 1);

        // still not empty?
        if (str.empty())
            return 0;

        // mark the value as a counter
        const_cast<bool&>(m_IsCounter) = true;
    }

    // convert raw data to integer
    int value;
    std::istringstream sstr(str);
    sstr >> value;

    // full string must be converted for success
    if (sstr.rdstate() == std::ios_base::eofbit)
        return value;

    return 0;
}
//---------------------------------------------------------------------------
long long FBXModel::IFBXData::GetLongLong() const
{
    // get raw value
    const std::string str = GetRaw();

    // found it?
    if (str.empty())
        return 0L;

    // convert raw data to long long
    long long value;
    std::istringstream sstr(str);
    sstr >> value;

    // full string must be converted for success
    if (sstr.rdstate() == std::ios_base::eofbit)
        return value;

    return 0L;
}
//---------------------------------------------------------------------------
float FBXModel::IFBXData::GetFloat() const
{
    // contains a floating point value?
    if (m_Type != IEDataType::IE_DT_Float)
        return 0.0f;

    // get raw value
    const std::string str = GetRaw();

    // found it?
    if (str.empty())
        return 0.0f;

    // convert raw data to float
    float value;
    std::istringstream sstr(str);
    sstr >> value;

    // full string must be converted for success
    if (sstr.rdstate() == std::ios_base::eofbit)
        return value;

    return 0.0f;
}
//---------------------------------------------------------------------------
double FBXModel::IFBXData::GetDouble() const
{
    // contains a floating point value?
    if (m_Type != IEDataType::IE_DT_Float)
        return 0.0;

    // get raw value
    const std::string str = GetRaw();

    // found it?
    if (str.empty())
        return 0.0;

    // convert raw data to double
    double value;
    std::istringstream sstr(str);
    sstr >> value;

    // full string must be converted for success
    if (sstr.rdstate() == std::ios_base::eofbit)
        return value;

    return 0.0;
}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXData::GetStr() const
{
    // contains a string value?
    if (m_Type != IEDataType::IE_DT_String)
        return "";

    return GetRaw();
}
//---------------------------------------------------------------------------
// FBXModel::IFBXItem
//---------------------------------------------------------------------------
FBXModel::IFBXItem::IFBXItem() :
    m_pParent(nullptr)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXItem::~IFBXItem()
{
    for (IFBXDataset::iterator it = m_Values.begin(); it != m_Values.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
void FBXModel::IFBXItem::SetName(const std::string& name)
{
    m_Name = name;
}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXItem::GetName() const
{
    return m_Name;
}
//---------------------------------------------------------------------------
void FBXModel::IFBXItem::SetParent(IFBXItem* pParent)
{
    m_pParent = pParent;
}
//---------------------------------------------------------------------------
FBXModel::IFBXItem* FBXModel::IFBXItem::GetParent() const
{
    return m_pParent;
}
//---------------------------------------------------------------------------
void FBXModel::IFBXItem::AddValue(IFBXData* pValue)
{
    if (!pValue)
        return;

    m_Values.push_back(pValue);
}
//---------------------------------------------------------------------------
FBXModel::IFBXData* FBXModel::IFBXItem::GetValue(std::size_t index) const
{
    if (index >= m_Values.size())
        return nullptr;

    return m_Values[index];
}
//---------------------------------------------------------------------------
std::size_t FBXModel::IFBXItem::GetValueCount() const
{
    return m_Values.size();
}
//---------------------------------------------------------------------------
// FBXModel::IFBXProperty
//---------------------------------------------------------------------------
FBXModel::IFBXProperty::IFBXProperty(IEPropType propType) :
    IFBXItem(),
    m_Cached(false),
    m_PropType(propType)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXProperty::~IFBXProperty()
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXProperty::GetPropName() const
{
    // is prop name already cached?
    if (m_PropName.empty())
    {
        // no available value?
        if (!GetValueCount())
            return "";

        // force the const constraint just this time to cache the property name
        const_cast<std::string&>(m_PropName) = GetValue(0)->GetRaw();
    }

    return m_PropName;
}
//---------------------------------------------------------------------------
FBXModel::IEPropType FBXModel::IFBXProperty::GetType() const
{
    return m_PropType;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXUnknownProperty
//---------------------------------------------------------------------------
FBXModel::IFBXUnknownProperty::IFBXUnknownProperty() :
    IFBXProperty(IEPropType::IE_PT_Unknown)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXUnknownProperty::~IFBXUnknownProperty()
{}
//---------------------------------------------------------------------------
// FBXModel::IFBXBoolProperty
//---------------------------------------------------------------------------
FBXModel::IFBXBoolProperty::IFBXBoolProperty() :
    IFBXProperty(IEPropType::IE_PT_Bool),
    m_Value(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXBoolProperty::~IFBXBoolProperty()
{}
//---------------------------------------------------------------------------
bool FBXModel::IFBXBoolProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return false;

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to boolean and cache it
    const_cast<bool&>(m_Value)  = GetValue(4)->GetBool();
    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXIntProperty
//---------------------------------------------------------------------------
FBXModel::IFBXIntProperty::IFBXIntProperty() :
    IFBXProperty(IEPropType::IE_PT_Int),
    m_Value(0)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXIntProperty::~IFBXIntProperty()
{}
//---------------------------------------------------------------------------
int FBXModel::IFBXIntProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return 0;

    // already cached?
    if (m_Cached)
        return m_Value;

    // cache value
    const_cast<int&> (m_Value)  = GetValue(4)->GetInt();
    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXDoubleProperty
//---------------------------------------------------------------------------
FBXModel::IFBXDoubleProperty::IFBXDoubleProperty() :
    IFBXProperty(IEPropType::IE_PT_Double),
    m_Value(0.0)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXDoubleProperty::~IFBXDoubleProperty()
{}
//---------------------------------------------------------------------------
double FBXModel::IFBXDoubleProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return 0.0;

    // already cached?
    if (m_Cached)
        return m_Value;

    // cache value
    const_cast<double&>(m_Value)  = GetValue(4)->GetDouble();
    const_cast<bool&>  (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXLongLongProperty
//---------------------------------------------------------------------------
FBXModel::IFBXLongLongProperty::IFBXLongLongProperty() :
    IFBXProperty(IEPropType::IE_PT_ULongLong),
    m_Value(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXLongLongProperty::~IFBXLongLongProperty()
{}
//---------------------------------------------------------------------------
long long FBXModel::IFBXLongLongProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return 0L;

    // already cached?
    if (m_Cached)
        return m_Value;

    // cache value
    const_cast<long long&>(m_Value)  = GetValue(4)->GetLongLong();
    const_cast<bool&>     (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXStringProperty
//---------------------------------------------------------------------------
FBXModel::IFBXStringProperty::IFBXStringProperty() :
    IFBXProperty(IEPropType::IE_PT_String)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXStringProperty::~IFBXStringProperty()
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXStringProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return "";

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to string and cache it
    const_cast<std::string&>(m_Value)  = GetValue(4)->GetRaw();
    const_cast<bool&>       (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXNumberProperty
//---------------------------------------------------------------------------
FBXModel::IFBXNumberProperty::IFBXNumberProperty() :
    IFBXProperty(IEPropType::IE_PT_Number)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXNumberProperty::~IFBXNumberProperty()
{}
//---------------------------------------------------------------------------
double FBXModel::IFBXNumberProperty::Get(std::size_t index) const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    // is index out of bounds?
    if (index >= m_Values.size())
        return 0.0;

    return m_Values[index];
}
//---------------------------------------------------------------------------
std::size_t FBXModel::IFBXNumberProperty::GetCount() const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    return m_Values.size();
}
//---------------------------------------------------------------------------
void FBXModel::IFBXNumberProperty::GetValues() const
{
    // already cached?
    if (m_Cached)
        return;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return;

    // read and cache values
    for (std::size_t i = 4; i < count; ++i)
        const_cast<std::vector<double>&>(m_Values).push_back
                (GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetDouble());

    const_cast<bool&>(m_Cached) = true;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXColorProperty
//---------------------------------------------------------------------------
FBXModel::IFBXColorProperty::IFBXColorProperty() :
    IFBXProperty(IEPropType::IE_PT_Color)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXColorProperty::~IFBXColorProperty()
{}
//---------------------------------------------------------------------------
ColorF FBXModel::IFBXColorProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return ColorF();

    // read and cache color components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<ColorF&>(m_Value).m_R = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<ColorF&>(m_Value).m_G = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<ColorF&>(m_Value).m_B = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<ColorF&>(m_Value).m_A = 1.0f;
    const_cast<bool&>(m_Cached)      = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXColorRGBProperty
//---------------------------------------------------------------------------
FBXModel::IFBXColorRGBProperty::IFBXColorRGBProperty() :
    IFBXProperty(IEPropType::IE_PT_ColorRGB)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXColorRGBProperty::~IFBXColorRGBProperty()
{}
//---------------------------------------------------------------------------
ColorF FBXModel::IFBXColorRGBProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return ColorF();

    // read and cache color components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<ColorF&>(m_Value).m_R = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<ColorF&>(m_Value).m_G = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<ColorF&>(m_Value).m_B = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<ColorF&>(m_Value).m_A = 1.0f;
    const_cast<bool&>(m_Cached)      = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXVectorProperty
//---------------------------------------------------------------------------
FBXModel::IFBXVectorProperty::IFBXVectorProperty() :
    IFBXProperty(IEPropType::IE_PT_Vector)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXVectorProperty::~IFBXVectorProperty()
{}
//---------------------------------------------------------------------------
Vector3F FBXModel::IFBXVectorProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return Vector3F();

    // read and cache vector components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<Vector3F&>(m_Value).m_X = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<Vector3F&>(m_Value).m_Y = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<Vector3F&>(m_Value).m_Z = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXVector3DProperty
//---------------------------------------------------------------------------
FBXModel::IFBXVector3DProperty::IFBXVector3DProperty() :
    IFBXProperty(IEPropType::IE_PT_Vector3D)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXVector3DProperty::~IFBXVector3DProperty()
{}
//---------------------------------------------------------------------------
Vector3F FBXModel::IFBXVector3DProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return Vector3F();

    // read and cache vector components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<Vector3F&>(m_Value).m_X = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<Vector3F&>(m_Value).m_Y = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<Vector3F&>(m_Value).m_Z = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXTimeProperty
//---------------------------------------------------------------------------
FBXModel::IFBXTimeProperty::IFBXTimeProperty() :
    IFBXProperty(IEPropType::IE_PT_Time),
    m_Value(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXTimeProperty::~IFBXTimeProperty()
{}
//---------------------------------------------------------------------------
time_t FBXModel::IFBXTimeProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return 0L;

    // already cached?
    if (m_Cached)
        return m_Value;

    // get raw value
    const std::string str = GetValue(4)->GetRaw();

    // found it?
    if (str.empty())
        return 0L;

    // convert raw data to time_t
    time_t value;
    std::istringstream sstr(str);
    sstr >> value;

    // full string must be converted for success
    if (sstr.rdstate() == std::ios_base::eofbit)
    {
        // cache value
        const_cast<time_t&>(m_Value)  = value;
        const_cast<bool&>  (m_Cached) = true;

        return m_Value;
    }

    return 0L;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXDateTimeProperty
//---------------------------------------------------------------------------
FBXModel::IFBXDateTimeProperty::IFBXDateTimeProperty() :
    IFBXProperty(IEPropType::IE_PT_DateTime)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXDateTimeProperty::~IFBXDateTimeProperty()
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXDateTimeProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return "";

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to string and cache it
    const_cast<std::string&>(m_Value)  = GetValue(4)->GetRaw();
    const_cast<bool&>       (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXEnumProperty
//---------------------------------------------------------------------------
FBXModel::IFBXEnumProperty::IFBXEnumProperty() :
    IFBXProperty(IEPropType::IE_PT_Enum),
    m_Value(0)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXEnumProperty::~IFBXEnumProperty()
{}
//---------------------------------------------------------------------------
int FBXModel::IFBXEnumProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return 0;

    // already cached?
    if (m_Cached)
        return m_Value;

    // cache value
    const_cast<int&> (m_Value)  = GetValue(4)->GetInt();
    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXLclTranslationProperty
//---------------------------------------------------------------------------
FBXModel::IFBXLclTranslationProperty::IFBXLclTranslationProperty() :
    IFBXProperty(IEPropType::IE_PT_LclTranslation)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXLclTranslationProperty::~IFBXLclTranslationProperty()
{}
//---------------------------------------------------------------------------
Vector3F FBXModel::IFBXLclTranslationProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return Vector3F();

    // read and cache vector components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<Vector3F&>(m_Value).m_X = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<Vector3F&>(m_Value).m_Y = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<Vector3F&>(m_Value).m_Z = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXLclRotationProperty
//---------------------------------------------------------------------------
FBXModel::IFBXLclRotationProperty::IFBXLclRotationProperty() :
    IFBXProperty(IEPropType::IE_PT_LclRotation)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXLclRotationProperty::~IFBXLclRotationProperty()
{}
//---------------------------------------------------------------------------
Matrix4x4F FBXModel::IFBXLclRotationProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return Matrix4x4F::Identity();

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    // read quaternion components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: x = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: y = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: z = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const float degToRad = (float)(M_PI / 180.0f);

    // get the x y and z rotation matrices based on their respective Euler angles
    Matrix4x4F rotMatX = Matrix4x4F::Identity();
    rotMatX.Rotate(x * degToRad, Vector3F(1.0f, 0.0f, 0.0f));
    Matrix4x4F rotMatY = Matrix4x4F::Identity();
    rotMatY.Rotate(y * degToRad, Vector3F(0.0f, 1.0f, 0.0f));
    Matrix4x4F rotMatZ = Matrix4x4F::Identity();
    rotMatZ.Rotate(z * degToRad, Vector3F(0.0f, 0.0f, 1.0f));

    // build the final rotation matrix and cache it
    const_cast<Matrix4x4F&>(m_Value)  = rotMatX.Multiply(rotMatY).Multiply(rotMatZ);
    const_cast<bool&>      (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXLclScalingProperty
//---------------------------------------------------------------------------
FBXModel::IFBXLclScalingProperty::IFBXLclScalingProperty() :
    IFBXProperty(IEPropType::IE_PT_LclScaling),
    m_Value(Vector3F(1.0f, 1.0f, 1.0f))
{}
//---------------------------------------------------------------------------
FBXModel::IFBXLclScalingProperty::~IFBXLclScalingProperty()
{}
//---------------------------------------------------------------------------
Vector3F FBXModel::IFBXLclScalingProperty::Get() const
{
    // already cached?
    if (m_Cached)
        return m_Value;

    const std::size_t count = GetValueCount();

    // no available value?
    if (count <= 4)
        return Vector3F(1.0f, 1.0f, 1.0f);

    // read and cache vector components
    for (std::size_t i = 4; i < count; ++i)
        switch (i)
        {
            case 4: const_cast<Vector3F&>(m_Value).m_X = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 5: const_cast<Vector3F&>(m_Value).m_Y = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
            case 6: const_cast<Vector3F&>(m_Value).m_Z = GetValue(i)->m_Type == IEDataType::IE_DT_Int ? (float)GetValue(i)->GetInt() : GetValue(i)->GetFloat(); continue;
        }

    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXVisibilityProperty
//---------------------------------------------------------------------------
FBXModel::IFBXVisibilityProperty::IFBXVisibilityProperty() :
    IFBXProperty(IEPropType::IE_PT_Visibility),
    m_Value(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXVisibilityProperty::~IFBXVisibilityProperty()
{}
//---------------------------------------------------------------------------
bool FBXModel::IFBXVisibilityProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return false;

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to boolean and cache it
    const_cast<bool&>(m_Value)  = GetValue(4)->GetBool();
    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXVisibilityInheritanceProperty
//---------------------------------------------------------------------------
FBXModel::IFBXVisibilityInheritanceProperty::IFBXVisibilityInheritanceProperty() :
    IFBXProperty(IEPropType::IE_PT_VisibilityInheritance),
    m_Value(false)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXVisibilityInheritanceProperty::~IFBXVisibilityInheritanceProperty()
{}
//---------------------------------------------------------------------------
bool FBXModel::IFBXVisibilityInheritanceProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return false;

    // already cached?
    if (m_Cached)
        return m_Value;

    // get raw value
    const std::string str = GetValue(4)->GetRaw();

    // found it?
    if (str.empty())
        return false;

    // convert raw data to boolean and cache it
    const_cast<bool&>(m_Value)  = IFBXHelper::StrToBool(str);
    const_cast<bool&>(m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXCompoundProperty
//---------------------------------------------------------------------------
FBXModel::IFBXCompoundProperty::IFBXCompoundProperty() :
    IFBXProperty(IEPropType::IE_PT_Compound)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXCompoundProperty::~IFBXCompoundProperty()
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXCompoundProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return "";

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to boolean and cache it
    const_cast<std::string&>(m_Value)  = GetValue(4)->GetRaw();
    const_cast<bool&>       (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXObjectProperty
//---------------------------------------------------------------------------
FBXModel::IFBXObjectProperty::IFBXObjectProperty() :
    IFBXProperty(IEPropType::IE_PT_Object)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXObjectProperty::~IFBXObjectProperty()
{}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXObjectProperty::Get() const
{
    // no available value?
    if (GetValueCount() <= 4)
        return "";

    // already cached?
    if (m_Cached)
        return m_Value;

    // convert raw data to boolean and cache it
    const_cast<std::string&>(m_Value)  = GetValue(4)->GetRaw();
    const_cast<bool&>       (m_Cached) = true;

    return m_Value;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXArrayProperty
//---------------------------------------------------------------------------
FBXModel::IFBXArrayProperty::IFBXArrayProperty(std::size_t capacity) :
    IFBXProperty(IEPropType::IE_PT_Array),
    m_Capacity(capacity)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXArrayProperty::~IFBXArrayProperty()
{}
//---------------------------------------------------------------------------
int FBXModel::IFBXArrayProperty::GetI(std::size_t index) const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    // is index out of bounds?
    if (index >= m_Values.size())
        return 0;

    return (int)m_Values[index];
}
//---------------------------------------------------------------------------
float FBXModel::IFBXArrayProperty::GetF(std::size_t index) const
{
    return (float)GetD(index);
}
//---------------------------------------------------------------------------
double FBXModel::IFBXArrayProperty::GetD(std::size_t index) const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    // is index out of bounds?
    if (index >= m_Values.size())
        return 0.0;

    return m_Values[index];
}
//---------------------------------------------------------------------------
std::size_t FBXModel::IFBXArrayProperty::GetCount() const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    return m_Values.size();
}
//---------------------------------------------------------------------------
const std::vector<double>* FBXModel::IFBXArrayProperty::GetPtr() const
{
    // already cached?
    if (!m_Cached)
        GetValues();

    return &m_Values;
}
//---------------------------------------------------------------------------
void FBXModel::IFBXArrayProperty::GetValues() const
{
    // already cached?
    if (m_Cached)
        return;

    const std::size_t count = GetValueCount();

    // no available value?
    if (!count)
        return;

    // reserve space for numbers
    if (m_Capacity)
        const_cast<std::vector<double>&>(m_Values).reserve(m_Capacity);

    // get the numbers
    for (std::size_t i = 0; i < count; ++i)
        switch (GetValue(i)->m_Type)
        {
            case IEDataType::IE_DT_Int:   const_cast<std::vector<double>&>(m_Values).push_back(GetValue(i)->GetInt());    continue;
            case IEDataType::IE_DT_Float: const_cast<std::vector<double>&>(m_Values).push_back(GetValue(i)->GetDouble()); continue;

            default:
                const_cast<std::vector<double>&>(m_Values).clear();
                return;
        }

    const_cast<bool&>(m_Cached) = true;
}
//---------------------------------------------------------------------------
// FBXModel::IFBXNode
//---------------------------------------------------------------------------
FBXModel::IFBXNode::IFBXNode() :
    IFBXItem()
{}
//---------------------------------------------------------------------------
FBXModel::IFBXNode::~IFBXNode()
{
    for (IFBXItems::iterator it = m_Props.begin(); it != m_Props.end(); ++it)
        delete (*it);

    for (IFBXItems::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
void FBXModel::IFBXNode::AddProp(IFBXProperty* pProp)
{
    if (!pProp)
        return;

    m_Props.push_back(pProp);
}
//---------------------------------------------------------------------------
FBXModel::IFBXProperty* FBXModel::IFBXNode::GetProp(std::size_t index) const
{
    if (index >= m_Props.size())
        return nullptr;

    return static_cast<IFBXProperty*>(m_Props[index]);
}
//---------------------------------------------------------------------------
std::size_t FBXModel::IFBXNode::GetPropCount() const
{
    return m_Props.size();
}
//---------------------------------------------------------------------------
void FBXModel::IFBXNode::AddChild(IFBXNode* pChild)
{
    if (!pChild)
        return;

    m_Children.push_back(pChild);
}
//---------------------------------------------------------------------------
FBXModel::IFBXNode* FBXModel::IFBXNode::GetChild(std::size_t index) const
{
    if (index >= m_Children.size())
        return nullptr;

    return static_cast<IFBXNode*>(m_Children[index]);
}
//---------------------------------------------------------------------------
std::size_t FBXModel::IFBXNode::GetChildCount() const
{
    return m_Children.size();
}
//---------------------------------------------------------------------------
// FBXModel::IFBXHelper
//---------------------------------------------------------------------------
std::string FBXModel::IFBXHelper::Trim(const std::string& str)
{
    const std::size_t strLength = str.length();
          std::size_t before    = 0;
          std::size_t after     = strLength;

    // search for first valid char
    for (std::size_t i = 0; i < strLength; ++i)
    {
        bool doBreak = false;

        switch (str[i])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':
                ++before;
                continue;

            default:
                doBreak = true;
                break;
        }

        if (doBreak)
            break;
    }

    // search for last valid char
    for (int i = (int)strLength - 1; i >= 0; --i)
    {
        bool doBreak = false;

        switch (str[i])
        {
            case '\r':
            case '\n':
            case '\t':
            case ' ':
                --after;
                continue;

            default:
                doBreak = true;
                break;
        }

        if (doBreak)
            break;
    }

    // extract the trimmed string
    return str.substr(before, after - before);
}
//---------------------------------------------------------------------------
std::string FBXModel::IFBXHelper::ToLower(const std::string& str)
{
    const std::size_t strLength = str.length();
    std::string loweredStr;

    loweredStr.resize(strLength);

    // iterate through char and lower all chars between a and z
    for (std::size_t i = 0; i < strLength; ++i)
    {
        // do lower the char?
        if (str[i] < 'A' || str[i] > 'Z')
        {
            loweredStr[i] = str[i];
            continue;
        }

        loweredStr[i] = str[i] + ('a' - 'A');
    }

    return loweredStr;
}
//---------------------------------------------------------------------------
void FBXModel::IFBXHelper::Split(const std::string&              str,
                                 const std::string&              delimiter,
                                       std::vector<std::string>& result)
{
    // if string is empty, do nothing
    if (str.empty())
    {
        result.clear();
        return;
    }

    std::size_t pos = 0;
    std::size_t i   = 0;

    do
    {
        // search for next token
        const std::size_t posEnd = str.find(delimiter, pos);

        // found token?
        if (posEnd != str.npos)
        {
            // extract string from token and calculate next start position
            result.push_back(str.substr(pos, posEnd - pos));
            pos = posEnd + delimiter.length();
        }
        else
        {
            // extract last string and break the loop
            result.push_back(str.substr(pos, str.length() - pos));
            break;
        }

        // update result index
        ++i;
    }
    while (1);
}
//---------------------------------------------------------------------------
bool FBXModel::IFBXHelper::StrToBool(const std::string& str)
{
    const std::string loweredStr = ToLower(str);

    return (loweredStr == "0" || loweredStr == "true");
}
//---------------------------------------------------------------------------
// FBXModel::IFBXLink
//---------------------------------------------------------------------------
FBXModel::IFBXLink::IFBXLink() :
    m_NodeType(IENodeType::IE_NT_Model),
    m_pNode(nullptr),
    m_pParent(nullptr)
{}
//---------------------------------------------------------------------------
FBXModel::IFBXLink::~IFBXLink()
{
    for (IFBXLinks::iterator it = m_Children.begin(); it != m_Children.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
// FBXModel::IMeshTemplate
//---------------------------------------------------------------------------
FBXModel::IMeshTemplate::IMeshTemplate():
    m_pVertices(nullptr),
    m_pIndices(nullptr),
    m_pNormals(nullptr),
    m_pNormalIndices(nullptr),
    m_pUVs(nullptr),
    m_pUVIndices(nullptr)
{}
//---------------------------------------------------------------------------
FBXModel::IMeshTemplate::~IMeshTemplate()
{}
//---------------------------------------------------------------------------
bool FBXModel::IMeshTemplate::IsValid() const
{
    return (m_pVertices && m_pIndices && m_pNormals && m_pNormalIndices && m_pUVs && m_pUVIndices);
}
//---------------------------------------------------------------------------
// FBXModel
//---------------------------------------------------------------------------
FBXModel::FBXModel() :
    m_pModel(nullptr),
    m_fOnLoadTexture(nullptr)
{}
//---------------------------------------------------------------------------
FBXModel::~FBXModel()
{
    if (m_pModel)
        delete m_pModel;

    for (IMeshTemplates::iterator it = m_Templates.begin(); it != m_Templates.end(); ++it)
        delete (*it);

    for (IFBXNodes::iterator it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
        delete (*it);

    for (IFBXLinks::iterator it = m_Links.begin(); it != m_Links.end(); ++it)
        delete (*it);
}
//---------------------------------------------------------------------------
void FBXModel::Clear()
{
    if (m_pModel)
        delete m_pModel;

    m_pModel = nullptr;

    for (IMeshTemplates::iterator it = m_Templates.begin(); it != m_Templates.end(); ++it)
        delete (*it);

    m_Templates.clear();

    for (IFBXNodes::iterator it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
        delete (*it);

    m_Nodes.clear();

    for (IFBXLinks::iterator it = m_Links.begin(); it != m_Links.end(); ++it)
        delete (*it);

    m_Links.clear();
    m_ItemDict.clear();
    m_Data.clear();
}
//---------------------------------------------------------------------------
bool FBXModel::Open(const std::string& fileName)
{
    // no file name?
    if (fileName.empty())
        return false;

    char*       pBuffer    = nullptr;
    std::FILE*  pStream    = nullptr;
    std::size_t fileSize   = 0;
    std::size_t bufferSize = 0;
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

        // copy file content to buffer
        pBuffer    = new char[fileSize + 1];
        bufferSize = std::fread(pBuffer, 1, fileSize, pStream);
    }
    catch (...)
    {
        success = false;
    }

    // close FBX file
    if (pStream)
        std::fclose(pStream);

    std::string data;

    try
    {
        // file read succeeded?
        if (success)
            // add read data to output
            data = std::string((const char*)pBuffer, bufferSize);
    }
    catch (...)
    {
        success = false;
    }

    // delete buffer, if needed
    if (pBuffer)
        delete[] pBuffer;

    return (success && (bufferSize == fileSize) && Read(data));
}
//---------------------------------------------------------------------------
bool FBXModel::Read(const std::string& data)
{
    Clear();

    m_Data = data;

    const std::size_t length = m_Data.length();
          std::size_t index  = 0;
          IFBXNode*   pNode  = nullptr;
          std::string name;
          IFBXDataset dataset;

    try
    {
        // iterate through FBX data
        while (index < length)
        {
            // get the next word
            std::unique_ptr<IFBXData> pFBXData(ReadWord(m_Data, length, index));

            if (!pFBXData)
                continue;

            // dispatch word
            switch (pFBXData->m_Type)
            {
                case IEDataType::IE_DT_Unknown:
                    continue;

                case IEDataType::IE_DT_Data:
                case IEDataType::IE_DT_Int:
                case IEDataType::IE_DT_Float:
                case IEDataType::IE_DT_String:
                    // any value is kept in a temporary dataset
                    dataset.push_back(pFBXData.get());
                    pFBXData.release();
                    continue;

                case IEDataType::IE_DT_KeySeparator:
                {
                    const std::size_t count = dataset.size();

                    // found a key separator, check the dataset content
                    if (!count)
                        return false;
                    else
                    if (count == 1)
                    {
                        // only one item in the dataset, should be the name of something
                        if (dataset[0]->m_Type != IEDataType::IE_DT_Data)
                        {
                            ClearDataset(dataset);
                            return false;
                        }

                        // get the next name and keep it locally
                        name = dataset[0]->m_pData->substr(dataset[0]->m_Start, dataset[0]->m_End - dataset[0]->m_Start);

                        // clear the local dataset
                        ClearDataset(dataset);
                    }
                    else
                    {
                        // many items in the dataset, should contains a property, followed by the name of the next item
                        if (!pNode)
                        {
                            ClearDataset(dataset);
                            return false;
                        }

                        // parse the property
                        if (!SetProperty(name, pNode, dataset, 0, count - 1))
                        {
                            ClearDataset(dataset);
                            return false;
                        }

                        // get the next name and keep it locally
                        const std::size_t dataIndex = count - 1;
                                          name      = dataset[dataIndex]->m_pData->substr(dataset[dataIndex]->m_Start,
                                                                                          dataset[dataIndex]->m_End - dataset[dataIndex]->m_Start);

                        // clear the local dataset
                        delete dataset[dataIndex];
                        dataset.clear();
                    }

                    continue;
                }

                case IEDataType::IE_DT_ValueSeparator:
                    continue;

                case IEDataType::IE_DT_StartNodeSeparator:
                {
                    if (!index)
                        return false;

                    // add the child node
                    SetNode(name, pNode, m_Nodes, dataset, m_ItemDict);

                    continue;
                }

                case IEDataType::IE_DT_EndNodeSeparator:
                {
                    const std::size_t count = dataset.size();

                    // found a closing child item, check the dataset content
                    if (count)
                    {
                        // found items in the dataset, should contains the last node property, followed by the name of the next item
                        if (!pNode)
                        {
                            ClearDataset(dataset);
                            return false;
                        }

                        // parse the property
                        if (!SetProperty(name, pNode, dataset, 0, count))
                        {
                            ClearDataset(dataset);
                            return false;
                        }

                        // clear the local dataset
                        dataset.clear();
                    }

                    // go up to parent node
                    if (pNode)
                    {
                        IFBXItem* pParent = pNode->GetParent();

                        if (pParent)
                            pNode = static_cast<IFBXNode*>(pParent);
                        else
                            pNode = nullptr;
                    }

                    continue;
                }

                case IEDataType::IE_DT_Comment:
                    // found a comment line, just skip it
                    SkipComment(m_Data, index);
                    continue;
            }
        }
    }
    catch (...)
    {
        // on error clear the nodes
        for (IFBXNodes::iterator it = m_Nodes.begin(); it != m_Nodes.end(); ++it)
            delete (*it);

        // then clear the local dataset
        ClearDataset(dataset);
        throw;
    }

    return PerformLinks() && BuildModel();
}
//---------------------------------------------------------------------------
Model* FBXModel::GetModel() const
{
    // no model?
    if (!m_pModel)
        return nullptr;

    const std::size_t templateCount = m_Templates.size();
    const std::size_t meshCount     = m_pModel->m_Mesh.size();

    if (templateCount != meshCount)
        return nullptr;

    // if mesh has no skeleton, perform a simple draw
    if (!m_pModel->m_pSkeleton)
    {
        for (std::size_t i = 0; i < templateCount; ++i)
        {
            // mesh should only contain 1 vertex buffer, unsupported otherwise
            if (m_pModel->m_Mesh[i]->m_VB.size() != 1)
                return nullptr;

            // vertex buffer was already built?
            if (m_pModel->m_Mesh[i]->m_VB[0]->m_Data.size())
                continue;

            // populate the vertex buffer
            if (!PopulateVertexBuffer(m_Templates[i], m_pModel->m_Mesh[i]->m_VB[0]))
                return nullptr;
        }

        return m_pModel;
    }

    // iterate through model meshes
    for (std::size_t i = 0; i < meshCount; ++i)
    {
        // get model mesh
        Mesh* pMesh = m_pModel->m_Mesh[i];

        // found it?
        if (!pMesh)
            continue;

        // normally each mesh should contain only one vertex buffer
        if (pMesh->m_VB.size() != 1)
            // unsupported if not (because cannot know which texture should be binded. If a such model
            // exists, a custom version of this function should also be written for it)
            continue;

        // vertex buffer was already built?
        if (pMesh->m_VB[0]->m_Data.size())
            continue;

        // malformed deformers?
        if (meshCount != m_pModel->m_Deformers.size())
            return nullptr;

        const std::size_t weightCount = m_pModel->m_Deformers[i]->m_SkinWeights.size();

        // mesh contains skin weights?
        if (!weightCount)
            return nullptr;

        std::vector<double> skinVertices = *m_Templates[i]->m_pVertices;

        // iterate through mesh skin weights
        for (std::size_t j = 0; j < weightCount; ++j)
        {
            // get the weight influence count
            const std::size_t weightInfluenceCount = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences.size();

            // iterate through weights influences
            for (std::size_t k = 0; k < weightInfluenceCount; ++k)
            {
                // get the vertex index count
                const std::size_t vertexIndexCount =
                        m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex.size();

                // iterate through weights influences vertex indices
                for (std::size_t l = 0; l < vertexIndexCount; ++l)
                {
                    // get the next vertex to which the next skin weight should be applied
                    const std::size_t iX = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3);
                    const std::size_t iY = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3) + 1;
                    const std::size_t iZ = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3) + 2;

                    // clear it
                    skinVertices[iX] = 0.0f;
                    skinVertices[iY] = 0.0f;
                    skinVertices[iZ] = 0.0f;
                }
            }
        }

        // iterate through mesh skin weights
        for (std::size_t j = 0; j < weightCount; ++j)
        {
            Matrix4x4F boneMatrix;

            // get the bone matrix
            m_pModel->GetBoneMatrix(m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_pBone, Matrix4x4F::Identity(), boneMatrix);

            // get the final matrix after bones transform
            const Matrix4x4F finalMatrix = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Matrix.Multiply(boneMatrix);

            // get the weight influence count
            const std::size_t weightInfluenceCount = m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences.size();

            // apply the bone and its skin weights to each vertices
            for (std::size_t k = 0; k < weightInfluenceCount; ++k)
            {
                // get the vertex index count
                const std::size_t vertexIndexCount =
                        m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex.size();

                // iterate through weights influences vertex indices
                for (std::size_t l = 0; l < vertexIndexCount; ++l)
                {
                    // get the next vertex to which the next skin weight should be applied
                    const std::size_t iX = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3);
                    const std::size_t iY = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3) + 1;
                    const std::size_t iZ = (m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_WeightInfluences[k]->m_VertexIndex[l] * 3) + 2;

                    Vector3F inputVertex;

                    // get input vertex
                    inputVertex.m_X = (float)(*m_Templates[i]->m_pVertices)[iX];
                    inputVertex.m_Y = (float)(*m_Templates[i]->m_pVertices)[iY];
                    inputVertex.m_Z = (float)(*m_Templates[i]->m_pVertices)[iZ];

                    // apply bone transformation to vertex
                    const Vector3F outputVertex = finalMatrix.Transform(inputVertex);

                    // apply the skin weights and calculate the final output vertex
                    skinVertices[iX] += (outputVertex.m_X * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                    skinVertices[iY] += (outputVertex.m_Y * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                    skinVertices[iZ] += (outputVertex.m_Z * (float)m_pModel->m_Deformers[i]->m_SkinWeights[j]->m_Weights[k]);
                }
            }
        }

        // get the mesh template to use to generate the final vertex buffer
        IMeshTemplate meshTemplate;
        meshTemplate.m_pVertices      = &skinVertices;
        meshTemplate.m_pIndices       = m_Templates[i]->m_pIndices;
        meshTemplate.m_pNormals       = m_Templates[i]->m_pNormals;
        meshTemplate.m_pNormalIndices = m_Templates[i]->m_pNormalIndices;
        meshTemplate.m_pUVs           = m_Templates[i]->m_pUVs;
        meshTemplate.m_pUVIndices     = m_Templates[i]->m_pUVIndices;

        // populate the vertex buffer
        if (!PopulateVertexBuffer(&meshTemplate, m_pModel->m_Mesh[i]->m_VB[0]))
            return nullptr;
    }

    return m_pModel;
}
//---------------------------------------------------------------------------
void FBXModel::Set_OnLoadTexture(Texture::ITfOnLoadTexture fOnLoadTexture)
{
    m_fOnLoadTexture = fOnLoadTexture;
}
//---------------------------------------------------------------------------
void FBXModel::ClearDataset(IFBXDataset& dataset) const
{
    for (IFBXDataset::iterator it = dataset.begin(); it != dataset.end(); ++it)
        delete (*it);

    dataset.clear();
}
//---------------------------------------------------------------------------
void FBXModel::SkipComment(const std::string& data, std::size_t& index) const
{
    // get data length
    const std::size_t dataLength = data.length();

    // skip all the line content since the comment symbol
    while (index < dataLength && data[index] != '\r' && data[index] != '\n')
        ++index;

    // now the line end is reached, skip the carriage return / line feed chars
    while (index < dataLength && (data[index] == '\r' || data[index] == '\n'))
        ++index;
}
//---------------------------------------------------------------------------
FBXModel::IFBXData* FBXModel::ReadWord(const std::string& data, std::size_t length, std::size_t& index) const
{
    std::unique_ptr<IFBXData> pFBXData(new IFBXData(&data));
    pFBXData->m_Start = index;
    pFBXData->m_End   = index;

    bool isReading = false;
    bool isStr     = false;
    bool isNumber  = true;
    bool foundDot  = false;
    bool foundExp  = false;

    // iterate through FBX data
    while (index < length)
    {
        bool doBreak = false;

        // dispatch the char
        switch (data[index])
        {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                // go to next char
                ++index;

                // already reading a data or a string?
                if (!isReading)
                {
                    // start to read the data
                    pFBXData->m_Start = index;
                    continue;
                }
                else
                if (isStr)
                    continue;

                // finalize the read
                pFBXData->m_End = index - 1;
                doBreak         = true;
                break;

            case ';':
                // already reading a string?
                if (isStr)
                {
                    // go to next char
                    ++index;
                    continue;
                }

                // already reading a data?
                if (isReading)
                {
                    pFBXData->m_End = index;
                    doBreak         = true;
                    break;
                }

                // go to next char
                ++index;

                return new IFBXData(IEDataType::IE_DT_Comment, &data, index - 1, index);

            case ':':
                // already reading a string?
                if (isStr)
                {
                    // go to next char
                    ++index;
                    continue;
                }

                // already reading a data?
                if (isReading)
                {
                    // finalize the read
                    pFBXData->m_End = index;
                    doBreak         = true;
                    break;
                }

                // go to next char
                ++index;

                // found a key separator
                return new IFBXData(IEDataType::IE_DT_KeySeparator, &data, index - 1, index);

            case ',':
                // already reading a string?
                if (isStr)
                {
                    // go to next char
                    ++index;
                    continue;
                }

                // already reading a data?
                if (isReading)
                {
                    // finalize the read
                    pFBXData->m_End = index;
                    doBreak         = true;
                    break;
                }

                // go to next char
                ++index;

                // found a value separator
                return new IFBXData(IEDataType::IE_DT_ValueSeparator, &data, index - 1, index);

            case '{':
                // already reading a string?
                if (isStr)
                {
                    // go to next char
                    ++index;
                    continue;
                }

                // already reading a data?
                if (isReading)
                {
                    // finalize the read
                    pFBXData->m_End = index;
                    doBreak         = true;
                    break;
                }

                // go to next char
                ++index;

                // found a start node separator
                return new IFBXData(IEDataType::IE_DT_StartNodeSeparator, &data, index - 1, index);

            case '}':
                // already reading a string?
                if (isStr)
                {
                    // go to next char
                    ++index;
                    continue;
                }

                // already reading a data?
                if (isReading)
                {
                    // finalize the read
                    pFBXData->m_End = index;
                    doBreak         = true;
                    break;
                }

                // go to next char
                ++index;

                // found an end node separator
                return new IFBXData(IEDataType::IE_DT_EndNodeSeparator, &data, index - 1, index);

            case '\"':
                // go to next char
                ++index;

                // already reading a string?
                if (isStr)
                {
                    // finalize the read
                    pFBXData->m_End = index - 1;
                    doBreak         = true;
                    break;
                }

                // started to read the data?
                if (isReading)
                    continue;

                // start to read a string
                pFBXData->m_Start = index;
                isReading         = true;
                isStr             = true;
                isNumber          = false;
                continue;

            default:
                // started to read the data?
                if (!isReading)
                    isReading = true;

                // check if numeric char, and belonging to floating point number
                if (isNumber                                &&
                   !isStr                                   &&
                   (data[index] < '0' || data[index] > '9') &&
                    data[index] != '-'                      &&
                    data[index] != '*'                      &&
                    data[index] != '.'                      &&
                    data[index] != 'e')
                    isNumber = false;
                else
                if (data[index] == '*' && index != pFBXData->m_Start)
                    isNumber = false;
                else
                if (data[index] == '.')
                    foundDot = true;
                else
                if (data[index] == 'e')
                    if (!foundExp)
                        foundExp = true;
                    else
                        isNumber = false;

                // go to next char
                ++index;
                continue;
        }

        if (doBreak)
            break;
    }

    // search for data type
    if (isReading)
        if (isStr)
            pFBXData->m_Type = IEDataType::IE_DT_String;
        else
        if (isNumber)
            pFBXData->m_Type = (foundDot || foundExp) ? IEDataType::IE_DT_Float : IEDataType::IE_DT_Int;
        else
            pFBXData->m_Type = IEDataType::IE_DT_Data;

    return pFBXData.release();
}
//---------------------------------------------------------------------------
void FBXModel::SetNode(const std::string&     name,
                             IFBXNode*&       pParent,
                             IFBXNodes&       nodes,
                             IFBXDataset&     dataset,
                             IItemDictionary& itemDict) const
{
    // found an opening child item, creates it
    std::unique_ptr<IFBXNode> pChild(new IFBXNode());
    pChild->SetName(name);
    pChild->SetParent(pParent);

    const std::size_t count = dataset.size();

    // add value list attached to the new child node
    for (std::size_t j = 0; j < count; ++j)
        pChild->AddValue(dataset[j]);

    if (count && name == "NodeAttribute"  ||
                 name == "Geometry"       ||
                 name == "Model"          ||
                 name == "Pose"           ||
                 name == "Material"       ||
                 name == "Deformer"       ||
                 name == "Video"          ||
                 name == "Texture"        ||
                 name == "AnimationStack" ||
                 name == "AnimationCurve" ||
                 name == "AnimationCurveNode")
        itemDict[dataset[0]->GetRaw()] = pChild.get();

    // clear the local dataset
    dataset.clear();

    // link node with its parent
    if (pParent)
        pParent->AddChild(pChild.get());
    else
        nodes.push_back(pChild.get());

    pParent = pChild.release();
}
//---------------------------------------------------------------------------
bool FBXModel::SetProperty(const std::string& name,
                                 IFBXNode*    pParent,
                           const IFBXDataset& dataset,
                                 std::size_t  start,
                                 std::size_t  end) const
{
    if (!pParent)
        return false;

    std::unique_ptr<IFBXProperty> pProp;

    // is a property group?
    if (name == "P")
    {
        // malformed dataset?
        if (dataset.size() <= 1)
            return false;

        // the second dataset item is the type
        const std::string type = m_Data.substr(dataset[1]->m_Start, dataset[1]->m_End - dataset[1]->m_Start);

        // create a property of correct type
        if (type == "bool")
            pProp.reset(new IFBXBoolProperty());
        else
        if (type == "int")
            pProp.reset(new IFBXIntProperty());
        else
        if (type == "double")
            pProp.reset(new IFBXDoubleProperty());
        else
        if (type == "ULongLong")
            pProp.reset(new IFBXLongLongProperty());
        else
        if (type == "KString")
            pProp.reset(new IFBXStringProperty());
        else
        if (type == "Number")
            pProp.reset(new IFBXNumberProperty());
        else
        if (type == "Color")
            pProp.reset(new IFBXColorProperty());
        else
        if (type == "ColorRGB")
            pProp.reset(new IFBXColorRGBProperty());
        else
        if (type == "Vector")
            pProp.reset(new IFBXVectorProperty());
        else
        if (type == "Vector3D")
            pProp.reset(new IFBXVector3DProperty());
        else
        if (type == "KTime")
            pProp.reset(new IFBXTimeProperty());
        else
        if (type == "DateTime")
            pProp.reset(new IFBXDateTimeProperty());
        else
        if (type == "enum")
            pProp.reset(new IFBXEnumProperty());
        else
        if (type == "Lcl Translation")
            pProp.reset(new IFBXLclTranslationProperty());
        else
        if (type == "Lcl Rotation")
            pProp.reset(new IFBXLclRotationProperty());
        else
        if (type == "Lcl Scaling")
            pProp.reset(new IFBXLclScalingProperty());
        else
        if (type == "Visibility")
            pProp.reset(new IFBXVisibilityProperty());
        else
        if (type == "Visibility Inheritance")
            pProp.reset(new IFBXVisibilityInheritanceProperty());
        else
        if (type == "Compound")
            pProp.reset(new IFBXCompoundProperty());
        else
        if (type == "object")
            pProp.reset(new IFBXObjectProperty());
        else
            pProp.reset(new IFBXUnknownProperty());
    }
    else
    if (name == "a")
    {
        std::size_t capacity = 0;

        if (pParent && pParent->GetValueCount() && pParent->GetValue(0)->m_Type == IEDataType::IE_DT_Int)
            capacity = pParent->GetValue(0)->GetInt();

        // property is an array
        pProp.reset(new IFBXArrayProperty(capacity));
    }
    else
        // unknown property
        pProp.reset(new IFBXUnknownProperty());

    // configure the property
    pProp->SetName(name);
    pProp->SetParent(pParent);

    // add the values
    for (std::size_t i = start; i < end; ++i)
        pProp->AddValue(dataset[i]);

    // link property to its parent
    pParent->AddProp(pProp.get());
    pProp.release();

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::PerformLinks()
{
    const std::size_t rootCount = m_Nodes.size();

    // iterate through FBX nodes
    for (std::size_t i = 0; i < rootCount; ++i)
        // is the connection node?
        if (m_Nodes[i]->GetName() == "Connections")
        {
            // get connection count
            const std::size_t connectionCount = m_Nodes[i]->GetPropCount();

            // iterate through connections
            for (std::size_t j = 0; j < connectionCount; ++j)
            {
                // get the next link to perform
                IFBXProperty* pLink = m_Nodes[i]->GetProp(j);

                // found it?
                if (!pLink)
                    return false;

                // already performed?
                if (m_UsedProps.find(pLink) != m_UsedProps.end())
                    continue;

                IEConnectionType type;
                std::size_t      srcID;
                std::size_t      dstID;
                std::string      propDesc;

                // extract the link data
                if (!GetLinkData(pLink, type, srcID, dstID, propDesc))
                    return false;

                // is a root?
                if (type == IEConnectionType::IE_CT_ObjObj && !dstID)
                {
                    // get root item to link
                    IFBXItem* pItem = m_ItemDict[pLink->GetValue(1)->GetRaw()];

                    // found it?
                    if (!pItem)
                        return false;

                    // root item should be a model
                    if (pItem->GetName() == "Model")
                    {
                        // mark link as processed
                        m_UsedProps.insert(pLink);

                        // create a new mesh
                        std::unique_ptr<IFBXLink> pMesh(new IFBXLink());
                        pMesh->m_NodeType = IENodeType::IE_NT_Model;
                        pMesh->m_pNode    = static_cast<IFBXNode*>(pItem);

                        // perform children links onto this mesh object
                        if (!PerformLink(pMesh.get(), srcID, m_Nodes[i]))
                            return false;

                        // add it in links list
                        m_Links.push_back(pMesh.get());
                        pMesh.release();
                    }
                }
            }

            // clear the used prop cache (no longer requires since now)
            m_UsedProps.clear();
        }

    #ifdef _DEBUG
        //std::string log;
        //LogLinks(log);
        //REM __TEMP(log);
    #endif

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::PerformLink(IFBXLink* pParentLink, std::size_t id, IFBXNode* pConnections)
{
    const std::size_t connectionCount = pConnections->GetPropCount();

    // iterate through connections
    for (std::size_t i = 0; i < connectionCount; ++i)
    {
        // get the next link to perform
        IFBXProperty* pLink = pConnections->GetProp(i);

        // found it?
        if (!pLink)
            return false;

        // already performed?
        if (m_UsedProps.find(pLink) != m_UsedProps.end())
            continue;

        IEConnectionType type;
        std::size_t      srcID;
        std::size_t      dstID;
        std::string      propDesc;

        // extract link data
        if (!GetLinkData(pLink, type, srcID, dstID, propDesc))
            return false;

        // found a connection to link with this parent?
        if (dstID == id)
        {
            // mark link as performed
            m_UsedProps.insert(pLink);

            // get source node
            IFBXItem* pItem = m_ItemDict[pLink->GetValue(1)->GetRaw()];

            // found it?
            if (!pItem)
                return false;

            // create the child link (the source) and add it to its parent (the destination)
            std::unique_ptr<IFBXLink> pChild(new IFBXLink());
            pChild->m_pParent  = pParentLink;
            pChild->m_pNode    = static_cast<IFBXNode*>(pItem);
            pChild->m_PropDesc = propDesc;

            // get node name
            const std::string name = pItem->GetName();

            // search for node type
            if (name == "Model")
                pChild->m_NodeType = IENodeType::IE_NT_Model;
            else
            if (name == "Geometry")
                pChild->m_NodeType = IENodeType::IE_NT_Geometry;
            else
            if (name == "Material")
                pChild->m_NodeType = IENodeType::IE_NT_Material;
            else
            if (name == "Texture")
                pChild->m_NodeType = IENodeType::IE_NT_Texture;
            else
            if (name == "Video")
                pChild->m_NodeType = IENodeType::IE_NT_Video;
            else
            if (name == "Deformer")
                pChild->m_NodeType = IENodeType::IE_NT_Deformer;
            else
            if (name == "NodeAttribute")
                pChild->m_NodeType = IENodeType::IE_NT_NodeAttribute;
            else
            if (name == "AnimationCurve")
                pChild->m_NodeType = IENodeType::IE_NT_AnimationCurve;
            else
            if (name == "AnimationCurveNode")
                pChild->m_NodeType = IENodeType::IE_NT_AnimationCurveNode;
            else
                return false;

            // perform the link
            if (!PerformLink(pChild.get(), srcID, pConnections))
                return false;

            // add it to its parent
            pParentLink->m_Children.push_back(pChild.get());
            pChild.release();
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::GetLinkData(IFBXProperty*     pProp,
                           IEConnectionType& type,
                           std::size_t&      srcID,
                           std::size_t&      dstID,
                           std::string&      propDesc) const
{
    // no property?
    if (!pProp)
        return false;

    // check if node may contain a connection link
    if (pProp->GetValueCount() < 3                          ||
        pProp->GetValue(1)->m_Type != IEDataType::IE_DT_Int ||
        pProp->GetValue(2)->m_Type != IEDataType::IE_DT_Int)
        return false;

    const std::string connectionType = pProp->GetValue(0)->GetRaw();

    // search for link connection type
    if (connectionType == "OO")
        type = IEConnectionType::IE_CT_ObjObj;
    else
    if (connectionType == "OP")
        type = IEConnectionType::IE_CT_ObjProp;
    else
    if (connectionType == "PO")
        type = IEConnectionType::IE_CT_PropObj;
    else
    if (connectionType == "PP")
        type = IEConnectionType::IE_CT_PropProp;
    else
        return false;

    // get the source and destination identifiers
    srcID = pProp->GetValue(1)->GetInt();
    dstID = pProp->GetValue(2)->GetInt();

    // sometimes the link may contain an extra value, which is a property description
    if (pProp->GetValueCount() >= 4)
        propDesc = pProp->GetValue(3)->GetStr();

    return true;
}
//---------------------------------------------------------------------------
#ifdef _DEBUG
    void FBXModel::LogLinks(std::string& log) const
    {
        for (IFBXLinks::const_iterator it = m_Links.begin(); it != m_Links.end(); ++it)
            LogLink(*it, 0, log);
    }
#endif
//---------------------------------------------------------------------------
#ifdef _DEBUG
    void FBXModel::LogLink(IFBXLink* pLink, unsigned tab, std::string& log) const
    {
        if (!pLink)
            return;

        for (unsigned i = 0; i < tab; ++i)
            log += " ";

        if (pLink->m_pNode->GetValueCount() >= 2)
            log += pLink->m_pNode->GetName() + ": " + pLink->m_pNode->GetValue(1)->GetRaw() + "\n";
        else
            log += pLink->m_pNode->GetName() + "\n";

        for (IFBXLinks::const_iterator it = pLink->m_Children.begin(); it != pLink->m_Children.end(); ++it)
            LogLink(*it, tab + 4, log);
    }
#endif
//---------------------------------------------------------------------------
bool FBXModel::BuildModel()
{
    std::unique_ptr<Model> pModel(new Model());

    // -1 to set the max possible value by default
    std::size_t skeletonIndex = -1;

    // get link count
    const std::size_t linkCount = m_Links.size();

    // iterate through links
    for (std::size_t i = 0; i < linkCount; ++i)
        // found the model skeleton?
        if (m_Links[i]->m_pNode                                   &&
            m_Links[i]->m_pNode->GetName()             == "Model" &&
            m_Links[i]->m_pNode->GetValueCount()       >= 3       &&
            m_Links[i]->m_pNode->GetValue(2)->GetRaw() == "Null")
        {
            // keep the skeleton index
            skeletonIndex = i;

            // create a new skeleton
            std::unique_ptr<Model::IBone> pSkeleton(new Model::IBone);

            // build it
            if (!BuildSkeleton(m_Links[i], pSkeleton.get(), true, m_BoneDict))
                return false;

            // add skeleton to model
            m_BoneDict[m_Links[i]->m_pNode] = pSkeleton.get();
            pModel->m_pSkeleton             = pSkeleton.release();

            break;
        }

    // iterate through links
    for (std::size_t i = 0; i < linkCount; ++i)
    {
        // ignore skeleton (already built)
        if (i == skeletonIndex)
            continue;

        // create a new vertex buffer and configure its format
        std::unique_ptr<VertexBuffer> pModelVB(new VertexBuffer());
        pModelVB->m_Format.m_Format = (VertexFormat::IEFormat)((unsigned)VertexFormat::IEFormat::IE_VF_Colors |
                                                               (unsigned)VertexFormat::IEFormat::IE_VF_TexCoords);

        // get link child count
        const std::size_t childCount = m_Links[i]->m_Children.size();

        // iterate through links
        for (std::size_t j = 0; j < childCount; ++j)
        {
            // search for mesh content
            if (m_Links[i]->m_Children[j]->m_NodeType == IENodeType::IE_NT_Geometry)
            {
                // found the geometry, create a template to keep the vertex buffer source data
                std::unique_ptr<IMeshTemplate> pTemplate(new IMeshTemplate());

                // get grand children count
                const std::size_t grandChildCount = m_Links[i]->m_Children[j]->m_pNode->GetChildCount();

                // search for vertices, normals and UVs tables
                for (std::size_t k = 0; k < grandChildCount; ++k)
                    if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetName() == "Vertices" &&
                        m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetPropCount())
                        pTemplate->m_pVertices =
                                static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetProp(0))->GetPtr();
                    else
                    if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetName() == "PolygonVertexIndex" &&
                        m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetPropCount())
                        pTemplate->m_pIndices =
                                static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetProp(0))->GetPtr();
                    else
                    if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetName() == "LayerElementNormal")
                    {
                        const std::size_t normalPropCount = m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChildCount();

                        for (std::size_t l = 0; l < normalPropCount; ++l)
                            if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetName() == "Normals" &&
                                m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetPropCount())
                                pTemplate->m_pNormals =
                                        static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetProp(0))->GetPtr();
                            else
                            if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetName() == "NormalsIndex" &&
                                m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetPropCount())
                                pTemplate->m_pNormalIndices =
                                        static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetProp(0))->GetPtr();
                    }
                    else
                    if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetName() == "LayerElementUV")
                    {
                        const std::size_t normalPropCount = m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChildCount();

                        for (std::size_t l = 0; l < normalPropCount; ++l)
                            if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetName() == "UV" &&
                                m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetPropCount())
                                pTemplate->m_pUVs =
                                        static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetProp(0))->GetPtr();
                            else
                            if (m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetName() == "UVIndex" &&
                                m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetPropCount())
                                pTemplate->m_pUVIndices =
                                        static_cast<IFBXArrayProperty*>(m_Links[i]->m_Children[j]->m_pNode->GetChild(k)->GetChild(l)->GetProp(0))->GetPtr();
                    }

                // found all required tables?
                if (!pTemplate->IsValid())
                    return false;

                // configure the mesh vertex buffer
                pModelVB->m_Format.m_Type        = VertexFormat::IEType::IE_VT_Triangles;
                pModelVB->m_Culling.m_Type       = VertexCulling::IECullingType::IE_CT_Back;
                pModelVB->m_Culling.m_Face       = VertexCulling::IECullingFace::IE_CF_CCW;
                pModelVB->m_Material.m_Color.m_R = 1.0f;
                pModelVB->m_Material.m_Color.m_G = 1.0f;
                pModelVB->m_Material.m_Color.m_B = 1.0f;
                pModelVB->m_Material.m_Color.m_A = 1.0f;

                // get the vertices and indices count
                const std::size_t indicesCount = pTemplate->m_pIndices->size();

                // reserve the vertex buffer data size (will be built later)
                pModelVB->m_Data.reserve(indicesCount * 9 * ((*pTemplate->m_pIndices)[4] < 0 ? 6 : 3));

                // keep the source template
                m_Templates.push_back(pTemplate.get());
                pTemplate.release();

                // get the grandchild links
                const std::size_t linkGrandchildCount = m_Links[i]->m_Children[j]->m_Children.size();

                // iterate through grandchild
                for (std::size_t k = 0; k < linkGrandchildCount; ++k)
                    // is a deformers node?
                    if (m_Links[i]->m_Children[j]->m_Children[k]->m_NodeType == IENodeType::IE_NT_Deformer)
                    {
                        // get deformers
                        IFBXLink* pDeformers = m_Links[i]->m_Children[j]->m_Children[k];

                        // found it?
                        if (!pDeformers)
                            return false;

                        // create a new model deformers
                        std::unique_ptr<Model::IDeformers> pModelDeformers(new Model::IDeformers());

                        // get the deformers children count
                        const std::size_t subDeformerCount = pDeformers->m_Children.size();

                        // iterate through the deformers children
                        for (std::size_t l = 0; l < subDeformerCount; ++l)
                            // found a sub-deformers node?
                            if (pDeformers->m_Children[l]->m_NodeType == IENodeType::IE_NT_Deformer)
                            {
                                // create a new skin weights
                                std::unique_ptr<Model::ISkinWeights> pModelSkinWeights(new Model::ISkinWeights());

                                // sub-deformers linked model exists?
                                if (pDeformers->m_Children[l]->m_Children.size() &&
                                    pDeformers->m_Children[l]->m_Children[0]->m_NodeType == IENodeType::IE_NT_Model)
                                {
                                    // get the bone to link with the skin weights
                                    IBoneDictionary::iterator it = m_BoneDict.find(pDeformers->m_Children[l]->m_Children[0]->m_pNode);

                                    // found it?
                                    if (it != m_BoneDict.end())
                                        pModelSkinWeights->m_pBone = it->second;
                                }

                                // get sub-deformers children count
                                const std::size_t deformerChildCount = pDeformers->m_Children[l]->m_pNode->GetChildCount();

                                // iterate through sub-deformers children
                                for (std::size_t m = 0; m < deformerChildCount; ++m)
                                {
                                    // get sub-deformers child node
                                    IFBXNode* pDefChildNode = pDeformers->m_Children[l]->m_pNode->GetChild(m);

                                    // found it and contains properties?
                                    if (pDefChildNode && pDefChildNode->GetPropCount())
                                    {
                                        // get property name
                                        const std::string propName = pDefChildNode->GetName();

                                        // search for indexes and weights
                                        if (propName == "Indexes")
                                        {
                                            // get source index table
                                            const std::vector<double>* pIndexes =
                                                    static_cast<IFBXArrayProperty*>(pDefChildNode->GetProp(0))->GetPtr();

                                            // found it?
                                            if (pIndexes)
                                            {
                                                // get index count
                                                const std::size_t indexCount = pIndexes->size();

                                                // resize the destination array
                                                pModelSkinWeights->m_WeightInfluences.resize(indexCount);

                                                // copy the indices
                                                for (std::size_t n = 0; n < indexCount; ++n)
                                                {
                                                    std::unique_ptr<Model::IWeightInfluence> pInfluence(new Model::IWeightInfluence());
                                                    pInfluence->m_VertexIndex.push_back((int)(*pIndexes)[n]);

                                                    pModelSkinWeights->m_WeightInfluences[n] = pInfluence.get();
                                                    pInfluence.release();
                                                }
                                            }
                                        }
                                        else
                                        if (propName == "Weights")
                                        {
                                            // get source weights table
                                            const std::vector<double>* pWeights =
                                                    static_cast<IFBXArrayProperty*>(pDefChildNode->GetProp(0))->GetPtr();

                                            // found it?
                                            if (pWeights)
                                            {
                                                // get weights count
                                                const std::size_t weightsCount = pWeights->size();

                                                // resize the destination array
                                                pModelSkinWeights->m_Weights.resize(weightsCount);

                                                // copy the weights
                                                for (std::size_t n = 0; n < weightsCount; ++n)
                                                    pModelSkinWeights->m_Weights[n] = (float)(*pWeights)[n];
                                            }
                                        }
                                        else
                                        if (propName == "Transform")
                                        {
                                            // get source matrix
                                            const std::vector<double>* pMatrix =
                                                    static_cast<IFBXArrayProperty*>(pDefChildNode->GetProp(0))->GetPtr();

                                            // found it?
                                            if (pMatrix)
                                            {
                                                if (pMatrix->size() != 16)
                                                    continue;

                                                // get and cache the weight transform matrix
                                                pModelSkinWeights->m_TransformMatrix.Set((float)(*pMatrix)[0], (float)(*pMatrix)[4], (float)(*pMatrix)[8],  (float)(*pMatrix)[12],
                                                                                         (float)(*pMatrix)[1], (float)(*pMatrix)[5], (float)(*pMatrix)[9],  (float)(*pMatrix)[13],
                                                                                         (float)(*pMatrix)[2], (float)(*pMatrix)[6], (float)(*pMatrix)[10], (float)(*pMatrix)[14],
                                                                                         (float)(*pMatrix)[3], (float)(*pMatrix)[7], (float)(*pMatrix)[11], (float)(*pMatrix)[15]);

                                                // inverse it
                                                float determinant = 0.0f;
                                                pModelSkinWeights->m_TransformMatrix = pModelSkinWeights->m_TransformMatrix.Inverse(determinant);
                                            }
                                        }
                                        else
                                        if (propName == "TransformLink")
                                        {
                                            // get source matrix
                                            const std::vector<double>* pMatrix =
                                                    static_cast<IFBXArrayProperty*>(pDefChildNode->GetProp(0))->GetPtr();

                                            // found it?
                                            if (pMatrix)
                                            {
                                                if (pMatrix->size() != 16)
                                                    continue;

                                                // get and cache the weight transform link matrix
                                                pModelSkinWeights->m_TransformLinkMatrix.Set((float)(*pMatrix)[0], (float)(*pMatrix)[4], (float)(*pMatrix)[8],  (float)(*pMatrix)[12],
                                                                                             (float)(*pMatrix)[1], (float)(*pMatrix)[5], (float)(*pMatrix)[9],  (float)(*pMatrix)[13],
                                                                                             (float)(*pMatrix)[2], (float)(*pMatrix)[6], (float)(*pMatrix)[10], (float)(*pMatrix)[14],
                                                                                             (float)(*pMatrix)[3], (float)(*pMatrix)[7], (float)(*pMatrix)[11], (float)(*pMatrix)[15]);

                                                // inverse it
                                                float determinant = 0.0f;
                                                pModelSkinWeights->m_TransformLinkMatrix =
                                                        pModelSkinWeights->m_TransformLinkMatrix.Inverse(determinant);
                                            }
                                        }
                                    }
                                }

                                // get the weight matrix to apply
                                pModelSkinWeights->m_Matrix = pModelSkinWeights->m_TransformLinkMatrix;

                                // add model skin weights to model deformers
                                pModelDeformers->m_SkinWeights.push_back(pModelSkinWeights.get());
                                pModelSkinWeights.release();
                            }

                        // add model deformers to model
                        pModel->m_Deformers.push_back(pModelDeformers.get());
                        pModelDeformers.release();
                    }
            }
            else
            if (m_Links[i]->m_Children[j]->m_NodeType == IENodeType::IE_NT_Material)
            {
                // get grandchildren count
                const std::size_t grandChildCount = m_Links[i]->m_Children[j]->m_Children.size();

                // iterate through grandchildren
                for (std::size_t k = 0; k < grandChildCount; ++k)
                    // is a texture?
                    if (m_Links[i]->m_Children[j]->m_Children[k]->m_NodeType == IENodeType::IE_NT_Texture)
                    {
                        // get texture node
                        IFBXNode* pTextureNode = m_Links[i]->m_Children[j]->m_Children[k]->m_pNode;

                        // found it?
                        if (!pTextureNode)
                            continue;

                        // get texture node properties count
                        const std::size_t texPropCount = pTextureNode->GetPropCount();

                        // iterate through texture node properties
                        for (std::size_t l = 0; l < texPropCount; ++l)
                            // is a file name?
                            if (pTextureNode->GetProp(l)->GetName() == "FileName" && pTextureNode->GetProp(l)->GetValueCount())
                                // load texture callback defined?
                                if (m_fOnLoadTexture)
                                {
                                    // let the caller load and create the texture
                                    std::unique_ptr<Texture> pTexture(m_fOnLoadTexture(pTextureNode->GetProp(l)->GetValue(0)->GetStr(), true));

                                    // successfully created?
                                    if (pTexture)
                                        // add it to model
                                        pModelVB->m_Material.m_pTexture = pTexture.release();
                                }
                    }
            }
        }

        // link the built vertex buffer to its mesh
        std::unique_ptr<Mesh> pMesh(new Mesh());
        pMesh->m_VB.push_back(pModelVB.get());
        pModelVB.release();

        // link the built mesh to its model
        pModel->m_Mesh.push_back(pMesh.get());
        pMesh.release();
    }

    // keep the built model
    m_pModel = pModel.release();

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::BuildSkeleton(IFBXLink* pLink, Model::IBone* pBone, bool isRoot, IBoneDictionary& boneDict) const
{
    if (!pLink)
        return false;

    if (!pBone)
        return false;

    // get link child count
    const std::size_t childCount = pLink->m_Children.size();

    // iterate through link children
    for (std::size_t i = 0; i < childCount; ++i)
        // found a model node?
        if (pLink->m_Children[i]->m_NodeType == IENodeType::IE_NT_Model)
        {
            // is the root link?
            if (isRoot)
            {
                // populate the bone
                if (!PopulateBone(pLink->m_Children[i]->m_pNode, pBone))
                    return false;

                // build children bones
                if (!BuildSkeleton(pLink->m_Children[i], pBone, false, boneDict))
                    return false;

                continue;
            }

            // create a new bone
            std::unique_ptr<Model::IBone> pChildBone(new Model::IBone);
            pChildBone->m_pParent = pBone;

            // populate the bone
            if (!PopulateBone(pLink->m_Children[i]->m_pNode, pChildBone.get()))
                return false;

            // build children bones
            if (!BuildSkeleton(pLink->m_Children[i], pChildBone.get(), false, boneDict))
                return false;

            // add bone to skeleton
            boneDict[pLink->m_Children[i]->m_pNode] = pChildBone.get();
            pBone->m_Children.push_back(pChildBone.get());
            pChildBone.release();
        }

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::PopulateBone(IFBXNode* pNode, Model::IBone* pBone) const
{
    if (!pNode)
        return false;

    if (!pBone)
        return false;

    std::string boneName;

    // get bone name, if any
    if (pNode->GetValueCount() >= 2)
        pBone->m_Name = pNode->GetValue(1)->GetStr();

    // get node child count
    const std::size_t count = pNode->GetChildCount();

    // iterate through child nodes
    for (std::size_t i = 0; i < count; ++i)
    {
        // get child
        IFBXNode* pChild = pNode->GetChild(i);

        // found it?
        if (!pChild)
            return false;

        // get properties set containing the bone data
        if (pChild->GetName() == "Properties70")
        {
            // get property count
            const std::size_t propCount = pChild->GetPropCount();

            Vector3F   translation;
            Vector3F   scaling(1.0f, 1.0f, 1.0f);
            Matrix4x4F rotMat = Matrix4x4F::Identity();

            // iterate through properties
            for (std::size_t j = 0; j < propCount; ++j)
            {
                // get property
                IFBXProperty* pProp = pChild->GetProp(j);

                // found it?
                if (!pProp)
                    return false;

                // property contains values?
                if (pProp->GetValueCount())
                {
                    // get property name
                    const std::string name = pProp->GetValue(0)->GetRaw();

                    // search for local translation, rotation an scaling
                    if (name == "Lcl Translation")
                    {
                        IFBXLclTranslationProperty* pLclTransProp = static_cast<IFBXLclTranslationProperty*>(pProp);

                        if (!pLclTransProp)
                            return false;

                        translation = pLclTransProp->Get();
                    }
                    else
                    if (name == "Lcl Rotation")
                    {
                        IFBXLclRotationProperty* pLclRotatProp = static_cast<IFBXLclRotationProperty*>(pProp);

                        if (!pLclRotatProp)
                            return false;

                        rotMat = pLclRotatProp->Get();
                    }
                    else
                    if (name == "Lcl Scaling")
                    {
                        IFBXLclScalingProperty* pLclScalingProp = static_cast<IFBXLclScalingProperty*>(pProp);

                        if (!pLclScalingProp)
                            return false;

                        scaling = pLclScalingProp->Get();
                    }
                }
            }

            // create the translation matrix
            Matrix4x4F transMat = Matrix4x4F::Identity();
            transMat.m_Table[3][0] = translation.m_X;
            transMat.m_Table[3][1] = translation.m_Y;
            transMat.m_Table[3][2] = translation.m_Z;

            // create the scale matrix
            Matrix4x4F scaleMat = Matrix4x4F::Identity();
            scaleMat.m_Table[0][0] = scaling.m_X;
            scaleMat.m_Table[1][1] = scaling.m_Y;
            scaleMat.m_Table[2][2] = scaling.m_Z;

            // build the bone matrix
            pBone->m_Matrix = rotMat.Multiply(transMat).Multiply(scaleMat);
        }
    }

    return true;
}
//---------------------------------------------------------------------------
bool FBXModel::PopulateVertexBuffer(const IMeshTemplate* pMeshTemplate, VertexBuffer* pModelVB) const
{
    if (!pMeshTemplate)
        return false;

    if (!pModelVB)
        return false;

    Vector3F vertex;
    Vector3F normal;
    Vector2F uv;

    // get the vertices and indices count
    const std::size_t verticesCount = pMeshTemplate->m_pVertices->size();
    const std::size_t indicesCount  = pMeshTemplate->m_pIndices->size();

    std::vector<int> indices;
    indices.reserve(4);

    std::vector<int> normalIndices;
    normalIndices.reserve(4);

    std::vector<int> uvIndices;
    uvIndices.reserve(4);

    // iterate through indices
    for (std::size_t i = 0; i < indicesCount; ++i)
    {
        // get indice
        const int indice = (int)(*pMeshTemplate->m_pIndices)[i];

        // is the last indice of a group?
        if (indice >= 0)
        {
            // no, keep it
            indices.push_back(indice);
            normalIndices.push_back((int)(*pMeshTemplate->m_pNormalIndices)[i]);
            uvIndices.push_back((int)(*pMeshTemplate->m_pUVIndices)[i]);
        }
        else
        {
            // yes, calculate and keep the last indice
            indices.push_back(std::abs(indice) - 1);
            normalIndices.push_back(std::abs((int)(*pMeshTemplate->m_pNormalIndices)[i]));
            uvIndices.push_back(std::abs((int)(*pMeshTemplate->m_pUVIndices)[i]));

            // is surface a polygon or a quad?
            switch (indices.size())
            {
                case 3:
                    // polygon, extract it
                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[0] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[0] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[0] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[0] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[1] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[1] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[1] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[1] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[1] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[1] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[1] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[1] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[2] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[2] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[2] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[2] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    break;

                case 4:
                    // quad, extract the first polygon...
                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[0] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[0] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[0] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[0] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[1] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[1] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[1] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[1] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[1] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[1] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[1] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[1] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[2] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[2] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[2] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[2] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    // ...and the second
                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[0] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[0] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[0] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[0] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[0] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[0] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[2] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[2] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[2] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[2] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[2] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[2] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    vertex.m_X = (float)(*pMeshTemplate->m_pVertices)[ indices[3] * 3];
                    vertex.m_Y = (float)(*pMeshTemplate->m_pVertices)[(indices[3] * 3) + 1];
                    vertex.m_Z = (float)(*pMeshTemplate->m_pVertices)[(indices[3] * 3) + 2];

                    normal.m_X = (float)(*pMeshTemplate->m_pNormals)[ normalIndices[3] * 3];
                    normal.m_Y = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[3] * 3) + 1];
                    normal.m_Z = (float)(*pMeshTemplate->m_pNormals)[(normalIndices[3] * 3) + 2];

                    uv.m_X     = (float)(*pMeshTemplate->m_pUVs)[ uvIndices[3] * 2];
                    uv.m_Y     = (float)(*pMeshTemplate->m_pUVs)[(uvIndices[3] * 2) + 1];

                    pModelVB->Add(&vertex, &normal, &uv, 0, nullptr);

                    break;
            }

            // clear the indice tables for the next round
            indices.clear();
            normalIndices.clear();
            uvIndices.clear();
        }
    }

    return true;
}
//---------------------------------------------------------------------------
//REM
#include <Windows.h>
void FBXModel::__TEMP(std::string& log) const
{
    ::OutputDebugStringA(log.c_str());
}
//---------------------------------------------------------------------------
