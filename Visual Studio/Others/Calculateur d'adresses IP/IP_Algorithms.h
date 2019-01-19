// **********************************************************************
// *                                                                    *
// *           Classe IP_Algorithms : Définitions de fonctions            *
// *                                                                    *
// **********************************************************************

#if !defined(AFX_IP_ALGORITHMS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_)
#define AFX_IP_ALGORITHMS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BIT32            2147483648
#define BIT31            1073741824

#define VALUE32BITS        4294967295

#define MASKOCTET1        4278190080
#define MASKOCTET2        16711680
#define MASKOCTET3        65280
#define MASKOCTET4        255

DWORD ConvertStringToDWORD(CString Value);

class IP_Algorithms  
{
    BOOL    Erreur;
    BOOL    UnlockBitMask;

    DWORD    IP1;
    DWORD    IP2;
    DWORD    IP3;
    DWORD    IP4;

    DWORD    Nb_Postes_Souhaites;
    DWORD    Nb_Reseaux_Souhaites;

    DWORD    Iterator;
    DWORD    Counter;

    DWORD    NbBits_Reserved;

    // Partie postes
    DWORD    Postes_Max;
    DWORD    Postes_NbBits;
    DWORD    Postes_Masque;

    // Partie Réseau
    DWORD    Reseaux_Max;
    DWORD    Reseaux_NbBits;
    DWORD    Reseaux_Masque;

    // Résultats généraux
    DWORD    Masque_de_sous_reseaux;
    DWORD    Pas;

    void CheckBitMask();

    void IP_GereBitsNonAttribues();

    void IP_CalculePostes();
    void IP_CalculeReseaux();
    void IP_Combine();

    public:

    CString    str_Plage1;
    CString    str_Plage2;

    CString str_Mask1;
    CString str_Mask2;
    CString str_Mask3;
    CString str_Mask4;

    CString strMaxP;
    CString strNbBitsP;
    CString strMaxSR;
    CString strNbBitsSR;
    CString strPas;

    CString strBit;

    CString strIP1;
    CString strIP2;
    CString strIP3;
    CString strIP4;

    CString str_UserP;
    CString str_UserSR;

    IP_Algorithms();

    DWORD GetBitMask();
    BOOL GetUnlock();

    void SetBitMask(DWORD Value);
    void SetIP1(DWORD Value);
    void SetIP2(DWORD Value);
    void SetIP3(DWORD Value);
    void SetIP4(DWORD Value);
    void SetNbPostes(DWORD Value);
    void SetNbReseaux(DWORD Value);
    void TestAdresses();
    void SetUnlock(BOOL Unlock);

    void IP_Calculate();

    void IP_Flush();
    void IP_Reset();
};

#endif // !defined(AFX_IP_ALGORITHMS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_)