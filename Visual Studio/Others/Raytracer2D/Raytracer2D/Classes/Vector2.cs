/****************************************************************************
 * ==> Vector2 -------------------------------------------------------------*
 ****************************************************************************
 * Description : 2D vector                                                  *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************
 * MIT License - 2D Raytracer                                               *
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

using System;

namespace Raytracer2D.Classes
{
    /**
    * 2D vector
    *@author Jean-Milost Reymond
    */
    class Vector2
    {
        /**
        * Gets or sets vector x coordinate
        */
        public float X { get; set; } = 0.0f;

        /**
        * Gets or sets vector y coordinate
        */
        public float Y { get; set; } = 0.0f;

        /**
        * Constructor
        */
        public Vector2()
        {}

        /**
        * Constructor
        *@param x - x coordinate
        *@param y - y coordinate
        */
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        /**
        * Copy constructor
        *@param other - other vector to copy from
        */
        public Vector2(Vector2 other)
        {
            X = other.X;
            Y = other.Y;
        }

        /**
        * Addition operator
        *@param lhs - first vector to add
        *@param rhs - second vector to add with
        *@return added vector
        */
        public static Vector2 operator + (Vector2 lhs, Vector2 rhs) => new Vector2(lhs.X + rhs.X, lhs.Y + rhs.Y);

        /**
        * Subtraction operator
        *@param lhs - first vector to subtract
        *@param rhs - second vector to subtract with
        *@return subtracted vector
        */
        public static Vector2 operator - (Vector2 lhs, Vector2 rhs) => new Vector2(lhs.X - rhs.X, lhs.Y - rhs.Y);

        /**
        * Multiplication operator
        *@param lhs - first vector to multiply
        *@param rhs - second vector to multiply with
        *@return multiplied vector
        */
        public static Vector2 operator * (Vector2 lhs, Vector2 rhs) => new Vector2(lhs.X * rhs.X, lhs.Y * rhs.Y);

        /**
        * Division operator
        *@param lhs - first vector to divide
        *@param rhs - second vector to divide by
        *@return divided vector
        */
        public static Vector2 operator / (Vector2 lhs, Vector2 rhs)
        {
            if (rhs.X == 0 || rhs.Y == 0)
                throw new Exception("Division by 0");

            return new Vector2(lhs.X / rhs.X, lhs.Y / rhs.Y);
        }

        /**
        * Equality operator
        *@param lhs - first vector to compare
        *@param rhs - second vector to compare with
        *@return true if vectors are equals, otherwise false
        */
        public static bool operator == (Vector2 lhs, Vector2 rhs) => lhs.Equals(rhs);

        /**
        * Difference operator
        *@param lhs - first vector to compare
        *@param rhs - second vector to compare with
        *@return true if vectors are different, otherwise false
        */
        public static bool operator != (Vector2 lhs, Vector2 rhs) => !lhs.Equals(rhs);

        /**
        * Checks if another object equals this object
        *@param other - other object to compare with
        *@return true if the other object equals this object, otherwise false
        */
        public override bool Equals(Object other)
        {
            // check for null and compare runtime types
            if ((other == null) || !GetType().Equals(other.GetType()))
                return false;

            return Equals((Vector2)other);
        }

        /**
        * Checks if another vector equals this vector
        *@param other - other vector to compare with
        *@return true if the other vector equals this vector, otherwise false
        */
        public bool Equals(Vector2 other)
        {
            return (X, Y).Equals((other.X, other.Y));
        }

        /**
        * Gets the hash code
        *@return the hash code
        */
        public override int GetHashCode()
        {
            return (X, Y).GetHashCode();
        }

        /**
        * Calculates the vector length
        *@return vector length
        */
        public float Length()
        {
            return (float)Math.Sqrt((X * X) + (Y * Y));
        }

        /**
        * Normalizes the vector
        *@return normalized vector
        */
        public Vector2 Normalize()
        {
            // calculate vector length
            float len = Length();

            // no vector length?
            if (len == 0.0f)
                return new Vector2();

            // normalize vector (thus values will always be between 0.0f, and 1.0f)
            return new Vector2(X / len, Y / len);
        }

        /**
        * Calculates cross product between 2 vectors
        *@param vector - other vector to cross with
        *@return the resulting vector
        */
        public Vector2 Cross(Vector2 vector)
        {
            return new Vector2((Y * vector.X) - (vector.Y * X),
                               (X * vector.Y) - (vector.X * Y));
        }

        /**
        * Calculates dot product between 2 vectors
        *@param vector - other vector to dot with
        *@return resulting angle
        */
        public float Dot(Vector2 vector)
        {
            return ((X * vector.X) + (Y * vector.Y));
        }
    }
}
