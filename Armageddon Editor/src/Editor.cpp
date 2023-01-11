#include <stdlib.h>  

#include "Application.h"
#include "Main.h"
#include "Log.h"

#if WINDOWS
    #include <crtdbg.h>
#endif

#include "Renderer\Camera\Camera.h"
#include "Renderer\Primitives.h"
#include "Renderer\Interface.h"
#include "Panel/ContentBrowser.h"
#include "Panel/EntityList.h"
#include "Panel/EntityProperties.h"
#include "Panel/MaterialEditor.h"
#include "Panel/BoneDebug.h"
#include "Panel/RessourceManager.h"
#include "Panel/OuputLog.h"

#include "Scene/Scene.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/ConstantBufferTypes.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Serializer.h"


#include "Scripting/ScriptEngine.h"
#include "Utils/Timer.h"
#include "imgui.h"
#include "ImGuizmo.h"

#define CAMERA_NEAR 0.1f
#define CAMERA_FAR 100.0f




class Editor : public Armageddon::Application
{
public:
	
	void OnUpdate() override;
	void OnRender() override;
	void ImGuiRender() override;
	void OnInit() override;
    void onMouseEvent(MouseEvent::MEventType e, float x, float y);
    void onKeyBoardEvent(const unsigned char keyCode);


	Editor()
	{
        Armageddon::Log::GetLogger()->info("EDITOR CONSTRUCTOR");
        m_Scene.m_SceneState = SceneState::Editor;
       

       //TODO REORGANISER TOUT LE PROJET

        m_PlayButton.Create(L"Ressources//Icones//Editor//PlayButton.png");

        auto materialRef = AssetManager::GetOrCreateMaterial("LightMaterial");

        AssetManager::m_MaterialMap[materialRef].SetVertexShader(L"Assets/Shaders/BillBoardVertex.cso");
        AssetManager::m_MaterialMap[materialRef].SetPixelShader(L"Assets/Shaders/BillBoardPixel.cso");
        AssetManager::m_MaterialMap[materialRef].SetAlbedoMap(L"Ressources//Icones//Editor//icone_point_light.png");

        AssetManager::m_MaterialMap[materialRef].RenderMode = 1;
	}

	~Editor()
	{
        for (auto& mat : AssetManager::v_material)
        {
            //m_serializer.SerializeMaterial("Assets/Materials/" + mat.m_AssetName + ".mat", mat);
        }
	}
    
private:
    ImVec2 CurrentWindowSize;
    
    Scene m_Scene;
    Serializer m_serializer = {&m_Scene};
    //Editor Panel
    ContentBrowser                  m_ContentBrowser;
    EntityList                      m_EntityList = { m_Scene };
    EntityProperties                m_EntityProperties = { m_Scene };
    MaterialEditor                  m_MaterialEditor = { m_Scene };
    BoneDebug                       m_BoneDebug = { m_Scene };
    RessourceManager                m_RessourceManager = { m_Scene };
    OutputWindow                    m_OutputWindow;

    int cascadeIndice = 0;
    bool cameraControlsActive = false;
    
    void CreateDockSpace();
    void DrawImGuiScene();
	void DrawGuizmos();

	


    Texture m_PlayButton;



};

Armageddon::Application* Armageddon::CreateApplication()
{
	return new Editor();
}
static float cameraSpeed = 0.3f;
void Editor::OnUpdate()
{

    m_Scene.UpdateScene();

    if (cameraControlsActive)
    {
        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_S))
        {
            //Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetUpVec() * cameraSpeed);
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetBackwardVec() * cameraSpeed);

        }
        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Space))
        {
            //Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetUpVec() * cameraSpeed);
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetUpVec() * cameraSpeed);

        }

        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Q))
        {
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetLeftVec() * cameraSpeed);

        }
        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_Z))
        {
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetForwardVec() * cameraSpeed);
        }
        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_D))
        {
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetRightVec() * cameraSpeed);
        }
        if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_A))
        {
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustRotation(0.0,0.1* cameraSpeed,0);
        }
       
    }
	if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_escape))
		EntityList::Seleceted = entt::null;
   
}


/*
* TODO am�liorer tout �a pour l'instant c'est tr�s chaotique car si je veut rajouter des choses je suis obliger 
* de modifier le dll donc pas tr�s bon ! 
*/
void Editor::OnRender()
{
    m_Scene.RenderScene();
}

static float i; 
void Editor::ImGuiRender()
{

    CreateDockSpace();
    DrawImGuiScene();

    m_ContentBrowser.ImGuiDraw();
    m_EntityList.ImGuiDraw();
    m_EntityProperties.ImGuiDraw();
    m_MaterialEditor.ImGuiDraw();
    m_BoneDebug.ImGuiDraw();
    m_RessourceManager.ImGuiDraw();
    m_OutputWindow.ImGuiDraw();

    ImGui::Begin("Debug shadow");
    ImGui::SliderInt("Cascade Index", &cascadeIndice, 0, 2);
    ImGui::Image(AG_GET_RENDERER().m_Cascade.m_CascadeShadowMap[cascadeIndice].GetRessourceView(), { 4096.0f, 2160.0f });
    ImGui::End();


    //World Panel (for post processing and other stuff)
    ImGui::Begin("WorldProperies");
    if (ImGui::TreeNodeEx("VolumetricFog",ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Volumetric Fog", &Armageddon::Renderer::g_volumetricBufferData.EnableVolumetricFog);
        ImGui::SliderFloat("Volumetric lerp factor", &Armageddon::Renderer::g_volumetricBufferData.LerpFactor, 0.0f, 1.0f, "%.3f", 0.1f);
        ImGui::SliderFloat("Volumetric density ", &Armageddon::Renderer::g_volumetricBufferData.density, 0.0f, 1.0f, "%.3f", 0.1f);
        ImGui::SliderFloat("Ambient Fog", &Armageddon::Renderer::g_volumetricBufferData.ambientFog, 0.0f, 1.0f, "%.3f", 0.1f);
        ImGui::SliderFloat("Fog intensity", &Armageddon::Renderer::g_volumetricBufferData.FogIntensity, 0.0f, 10.0f, "%.3f", 0.1f);
        ImGui::SliderFloat("Fog Anisotropy", &Armageddon::Renderer::g_volumetricBufferData.anisotropy, 0.0f, 1.0f, "%.3f", 0.1f);
    }
    ImGui::End();
    
}

void Editor::OnInit()
{
    ImGui::SetCurrentContext(Armageddon::Application::GetWindow()->GetRenderer().GetImGuiContext());
    ImGuizmo::SetImGuiContext(Armageddon::Application::GetWindow()->GetRenderer().GetImGuiContext());
	Armageddon::Log::GetLogger()->trace("OnInit Event Reached");
    Armageddon::Application::GetWindow()->SetMouseCallBack(std::bind(&Editor::onMouseEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    Armageddon::Application::GetWindow()->setKeyBoardCallBack(std::bind(&Editor::onKeyBoardEvent, this, std::placeholders::_1));
  
    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetPosition(0.0f,0.0f,-1.0f);
    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetProjectionValues(90.0f,Armageddon::Application::GetWindow()->GetAspectRatio(),CAMERA_NEAR,CAMERA_FAR);

    Armageddon::Log::GetLogger()->trace("OnInit Event Reached");    
    m_Scene.InitScene();
    
  
}
static float CameraZoom = 1.0f;

void Editor::onMouseEvent(MouseEvent::MEventType e, float x, float y)
{
    if (cameraControlsActive)
    {
        switch (e)
        {
        case  MouseEvent::MEventType::Wheeldown:
            CameraZoom -= 0.1f;
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetBackwardVec() * cameraSpeed);

            break;
        case  MouseEvent::MEventType::Wheelup:
            CameraZoom += 0.1f;
            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustPosition(Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetForwardVec() * cameraSpeed);


            break;

        case MouseEvent::MEventType::Move:

            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustRotation(y * -0.01f, x * 0.01f, 0.0f);
            break;
        }
    }
}

void Editor::onKeyBoardEvent(const unsigned char keyCode)
{

    if (Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_ctrl))
    {
        if (!Armageddon::Application::GetWindow()->GetKeyBoard().KeyIsPressed(AG_KEY_D) && keyCode == AG_KEY_D)
        {
            if (EntityList::Seleceted != entt::null)
            {
               // m_Scene.DuplicateEntity(m_Scene.GetEntityByID(EntityList::Seleceted));
            }
        }

    }
    if ( keyCode == AG_KEY_delete) 
    {
        if (EntityList::Seleceted != entt::null) {
            m_Scene.DeleteEntity(m_Scene.GetEntityByID(EntityList::Seleceted));
            EntityList::Seleceted == entt::null;
        }
    }
    if (keyCode == AG_KEY_O)
    {
        
    }
}

void Editor::CreateDockSpace()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", 0, window_flags);
	ImGui::PopStyleVar();


	ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	}


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Save Scene", ""))
			{
                
                for (auto iterator = m_Scene.EntityMap.begin(); iterator != m_Scene.EntityMap.cend(); iterator++)
                {
                    m_serializer.SerializeScene("Assets/Scenes/TestScene.mat", iterator->second,m_Scene.g_registry);
                }
            }

			ImGui::Separator();

			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}

	ImGui::End();



	ImGui::Begin("Debug");

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    for (auto iterator = ProfilerData::m_profilerData.begin(); iterator != ProfilerData::m_profilerData.cend(); iterator++)
    {
        ImGui::Text("%s : %.3f ms", iterator->first.c_str(), iterator->second);

    }

    ProfilerData::Clear();
	ImGui::End();
}

void Editor::DrawImGuiScene()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f,0.0f});
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, { 0.0f,0.0f });
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0862745098f, 0.0862745098f, 0.0862745098f, 1.0f));

    

    ImGui::Begin("##SceneToolBar", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);
    ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5) - (ImGui::GetWindowHeight() - 4.0f * 0.5f));
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4( 35/255, 35/255, 35/255 ,1.0f));
    if (ImGui::ImageButton(m_PlayButton.GetRessourceView(), { 20,20 }))
    {
        if (m_Scene.m_SceneState == SceneState::Editor)
        {
            m_Scene.m_SceneState = SceneState::Runtime;
            m_Scene.OnRuntimeStart();

        }
        if (m_Scene.m_SceneState == SceneState::Runtime)
        {
        }
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar(2);

    ImGui::End();


    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });
    ImGuiWindowClass window_class;
    window_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&window_class);
    ImGui::Begin("Scene", 0,ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar );
    DrawGuizmos();
    ImVec2 WindowSize = ImGui::GetWindowSize();
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    vMin.x += ImGui::GetWindowPos().x;
    vMin.y += ImGui::GetWindowPos().y;
    vMax.x += ImGui::GetWindowPos().x;
    vMax.y += ImGui::GetWindowPos().y;
    if ((ImGui::IsWindowFocused() || ImGui::IsWindowHovered()) && GetAsyncKeyState(AG_KEY_shift) && GetAsyncKeyState(AG_KEY_ctrl))
    {

        cameraControlsActive = true;
    }
    else
    {

        cameraControlsActive = false;

    }
    if (CurrentWindowSize.x != WindowSize.x || CurrentWindowSize.y != WindowSize.y)
    {
		const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
       if (viewportPanelSize.x / viewportPanelSize.y > 0.0f) {
            /* on resize que le offscreen render target view*/
           float width = vMax.x - vMin.x;
           float height = vMax.y - vMin.y;
           AG_GET_RENDERER().m_FrameBuffer.ResizeTexture(width,height);
           AG_GET_RENDERER().m_Composite.ResizeTexture(width, height);
          Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().CreateViewPort(vMax.x - vMin.x, vMax.y - vMin.y);
		    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetProjectionValues(90.0f, viewportPanelSize.x / viewportPanelSize.y,CAMERA_NEAR, CAMERA_FAR);
            CurrentWindowSize = ImGui::GetWindowSize();
        }
    }

    ImGui::Image(
        AG_GET_RENDERER().m_Composite.GetRessourceView(), 
        {AG_GET_RENDERER().m_Composite.GetImageX(),AG_GET_RENDERER().m_Composite.GetImageY()});

    if (ImGui::BeginDragDropTarget())
    {
        if (auto payload = ImGui::AcceptDragDropPayload("ASSET", ImGuiInputTextFlags_ReadOnly))
        {
            const char* str = (const char*)payload->Data;

            //TODO verify this is a scene
            m_Scene.LoadScene(str);
            Armageddon::Log::GetLogger()->trace(str);
        }

        ImGui::EndDragDropTarget();

    }

    ImGui::PopStyleVar(3);

    ImGui::End();
}

void Editor::DrawGuizmos()
{

   ImGuizmo::SetOrthographic(false);
    ImGuizmo::Enable(true);
	ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());

    if (EntityList::Seleceted != entt::null)
    {
        auto entity = m_Scene.GetEntityByID(EntityList::Seleceted);
        if (entity.HasComponent<TransformComponent>())
        {
            auto& component = entity.GetComponent<TransformComponent>();
            ImVec2 vMin = ImGui::GetWindowContentRegionMin();
            ImVec2 vMax = ImGui::GetWindowContentRegionMax();

			vMin.x += ImGui::GetWindowPos().x;
			vMin.y += ImGui::GetWindowPos().y;
			vMax.x += ImGui::GetWindowPos().x;
			vMax.y += ImGui::GetWindowPos().y;
            DirectX::XMFLOAT4X4 ViewMat;
            DirectX::XMFLOAT4X4 ProjMat;
            DirectX::XMFLOAT4X4 TransformMat;
            
            DirectX::XMStoreFloat4x4(&ViewMat, Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetViewMatrix());
			DirectX::XMStoreFloat4x4(&ProjMat, Armageddon::Application::GetWindow()->GetRenderer().m_camera.GetProjectionMatrix());
			DirectX::XMStoreFloat4x4(&TransformMat, component.GetTransformMatrix());

            ImGuizmo::SetRect(vMin.x, vMin.y, vMax.x - vMin.x, vMax.y - vMin.y);
           // ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
            if (ImGuizmo::Manipulate(*ViewMat.m, *ProjMat.m, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, *TransformMat.m))
            {
				component.Translation.x = TransformMat.m[3][0];
				component.Translation.y = TransformMat.m[3][1];
				component.Translation.z = TransformMat.m[3][2];
            }
        }
    }
}
