/****************************************************************************
 * ==> QR_MD2CfgFile -------------------------------------------------------*
 ****************************************************************************
 * Description : Provides a parser for the MD2 configuration file           *
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

#ifndef QR_MD2CfgFileH
#define QR_MD2CfgFileH

// qr engine
#include "QR_Types.h"
#include "QR_Script.h"

/**
* MD2 configuration file parser
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_MD2CfgFile : public QR_Script
{
    public:
        /**
        * Standard MD2 animations gestures enumeration
        *@note These values should never be reordered, as they match with the order in file
        */
        enum IEGesture
        {
            IE_AG_Stand,
            IE_AG_Run,
            IE_AG_Attack,
            IE_AG_Pain1,
            IE_AG_Pain2,
            IE_AG_Pain3,
            IE_AG_Jump,
            IE_AG_Flip,
            IE_AG_Salute,
            IE_AG_Taunt,
            IE_AG_Wave,
            IE_AG_Point,
            IE_AG_CRStand,
            IE_AG_CRWalk,
            IE_AG_CRAttack,
            IE_AG_CRPain,
            IE_AG_CRDeath,
            IE_AG_CRDeath2,
            IE_AG_CRDeath3,
            IE_AG_CRDeath4,
        };

        /**
        * MD2 Gesture, contains the information that defines a gesture in the MD2 model
        */
        struct IGesture
        {
            QR_SizeT m_StartFrame;
            QR_SizeT m_FrameCount;
            QR_SizeT m_FrameLoop;
            QR_SizeT m_FramesPerSecond;

            IGesture();
            virtual ~IGesture();
        };

        typedef std::map<QR_SizeT, IGesture*> IGestureDictionary;

        /**
        * Constructor
        */
        QR_MD2CfgFile();

        /**
        * Destructor
        */
        virtual ~QR_MD2CfgFile();

        /**
        * Clears script
        */
        virtual void Clear();

        /**
        * Copies content from another configuration file
        *@param other - other configuration file to copy from
        */
        virtual void Copy(const QR_MD2CfgFile& other);

        /**
        * Gets gesture at index
        *@return gesture at index, NULL if not found or on error
        */
        virtual IGesture* GetGesture(QR_SizeT index) const;

        /**
        * Gets gesture count
        *@return gesture count
        */
        virtual QR_SizeT GetGestureCount() const;

    protected:
        IGestureDictionary m_Gestures;

        /**
        * Called when script line should be parsed
        *@param line - line to parse
        *@param linbeNb - line number
        *@return ture on success, otherwise false
        */
        virtual bool OnParseLine(const std::string& line, QR_SizeT lineNb);

        /**
        * Parses a word found in script line
        *@param word - word to parse
        *@param lineNb - current parsing line number
        *@return true on success, otherwise false
        */
        virtual bool ParseWord(const std::string& word, QR_SizeT lineNb);

    private:
        QR_SizeT m_Column;
        QR_SizeT m_StartLine;
        QR_SizeT m_CurLine;
        QR_SizeT m_Gesture;
        bool     m_LongComment;

        /**
        * Deletes all content and clears memory
        */
        void DelAndClear();
};

#endif
