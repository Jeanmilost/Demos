/****************************************************************************
 * ==> String Sorting ------------------------------------------------------*
 ****************************************************************************
 * Description : Demo showing the string sorting difference between the     *
 *               lexicographical sorting (used by default by the std) and   *
 *               the natural order (or alphanumerical) sorting (used by     *
 *               Windows Explorer)                                          *
 * Developer   : Jean-Milost Reymond                                        *
 ****************************************************************************/

#include <string.h>
#include <vector>
#include <algorithm>
#include <iostream>

#include <Shlwapi.h>

typedef std::vector<std::wstring> IStrings;

//------------------------------------------------------------------------------
bool LexicographicalComp(const std::wstring& a, const std::wstring& b)
{
    return a < b;
}
//------------------------------------------------------------------------------
bool NaturalOrderComp(const std::wstring& a, const std::wstring& b)
{
    return ::StrCmpLogicalW(a.c_str(), b.c_str()) < 0;
}
//------------------------------------------------------------------------------
int main()
{
    IStrings fileList;
    fileList.push_back(L"z1.doc");
    fileList.push_back(L"z2.doc");
    fileList.push_back(L"z3.doc");
    fileList.push_back(L"z4.doc");
    fileList.push_back(L"z5.doc");
    fileList.push_back(L"z6.doc");
    fileList.push_back(L"z7.doc");
    fileList.push_back(L"z8.doc");
    fileList.push_back(L"z9.doc");
    fileList.push_back(L"z10.doc");
    fileList.push_back(L"z11.doc");
    fileList.push_back(L"z12.doc");
    fileList.push_back(L"z13.doc");
    fileList.push_back(L"z14.doc");
    fileList.push_back(L"z15.doc");
    fileList.push_back(L"z16.doc");
    fileList.push_back(L"z17.doc");
    fileList.push_back(L"z18.doc");
    fileList.push_back(L"z19.doc");
    fileList.push_back(L"z20.doc");
    fileList.push_back(L"z100.doc");
    fileList.push_back(L"z101.doc");
    fileList.push_back(L"z102.doc");

    // sort by lexicographical order. This is the default algorithm used by the std
    std::sort(fileList.begin(), fileList.end(), LexicographicalComp);

    std::cout << "lexicographical sorting:\n";

    // show the sorted content
    for (std::size_t i = 0; i < fileList.size(); ++i)
        std::wcout << fileList[i] << L"\n";

    std::cout << "\n";

    // sort by natural order (or alphanumerical). This algorithm is used in Windows Explorer
    // see: https://web.archive.org/web/20210207124255/davekoelle.com/alphanum.html for further info
    std::sort(fileList.begin(), fileList.end(), NaturalOrderComp);

    std::cout << "natural order sorting:\n";

    // show the sorted content
    for (std::size_t i = 0; i < fileList.size(); ++i)
        std::wcout << fileList[i] << L"\n";
}
//------------------------------------------------------------------------------
