/*****************************************************************************
 * ==> Classe E_Mesh --------------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe permet de charger et d'afficher un objet       *
 *               depuis un fichier .x.                                       *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#include "e_mesh.h"

// Constructeur de la classe E_Mesh.
E_Mesh::E_Mesh()
{
    p_Mesh             = NULL;
    p_MeshMaterials    = NULL;
    p_MeshTextures     = NULL;
    p_PolygonList      = NULL;
    p_Filename         = NULL;

    p_AdressU          = 0;
    p_AdressV          = 0;
    p_ZWriteEnable     = 0;
    p_AlphaBlendEnable = 0;
    p_AlphaTestEnable  = 0;
    p_SrcBlend         = 0;
    p_DestBlend        = 0;
    p_NumMaterials     = 0;

    p_rX               = 0.0f;
    p_rY               = 0.0f;
    p_rZ               = 0.0f;
    p_tX               = 0.0f;
    p_tY               = 0.0f;
    p_tZ               = 0.0f;

    p_IsAlphaBlended   = false;
}

// Destructeur de la classe E_Mesh.
E_Mesh::~E_Mesh()
{
    // On libère les ressources utilisées par la liste de polygones.
    if ( p_PolygonList != NULL )
    {
        delete p_PolygonList;
        p_PolygonList = NULL;
    }

    // On libère les ressources utilisées par les matériaux de l'objet.
    if( p_MeshMaterials != NULL )
    {
        delete[] p_MeshMaterials;
        p_MeshMaterials = NULL;
    }

    // On libère les ressources utilisées par les textures de l'objet.
    if( p_MeshTextures != NULL )
    {
        // On commence par libérer les ressources de chaque texture.
        for( DWORD i = 0; i < p_NumMaterials; i++ )
        {
            if( p_MeshTextures[i] != NULL )
            {
                p_MeshTextures[i]->Release();
                p_MeshTextures[i] = NULL;
            }
        }

        // Puis, on libère le conteneur de textures.
        delete[] p_MeshTextures;
    }

    // On libère les ressources utilisées par l'objet.
    if( p_Mesh != NULL )
    {
        p_Mesh->Release();
        p_Mesh = NULL;
    }
}

// Obtient les données des polygones de l'objet.
E_PolygonList* E_Mesh::GetPolygonList()
{
    return p_PolygonList;
}

// Obtient la boîte de collisions de l'objet.
E_BoundingBox E_Mesh::GetBoundingBox()
{
    if ( p_PolygonList == NULL )
    {
        return E_BoundingBox();
    }

    return p_PolygonList->GetBoundingBox();
}

// Obtient la sphere de collisions de l'objet.
E_BoundingSphere E_Mesh::GetBoundingSphere()
{
    if ( p_PolygonList == NULL )
    {
        return E_BoundingSphere();
    }

    return p_PolygonList->GetBoundingSphere();
}

// Obtient le nom du fichier de l'objet.
LPSTR E_Mesh::GetFilename()
{
    return p_Filename;
}

// Définit le nom du fichier de l'objet.
void E_Mesh::SetFilename( LPSTR Filename )
{
    p_Filename = Filename;
}

// Obtient l'angle de rotation de l'objet sur l'axe X.
float E_Mesh::GetRotationX()
{
    return p_rX;
}

// Définit l'angle de rotation de l'objet sur l'axe X.
void E_Mesh::SetRotationX( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rX = Value;
    }
    else
    {
        p_rX = 0;
    }
}

// Obtient l'angle de rotation de l'objet sur l'axe Y.
float E_Mesh::GetRotationY()
{
    return p_rY;
}

// Définit l'angle de rotation de l'objet sur l'axe Y.
void E_Mesh::SetRotationY( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rY = Value;
    }
    else
    {
        p_rY = 0;
    }
}

// Obtient l'angle de rotation de l'objet sur l'axe Z.
float E_Mesh::GetRotationZ()
{
    return p_rZ;
}

// Définit l'angle de rotation de l'objet sur l'axe Z.
void E_Mesh::SetRotationZ( float Value )
{
    if ( Value > -(float)D3DX_PI * 2 && Value < (float)D3DX_PI * 2 )
    {
        p_rZ = Value;
    }
    else
    {
        p_rZ = 0;
    }
}

// Obtient la valeur de la position de l'objet sur l'axe X.
float E_Mesh::GetTranslationX()
{
    return p_tX;
}

// Définit la valeur de la position de l'objet sur l'axe X.
void E_Mesh::SetTranslationX( float Value )
{
    p_tX = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Y.
float E_Mesh::GetTranslationY()
{
    return p_tY;
}

// Définit la valeur de la position de l'objet sur l'axe Y.
void E_Mesh::SetTranslationY( float Value )
{
    p_tY = Value;
}

// Obtient la valeur de la position de l'objet sur l'axe Z.
float E_Mesh::GetTranslationZ()
{
    return p_tZ;
}

// Définit la valeur de la position de l'objet sur l'axe Z.
void E_Mesh::SetTranslationZ( float Value )
{
    p_tZ = Value;
}

// Définit l'état du moteur d'Aplha Blending.
void E_Mesh::SetAlphaBlending( bool Value )
{
    p_IsAlphaBlended = Value;
}

// Retourne l'état du moteur d'aplha Blending.
bool E_Mesh::IsAlphaBlended()
{
    return p_IsAlphaBlended;
}

// Cette fonction configure l'affichage pour permettre la transparence.
void E_Mesh::SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // En définissant la source et la destination de l'alpha blending à ONE,
    // l'objet devient transparent. Attention toutefois, le Z-Buffer est
    // désactivé pour cette opération, l'objet doit donc être rendu à la
    // fin de la scène, éventuellement trié par ordre de profondeur s'il y a
    // plusieurs objets de ce type à dessiner, afin d'obtenir un affichage
    // correct.
    theD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
    theD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE,     FALSE );
    theD3DDevice->SetRenderState( D3DRS_SRCBLEND,         D3DBLEND_ONE );
    theD3DDevice->SetRenderState( D3DRS_DESTBLEND,        D3DBLEND_ONE );
}

// Cette fonction sauvegarde l'état du Device Direct3D avant utilisation.
void E_Mesh::PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSU,   &p_AdressU );
    theD3DDevice->GetSamplerState( 0, D3DSAMP_ADDRESSV,   &p_AdressV );
    theD3DDevice->GetRenderState ( D3DRS_ZWRITEENABLE,    &p_ZWriteEnable );
    theD3DDevice->GetRenderState ( D3DRS_SRCBLEND,        &p_SrcBlend );
    theD3DDevice->GetRenderState ( D3DRS_DESTBLEND,       &p_DestBlend );
    theD3DDevice->GetRenderState ( D3DRS_ALPHATESTENABLE, &p_AlphaTestEnable );
    theD3DDevice->GetRenderState ( D3DRS_ALPHABLENDENABLE,
                                   &p_AlphaBlendEnable );
}

// Cette fonction restaure l'état du Device Direct3D après utilisation.
void E_Mesh::PopDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice )
{
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU,    p_AdressU );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV,    p_AdressV );
    theD3DDevice->SetRenderState ( D3DRS_ZWRITEENABLE,     p_ZWriteEnable );
    theD3DDevice->SetRenderState ( D3DRS_SRCBLEND,         p_SrcBlend );
    theD3DDevice->SetRenderState ( D3DRS_DESTBLEND,        p_DestBlend );
    theD3DDevice->SetRenderState ( D3DRS_ALPHATESTENABLE,  p_AlphaTestEnable );
    theD3DDevice->SetRenderState ( D3DRS_ALPHABLENDENABLE, p_AlphaBlendEnable );
}

// Cette fonction permet l'initialisation de l'objet.
bool E_Mesh::Initialize( LPDIRECT3DDEVICE9 theD3DDevice )
{
    TCHAR        Directory[MAX_PATH];
    LPD3DXBUFFER D3DXMtrlBuffer = NULL;

    // On contrôle qu'un nom de fichier ait bien été défini.
    if ( p_Filename == NULL )
    {
        // Si non, on ne peut pas initialiser.
        return false;
    }
    else
    {
        // Si oui, on recherche si un nom de répertoire est défini dans le nom
        // du fichier, et on copie celui-ci s'il existe.
        TCHAR Buffer[MAX_PATH];

        // On commence par rechercher la longeur du nom du fichier.
        size_t NameLength = lstrlen( p_Filename );

        // On crée ensuite une boucle qui parcourt tout le nom.
        for ( size_t i = 0; i < NameLength; i++ )
        {
            // On copie le nom du fichier, caractère par caractère, dans un
            // buffer local.
            Buffer[i] = p_Filename[i];

            // Puis, on teste si le caractère courant est un symbole de
            // séparation de répertoire.
            if ( p_Filename[i] == '\\' )
            {
                // Si c'est le cas, on copie le buffer local dans la variable
                // Directory, et on ajoute le symbole de fin de chaîne.
                lstrcpyn( Directory, Buffer, MAX_PATH );
                lstrcpyn( Directory + i + 1, "\0", MAX_PATH - i - 1 );
            }
        }
    }

    // On tente de charger le fichier .x en mémoire.
    if( FAILED( D3DXLoadMeshFromX( p_Filename,
                                   D3DXMESH_SYSTEMMEM,
                                   theD3DDevice,
                                   NULL,
                                   &D3DXMtrlBuffer,
                                   NULL,
                                   &p_NumMaterials,
                                   &p_Mesh ) ) )
    {
        return false;
    }

    // On teste si une donnée de matériau existe pour cet objet.
    if ( D3DXMtrlBuffer != NULL && p_NumMaterials > 0 )
    {
        // Si oui, on extrait les données du matériau contenu dans le fichier.
        D3DXMATERIAL* D3DXMaterials =
            (D3DXMATERIAL*)D3DXMtrlBuffer->GetBufferPointer();

        p_MeshMaterials = new D3DMATERIAL9[p_NumMaterials];

        // On teste si le tableau contenant les matériaux est correctement
        // initialisé en mémoire.
        if ( p_MeshMaterials == NULL )
        {
            return false;
        }

        p_MeshTextures = new LPDIRECT3DTEXTURE9[p_NumMaterials];

        // On teste si le tableau contenant les textures est correctement
        // initialisé en mémoire.
        if ( p_MeshTextures == NULL )
        {
            return false;
        }

        // On passe en revue tous les matériaux existants.
        for( DWORD i = 0; i < p_NumMaterials; i++ )
        {
            // On copie le matériau présent en mémoire.
            p_MeshMaterials[i] = D3DXMaterials[i].MatD3D;

            // Puis, on copie la lumière ambiante depuis la lumière diffuse,
            // car Direct3D ne le fait pas automatiquement.
            p_MeshMaterials[i].Ambient = p_MeshMaterials[i].Diffuse;

            p_MeshTextures[i] = NULL;

            // On tente ensuite de créer la texture pour le matériau.
            if( D3DXMaterials[i].pTextureFilename           != NULL &&
                lstrlen( D3DXMaterials[i].pTextureFilename ) > 0 )
            {
                // On doit ajouter le chemin du répertoire où se trouve l'objet
                // au nom du fichier de la texture. On commence par créer une
                // variable, nommée TextureName, qui contiendra le nom complet.
                TCHAR TextureName[MAX_PATH];

                // Puis, on obtient la longeur du nom du répertoire.
                const int DirNameLength = lstrlen( Directory );

                // On copie ensuite le chemin du répertoire dans la variable
                // TextureName, ...
                lstrcpyn( TextureName, Directory, MAX_PATH );

                // ... puis, on copie le nom du fichier de la texture dans la
                // variable TextureName, après le répertoire.
                lstrcpyn( TextureName + DirNameLength,
                          D3DXMaterials[i].pTextureFilename,
                          MAX_PATH - DirNameLength );

                // On tente ensuite de charger la texture en mémoire.
                if( FAILED( D3DXCreateTextureFromFile
                                ( theD3DDevice,
                                  TextureName,
                                  &p_MeshTextures[i] ) ) )
                {
                    // Si le chargement échoue, on initialise le pointeur de
                    // texture du matériau courant à NULL, ainsi on peut quand
                    // même finir de charger l'objet.
                    p_MeshTextures[i] = NULL;
                }
            }
        }

        // Pour finir, on libère les ressources utilisées par le buffer
        // de matériaux.
        D3DXMtrlBuffer->Release();
    }
    else
    {
        // Si aucun matériau n'est défini dans le fichier, on en crée un
        // par défaut.
        p_NumMaterials = 1;

        p_MeshMaterials = new D3DMATERIAL9[p_NumMaterials];

        // On teste si le tableau contenant les matériaux est correctement
        // initialisé en mémoire.
        if ( p_MeshMaterials == NULL )
        {
            return false;
        }

        p_MeshTextures = new LPDIRECT3DTEXTURE9[p_NumMaterials];

        // On teste si le tableau contenant les textures est correctement
        // initialisé en mémoire.
        if ( p_MeshTextures == NULL )
        {
            return false;
        }

        memset( p_MeshMaterials, 0, sizeof( D3DMATERIAL9 ) );

        // On configure le paramètre ambiant à 1.0f. On laisse les autres à 0.
        p_MeshMaterials[0].Ambient.r = 1.0f;
        p_MeshMaterials[0].Ambient.g = 1.0f;
        p_MeshMaterials[0].Ambient.b = 1.0f;

        // Aucune texture disponible pour ce matériau.
        p_MeshTextures[0] = NULL;
    }

    // On crée ensuite la liste des polygones de l'objet.
    p_PolygonList = new E_PolygonList();

    // Puis, on teste si la liste a été correctement initialisée.
    if ( p_PolygonList == NULL )
    {
        return false;
    }

    LPD3DXMESH Clone = NULL;

    // On crée une copie de l'objet ne contenant que les valeurs XYZ.
    if ( FAILED( p_Mesh->CloneMeshFVF( D3DXMESH_SYSTEMMEM,
                                       D3DFVF_XYZ,
                                       theD3DDevice,
                                       &Clone ) ) )
    {
        return false;
    }

    // Puis, on commande la création de la liste à partir de la copie.
    bool Result = p_PolygonList->CreateListFromMesh( Clone );

    // On libère ensuite la ressource utilisée par la copie.
    if ( Clone != NULL )
    {
        Clone->Release();
        Clone = NULL;
    }

    // On teste si une erreur est survenue pendant la création de la liste.
    if ( Result == false )
    {
        // Si c'est le cas, on retourne une erreur.
        return false;
    }

    // Puis, on commande la construction des objets de collisions.
    p_PolygonList->CreateBoundingBox   ();
    p_PolygonList->CreateBoundingSphere();

    // Enfin, on retourne le message de réussite.
    return true;
}

// Cette fonction construit la matrice d'affichage locale pour l'objet.
D3DXMATRIXA16 E_Mesh::GetWorldMatrix()
{
    D3DXMATRIXA16 RX;
    D3DXMATRIXA16 RY;
    D3DXMATRIXA16 RZ;
    D3DXMATRIXA16 ObjectMatrix;

    D3DXMatrixIdentity( &RX );
    D3DXMatrixIdentity( &RY );
    D3DXMatrixIdentity( &RZ );
    D3DXMatrixIdentity( &ObjectMatrix );

    D3DXMatrixRotationX( &RX, p_rX );
    D3DXMatrixRotationY( &RY, p_rY );
    D3DXMatrixRotationZ( &RZ, p_rZ );

    D3DXMatrixMultiply( &ObjectMatrix, &RX, &ObjectMatrix );
    D3DXMatrixMultiply( &ObjectMatrix, &RY, &ObjectMatrix );
    D3DXMatrixMultiply( &ObjectMatrix, &RZ, &ObjectMatrix );

    ObjectMatrix._41 = p_tX;
    ObjectMatrix._42 = p_tY;
    ObjectMatrix._43 = p_tZ;

    return ObjectMatrix;
}

// Cette fonction permet de rendu de l'objet.
void E_Mesh::Render( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On teste tout d'abord si le Device Direct3D est correctement initialisé.
    if ( theD3DDevice == NULL )
    {
        // Si ce n'est pas le cas, on abandonne l'opération.
        return;
    }

    // On demande de sauvegarder l'état du Device Direct3D.
    PushDeviceProperties( theD3DDevice );

    // On teste ensuite si le moteur d'alpha blending est actif.
    if ( p_IsAlphaBlended == true )
    {
        // Si oui, on configure l'alpha blending pour le rendu.
        SetAlphaBlending( theD3DDevice );
    }

    // On configure ensuite la matrice de monde pour cet objet.
    D3DXMATRIXA16 WorldMatrix = GetWorldMatrix();
    theD3DDevice->SetTransform( D3DTS_WORLD, &WorldMatrix );

    // Puis, on s'assure que le sampler pour les textures est configuré
    // correctement pour le rendu de l'objet.
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
    theD3DDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );

    // L'objet est divisé en sous-objets, un par matériau.
    for( DWORD i = 0; i < p_NumMaterials; i++ )
    {
        // On configure le matériau et la texture pour le sous-objet.
        theD3DDevice->SetMaterial( &p_MeshMaterials[i] );
        theD3DDevice->SetTexture ( 0, p_MeshTextures[i] );

        // Puis, on commande le rendu du sous-objet.
        p_Mesh->DrawSubset( i );
    }

    // Enfin, on restaure l'état d'origine du Device Direct3D.
    PopDeviceProperties( theD3DDevice );
}

// Cette fonction permet de restaurer l'objet après un changement de Device.
bool E_Mesh::Restore( LPDIRECT3DDEVICE9 theD3DDevice )
{
    // On détruit tous les matériaux de l'objet.
    if( p_MeshMaterials != NULL )
    {
        delete[] p_MeshMaterials;
        p_MeshMaterials = NULL;
    }

    // Puis, on détruit toutes les textures de l'objet.
    if( p_MeshTextures != NULL )
    {
        for( DWORD i = 0; i < p_NumMaterials; i++ )
        {
            if( p_MeshTextures[i] != NULL )
            {
                p_MeshTextures[i]->Release();
                p_MeshTextures[i] = NULL;
            }
        }

        delete[] p_MeshTextures;
    }

    // On détruit l'objet.
    if( p_Mesh != NULL )
    {
        p_Mesh->Release();
        p_Mesh = NULL;
    }

    // Puis, on tente de réinitialiser l'objet.
    return Initialize( theD3DDevice );
}
