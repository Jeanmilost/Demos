/******************************************************************************
 * ==> QR_Vector2D -----------------------------------------------------------*
 ******************************************************************************
 * Description : Euclidean vector 2D                                          *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#ifndef QR_Vector2DH
#define QR_Vector2DH

#ifdef USING_DIRECTX
    #include <d3dx9math.h>
#endif

/**
* Vector 2D
*@note Do not inherit this class, or don't use std::memset to initialize it
*@author Jean-Milost Reymond
*/
class QR_Vector2D
{
    public:
        float m_X; // x coordinate for the 2D vector
        float m_Y; // y coordinate for the 2D vector

        /**
        * Constructor
        */
        QR_Vector2D();

        /**
        * Constructor
        *@param x - vector x value
        *@param y - vector y value
        */
        QR_Vector2D(const float& x, const float& y);

        #ifdef USING_DIRECTX
            /**
            * Constructor
            *@param d3dxVector - Direct3D vector
            */
            QR_Vector2D(const D3DXVECTOR2& d3dxVector);
        #endif

        /**
        * Destructor
        */
        /*virtual*/ ~QR_Vector2D();

        /**
        * Operator +
        *@param other - other vector to add
        *@return resulting added vector
        */
        QR_Vector2D operator + (const QR_Vector2D& other) const;

        /**
        * Operator -
        *@return inverted vector
        */
        QR_Vector2D operator - () const;

        /**
        * Operator -
        *@param other - other vector to substract
        *@return resulting substracted vector
        */
        QR_Vector2D operator - (const QR_Vector2D& other) const;

        /**
        * Operator *
        *@param other - other vector to multiply
        *@return resulting multiplicated vector
        */
        QR_Vector2D operator * (const QR_Vector2D& other) const;

        /**
        * Operator /
        *@param other - other vector to divide
        *@return resulting divided vector
        */
        QR_Vector2D operator / (const QR_Vector2D& other) const;

        /**
        * Operator +=
        *@param other - other vector to add
        *@return resulting added vector
        */
        const QR_Vector2D& operator += (const QR_Vector2D& other);

        /**
        * Operator -=
        *@param other - other vector to substract
        *@return resulting substracted vector
        */
        const QR_Vector2D& operator -= (const QR_Vector2D& other);

        /**
        * Operator *=
        *@param other - other vector to multiply
        *@return resulting multiplicated vector
        */
        const QR_Vector2D& operator *= (const QR_Vector2D& other);

        /**
        * Operator -=
        *@param other - other vector to divide
        *@return resulting divided vector
        */
        const QR_Vector2D& operator /= (const QR_Vector2D& other);

        /**
        * Operator ==
        *@param other - other vector to compare
        *@return true if vectors are identical, otherwise false
        */
        bool operator == (const QR_Vector2D& other) const;

        /**
        * Operator !=
        *@param other - other vector to compare
        *@return true if vectors are not identical, otherwise false
        */
        bool operator != (const QR_Vector2D& other) const;

        /**
        * Gets the vector length
        *@return the vector length
        */
        float Length() const;

        /**
        * Normalizes the vector
        *@return normalized vector
        */
        QR_Vector2D Normalize() const;

        /**
        * Calculates the cross product between 2 vectors
        *@param vector - vector with which cross product is calculated
        *@return cross product
        */
        QR_Vector2D Cross(const QR_Vector2D& vector) const;

        /**
        * Calculates the dot product between 2 vectors
        *@param vector - vector with which dot product is calculated
        *@return dot product
        */
        float Dot(const QR_Vector2D& vector) const;

        #ifdef USING_DIRECTX
            /**
            * Convert to Direct3D vector
            *@return Direct3D vector
            */
            D3DXVECTOR2 ToD3DXVector2() const;
        #endif
};

#endif // QR_Vector2DH
