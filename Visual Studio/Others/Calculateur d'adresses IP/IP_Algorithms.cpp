// **********************************************************************
// *                                                                    *
// *                Classe IP_Algorithms : Fonctions                    *
// *                                                                    *
// **********************************************************************

#include "stdafx.h"
#include "IP_Calculator.h"
#include "IP_Algorithms.h"
#include "IP_Attribuation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Convertit une chaîne de caractères alphanumérique en nombre.
DWORD ConvertStringToDWORD(CString Value)
{
    DWORD NumValue=0;
    DWORD FinalValue=0;
    DWORD Multiply=1;

    for (DWORD i=0; i<(DWORD)Value.GetLength(); i++)
    {
        TCHAR Unity=Value.GetAt(Value.GetLength()-i-1);

        switch (Unity)
        {
            case '0':
            {
                NumValue=0;
                break;
            }

            case '1':
            {
                NumValue=1;
                break;
            }

            case '2':
            {
                NumValue=2;
                break;
            }

            case '3':
            {
                NumValue=3;
                break;
            }

            case '4':
            {
                NumValue=4;
                break;
            }

            case '5':
            {
                NumValue=5;
                break;
            }

            case '6':
            {
                NumValue=6;
                break;
            }

            case '7':
            {
                NumValue=7;
                break;
            }

            case '8':
            {
                NumValue=8;
                break;
            }

            case '9':
            {
                NumValue=9;
                break;
            }

            default:
            {
                return 0;
                break;
            }
        }

        FinalValue+=NumValue*Multiply;

        Multiply*=10;
    }

    return FinalValue;
}

// ********************************** Classe IP_Algorithms ************************************

// Constructeur de la classe.
IP_Algorithms::IP_Algorithms()
{
    UnlockBitMask=FALSE;

    IP_Reset();
}

// Attribue la première plage de valeur IP
void IP_Algorithms::SetIP1(DWORD Value)
{
    IP1=Value;

    strIP1.Format(_T("%d"), IP1);
}

// Attribue la deuxième plage de valeur IP
void IP_Algorithms::SetIP2(DWORD Value)
{
    IP2=Value;

    strIP2.Format(_T("%d"), IP2);
}

// Attribue la troisième plage de valeur IP
void IP_Algorithms::SetIP3(DWORD Value)
{
    IP3=Value;

    strIP3.Format(_T("%d"), IP3);
}

// Attribue la quatrième plage de valeur IP
void IP_Algorithms::SetIP4(DWORD Value)
{
    IP4=Value;

    strIP4.Format(_T("%d"), IP4);
}

// Attribue la valeur du masque de bits d'entrée
void IP_Algorithms::SetBitMask(DWORD Value)
{
    NbBits_Reserved=Value;
    strBit.Format(_T("%d"), NbBits_Reserved);
}

// Retourne la valeur du masque de bits
DWORD IP_Algorithms::GetBitMask()
{
    CheckBitMask();

    return NbBits_Reserved;
}

// Vérifie et bride la valeur du masque de bits
void IP_Algorithms::CheckBitMask()
{
    if (UnlockBitMask==FALSE)
    {
        DWORD MinNbBits=0;

        if (IP1!=0) MinNbBits=8;
        if (IP2!=0) MinNbBits=16;
        if (IP3!=0) MinNbBits=24;

        if (NbBits_Reserved<MinNbBits) NbBits_Reserved=MinNbBits;
    }

    strBit.Format(_T("%d"), NbBits_Reserved);
}

// Verrouille / Déverrouille le masque de bits.
void IP_Algorithms::SetUnlock(BOOL Unlock)
{
    UnlockBitMask=Unlock;
}

// Retourne le verrou du masque de bits.
BOOL IP_Algorithms::GetUnlock()
{
    return UnlockBitMask;
}

// Attribue le nombre de postes d'entrée
void IP_Algorithms::SetNbPostes(DWORD Value)
{
    Nb_Postes_Souhaites=Value;
}

// Attribue le nombre de réseaux d'entrée
void IP_Algorithms::SetNbReseaux(DWORD Value)
{
    Nb_Reseaux_Souhaites=Value;
}

// Cherche le nombre de bits inutilisés, et les attribuent.
void IP_Algorithms::IP_GereBitsNonAttribues()
{
    IP_CalculePostes();
    IP_CalculeReseaux();

    if (Postes_NbBits+Reseaux_NbBits+NbBits_Reserved<32)
    {
        CIP_Attribuation Attrib(32-(Postes_NbBits+Reseaux_NbBits+NbBits_Reserved));

        if (Attrib.DoModal()==IDOK)
        {
            Postes_NbBits+=Attrib.NbToPosts;
            Iterator=1;

            for (DWORD i=0; i<Postes_NbBits; i++)
            {
                Iterator<<=1;
            }

            Nb_Postes_Souhaites=Iterator-2;

            Reseaux_NbBits+=Attrib.NbToSR;

            Iterator=1;

            for (std::size_t i=0; i<Reseaux_NbBits; i++)
            {
                Iterator<<=1;
            }

            Nb_Reseaux_Souhaites=Iterator-2;
        }
    }

    Postes_Max                = 0;
    Postes_NbBits            = 0;
    Postes_Masque            = 0;
    Reseaux_Max                = 0;
    Reseaux_NbBits            = 0;
    Reseaux_Masque            = 0;
}

// Résoud la valeur des postes dans l'adressage IP
void IP_Algorithms::IP_CalculePostes()
{
    Iterator=BIT31;
    Counter=31;

    while (Counter!=1)
    {
        if ((Iterator-1)<=Nb_Postes_Souhaites)
        {
            Postes_Max=(Iterator*2)-2;
            Postes_NbBits=Counter;

            for (int i=Counter; i>0; i--)
            {
                Postes_Masque|=Iterator;
                Iterator>>=1;
            }

            return;
        }
        else
        {
            Counter--;
            Iterator>>=1;
        }
    }

    Postes_Max                = 0;
    Postes_NbBits            = 0;
    Postes_Masque            = 0;
}

// Résoud la valeur des sous-réseaux dans l'adressage IP
void IP_Algorithms::IP_CalculeReseaux()
{
    Iterator=BIT31;
    Counter=31;

    while (Counter!=1)
    {
        if ((Iterator-1)<=Nb_Reseaux_Souhaites)
        {
            Reseaux_Max=(Iterator*2)-2;
            Reseaux_NbBits=Counter;

            for (int i=Counter; i>0; i--)
            {
                Reseaux_Masque|=Iterator;
                Iterator>>=1;
            }

            return;
        }
        else
        {
            Counter--;
            Iterator>>=1;
        }
    }

    Reseaux_Max                = 0;
    Reseaux_NbBits            = 0;
    Reseaux_Masque            = 0;
}

// Combine les valeurs des postes et des sous-réseaux de l'adresse IP
void IP_Algorithms::IP_Combine()
{
    if (Postes_NbBits!=0 && Reseaux_NbBits==0)
    {
        Masque_de_sous_reseaux=(~Postes_Masque);

        if ((NbBits_Reserved+Postes_NbBits)>=31)
        {
            IP_SetError(Err_32Overflow1);
            Erreur=TRUE;

            return;
        }
        else Reseaux_NbBits=32-(NbBits_Reserved+Postes_NbBits);

        Iterator=1;

        for (DWORD i=0; i<Reseaux_NbBits; i++)
        {
            Reseaux_Masque|=Iterator;
            Iterator<<=1;
        }

        Reseaux_Max=Iterator-2;
        Reseaux_Masque<<=Postes_NbBits;
    }
    else if (Postes_NbBits==0 && Reseaux_NbBits!=0)
    {
        if ((NbBits_Reserved+Reseaux_NbBits)>=31)
        {
            IP_SetError(Err_32Overflow2);
            Erreur=TRUE;

            return;
        }
        else
        {
            if (NbBits_Reserved==0) Reseaux_Masque<<=32-Reseaux_NbBits;
            else Reseaux_Masque<<=32-(NbBits_Reserved+Reseaux_NbBits);

            Postes_NbBits=32-(NbBits_Reserved+Reseaux_NbBits);

            Iterator=BIT32;

            for (DWORD i=0; i<32-Postes_NbBits; i++)
            {
                Masque_de_sous_reseaux|=Iterator;
                Iterator>>=1;
            }

            Postes_Masque=~(VALUE32BITS&Masque_de_sous_reseaux);
            Postes_Max=(Iterator*2)-2;
        }
    }
    else if (Postes_NbBits!=0 && Reseaux_NbBits!=0)
    {
        if ((NbBits_Reserved+Reseaux_NbBits+Postes_NbBits)>32)
        {
            IP_SetError(Err_32Overflow3);
            Erreur=TRUE;

            return;
        }

        Reseaux_Masque<<=Postes_NbBits;

        Iterator=BIT32;

        for (DWORD i=0; i<32-Postes_NbBits; i++)
        {
            Masque_de_sous_reseaux|=Iterator;
            Iterator>>=1;
        }
    }
    else
    {
        IP_SetError(Err_Vide);

        Erreur=TRUE;

        return;
    }
}

// Teste si l'adresse d'entrée est réservée
void IP_Algorithms::TestAdresses()
{
    if (IP1==192 && IP2==168) IP_SetWarning(War_IP192168);
    else if (IP1==10) IP_SetWarning(War_IP10);
    else if (IP1==169 && IP2==254) IP_SetWarning(War_IP169254);
    else if (IP1==172 && IP2>=16 && IP2<=31) IP_SetWarning(War_IP1721631);
    else if (IP1==127) IP_SetWarning(War_IP127);
}

// Calcule les valeurs finales
void IP_Algorithms::IP_Calculate()
{
    strIP1.Format(_T("%d"), IP1);
    strIP2.Format(_T("%d"), IP2);
    strIP3.Format(_T("%d"), IP3);
    strIP4.Format(_T("%d"), IP4);

    TestAdresses();
    CheckBitMask();

    DWORD OldP=Nb_Postes_Souhaites;
    DWORD OldSR=Nb_Reseaux_Souhaites;

    if (Nb_Postes_Souhaites!=0 &&
        Nb_Reseaux_Souhaites!=0 &&
        UnlockBitMask==FALSE &&
        (IP1!=0 || IP2!=0 || IP3!=0 || IP4!=0))
    {
        IP_GereBitsNonAttribues();

        if ((OldP!=Nb_Postes_Souhaites) || (OldSR!=Nb_Reseaux_Souhaites))
            IP_SetMessage(Msg_Reattribution);
    }

    IP_CalculePostes();
    IP_CalculeReseaux();
    IP_Combine();

    if (Erreur==TRUE)
    {
        IP_Flush();
        return;
    }

    DWORD Octet1=Masque_de_sous_reseaux&MASKOCTET1;
    DWORD Octet2=Masque_de_sous_reseaux&MASKOCTET2;
    DWORD Octet3=Masque_de_sous_reseaux&MASKOCTET3;
    DWORD Octet4=Masque_de_sous_reseaux&MASKOCTET4;

    Octet1>>=24;
    Octet2>>=16;
    Octet3>>=8;

    str_Mask1.Format(_T("%d"), Octet1);
    str_Mask2.Format(_T("%d"), Octet2);
    str_Mask3.Format(_T("%d"), Octet3);
    str_Mask4.Format(_T("%d"), Octet4);

    // S'il n'y a pas de réseaux souhaités au départ, il faut tenir
    // compte des octets "remplis", à laisser en réserve.
    if (Nb_Reseaux_Souhaites==0)
    {
        DWORD Counter=0;
        DWORD ResIterator=0;

        if (Reseaux_NbBits<=9)
        {
            ResIterator=Reseaux_Max+2;
            Counter=Reseaux_NbBits;
        }

        if (Reseaux_NbBits>9)
        {
            Reseaux_Masque&=~MASKOCTET1;

            DWORD Iterator=BIT32;
            ResIterator=1;
            Counter=0;

            while (Iterator!=0)
            {
                if (Iterator&Reseaux_Masque)
                {
                    Counter++;
                    ResIterator<<=1;
                }

                Iterator>>=1;
            }
        }

        if (Reseaux_NbBits>17)
        {
            Reseaux_Masque&=~MASKOCTET2;

            DWORD Iterator=BIT32;
            ResIterator=1;
            Counter=0;

            while (Iterator!=0)
            {
                if (Iterator&Reseaux_Masque)
                {
                    Counter++;
                    ResIterator<<=1;
                }

                Iterator>>=1;
            }
        }

        if (Reseaux_NbBits>25)
        {
            Reseaux_Masque&=~MASKOCTET3;

            DWORD Iterator=BIT32;
            ResIterator=1;
            Counter=0;

            while (Iterator!=0)
            {
                if (Iterator&Reseaux_Masque)
                {
                    Counter++;
                    ResIterator<<=1;
                }

                Iterator>>=1;
            }
        }

        Reseaux_Max=ResIterator-2;
        Reseaux_NbBits=Counter;
    }

    // Calcule le pas.
    Pas=1;
    Pas<<=Postes_NbBits;

    // Calcule la première plage.
    DWORD Plage1[8];

    DWORD FMaskMin=Pas;
    DWORD FMaskMax=Reseaux_Masque-Pas;

    Plage1[1]=FMaskMin&MASKOCTET1;
    Plage1[2]=FMaskMin&MASKOCTET2;
    Plage1[3]=FMaskMin&MASKOCTET3;
    Plage1[4]=FMaskMin&MASKOCTET4;

    Plage1[5]=FMaskMax&MASKOCTET1;
    Plage1[6]=FMaskMax&MASKOCTET2;
    Plage1[7]=FMaskMax&MASKOCTET3;
    Plage1[8]=FMaskMax&MASKOCTET4;

    Plage1[1]>>=24;
    Plage1[2]>>=16;
    Plage1[3]>>=8;

    Plage1[5]>>=24;
    Plage1[6]>>=16;
    Plage1[7]>>=8;

    DWORD FIP1=IP1&Octet1;
    DWORD FIP2=IP2&Octet2;
    DWORD FIP3=IP3&Octet3;
    DWORD FIP4=IP4&Octet4;

    Plage1[1]+=FIP1;
    Plage1[2]+=FIP2;
    Plage1[3]+=FIP3;
    Plage1[4]+=FIP4;
    Plage1[5]+=FIP1;
    Plage1[6]+=FIP2;
    Plage1[7]+=FIP3;
    Plage1[8]+=FIP4;

    if (Plage1[1]>255 ||
        Plage1[2]>255 ||
        Plage1[3]>255 ||
        Plage1[4]>255 ||
        Plage1[5]>255 ||
        Plage1[6]>255 ||
        Plage1[7]>255 ||
        Plage1[8]>255)
    {
        IP_SetError(Err_BadAdressIP);
        IP_Flush();
        return;
    }

    str_Plage1.Format(_T("%d.%d.%d.%d - %d.%d.%d.%d"),
                      Plage1[1],
                      Plage1[2],
                      Plage1[3],
                      Plage1[4],
                      Plage1[5],
                      Plage1[6],
                      Plage1[7],
                      Plage1[8]);

    // Calcule la deuxième plage.
    for (DWORD i=0; i!=(Pas*Reseaux_Max); i+=Pas)
    {
        DWORD PostMin=1;
        DWORD PostMax=0;

        DWORD Iterator=1;

        for (DWORD Posts=0; Posts<Postes_NbBits; Posts++)
        {
            PostMax|=Iterator;
            Iterator<<=1;
        }

        PostMax-=1;

        DWORD Resx=Pas+i;

        DWORD FMaskMin=Resx+PostMin;
        DWORD FMaskMax=Resx+PostMax;

        DWORD R1=FMaskMin&MASKOCTET1;
        DWORD R2=FMaskMin&MASKOCTET2;
        DWORD R3=FMaskMin&MASKOCTET3;
        DWORD R4=FMaskMin&MASKOCTET4;

        DWORD R5=FMaskMax&MASKOCTET1;
        DWORD R6=FMaskMax&MASKOCTET2;
        DWORD R7=FMaskMax&MASKOCTET3;
        DWORD R8=FMaskMax&MASKOCTET4;

        R1>>=24;
        R2>>=16;
        R3>>=8;

        R5>>=24;
        R6>>=16;
        R7>>=8;

        DWORD FIP1=IP1&Octet1;
        DWORD FIP2=IP2&Octet2;
        DWORD FIP3=IP3&Octet3;
        DWORD FIP4=IP4&Octet4;

        R1+=FIP1;
        R2+=FIP2;
        R3+=FIP3;
        R4+=FIP4;
        R5+=FIP1;
        R6+=FIP2;
        R7+=FIP3;
        R8+=FIP4;

        if (R1>255 || R2>255 || R3>255 || R4>255 || R5>255 || R6>255 || R7>255 || R8>255)
        {
            IP_SetError(Err_BadAdressIP);
            IP_Flush();
            return;
        }

        CString TmpString;

        TmpString.Format(_T("%d.%d.%d.%d - %d.%d.%d.%d\r\n"),
                         R1,
                         R2,
                         R3,
                         R4,
                         R5,
                         R6,
                         R7,
                         R8);
        
        str_Plage2+=TmpString;
        TmpString=_T("");
    }

    DWORD PasOctet1=Pas&MASKOCTET1;
    DWORD PasOctet2=Pas&MASKOCTET2;
    DWORD PasOctet3=Pas&MASKOCTET3;
    DWORD PasOctet4=Pas&MASKOCTET4;

    PasOctet1>>=24;
    PasOctet2>>=16;
    PasOctet3>>=8;

    if (PasOctet1!=0) Pas=PasOctet1;
    else if (PasOctet2!=0) Pas=PasOctet2;
    else if (PasOctet3!=0) Pas=PasOctet3;
    else if (PasOctet4!=0) Pas=PasOctet4;

    strMaxP.Format(_T("%d"), Postes_Max);
    strNbBitsP.Format(_T("%d"), Postes_NbBits);
    strMaxSR.Format(_T("%d"), Reseaux_Max);
    strNbBitsSR.Format(_T("%d"), Reseaux_NbBits);
    strPas.Format(_T("%d"), Pas);

    str_UserP.Format(_T("%d"), Nb_Postes_Souhaites);
    str_UserSR.Format(_T("%d"), Nb_Reseaux_Souhaites);

    Iterator=BIT32;

    NbBits_Reserved=0;

    for (std::size_t i=0; i<32; i++)
    {
        if (Masque_de_sous_reseaux&Iterator) NbBits_Reserved++;

        Iterator>>=1;
    }

    strBit.Format(_T("%d"), NbBits_Reserved);
}

// Réinitialise les valeurs de sorties
void IP_Algorithms::IP_Flush()
{
    Iterator                = BIT32;
    Counter                    = 0;

    Erreur                    = FALSE;

    Postes_Max                = 0;
    Postes_NbBits            = 0;
    Postes_Masque            = 0;

    Reseaux_Max                = 0;
    Reseaux_NbBits            = 0;
    Reseaux_Masque            = 0;

    Masque_de_sous_reseaux    = 0;
    Pas                        = 0;

    NbBits_Reserved            = 0;

    str_Plage1                =_T("");
    str_Plage2                =_T("");

    str_Mask1                =_T("");
    str_Mask2                =_T("");
    str_Mask3                =_T("");
    str_Mask4                =_T("");

    strMaxP                    =_T("");
    strNbBitsP                =_T("");
    strMaxSR                =_T("");
    strNbBitsSR                =_T("");
    strPas                    =_T("");
}

// Réinitialise toutes les valeurs
void IP_Algorithms::IP_Reset()
{
    Iterator                = BIT32;
    Counter                    = 0;

    Erreur                    = FALSE;

    IP1                        = 0;
    IP2                        = 0;
    IP3                        = 0;
    IP4                        = 0;

    Nb_Postes_Souhaites        = 0;
    Nb_Reseaux_Souhaites    = 0;

    NbBits_Reserved            = 0;

    Postes_Max                = 0;
    Postes_NbBits            = 0;
    Postes_Masque            = 0;

    Reseaux_Max                = 0;
    Reseaux_NbBits            = 0;
    Reseaux_Masque            = 0;

    Masque_de_sous_reseaux    = 0;
    Pas                        = 0;

    strIP1                    =_T("");
    strIP2                    =_T("");
    strIP3                    =_T("");
    strIP4                    =_T("");

    str_Plage1                =_T("");
    str_Plage2                =_T("");

    str_Mask1                =_T("");
    str_Mask2                =_T("");
    str_Mask3                =_T("");
    str_Mask4                =_T("");

    strMaxP                    =_T("");
    strNbBitsP                =_T("");
    strMaxSR                =_T("");
    strNbBitsSR                =_T("");
    strPas                    =_T("");

    strBit                    =_T("");

    str_UserP                =_T("");
    str_UserSR                =_T("");
}