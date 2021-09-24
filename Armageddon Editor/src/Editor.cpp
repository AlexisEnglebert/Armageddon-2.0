#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#include "Application.h"
#include "Main.h"
#include "Log.h"
#include "Renderer\Camera\Camera.h"
#include "Renderer\Primitives.h"
#include "Renderer\Interface.h"
#include "imgui.h"
#include "Panel/ContentBrowser.h"
#include "Panel/EntityList.h"
#include "Panel/EntityProperties.h"
#include "Panel/MaterialEditor.h"
#include "Panel/BoneDebug.h"

#include "Scene/Scene.h"
#include "Renderer/EnvMap.h"
#include "Renderer/ConstantBuffer.h"
#include "Renderer/ConstantBufferTypes.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Serializer.h"
#include "Renderer/CascadeShadow.h"

#include "ImGuizmo.h"
class Editor : public Armageddon::Application
{
public:
	
	void OnUpdate() override;
	void OnRender() override;
	void ImGuiRender() override;
	void OnInit() override;
    void onMouseEvent(MouseEvent::MEventType e, float x, float y);
    void RenderScene(bool BindMat);

	Editor()
	{
        m_Envmap = EnvMap(L"..\\Armageddon Editor\\Assets\\Texture\\Skybox\\HDR\\hilly_terrain_01_1k.hdr");
	}

	~Editor()
	{
        for (auto& mat : AssetManager::v_material)
        {
            m_serializer.SerializeMaterial("Assets/Materials/" + mat.m_name + ".mat", mat);
        }
	}
    
private:
    ImVec2 CurrentWindowSize;
    
    Scene m_Scene;
    Serializer m_serializer = {&m_Scene};

    ContentBrowser m_ContentBrowser;
    EntityList m_EntityList = { m_Scene };
    EntityProperties m_EntityProperties = { m_Scene };
    MaterialEditor m_MaterialEditor = { m_Scene };
    BoneDebug      m_BoneDebug = { m_Scene };
    bool cameraControlsActive = false;

    void CreateDockSpace();
    void DrawImGuiScene();
	void DrawGuizmos();

	EnvMap m_Envmap;
    Armageddon::CascadeShadow m_Cascade = { &Armageddon::Application::GetWindow()->GetRenderer().m_camera,m_Scene };


    Mesh m_quad = Armageddon::Renderer2D::GeneratePlane();
	Mesh Msphere;

   


};

Armageddon::Application* Armageddon::CreateApplication()
{
	return new Editor();
}
static float cameraSpeed = 0.3f;
void Editor::OnUpdate()
{
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
* TODO améliorer tout ça pour l'instant c'est très chaotique car si je veut rajouter des choses je suis obliger 
* de modifier le dll donc pas très bon ! 
*/
void Editor::OnRender()
{
    auto group = m_Scene.g_registry.group<LightComponent>(entt::get<TagComponent>);
    for (int i = 0 ; i < group.size();i++)
    {
        auto ent = group[i];
        auto entity = m_Scene.GetEntityByID(ent);
        auto& component = entity.GetComponent<LightComponent>();
        switch (component.type)
        {
        case 0:
        {
			Armageddon::Renderer::g_LightBufferData.PointLights[i].Color = component.m_pointLight.Color;
			Armageddon::Renderer::g_LightBufferData.PointLights[i].Position = component.m_pointLight.Position;
			Armageddon::Renderer::g_LightBufferData.PointLights[i].Intensity = component.m_pointLight.Intensity;
			Armageddon::Renderer::g_LightBufferData.PointLights[i].Radius = component.m_pointLight.Radius;
            break;
        }
        case 1:
        {
			Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Color = component.m_directionalLight.Color;
			Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Direction = component.m_directionalLight.Direction;
			Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Intensity = component.m_directionalLight.Intensity;
            break;
        }
        default:
            break;
        }


    }

	Armageddon::Renderer::g_LightBufferData.PointLightCount = Armageddon::Renderer::g_PointLightsVector.size();
    Armageddon::Renderer::g_LightBufferData.DirectionalLightCount = Armageddon::Renderer::g_DirectLightsVector.size();
 
    m_Cascade.CalculateMatrices();

	//Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, m_Cascade.m_CascadeLightTex.pTargetView.GetAddressOf(), nullptr);
  
    
    
    //HERE IS WHERE I Bind the RenderTargetView 
    m_Cascade.m_CascadeLightTex.Bind(Armageddon::Interface::GetDeviceContext().Get());
    m_Cascade.m_CascadeLightTex.Clear(Armageddon::Interface::GetDeviceContext().Get());

    //I DRAW MY SCENE
	 for (auto& ent : m_Scene.v_Entity)
	{
		if (ent.HasComponent<MeshComponent>() && !ent.HasComponent<LightComponent>())
		{
            
			auto& comp = ent.GetComponent<MeshComponent>();
			if (ent.HasComponent<TransformComponent>())
			{
				auto& transform = ent.GetComponent<TransformComponent>();
                comp.m_mesh.GetTransform()->WorldMat *= transform.GetTransformMatrix();

			}
            

			Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(comp.m_mesh.GetTransform());
			Armageddon::Renderer::g_TransformCBuffer.BindPS();
			Armageddon::Renderer::g_TransformCBuffer.BindVS();

			m_Cascade.Update();
			Armageddon::Renderer::g_LightBufferData.LightViewProjection = m_Cascade.LightView * m_Cascade.LightProjection;
			Armageddon::Renderer::g_LightBufferData.CameraPos = Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetPos();


			Armageddon::Renderer::g_LightCBuffer.SetDynamicData(&Armageddon::Renderer::g_LightBufferData);
			Armageddon::Renderer::g_LightCBuffer.BindPS();
			Armageddon::Renderer::g_LightCBuffer.BindVS();


			for (auto& submesh : comp.m_mesh.v_SubMeshes)
			{

                Armageddon::Interface::GetDeviceContext()->IASetInputLayout(m_Cascade.vx.GetInputLayout());
                Armageddon::Interface::GetDeviceContext()->PSSetShader(m_Cascade.px.GetShader(),nullptr,0);
                Armageddon::Interface::GetDeviceContext()->VSSetShader(m_Cascade.vx.GetShader(),nullptr,0);
				submesh.BindIndexBuffer();
				submesh.BindVertexBuffer();
				submesh.DrawIndexed(false);
			}
		}
	}

    // HERE I CHANGE THE RenderTargetView

	Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().Bind(Armageddon::Interface::GetDeviceContext().Get());
	Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().Clear(Armageddon::Interface::GetDeviceContext().Get());
    //I REDRAW MY SCENE WHERE I'M BINDING THE RESSOURCE


    RenderScene(true);
    


   /* m_quad.UpdtateTransform(&Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera);
	Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(m_quad.GetTransform());
	Armageddon::Renderer::g_TransformCBuffer.BindPS();
	Armageddon::Renderer::g_TransformCBuffer.BindVS();
	m_quad.BindShaders();
	m_quad.BindInputLayout();
	m_quad.BindVertexBuffer();
	m_quad.BindIndexBuffer();
	m_quad.DrawIndexed();*/

	m_Envmap.Render(&Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera);

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
    ImGui::Begin("Debug shadow");
	ImGui::Image(
        m_Cascade.m_CascadeLightTex.DephtResourceView.Get()
		, { m_Cascade.m_CascadeLightTex.GetImageX(),
        m_Cascade.m_CascadeLightTex.GetImageY() });

    ImGui::End();
}

void Editor::OnInit()
{
	Armageddon::Log::GetLogger()->trace("OnInit Event Reached");
    Armageddon::Application::GetWindow()->SetMouseCallBack(std::bind(&Editor::onMouseEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
  
    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetPosition(0.0f,0.0f,-1.0f);
    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetProjectionValues(90.0f,Armageddon::Application::GetWindow()->GetAspectRatio(),0.1f,10000.0f);

    ImGui::SetCurrentContext(Armageddon::Application::GetWindow()->GetRenderer().GetImGuiContext());
    ImGuizmo::SetImGuiContext(Armageddon::Application::GetWindow()->GetRenderer().GetImGuiContext());
    Armageddon::Log::GetLogger()->trace("OnInit Event Reached");
    Msphere = Armageddon::Primitives::GenerateSphere(1, 38, 20);

    //m_Scene.LoadScene("Assets/Scenes/TestScene.mat");
    m_serializer.DeserializeScene("Assets/Scenes/TestScene.mat");
   // Armageddon::Renderer

   // Mesh model = Mesh("..\\Armageddon Editor\\Assets\\Models\\suzanne.obj");

   /* Mesh model = Mesh("..\\Armageddon Editor\\Assets\\Models\\Nature Kit\\Models\\OBJ format\\canoe.obj");
    model.SetTransform(&Armageddon::Application::GetWindow()->GetRenderer().m_camera);
   Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().SubmitMesh(model);*/

   /*auto entity = m_Scene.CreateEntity();
   entity.AddComponent<TagComponent>("test");
   entity.AddComponent<MeshComponent>("..\\Armageddon Editor\\Assets\\Models\\Nature Kit\\Models\\OBJ format\\canoe.obj");*/

    
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

            Armageddon::Application::GetWindow()->GetRenderer().m_camera.AdjustRotation(y * 0.01f, x * 0.01f, 0.0f);
            break;
        }
    }
}

void Editor::RenderScene(bool BindMat)
{
	for (auto& ent : m_Scene.v_Entity)
	{

        //Armageddon::Interface::GetDeviceContext()->CSSetShader()

		if (ent.HasComponent<MeshComponent>())
		{
			auto& component = ent.GetComponent<MeshComponent>();
			if (!component.m_mesh.IsEmpty()) {
				component.m_mesh.UpdtateTransform(&Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera);
				if (ent.HasComponent<LightComponent>())
				{
					auto& Lightcomponent = ent.GetComponent<LightComponent>();

					component.m_mesh.GetTransform()->WorldMat *= DirectX::XMMatrixTranslation(Lightcomponent.m_pointLight.Position.x, Lightcomponent.m_pointLight.Position.y, Lightcomponent.m_pointLight.Position.z);

				}
				if (ent.HasComponent<TransformComponent>())
				{
					auto& transform = ent.GetComponent<TransformComponent>();
					component.m_mesh.GetTransform()->WorldMat *= transform.GetTransformMatrix();
				}
				if (component.m_mesh.m_skeleton.m_JointsCount > 0)
				{
					for (UINT i = 0; i < component.m_mesh.m_skeleton.m_JointsCount; i++)
					{
						Msphere.UpdtateTransform(&Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera);

						Msphere.GetTransform()->WorldMat *= component.m_mesh.m_skeleton.m_aJoints[i].m_inverseBlindPose;

						Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(Msphere.GetTransform());
						Armageddon::Renderer::g_TransformCBuffer.BindPS();
						Armageddon::Renderer::g_TransformCBuffer.BindVS();

                        Msphere.v_SubMeshes[0].BindVertexBuffer();
                        Msphere.v_SubMeshes[0].BindIndexBuffer();
                        Msphere.v_SubMeshes[0].DrawIndexed(BindMat);
					}
				}


				Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(component.m_mesh.GetTransform());
				Armageddon::Renderer::g_TransformCBuffer.BindPS();
				Armageddon::Renderer::g_TransformCBuffer.BindVS();

				Armageddon::Renderer::g_LightBufferData.CameraPos = Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().m_camera.GetPos();
				Armageddon::Renderer::g_LightBufferData.LightViewProjection = m_Cascade.LightView * m_Cascade.LightProjection;
	


				Armageddon::Renderer::g_LightCBuffer.SetDynamicData(&Armageddon::Renderer::g_LightBufferData);
				Armageddon::Renderer::g_LightCBuffer.BindPS();
				Armageddon::Renderer::g_LightCBuffer.BindVS();



				Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(6, 1, m_Envmap.m_convEnvMapTexture.GetRessourceViewPtr());
				Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(7, 1, m_Envmap.m_PreFilteredEnvMap.GetRessourceViewPtr());
				Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(8, 1, m_Envmap.m_BRFLutTexture.GetRessourceViewPtr());
				Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(9, 1, m_Cascade.m_CascadeLightTex.DephtResourceView.GetAddressOf());

				
				// component.m_mesh.BindShaders();
                
                for (auto& Submesh : component.m_mesh.v_SubMeshes)
                {

                    Submesh.BindVertexBuffer();
                    Submesh.BindIndexBuffer();
                    component.m_mesh.BindMaterial(Submesh.m_materialIndex);
                    Submesh.DrawIndexed(true);
                }


			}
		}
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
                
                for (auto& pute : m_Scene.v_Entity)
                {
                    if (pute.HasComponent<TagComponent>())
                    {
                        Armageddon::Log::GetLogger()->trace("TAG COMOPONENENENEN");

                    }
                    if (pute.HasComponent<MeshComponent>())
                    {
                        Armageddon::Log::GetLogger()->trace("TU AS LE MESH COMPONENT ALORS CASSE PAS LES COUIILLES");
                    }
                    if (pute.HasComponent<TransformComponent>())
                    {
                        Armageddon::Log::GetLogger()->trace("qsdqsdqsdqsdqsdqsdqsd");
                    }

                    m_serializer.SerializeScene("Assets/Scenes/TestScene.mat", pute,m_Scene.g_registry);
                }
               // m_Scene.g_registry.data
                Armageddon::Log::GetLogger()->trace(m_Scene.g_registry.capacity<TransformComponent>());
                Armageddon::Log::GetLogger()->trace(m_Scene.g_registry.capacity<TagComponent>());
            }

			ImGui::Separator();

			ImGui::EndMenu();
		}


		ImGui::EndMenuBar();
	}

	ImGui::End();



	ImGui::Begin("Debug");
   // ImGui::DragFloat("Rougness", &m_PBRBUFFER.Roughness, 0.01, 0, 1);
   // ImGui::DragFloat("Metalic", &m_PBRBUFFER.Metalic, 0.01, 0, 1);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}

void Editor::DrawImGuiScene()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f,0.0f });

    ImGui::Begin("Scene", 0);
    ImGui::PopStyleVar(3);
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
      //  if (viewportPanelSize.x / viewportPanelSize.y > 0.0f) {
            /* on resize que le offscreen render target view*/
           /*  Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().ResizeRenderTargetView(
                vMax.x - vMin.x, vMax.y - vMin.y,
                nullptr);*/
       
		    Armageddon::Application::GetWindow()->GetRenderer().m_camera.SetProjectionValues(90.0f, viewportPanelSize.x / viewportPanelSize.y, 0.1f, 10000.0f);

            CurrentWindowSize = ImGui::GetWindowSize();
        //}
    }



    ImGui::Image(
        Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().GetShaderRessource()
        , { Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().GetImageX(),
        Armageddon::Application::GetApplicationInsatnce()->GetWindow()->GetRenderer().GetOffScreenRenderTarget().GetImageY() });


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

            ImGuizmo::SetRect(vMin.x, vMin.y, static_cast<float>(Armageddon::Application::GetApplicationInsatnce()->GetWindow()->w_width),
                static_cast<float>(Armageddon::Application::GetApplicationInsatnce()->GetWindow()->w_height));
            ImGui::GetForegroundDrawList()->AddRect(vMin, vMax, IM_COL32(255, 255, 0, 255));
            if (ImGuizmo::Manipulate(*ViewMat.m, *ProjMat.m, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, *TransformMat.m))
            {
				component.Translation.x = TransformMat.m[3][0];
				component.Translation.y = TransformMat.m[3][1];
				component.Translation.z = TransformMat.m[3][2];
            }
        }
    }
}
