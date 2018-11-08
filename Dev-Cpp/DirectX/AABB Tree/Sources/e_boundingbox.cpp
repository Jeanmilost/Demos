/*****************************************************************************
 * ==> Classe E_BoundingBox -------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe repr�sente une bo�te de collisions pour un     *
 *               objet.                                                      *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_boundingbox.h"

// Constructeur de la classe E_BoundingBox.
E_BoundingBox::E_BoundingBox()
{
    p_Min              = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Max              = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

    p_Box              = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
    p_AdressU          = 0;
    p_AdressV          = 0;
    p_ZWriteEnable     = 0;
    p_AlphaBlendEnable = 0;
    p_AlphaTestEnable  = 0;
    p_SrcBlend         = 0;
    p_DestBlend        = 0;
    p_CullMode         = 0;
    p_NumMaterials     = 0;
}

// Deuxi�me constructeur de la classe E_BoundingBox.
E_BoundingBox::E_BoundingBox( D3DXVECTOR3 Min, D3DXVECTOR3 Max )
{
    p_Min              = Min;
    p_Max              = Max;

    p_Box              = NULL;

    p_R                = 0;
    p_G                = 0;
    p_B                = 0;
    p_AdressU          = 0;
    p_AdressV          = 0;
    p_ZWriteEnable     = 0;
    p_AlphaBlendEnable = 0;
    p_AlphaTestEnable  = 0;
    p_SrcBlend         = 0;
    p_DestBlend        = 0;
    p_CullMode         = 0;
    p_NumMaterials     = 0;
}

// Destructeur de la classe E_BoundingBox.
E_BoundingBox::~E_BoundingBox()
{
    // On commande la destruction de l'objet virtuel, s'il a �t� cr�e.
    ReleaseVirtualObject();
}

// Obtient la coordonn�e du coin inf�rieur de la bo�te.
D3DXVECTOR3 E_BoundingBox::GetMin()
{
    return p_Min;
}

// D�finit la coordonn�e du coin inf�rieur de la bo�te.
void E_BoundingBox::SetMin( D3DXVECTOR3 Min )
{
    p_Min = Min;
}

// Obtient la coordonn�e du coin sup�rieur de la bo�te.
D3DXVECTOR3 E_BoundingBox::GetMax()
{
    return p_Max;
}

// D�finit la coordonn�e du coin sup�rieur de la bo�te.
void E_BoundingBox::SetMax( D3DXVECTOR3 Max )
{
    p_Max = Max;
}

// Obtient le point central de la bo�te de collisions.
D3DXVECTOR3 E_BoundingBox::GetCenter()
{
    return D3DXVECTOR3( ( p_Min.x + p_Max.x ) / 2.0f,
                        ( p_Min.y + p_Max.y ) / 2.0f,
                        ( p_Min.z + p_Max.z ) / 2.0f );
}

// Cette fonction permet de d�terminer l'axe le plus long.
E_Axis E_BoundingBox::GetLongestAxis()
{
    // On calcule d'abord la longeur de chaque c�t�.
    float x = p_Max.x - p_Min.x;
    float y = p_Max.y - p_Min.y;
    float z = p_Max.z - p_Min.z;

    // Puis, on s'assure qu'il n'y ait pas de valeurs n�gatives.
    x = x < 0.0f ? -x : x;
    y = y < 0.0f ? -y : y;
    z = z < 0.0f ? -z : z;

    // On teste ensuite quel c�t� est le plus long.
    if ( x >= y && x >= z )
    {
        // Si c'est X, on retourne la r�ponse.
        return E_X_AXIS;
    }
    else if ( y >= x && y >= z )
    {
        // Si c'est Y, on retourne la r�ponse.
        return E_Y_AXIS;
    }
    else
    {
        // Si c'est Z, on retourne la r�ponse.
        return E_Z_AXIS;
    }
}

// Cette fonction permet de cr�er un plan de coupe sur un axe donn�.
D3DXPLANE E_BoundingBox::GetSplittingPlane( E_Axis Axis )
{
    // On d�termine quel axe a �t� choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -GetCenter().x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -GetCenter().y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -GetCenter().z );
        }

        // Le choix est ind�fini.
        default:
        {
            // On retourne un plan vide.
            return D3DXPLANE( 0.0f, 0.0f, 0.0f, 0.0f );
        }
    }
}

// Cette fonction cr�e un plan de coupe sur un axe depuis un point donn�.
D3DXPLANE E_BoundingBox::GetSplittingPlane( E_Axis Axis, D3DXVECTOR3 Center )
{
    // On d�termine quel axe a �t� choisi.
    switch ( Axis )
    {
        // On veut un plan de coupe sur l'axe des X.
        case E_X_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 1.0f, 0.0f, 0.0f, -Center.x );
        }

        // On veut un plan de coupe sur l'axe des Y.
        case E_Y_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 1.0f, 0.0f, -Center.y );
        }

        // On veut un plan de coupe sur l'axe des Z.
        case E_Z_AXIS:
        {
            // On cr�e, puis on retourne le plan.
            return D3DXPLANE( 0.0f, 0.0f, 1.0f, -Center.z );
        }

        // Le choix est ind�fini.
        default:
        {
            // On retourne un plan vide.
            return D3DXPLANE( 0.0f, 0.0f, 0.0f, 0.0f );
        }
    }
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_BoundingBox::SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // En d�finissant la source et la destination de l'alpha blending � ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // d�sactiv� pour cette op�ration, l'objet doit donc �tre rendu � la
    // fin de la sc�ne, �ventuellement tri� par ordre de profondeur s'il y a
    // plusieurs objets de ce type � dessiner, afin d'obtenir un affichage
    // correct.
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    theD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
    theD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
    theD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}

// Cette fonction sauvegarde l'�tat du Device Direct3D avant utilisation.
void E_BoundingBox::PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSU,   &p_AdressU );
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSV,   &p_AdressV );
    theD3DDevice->GetRenderState ( D3DRS_CULLMODE,        &p_CullMode );
    theD3DDevice->GetRenderState ( D3DRS_ZWRITEENABLE,    &p_ZWriteEnable );
    theD3DDevice->GetRenderState ( D3DRS_SRCBLEND,        &p_SrcBlend );
    theD3DDevice->GetRenderState ( D3DRS_DESTBLEND,       &p_DestBlend );
    theD3DDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &p_AlphaTestEnable );
    theD3DDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,
                                   &p_AlphaBlendEnable );
}

// Cette fonction restaure l'�tat du Device Direct3D apr�s utilisation.
void E_BoundingBox::PopDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,    p_AdressU );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,    p_AdressV );
    theD3DDevice->SetRenderState ( D3DRS_CULLMODE,         p_CullMode );
    theD3DDevice->SetRenderState ( D3DRS_ZWRITEENABLE,     p_ZWriteEnable );
    theD3DDevice->SetRenderState ( D3DRS_SRCBLEND,         p_SrcBlend );
    theD3DDevice->SetRenderState ( D3DRS_DESTBLEND,        p_DestBlend );
    theD3DDevice->SetRenderState ( D3DRS_ALPHATESTENABLE,  p_AlphaTestEnable );
    theD3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, p_AlphaBlendEnable );
}

// Cette fonction cr�e un objet virtuel repr�sentant la bo�te dans le monde 3D.
bool E_BoundingBox::CreateVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice,
                                         unsigned char     r,
                                         unsigned char     g,
                                         unsigned char     b )
{
    // On teste si un objet virtuel a d�j� �t� cr�e.
    if ( p_Box != NULL )
    {
        // Si c'est le cas, on retourne une erreur.
        return false;
    }

    // On copie les valeurs pour la couleur de l'objet.
    p_R = r;
    p_G = g;
    p_B = b;

    LPD3DXMESH Box = NULL;

    // On demande au Device Direct3D de cr�er un mesh repr�sentant une bo�te.
    if ( FAILED( D3DXCreateBox( theD3DDevice,
                                p_Max.x - p_Min.x,
                                p_Max.y - p_Min.y,
                                p_Max.z - p_Min.z,
                                &Box,
                                NULL ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonn�es et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Box->CloneMeshFVF( D3DXMESH_MANAGED,
                                    D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                    theD3DDevice,
                                    &p_Box ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On lib�re les ressources du Mesh cr�e par Direct3D.
    Box->Release();

    // Ensuite, on cr�e un acc�s au VertexBuffer de la bo�te.
    LPDIRECT3DVERTEXBUFFER9 BoxVertexBuffer = NULL;

    // Puis, on tente d'obtenir les donn�es du VertexBuffer.
    if ( FAILED( p_Box->GetVertexBuffer( &BoxVertexBuffer ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On d�finit les variables pour lire dans le buffer.
    PBYTE         VertexData = NULL;
    DWORD         NbVertices = p_Box->GetNumVertices();
    DWORD         FVFSize    = D3DXGetFVFVertexSize( p_Box->GetFVF() );
    E_BOX_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / �criture.
    if ( FAILED( BoxVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donn�e du sommet.
        CurVertex = (E_BOX_VERTEX*)VertexData;

        // On red�finit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on d�verrouille le VertexBuffer, et on lib�re les ressources
    // utilis�es par celui-ci. La copie se trouve � l'int�rieur du Mesh.
    BoxVertexBuffer->Unlock();
    BoxVertexBuffer->Release();

    // On configure le mat�riau pour afficher la bo�te.
    p_BoxMaterial.Ambient.r = 1.0f;
    p_BoxMaterial.Ambient.g = 1.0f;
    p_BoxMaterial.Ambient.b = 1.0f;
    p_BoxMaterial.Ambient.a = 1.0f;

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction d�truit l'objet virtuel.
void E_BoundingBox::ReleaseVirtualObject()
{
    // On lib�re les ressources du Mesh.
    if ( p_Box != NULL )
    {
        p_Box->Release();
        p_Box = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void E_BoundingBox::RenderVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice,
                                         D3DXMATRIXA16     ObjectMatrix )
{
    // On teste si le Device Direct3D existe, et si l'objet a �t� correctement
    // initialis�.
    if ( theD3DDevice == NULL || p_Box == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'op�ration.
        return;
    }

    // On demande de sauvegarder l'�tat du Device Direct3D.
    PushDeviceProperties( theD3DDevice );

    // Puis, on configure l'alpha blending pour le rendu.
    SetAlphaBlending( theD3DDevice );

    // On supprime le Culling pour le rendu.
    theD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    D3DXVECTOR3 Center = GetCenter();

    // On ajoute ensuite les valeurs du centre � la matrice de transformation.
    ObjectMatrix._41 += Center.x;
    ObjectMatrix._42 += Center.y;
    ObjectMatrix._43 += Center.z;

    // Puis, on configure la matrice de monde pour cet objet.
    theD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On d�salloue le pointeur de texture, et on d�finit le mat�riau.
    theD3DDevice->SetTexture ( 0, NULL );
    theD3DDevice->SetMaterial( &p_BoxMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Box->DrawSubset( 0 );

    // Enfin, on restaure l'�tat d'origine du Device Direct3D.
    PopDeviceProperties( theD3DDevice );
}

// Cette fonction restaure l'objet virtuel apr�s un changement de Device.
bool E_BoundingBox::RestoreVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();

    // Puis, on tente de cr�er un nouvel objet, et on retourne la r�ponse.
    return CreateVirtualObject( theD3DDevice, p_R, p_G, p_B );
}
