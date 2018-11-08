/*****************************************************************************
 * ==> Classe E_MD2Mesh -----------------------------------------------------*
 * ***************************************************************************
 * Description : Cette classe représente un objet animé, en utilisant la     *
 *               méthode MD2 de Quake.                                       *
 * Version     : 1.0                                                         *
 * Développeur : Jean-Milost Reymond                                         *
 *****************************************************************************/

#ifndef E_MD2MESH_H
#define E_MD2MESH_H

#include <stdio.h>
#include <stdlib.h>

#include <d3dx9.h>
#include <windows.h>

// On définit quelques constantes pour les fichiers MD2.
#define MD2_IDENT    ( ( '2' << 24 ) + ( 'P' << 16 ) + ( 'D' << 8 ) + 'I' )
#define MD2_VERSION  8
#define MD2_NVERSION 1.0f

// ********************* Diverses données pour le rendu. **********************
// On déclare ici diverses structures et énumérateurs pour le rendu de l'objet.

/*
 * Cet énumérateur contient les paramètres possibles du moteur de lumières.
 */
enum E_MD2LightningMode
{
    E_MD2_LIGHTNING_UNKNOW        = 0,
    E_MD2_LIGHTNING_NONE          = 1,
    E_MD2_LIGHTNING_PRECALCULATED = 2,
    E_MD2_LIGHTNING_DIRECT3D      = 3
};

/*
 * Cette structure permet de contenir un vertex pour le rendu Direct3D.
 */
struct E_MD2_D3DVERTEX
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DCOLOR    Color;
    FLOAT       Tu;
    FLOAT       Tv;

    static const DWORD FVF;
};

// **************** Structures pour la lecture du fichier MD2. ****************
// Les structures qui suivent représentent les données contenues dans le
// fichier MD2. Attention aux modifications : Le format est important,
// parce-qu'on s'en sert pour lire le fichier.

/*
 * Cette structure permet de contenir l'en-tête des fichiers MD2.
 */
struct E_MD2Header
{
    int ID;
    int Version;

    int SkinWidth;
    int SkinHeight;
    int FrameSize;

    int Nb_Skins;
    int Nb_XYZ;
    int Nb_UV;
    int Nb_Triangles;
    int Nb_GlCmds;
    int Nb_Frames;

    int Offset_Skins;
    int Offset_UV;
    int Offset_Triangles;
    int Offset_Frames;
    int Offset_GlCmds;
    int Offset_End;
};

/*
 * Cette structure permet de contenir un skin.
 */
struct E_MD2Skin
{
    char Name[64];
};

/*
 * Cette structure permet de contenir un sommet.
 */
struct E_MD2Vertex
{
    unsigned char v[3];
    unsigned char LightNormalIndex;
};

/*
 * Cette structure permet de contenir une coordonnée de texture.
 */
struct E_MD2TextureCoord
{
    short u;
    short v;
};

/*
 * Cette structure permet de contenir un frame.
 */
struct E_MD2Frame
{
    float        Scale[3];
    float        Translate[3];
    char         Name[16];
    E_MD2Vertex* Vertex;
};

/*
 * Cette structure permet de contenir un triangle.
 */
struct E_MD2Triangle
{
    short Index_XYZ[3];
    short Index_UV[3];
};

// *************************** Structure E_MD2Color ***************************
// Cette structure permet de contenir les données de la couleur de la lumière
// pré-calculée de l'objet. Ceci est valable pour la lumière d'ambiance et pour
// la lumière directionnelle.

/*
 * Cette structure permet de contenir une couleur.
 */
struct E_MD2Color
{
    int r;
    int g;
    int b;

    E_MD2Color();
    E_MD2Color( int Red, int Green, int Blue );
};

// *************************** Structure E_MD2Light ***************************
// Cette structure permet de contenir les données pour créer la lumière pré-
// calculée de l'objet.

/*
 * Cette structure permet de contenir une lumière pré-calculée.
 */
struct E_MD2Light
{
    E_MD2Color Light;
    E_MD2Color Ambient;

    D3DXVECTOR3 Direction;

    E_MD2Light();
    E_MD2Light( E_MD2Color Light, E_MD2Color Ambient, D3DXVECTOR3 Direction );
};

// ************************* Structure E_MD2Animation *************************
// Cette structure permet de contenir les données nécessaire pour configurer
// l'animation courante de l'objet.

/*
 * Cette structure permet de contenir une animation.
 */
struct E_MD2Animation
{
    int   Start;
    int   End;
    LPSTR Name;

    E_MD2Animation();
    E_MD2Animation( int Start, int End, LPSTR Name );
};

// ***************************** Classe E_MD2Mesh *****************************
// Cette classe représente un objet animé. Les algorythmes d'animation et de
// rendu sont ceux utilisés par les fichiers MD2 de Quake.

/*
 * Cette classe permet la gestion d'un mesh animé, avec la méthode MD2.
 */
class E_MD2Mesh
{
  private:

    E_MD2Header             p_Header;

    E_MD2Skin*              p_Skin;
    E_MD2TextureCoord*      p_TexCoord;
    E_MD2Triangle*          p_Triangle;
    E_MD2Frame*             p_Frame;
    E_MD2Light              p_PreCalculatedLight;
    E_MD2Animation          p_Animation;
    E_MD2LightningMode      p_LightningMode;
    int*                    p_GlCmds;

    LPDIRECT3DVERTEXBUFFER9 p_VertexBuffer;
    D3DMATERIAL9            p_Material;
    LPDIRECT3DTEXTURE9      p_Texture;
    D3DXVECTOR3             p_LightDirection;
    D3DXVECTOR3*            p_NormalsTable;

    DWORD                   p_NumMaterials;
    DWORD                   p_AdressU;
    DWORD                   p_AdressV;
    DWORD                   p_CullMode;
    DWORD                   p_ZWriteEnable;
    DWORD                   p_AlphaBlendEnable;
    DWORD                   p_AlphaTestEnable;
    DWORD                   p_SrcBlend;
    DWORD                   p_DestBlend;

    LPSTR                   p_ModelFilename;
    LPSTR                   p_TextureFilename;
    LPSTR                   p_NormalsTableFilename;

    float                   p_Interpolation;
    float                   p_rX;
    float                   p_rY;
    float                   p_rZ;
    float                   p_tX;
    float                   p_tY;
    float                   p_tZ;

    double                  p_CurTime;
    double                  p_LastTime;

    int                     p_CurFrameID;
    int                     p_NextFrameID;
    int                     p_NormalsTableLength;

    bool                    p_IsAlphaBlended;

  public:

    E_MD2Mesh();
    ~E_MD2Mesh();

  public:

    void  SetModelFilename       ( LPSTR Filename );
    LPSTR GetModelFilename       ();
    void  SetTextureFilename     ( LPSTR Filename );
    LPSTR GetTextureFilename     ();
    void  SetNormalsTableFilename( LPSTR Filename );
    LPSTR GetNormalsTableFilename();
    float GetRotationX           ();
    void  SetRotationX           ( float Value );
    float GetRotationY           ();
    void  SetRotationY           ( float Value );
    float GetRotationZ           ();
    void  SetRotationZ           ( float Value );
    float GetTranslationX        ();
    void  SetTranslationX        ( float Value );
    float GetTranslationY        ();
    void  SetTranslationY        ( float Value );
    float GetTranslationZ        ();
    void  SetTranslationZ        ( float Value );
    void  SetAlphaBlending       ( bool Value );
    bool  IsAlphaBlended         ();

    D3DXMATRIXA16 GetWorldMatrix();

    void SetLightningMode( E_MD2LightningMode Mode );
    void SetPreCalculatedLightParameters( E_MD2Light Light );

    bool SetAnimationParameters( E_MD2Animation Animation );
    bool Initialize( LPDIRECT3DDEVICE9 theD3DDevice );
    bool Restore( LPDIRECT3DDEVICE9 theD3DDevice );

    void Render( LPDIRECT3DDEVICE9 theD3DDevice );

  private:

    bool CreateNormalsTable();
    bool Animate();
    bool PopulateVertexBuffer();

    D3DXVECTOR3 GetVertice( E_MD2Frame* theFrame, E_MD2Vertex* theVertex );
    D3DXVECTOR3 GetInterpolation( D3DXVECTOR3 FirstVector,
                                  D3DXVECTOR3 SecondVector );

    DWORD GetPreCalculatedLightning( D3DXVECTOR3 PolygonNormal );
    DWORD GetVertexColor( D3DXVECTOR3 PolygonNormal );

    void SetAlphaBlending( LPDIRECT3DDEVICE9 theD3DDevice );
    void PushDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice );
    void PopDeviceProperties( LPDIRECT3DDEVICE9 theD3DDevice );
};

#endif // E_MD2MESH_H
