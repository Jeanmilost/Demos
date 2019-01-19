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

const char Err_Vide[]            ="Erreur : Il n'y a ni postes, ni réseaux. Aucune adresse n'a pu être calculée.";
const char Err_32Overflow1[]    ="Erreur : Le nombre de bits réservés ou le nombre de postes est trop grand. Il n'y a plus de place pour l'adressage du masque de sous-réseau.";
const char Err_32Overflow2[]    ="Erreur : Le nombre de réseaux ou le nombre de bits réservés est trop grand. Il n'y a plus de place pour l'adressage des machines hôtes.";
const char Err_32Overflow3[]    ="Erreur : Le nombre de sous-réseaux, le nombre de postes ou le nombre de bits réservés sont trop grands. La valeur 32 bits est dépassée.";
const char Err_BadAdressIP[]    ="Erreur : L'adresse IP est illogique. Le nombre de bits de cette adresse est plus grand que le nombre de bits réservés pour le masque de sous-réseaux.";

const char War_IP192168[]        ="Attention : Les adresses IP 192.168.x.x sont réservées pour les réseaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP10[]            ="Attention : Les adresses IP 10.x.x.x sont réservées pour les réseaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP169254[]        ="Attention : Les adresses IP 169.254.x.x sont réservées par Microsoft pour L'APIPA. (Automatic Private IP Adress) Elles sont attribuées automatiquement par les machines, et sont donc à proscrire pour la mise en service d'un réseau.";
const char War_IP1721631[]        ="Attention : Les adresses IP 172.16.0.0 à 172.31.255.255 sont réservées pour les réseaux LAN d'entreprise, car elles ne sont pas rootable sur internet.";
const char War_IP127[]            ="Attention : Les adresses IP 127.0.0.0 à 127.255.255.255 sont des adresses de bouclage. Elles ne sont pas utilisables pour un réseau.";
const char War_UnlockMask[]        ="Attention : Vous avez choisi de déverrouiller le masque de bits d'entrée. Les calculs réalisés ne seront plus garantis aux normes. Vous devrez également faire attention à gérer vous-même la valeur du masque d'entrée";

const char Msg_Reattribution[]    ="Les valeurs d'entrée (Nombre de postes et nombre de sous-réseaux) ont été mises à jour.";

void IP_SetError(LPCTSTR Error);
void IP_SetWarning(LPCTSTR Warning);
void IP_SetMessage(LPCTSTR Message);

#endif // !defined(AFX_IP_ERRORS_H__47594DD0_12B3_4AF4_9D38_CE3223963BAF__INCLUDED_)