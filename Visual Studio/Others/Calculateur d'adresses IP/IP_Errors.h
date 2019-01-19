// **************************************************************
// *                                                            *
// *                     Gestion des erreurs                    *
// *                                                            *
// **************************************************************

#if !defined(AFX_IP_ERRORS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_)
#define AFX_IP_ERRORS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const char Title[]="Calculateur d'adresses IP";

const char Err_Vide[]            ="Erreur : Il n'y a ni postes, ni r�seaux. Aucune adresse n'a pu �tre calcul�e.";
const char Err_32Overflow1[]    ="Erreur : Le nombre de bits r�serv�s ou le nombre de postes est trop grand. Il n'y a plus de place pour l'adressage du masque de sous-r�seau.";
const char Err_32Overflow2[]    ="Erreur : Le nombre de r�seaux ou le nombre de bits r�serv�s est trop grand. Il n'y a plus de place pour l'adressage des machines h�tes.";
const char Err_32Overflow3[]    ="Erreur : Le nombre de sous-r�seaux, le nombre de postes ou le nombre de bits r�serv�s sont trop grands. La valeur 32 bits est d�pass�e.";
const char Err_BadAdressIP[]    ="Erreur : L'adresse IP est illogique. Le nombre de bits de cette adresse est plus grand que le nombre de bits r�serv�s pour le masque de sous-r�seaux.";

const char War_IP192168[]        ="Attention : Les adresses IP 192.168.x.x sont r�serv�es pour les r�seaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP10[]            ="Attention : Les adresses IP 10.x.x.x sont r�serv�es pour les r�seaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP169254[]        ="Attention : Les adresses IP 169.254.x.x sont r�serv�es par Microsoft pour L'APIPA. (Automatic Private IP Adress) Elles sont attribu�es automatiquement par les machines, et sont donc � proscrire pour la mise en service d'un r�seau.";
const char War_IP1721631[]        ="Attention : Les adresses IP 172.16.0.0 � 172.31.255.255 sont r�serv�es pour les r�seaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP127[]            ="Attention : Les adresses IP 127.0.0.0 � 127.255.255.255 sont des adresses de bouclage. Elles ne sont pas utilisables pour un r�seau.";
const char War_UnlockMask[]        ="Attention : Vous avez choisi de d�verrouiller le masque de bits d'entr�e. Les calculs r�alis�s ne seront plus garantis aux normes. Vous devrez �galement faire attention � g�rer vous-m�me la valeur du masque d'entr�e";

const char Msg_Reattribution[]    ="Les valeurs d'entr�e (Nombre de postes et nombre de sous-r�seaux) ont �t� mises � jour.";

void IP_SetError(LPCTSTR Error);
void IP_SetWarning(LPCTSTR Warning);
void IP_SetMessage(LPCTSTR Message);

#endif // !defined(AFX_IP_ERRORS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_)