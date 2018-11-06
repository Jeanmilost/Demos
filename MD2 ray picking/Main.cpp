/******************************************************************************
 * ==> Main ------------------------------------------------------------------*
 ******************************************************************************
 * Description : MD2 demo main form                                           *
 * Developer   : Jean-Milost Reymond                                          *
 ******************************************************************************/

#include <vcl.h>
#pragma hdrstop
#include "Main.h"

// qr engine
#include "QR_STDTools.h"
#include "QR_GeometryHelper.h"

// resources
#include "Resources.rh"

#pragma package(smart_init)
#pragma link "glewSL.lib"
#pragma resource "*.dfm"

//------------------------------------------------------------------------------
// TMainForm
//------------------------------------------------------------------------------
TMainForm* MainForm;
//------------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* pOwner) :
    TForm(pOwner),
    m_pMemoryDir(NULL),
    m_PreviousTime(::GetTickCount())
{
    m_pMemoryDir = new QR_MemoryDir(true);
}
//------------------------------------------------------------------------------
__fastcall TMainForm::~TMainForm()
{
    // delete model
    if (m_pMD2)
    {
        delete m_pMD2;
        m_pMD2 = NULL;
    }

    // delete memory dir
    if (m_pMemoryDir)
        delete m_pMemoryDir;

    // shutdown OpenGL
    m_Renderer.DisableOpenGL(Handle);
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject* pSender)
{
    // initialize OpenGL
    if (!m_Renderer.EnableOpenGL(Handle))
    {
        MessageDlg("OpenGL could not be initialized.\r\n\r\nApplication will close.", mtError,
                TMsgDlgButtons() << mbOK, 0);;
        Application->Terminate();
        return;
    }

    ConfigOpenGL();

    // calculate aspect ratio
    const float aspect = float(ClientWidth) / float(ClientHeight);

    // calculate the projection matrix
    m_Projection = m_Renderer.GetPerspective(45.0f, aspect, 0.1f, 100.0f);

    m_Renderer.CreateViewport(ClientWidth, ClientHeight);

    // load resources
    std::auto_ptr<TResourceStream> pModelStream(new TResourceStream((int)HInstance,
                                                                    ID_MD2_MODEL,
                                                                    L"DATA"));
    std::auto_ptr<TResourceStream> pTextureStream(new TResourceStream((int)HInstance,
                                                                      ID_MD2_TEXTURE,
                                                                      L"DATA"));
    std::auto_ptr<TResourceStream> pConfigStream(new TResourceStream((int)HInstance,
                                                                     ID_MD2_CONFIGURATION,
                                                                     L"DATA"));
    std::auto_ptr<TResourceStream> pNTStream(new TResourceStream((int)HInstance,
                                                                 ID_MD2_NORMALS_TABLE,
                                                                 L"DATA"));

    // convert model stream to memory file
    std::auto_ptr<QR_MemoryBuffer> pModelBuffer(new QR_MemoryBuffer());
    pModelStream->Seek(0, 0);
    pModelBuffer->Write(pModelStream->Memory, pModelStream->Size);

    // convert texture stream to memory file
    std::auto_ptr<QR_MemoryBuffer> pTextureBuffer(new QR_MemoryBuffer());
    pTextureStream->Seek(0, 0);
    pTextureBuffer->Write(pTextureStream->Memory, pTextureStream->Size);

    // convert configuration stream to memory file
    std::auto_ptr<QR_MemoryBuffer> pConfigBuffer(new QR_MemoryBuffer());
    pConfigStream->Seek(0, 0);
    pConfigBuffer->Write(pConfigStream->Memory, pConfigStream->Size);

    // convert normals table stream to memory file
    std::auto_ptr<QR_MemoryBuffer> pNTBuffer(new QR_MemoryBuffer());
    pNTStream->Seek(0, 0);
    pNTBuffer->Write(pNTStream->Memory, pNTStream->Size);

    // add model file to memory dir
    m_pMemoryDir->AddFile(L"marvin.md2", pModelBuffer.get(), false);
    pModelBuffer.release();

    // add texture file to memory dir
    m_pMemoryDir->AddFile(L"marvin.bmp", pTextureBuffer.get(), false);
    pTextureBuffer.release();

    // add configuration file to memory dir
    m_pMemoryDir->AddFile(L"marvin.cfg", pConfigBuffer.get(), false);
    pConfigBuffer.release();

    // add normals table file to memory dir
    m_pMemoryDir->AddFile(L"normals.bin", pNTBuffer.get(), false);
    pNTBuffer.release();

    // create and populate model info
    QR_MD2Group::IInfo md2Info;
    md2Info.m_ModelFileName   = L"marvin.md2";
    md2Info.m_TextureFileName = L"marvin.bmp";
    md2Info.m_ConfigFileName  = L"marvin.cfg";
    md2Info.m_NormalsFileName = L"normals.bin";

    // create and load model
    std::auto_ptr<QR_MD2Group> pMD2(new QR_MD2Group(&m_Renderer, &m_Resources));
    pMD2->Load(*m_pMemoryDir, md2Info);
    pMD2->SetTranslation(QR_Vector3DF(0.0f, 0.0f, -10.0f));
    pMD2->SetRotationX(-M_PI / 2.0f);
    pMD2->SetRotationY(-M_PI / 4.0f);
    pMD2->SetScaling(QR_Vector3DF(0.125f, 0.125f, 0.125f));
    pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Stand);

    QR_DirectionalLight light;
    light.m_Color     = QR_Color(255, 255, 255, 255);
    light.m_Ambient   = QR_Color(32, 32, 32, 255);
    light.m_Direction = QR_Vector3DP(0.5f, 0.0f, 0.5f);
    light.m_Enabled   = false;

    pMD2->SetLight(light);
    pMD2->SetVertexFormat(QR_Vertex::IEFormat(QR_Vertex::IE_VF_Normals |
                                              QR_Vertex::IE_VF_Colors  |
                                              QR_Vertex::IE_VF_TexCoords));

    m_pMD2 = pMD2.release();
    m_pMD2->Set_OnDetectCollision(OnDetectCollision);

    // from now, OpenGL will draw scene every time the thread do nothing else
    Application->OnIdle = IdleLoop;
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::FormResize(TObject* pSender)
{
    // calculate aspect ratio
    const float aspect = float(ClientWidth) / float(ClientHeight);

    // calculate the projection matrix
    m_Projection = m_Renderer.GetPerspective(45.0f, aspect, 0.1f, 100.0f);

    m_Renderer.CreateViewport(ClientWidth, ClientHeight);
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::FormPaint(TObject* pSender)
{
    RenderGLScene();
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::miLightingClick(TObject* pSender)
{
    // check or uncheck menu item
    miLighting->Checked = !miLighting->Checked;

    // switch between color or precalculated light mode
    m_pMD2->EnableLight(miLighting->Checked);
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::miPrevAnimClick(TObject* pSender)
{
    // get current running animation
    const QR_MD2CfgFile::IEGesture gesture = m_pMD2->GetGesture();

    // select prev animation
    if (gesture == QR_MD2CfgFile::IE_AG_Stand)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath4);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Run)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Stand);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Attack)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Run);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain1)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Attack);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain2)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain1);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain3)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain2);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Jump)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain3);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Flip)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Jump);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Salute)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Flip);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Taunt)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Salute);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Wave)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Taunt);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Point)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Wave);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRStand)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Point);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRWalk)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRStand);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRAttack)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRWalk);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRPain)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRAttack);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRPain);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath2)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath3)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath2);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath4)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath3);
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::miNextAnimClick(TObject* pSender)
{
    // get current running animation
    const QR_MD2CfgFile::IEGesture gesture = m_pMD2->GetGesture();

    // select next animation
    if (gesture == QR_MD2CfgFile::IE_AG_Stand)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Run);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Run)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Attack);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Attack)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain1);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain1)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain2);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain2)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Pain3);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Pain3)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Jump);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Jump)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Flip);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Flip)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Salute);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Salute)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Taunt);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Taunt)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Wave);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Wave)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Point);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_Point)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRStand);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRStand)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRWalk);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRWalk)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRAttack);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRAttack)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRPain);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRPain)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath2);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath2)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath3);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath3)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_CRDeath4);
    else
    if (gesture == QR_MD2CfgFile::IE_AG_CRDeath4)
        m_pMD2->SetAnimation(QR_MD2CfgFile::IE_AG_Stand);
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::IdleLoop(TObject* pSender, bool& done)
{
    done = false;
    RenderGLScene();
}
//------------------------------------------------------------------------------
void __fastcall TMainForm::RenderGLScene()
{
    // calculate time interval
    const std::time_t now            = ::GetTickCount();
    const double      elapsedTime    = (now - m_PreviousTime);
                      m_PreviousTime = now;

    // begin the scene
    m_Renderer.BeginScene(m_Color,
                          QR_Renderer::IESceneFlags(QR_Renderer::IE_SF_ClearColor |
                                                    QR_Renderer::IE_SF_ClearDepth));

    // load the projection and view matrices
    m_Renderer.SetProjectionMatrix(m_Projection);
    m_Renderer.SetViewMatrix(m_View);

    if (m_pMD2)
        m_pMD2->Draw(elapsedTime);

    // end the scene
    m_Renderer.EndScene();
}
//--------------------------------------------------------------------------------------------------
void TMainForm::DrawPolygons(const QR_PolygonsP& polygons, const QR_Matrix16P& matrix) const
{
    const QR_SizeT polygonCount = polygons.size();

    // polygons to draw?
    if (!polygonCount)
        return;

    QR_Mesh mesh;
    mesh.resize(1);

    try
    {
        // create a vertex buffer and populate it with polygons in collisions
        mesh[0]              = new QR_Vertex();
        mesh[0]->m_Type      = QR_Vertex::IE_VT_Triangles;
        mesh[0]->m_CoordType = QR_Vertex::IE_VC_XYZ;
        mesh[0]->m_Stride    = 7;
        mesh[0]->m_Format    = QR_Vertex::IE_VF_Colors;
        mesh[0]->m_Buffer.resize(polygonCount * (mesh[0]->m_Stride * 3));

        QR_SizeT offset = 0;

        // iterate through polygons to draw
        for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
        {
            // build polygon to show
            mesh[0]->m_Buffer[offset]      = (*it)->GetVertex1().m_X;
            mesh[0]->m_Buffer[offset + 1]  = (*it)->GetVertex1().m_Y;
            mesh[0]->m_Buffer[offset + 2]  = (*it)->GetVertex1().m_Z;
            mesh[0]->m_Buffer[offset + 3]  = 1.0f;
            mesh[0]->m_Buffer[offset + 4]  = 0.0f;
            mesh[0]->m_Buffer[offset + 5]  = 0.0f;
            mesh[0]->m_Buffer[offset + 6]  = 1.0f;
            mesh[0]->m_Buffer[offset + 7]  = (*it)->GetVertex2().m_X;
            mesh[0]->m_Buffer[offset + 8]  = (*it)->GetVertex2().m_Y;
            mesh[0]->m_Buffer[offset + 9]  = (*it)->GetVertex2().m_Z;
            mesh[0]->m_Buffer[offset + 10] = 0.8f;
            mesh[0]->m_Buffer[offset + 11] = 0.0f;
            mesh[0]->m_Buffer[offset + 12] = 0.2f;
            mesh[0]->m_Buffer[offset + 13] = 1.0f;
            mesh[0]->m_Buffer[offset + 14] = (*it)->GetVertex3().m_X;
            mesh[0]->m_Buffer[offset + 15] = (*it)->GetVertex3().m_Y;
            mesh[0]->m_Buffer[offset + 16] = (*it)->GetVertex3().m_Z;
            mesh[0]->m_Buffer[offset + 17] = 1.0f;
            mesh[0]->m_Buffer[offset + 18] = 0.12f;
            mesh[0]->m_Buffer[offset + 19] = 0.2f;
            mesh[0]->m_Buffer[offset + 20] = 1.0f;

            // go to next polygon
            offset += (mesh[0]->m_Stride * 3);
        }

        // configure OpenGL to draw polygons in collision
        glDisable(GL_TEXTURE_2D);
        glCullFace(GL_NONE);
        glDisable(GL_DEPTH_TEST);

        #ifdef USE_SHADER
            // prepare shader to draw the model
            m_Renderer.ConnectProjectionMatrixToShader(m_pMouseColDetShader,
                                                      *m_pScene->GetProjectionMatrix());
            m_Renderer.ConnectViewMatrixToShader(m_pMouseColDetShader,
                                                *m_pScene->GetViewMatrix());

            QR_ModelTextures textures;

            // draw polygons in collision with mouse pointer
            m_Renderer.Draw(mesh, matrix, textures, m_pMouseColDetShader);
        #else
            glMatrixMode(GL_MODELVIEW);

            glPushMatrix();

            // place triangles into 3D world
            glLoadMatrixf(const_cast<QR_Matrix16P&>(matrix).GetPtr());

            QR_ModelTextures textures;

            // draw polygons in collision with mouse pointer
            m_Renderer.Draw(mesh, matrix, textures);

            glPopMatrix();
        #endif

        // restore previous OpenGL parameters
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_FRONT);
        glEnable(GL_TEXTURE_2D);

        glFlush();
    }
    catch (...)
    {
        if (mesh.size() && mesh[0])
            delete mesh[0];

        throw;
    }

    if (mesh.size() && mesh[0])
        delete mesh[0];
}
//--------------------------------------------------------------------------------------------------
void TMainForm::OnDetectCollision(const QR_ModelGroup* pSender,
                                  const QR_Mesh*       pMesh,
                                  const QR_Matrix16P&  modelMatrix,
                                  const QR_AABBTree*   pAABBTree)
{
    if (!pAABBTree)
        return;

    // calculate client rect in OpenGL coordinates
    QR_RectP rect(-1.0f, 1.0f, 1.0f, 3.0f);

    int   width  = ClientWidth;
    int   height = ClientHeight;
    float zNear  = 0.1f;
    float zFar   = 100.0f;

    TPoint mousePos = Mouse->CursorPos;

    if (!::ScreenToClient(Handle, &mousePos))
        return;

    // convert mouse position to viewport, that will be used as ray start pos
    QR_Vector3DP rayPos = QR_Renderer::MousePosToViewportPos(QR_PointI(mousePos.X, mousePos.Y),
                                                             QR_RectI(0, 0, width, height),
                                                             rect);

    // create ray dir
    #ifdef COLLISION_DETECTION_FIRST_SHOOTER_MODE
        QR_Vector3DP rayDir(0.0f, 0.0f, -1.0f);
    #else
        QR_Vector3DP rayDir(rayPos.m_X, rayPos.m_Y, -1.0f);
    #endif

    // unproject the ray to make it inside the 3d world coordinates
    QR_Renderer::Unproject(m_Projection, m_View, rayPos, rayDir);

    #ifdef COLLISION_DETECTION_DRAW_DEBUG_RAY
        const QR_Vector3DF farRayPos(rayPos.m_X + (rayDir.m_X * 100.0f),
                                     rayPos.m_Y + (rayDir.m_Y * 100.0f),
                                     rayPos.m_Z + (rayDir.m_Z * 100.0f));

        glDisable(GL_TEXTURE_2D);
        glLineWidth(2.5f);
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(rayPos.m_X, rayPos.m_Y, rayPos.m_Z);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(farRayPos.m_X, farRayPos.m_Y, farRayPos.m_Z);
        glEnd();
        glEnable(GL_TEXTURE_2D);
    #endif

    float determinant;

    // now transform the ray to match with the model position
    const QR_Matrix16P invertModel = const_cast<QR_Matrix16P&>(modelMatrix).Inverse(determinant);
    rayPos                         = invertModel.Transform(rayPos);
    rayDir                         = invertModel.TransformNormal(rayDir);
    rayDir                         = rayDir.Normalize();

    #ifdef USE_DETECTION_COLLISION_DEBUG_TOOLS
        #ifdef OS_WIN
            #ifdef CP_EMBARCADERO
                m_pCollisionToolbox->vmProjection->Show(L"Projection matrix", projectionMatrix);
                m_pCollisionToolbox->vmView->Show(L"View matrix", viewMatrix);
                m_pCollisionToolbox->vmModel->Show(L"Model matrix", modelMatrix);
                m_pCollisionToolbox->vvRayPos->Show(L"Ray pos", rayPos);
                m_pCollisionToolbox->vvRayDir->Show(L"Ray dir", rayDir);

                QR_BoxP* pBBox = pAABBTree->GetBoundingBox();

                if (pBBox)
                {
                    m_pCollisionToolbox->vvBoxMin->Show(L"Bounding box min", pBBox->m_Min);
                    m_pCollisionToolbox->vvBoxMax->Show(L"Bounding box max", pBBox->m_Max);
                }
            #endif
        #endif
    #endif

    // create and populate ray from mouse position
    std::auto_ptr<QR_RayP> pRay(new QR_RayP());
    pRay->SetPos(rayPos);
    pRay->SetDir(rayDir);

    QR_PolygonsP polygons;

    try
    {
        // get polygons to check for collision by resolving AABB tree
        pAABBTree->Resolve(pRay.get(), polygons);

        QR_PolygonsP pickedPolygons;
        QR_SizeT     polygonCount = polygons.size();

        // iterate through polygons to check
        if (polygonCount > 0)
            for (QR_PolygonsP::const_iterator it = polygons.begin(); it != polygons.end(); ++it)
                // is polygon intersecting ray?
                if (QR_GeometryHelper::RayIntersectsPolygon(*(pRay.get()), *(*it)))
                    // add polygon in collision to resulting list
                    pickedPolygons.push_back(*it);

        // found collision?
        if (pickedPolygons.size())
            // draw polygons in collision if required
            DrawPolygons(pickedPolygons, modelMatrix);
    }
    catch (...)
    {
        QR_STDTools::DelAndClear(polygons);
        throw;
    }

    QR_STDTools::DelAndClear(polygons);
}
//------------------------------------------------------------------------------
void TMainForm::ConfigOpenGL()
{
    // configure OpenGL
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // enable blender (not required but add a nice ghosted effect)
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);

    glEnable(GL_TEXTURE_2D);
}
//------------------------------------------------------------------------------
