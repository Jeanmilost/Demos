/****************************************************************************
 * ==> QR_STDTools ---------------------------------------------------------*
 ****************************************************************************
 * Description : Some basic STD library helper functions                    *
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

#ifndef QR_STDToolsH
#define QR_STDToolsH

// std
#include <vector>
#include <list>
#include <map>

// qr engine
#include "QR_Types.h"

/**
* Some basic STD library tools functions
*@note This class is cross-platform
*@author Jean-Milost Reymond
*/
class QR_STDTools
{
    public:
        /**
        * Deletes all vector content, clears vector at the end
        *@param vec - vector to delete and clear
        */
        template <class T>
        static void DelAndClear(std::vector<T*>& vec);

        /**
        * Deletes all list content, clears list at the end
        *@param lst - list to delete and clear
        */
        template <class T>
        static void DelAndClear(std::list<T*>& lst);

        /**
        * Deletes all map content, clears map at the end
        *@param mp - map to delete and clear
        */
        template <class T, class U>
        static void DelAndClear(std::map<T, U*>& mp);
};
//---------------------------------------------------------------------------
// QR_STDTools
//---------------------------------------------------------------------------
template <class T>
void QR_STDTools::DelAndClear(std::vector<T*>& vec)
{
    // get vector item count
    const QR_SizeT count = vec.size();

    // iterate through vector items
    for (QR_SizeT i = 0; i < count; ++i)
        // delete vector item
        delete vec[i];

    // clear vector completely
    vec.clear();
}
//---------------------------------------------------------------------------
template <class T>
void QR_STDTools::DelAndClear(std::list<T*>& lst)
{
    // iterate through list items
    for (typename std::list<T*>::iterator it = lst.begin(); it != lst.end(); ++it)
        // delete list item
        delete (*it);

    // clear list completely
    lst.clear();
}
//---------------------------------------------------------------------------
template <class T, class U>
void QR_STDTools::DelAndClear(std::map<T, U*>& mp)
{
    // iterate through list items
    for (typename std::map<T, U*>::iterator it = mp.begin(); it != mp.end(); ++it)
        // delete vector item
        delete it->second;

    // clear vector completely
    mp.clear();
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// RAD studio
//---------------------------------------------------------------------------
#if defined(CP_EMBARCADERO)
    // needed to avoid the W8058 error "Cannot create pre-compiled header: header incomplete"
    // warning in BCC compilers
    ;
#endif
//---------------------------------------------------------------------------

#endif
