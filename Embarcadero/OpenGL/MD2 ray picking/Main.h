/******************************************************************************
 * ==> Main ------------------------------------------------------------------*
 ******************************************************************************
 * Description : MD2 demo main form                                           *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#ifndef MainH
#define MainH

// vcl
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>

// qr engine
#include "QR_Color.h"
#include "QR_Matrix16.h"
#include "QR_MD2Group.h"
#include "QR_Renderer_OpenGL.h"
#include "QR_Resources.h"

// opengl
#define GLEW_STATIC
#include <GL/glew.h>
#include <gl\gl.h>
#include <gl\glu.h>

/**
* MD2 demo main form
*@author Jean-Milost Reymond
*/
class TMainForm : public TForm
{
    __published:
        TPopupMenu *pmOptions;
        TMenuItem *miLighting;
        TMenuItem *miSeparator1;
        TMenuItem *miPrevAnim;
        TMenuItem *miNextAnim;

        void __fastcall FormCreate(TObject* pSender);
        void __fastcall FormResize(TObject* pSender);
        void __fastcall FormPaint(TObject* pSender);
        void __fastcall miLightingClick(TObject* pSender);
        void __fastcall miPrevAnimClick(TObject* pSender);
        void __fastcall miNextAnimClick(TObject* pSender);

    public:
        /**
        * Constructor
        *@param pOwner - form owner
        */
        __fastcall TMainForm(TComponent* pOwner);

        /**
        * Destructor
        */
        virtual __fastcall ~TMainForm();

    protected:
        /**
        * Configure OpenGL with correct format descriptor
        */
        void __fastcall SetPixelFormatDescriptor();

        /**
        * Called when thread do nothing else
        *@param pSEnder - event sender
        *@param[in, out] done - if true, idle loop was ternimated and may do nothing else
        */
        void __fastcall IdleLoop(TObject* pSender, bool& done);

        /**
        * Renders (i.e. prepares and draws) scene
        */
        void __fastcall RenderGLScene();

        /**
        * Draws scene
        *@param elapsedTime - elapsed time since last draw
        */
        void __fastcall Draw(const double& elapsedTime);

    private:
        QR_Renderer_OpenGL m_Renderer;
        QR_Resources       m_Resources;
        QR_Matrix16P       m_Projection;
        QR_Matrix16P       m_View;
        QR_MemoryDir*      m_pMemoryDir;
        QR_MD2Group*       m_pMD2;
        QR_Color           m_Color;
        std::time_t        m_PreviousTime;

        /**
        * Draws a polygon list
        *@param polygons - polygon list to draw
        *@param matrix - matrix to apply to polygons
        */
        void DrawPolygons(const QR_PolygonsP& polygons, const QR_Matrix16P& matrix) const;

        /**
        * Called when collision should be detected
        *@param pSender - model group that raised the event
        *@param pMesh - mesh to check
        *@param modelMatrix - model matrix
        *@param pAABBTree - aligned-axis bounding box tree containing polygons to check
        */
        void OnDetectCollision(const QR_ModelGroup* pSender,
                               const QR_Mesh*       pMesh,
                               const QR_Matrix16P&  modelMatrix,
                               const QR_AABBTree*   pAABBTree);

        /**
        * Configures OpenGL
        */
        void ConfigOpenGL();
};
extern PACKAGE TMainForm* MainForm;
#endif
