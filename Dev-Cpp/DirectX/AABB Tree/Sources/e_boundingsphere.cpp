/*****************************************************************************
 * ==> Classe E_BoundingSphere ----------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe repr�sente une sph�re de collisions pour un    *
 *               objet.                                                      *
 * Version     : 1.0                                                         *
 * D�veloppeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_boundingsphere.h"

// Constructeur de la classe E_BoundingSphere.
E_BoundingSphere::E_BoundingSphere()
{
    p_Center           = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    p_Radius           = 0.0f;

    p_Sphere           = NULL;

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

// Deuxi�me constructeur de la classe E_BoundingSphere.
E_BoundingSphere::E_BoundingSphere( D3DXVECTOR3 Center, float Radius )
{
    p_Center           = Center;
    p_Radius           = Radius;

    p_Sphere           = NULL;

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

// Destructeur de la classe E_BoundingSphere.
E_BoundingSphere::~E_BoundingSphere()
{
    // On commande la destruction de l'objet virtuel, s'il a �t� cr�e.
    ReleaseVirtualObject();
}

// D�finit la coordonn�e du centre de la sph�re.
void E_BoundingSphere::SetCenter( D3DXVECTOR3 Center )
{
    p_Center = Center;
}

// Obtient la coordonn�e du centre de la sph�re.
D3DXVECTOR3 E_BoundingSphere::GetCenter()
{
    return p_Center;
}

// D�finit le rayon de la sph�re.
void E_BoundingSphere::SetRadius( float Radius )
{
    p_Radius = Radius;
}

// Obtient le rayon de la sph�re.
float E_BoundingSphere::GetRadius()
{
    return p_Radius;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_BoundingSphere::SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
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
void E_BoundingSphere::PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
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
void E_BoundingSphere::PopDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
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

// Cette fonction cr�e un objet virtuel repr�sentant la sph�re dans le monde 3D.
bool E_BoundingSphere::CreateVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice,
                                            unsigned char     r,
                                            unsigned char     g,
                                            unsigned char     b )
{
    // On teste si un objet virtuel a d�j� �t� cr�e.
    if ( p_Sphere != NULL )
    {
        // Si c'est le cas, on retourne une erreur.
        return false;
    }

    // On copie les valeurs pour la couleur de l'objet.
    p_R = r;
    p_G = g;
    p_B = b;

    LPD3DXMESH Sphere = NULL;

    // On demande au Device Direct3D de cr�er un mesh repr�sentant une sph�re.
    if ( FAILED( D3DXCreateSphere( theD3DDevice,
                                   p_Radius,
                                   20,
                                   20,
                                   &Sphere,
                                   NULL ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // Puis, on clone le mesh, pour ne garder que les coordonn�es et la couleur
    // diffuse des sommets dans le VertexBuffer.
    if ( FAILED( Sphere->CloneMeshFVF( D3DXMESH_MANAGED,
                                       D3DFVF_XYZ | D3DFVF_DIFFUSE,
                                       theD3DDevice,
                                       &p_Sphere ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On lib�re les ressources du Mesh cr�e par Direct3D.
    Sphere->Release();

    // Ensuite, on cr�e un acc�s au VertexBuffer de la sph�re.
    LPDIRECT3DVERTEXBUFFER9 SphereVertexBuffer = NULL;

    // Puis, on tente d'obtenir les donn�es du VertexBuffer.
    if ( FAILED( p_Sphere->GetVertexBuffer( &SphereVertexBuffer ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On d�finit les variables pour lire dans le buffer.
    PBYTE            VertexData = NULL;
    DWORD            NbVertices = p_Sphere->GetNumVertices();
    DWORD            FVFSize    = D3DXGetFVFVertexSize( p_Sphere->GetFVF() );
    E_SPHERE_VERTEX* CurVertex;

    // Puis, on tente de verrouiller le VertexBuffer en lecture / �criture.
    if ( FAILED( SphereVertexBuffer->Lock( 0, 0, (void**)&VertexData, 0 ) ) )
    {
        // Si l'op�ration �choue, on retourne une erreur.
        return false;
    }

    // On obtient chaque Vertex contenu dans le VertexBuffer.
    for ( DWORD i = 0; i < NbVertices; i++ )
    {
        // On obtient la donn�e du sommet.
        CurVertex = (E_SPHERE_VERTEX*)VertexData;

        // On red�finit la couleur du sommet.
        CurVertex->Color = D3DCOLOR_XRGB( p_R, p_G, p_B );

        // Puis, on enregistre la modification dans le VertexBuffer, et on
        // passe au sommet suivant.
        VertexData  = (PBYTE)CurVertex;
        VertexData += FVFSize;
    }

    // Ensuite, on d�verrouille le VertexBuffer, et on lib�re les ressources
    // utilis�es par celui-ci. La copie se trouve � l'int�rieur du Mesh.
    SphereVertexBuffer->Unlock();
    SphereVertexBuffer->Release();

    // On configure le mat�riau pour afficher la sph�re.
    p_SphereMaterial.Ambient.r = 1.0f;
    p_SphereMaterial.Ambient.g = 1.0f;
    p_SphereMaterial.Ambient.b = 1.0f;
    p_SphereMaterial.Ambient.a = 1.0f;

    // Si tout s'est bien pass�, on retourne le message de r�ussite.
    return true;
}

// Cette fonction d�truit l'objet virtuel.
void E_BoundingSphere::ReleaseVirtualObject()
{
    // On lib�re les ressources du Mesh.
    if ( p_Sphere != NULL )
    {
        p_Sphere->Release();
        p_Sphere = NULL;
    }
}

// Cette fonction permet le rendu de l'objet virtuel.
void E_BoundingSphere::RenderVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice,
                                            D3DXMATRIXA16     ObjectMatrix )
{
    // On teste si le Device Direct3D existe, et si l'objet a �t� correctement
    // initialis�.
    if ( theD3DDevice == NULL || p_Sphere == NULL )
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

    // On ajoute ensuite les valeurs du centre � la matrice de transformation.
    ObjectMatrix._41 += p_Center.x;
    ObjectMatrix._42 += p_Center.y;
    ObjectMatrix._43 += p_Center.z;

    // Puis, on configure la matrice de monde pour cet objet.
    theD3DDevice->SetTransform( D3DTS_WORLD, &ObjectMatrix );

    // On d�salloue le pointeur de texture, et on d�finit le mat�riau.
    theD3DDevice->SetTexture ( 0, NULL );
    theD3DDevice->SetMaterial( &p_SphereMaterial );

    // Puis, on commande le rendu de l'objet.
    p_Sphere->DrawSubset( 0 );

    // Enfin, on restaure l'�tat d'origine du Device Direct3D.
    PopDeviceProperties( theD3DDevice );
}

// Cette fonction restaure l'objet virtuel apr�s un changement de Device.
bool E_BoundingSphere::RestoreVirtualObject( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On commande la destruction de l'objet courant.
    ReleaseVirtualObject();

    // Puis, on tente de cr�er un nouvel objet, et on retourne la r�ponse.
    return CreateVirtualObject( theD3DDevice, p_R, p_G, p_B );
}
