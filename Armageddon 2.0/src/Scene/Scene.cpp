#include "Scene.h"
#include "Entity.h"
#include "Serializer.h"
#include "../Scripting/ScriptEngine.h"

Entity Scene::CreateEntity()
{
    Entity ent = { g_registry.create(), this };
    EntityMap.emplace((int)ent.GetHandle(), ent);
    return ent;
}

Entity& Scene::GetEntityByID(entt::entity ID)
{
    if (EntityMap.find(int(ID)) != EntityMap.end())
    {
        return EntityMap.at(int(ID));
    }



}

void Scene::UpdateScene()
{
    Scenetime++;

    if (m_SceneState == SceneState::Runtime)
    {
        auto view = g_registry.view<ScriptComponent>();
        for (auto& entt : view)
        {
            auto& entity = GetEntityByID(entt);
            auto& component = entity.GetComponent<ScriptComponent>();
            if (ScriptEngine::ClassExist(component.ClassName))
            {
                ScriptEngine::OnUpdateEntity(entity);

            }
        }
    }
}

void Scene::RenderScene()
{
    ID3D11RenderTargetView*     RTVNULL = nullptr;
    ID3D11ShaderResourceView* SRVNULL[] = { NULL };
    ID3D11UnorderedAccessView* UAVNULL[] = { NULL };

    float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };

    Profiler m_RenderTime("RenderScene");

    #pragma region ConstantBuffer Update

    //Update Light Sources and Constant Buffers

    //Update Scene Time
    m_SceneBuffer.Time = Scenetime;

    //Update camera info
    Armageddon::Renderer::g_CameraBufferData.ViewMat = AG_GET_RENDERER().m_camera.GetViewMatrix();
    Armageddon::Renderer::g_CameraBufferData.ProjectionMat = AG_GET_RENDERER().m_camera.GetProjectionMatrix();
    Armageddon::Renderer::g_CameraBufferData.ProjectionMat = AG_GET_RENDERER().m_camera.GetProjectionMatrix();
    Armageddon::Renderer::g_CameraBufferData.ViewMat = AG_GET_RENDERER().m_camera.GetViewMatrix();
    Armageddon::Renderer::g_CameraBufferData.MVP = DirectX::XMMatrixMultiply(AG_GET_RENDERER().m_camera.GetViewMatrix(), AG_GET_RENDERER().m_camera.GetProjectionMatrix());
    Armageddon::Renderer::g_CameraBufferData.InverseProjectionMat = DirectX::XMMatrixInverse(nullptr, AG_GET_RENDERER().m_camera.GetProjectionMatrix());
    Armageddon::Renderer::g_CameraBufferData.InverseViewMat = DirectX::XMMatrixInverse(nullptr, AG_GET_RENDERER().m_camera.GetViewMatrix());
    //Yep this is Inverse ViewProjection : don't forget (AB)-1 = B-1*A-1 so we multiply inverse projection with inverse view
    Armageddon::Renderer::g_CameraBufferData.InverseMVP = DirectX::XMMatrixMultiply(Armageddon::Renderer::g_CameraBufferData.InverseProjectionMat, Armageddon::Renderer::g_CameraBufferData.InverseViewMat);
    Armageddon::Renderer::g_CameraBufferData.CameraPos = AG_GET_RENDERER().m_camera.GetPos();
    Armageddon::Renderer::g_CameraBufferData.nearPlane = AG_GET_RENDERER().m_camera.GetNearPlane();
    Armageddon::Renderer::g_CameraBufferData.farPlane = AG_GET_RENDERER().m_camera.GetFarPlane();
    Armageddon::Renderer::g_CameraCBuffer.SetDynamicData(&Armageddon::Renderer::g_CameraBufferData);

    //Update Light source
    Armageddon::Renderer::g_LightBufferData.PointLightCount = Armageddon::Renderer::g_PointLightsVector.size();
    Armageddon::Renderer::g_LightBufferData.DirectionalLightCount = Armageddon::Renderer::g_DirectLightsVector.size();

    //Update Point light
    auto Pointgroup = g_registry.group<PointLightComponent>(entt::get<TagComponent>);
    for (unsigned int i = 0 ; i < Pointgroup.size();i++)
    {
        auto entity = GetEntityByID(Pointgroup[i]);
        auto& component = entity.GetComponent<PointLightComponent>();
        Armageddon::Renderer::g_LightBufferData.PointLights[i].Color = component.m_pointLight.Color;
        Armageddon::Renderer::g_LightBufferData.PointLights[i].Position = component.m_pointLight.Position;
        Armageddon::Renderer::g_LightBufferData.PointLights[i].Intensity = component.m_pointLight.Intensity;
        Armageddon::Renderer::g_LightBufferData.PointLights[i].Radius = component.m_pointLight.Radius;
    }

    //Update Directional light
    auto Dirgroup = g_registry.group<DirectionalLightComponent>(entt::get<TagComponent>);
    for (unsigned int i = 0; i < Dirgroup.size(); i++)
    {
        auto entity = GetEntityByID(Dirgroup[i]);
        auto& component = entity.GetComponent<DirectionalLightComponent>();
        Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Color = component.m_directionalLight.Color;
        Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Direction = component.m_directionalLight.Direction;
        Armageddon::Renderer::g_LightBufferData.DirectionalLights[i].Intensity = component.m_directionalLight.Intensity;

    }

    //UPDATE SHADOW CASCADE
    AG_GET_RENDERER().m_Cascade.CalculateMatrices();
    for (size_t i = 0; i < 3; i++)
    {
        Armageddon::Renderer::g_LightBufferData.LightViewProjectionCascade[i] = AG_GET_RENDERER().m_Cascade.m_LightViewMatrices[i] * AG_GET_RENDERER().m_Cascade.m_LightProjMatrices[i];
    }
    Armageddon::Renderer::g_LightBufferData.FarPlane.x = AG_GET_RENDERER().m_Cascade.m_SplitDepth[0];
    Armageddon::Renderer::g_LightBufferData.FarPlane.y = AG_GET_RENDERER().m_Cascade.m_SplitDepth[1];
    Armageddon::Renderer::g_LightBufferData.FarPlane.z = AG_GET_RENDERER().m_Cascade.m_SplitDepth[2];


    //Update Mesh Transform
    auto Meshgroup = g_registry.group<MeshComponent>(entt::get<TransformComponent>);
    for (unsigned int i = 0; i < Meshgroup.size(); i++)
    {
        auto entity = GetEntityByID(Meshgroup[i]);
        auto& Meshcomponent = entity.GetComponent<MeshComponent>();
        auto& Transformcomponent = entity.GetComponent<TransformComponent>();
        Meshcomponent.m_mesh.GetTransform()->WorldMat = Transformcomponent.GetTransformMatrix();
    }

    //Update Volumetric data
    Armageddon::Renderer::g_VolumetricBuffer.SetDynamicData(&Armageddon::Renderer::g_volumetricBufferData);

    #pragma endregion

    #pragma region ShadowPass

    {
        Profiler m_CascadeTimer("CascadeShadowPass");

        //Set the shadow Viewport 
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = SHADOW_MAP_WIDTH;
        viewport.Height = SHADOW_MAP_HEIGHT;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);
        
        //TODO MOVE THIS IN A GEOMETRY SHADER
        for (uint8_t i = 0; i < NUM_CASCADE; i++)
        {
            //Bind only DepthStencil because we only need the depth 
            Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(0, &RTVNULL, AG_GET_RENDERER().m_Cascade.m_CascadeShadowMap[i].DephtStencilView.Get());
            Armageddon::Interface::GetDeviceContext()->ClearDepthStencilView(AG_GET_RENDERER().m_Cascade.m_CascadeShadowMap[i].DephtStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);

            auto group = g_registry.group<MeshComponent>(entt::get<TagComponent>);
            for (auto& enityID : group)
            {
                auto entity = GetEntityByID(enityID);
                auto& component = entity.GetComponent<MeshComponent>();
               

                //Bind constantBuffer
                Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(component.m_mesh.GetTransform());
                Armageddon::Renderer::g_TransformCBuffer.BindVS();
                
                Armageddon::Renderer::g_LightBufferData.CascadeIndice = i;
                Armageddon::Renderer::g_LightCBuffer.SetDynamicData(&Armageddon::Renderer::g_LightBufferData);
                Armageddon::Renderer::g_LightCBuffer.BindVS();

                Armageddon::Renderer::g_CameraCBuffer.SetDynamicData(&Armageddon::Renderer::g_CameraBufferData);
                Armageddon::Renderer::g_CameraCBuffer.BindVS();

                for (auto& submesh : component.m_mesh.v_SubMeshes)
                {

                    Armageddon::Interface::GetDeviceContext()->IASetInputLayout(AG_GET_RENDERER().m_Cascade.vx.GetInputLayout());
                    Armageddon::Interface::GetDeviceContext()->VSSetShader(AG_GET_RENDERER().m_Cascade.vx.GetShader(), nullptr, 0);
                    Armageddon::Interface::GetDeviceContext()->PSSetShader(nullptr, nullptr, 0);
                    submesh.BindIndexBuffer();
                    submesh.BindVertexBuffer(); //HERE ERROR :(
                    submesh.DrawIndexed();
                }
            }

            Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RTVNULL, nullptr);
        }
    }
    #pragma endregion
    

    #pragma region VolumetricCompute
    {
        if (Armageddon::Renderer::g_volumetricBufferData.EnableVolumetricFog)
        {
            Profiler VolumetricCompute("VolumetricComputePass");

            //Volumetric Light & density calculation
            Armageddon::Interface::GetDeviceContext()->CSSetShader(AG_GET_RENDERER().m_VolumetricFog.m_VolumetricInjectShader.GetShader(), NULL, 0);
            Armageddon::Interface::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &AG_GET_RENDERER().m_VolumetricFog.m_VolumetricIntegration.m_UAV, NULL);
            Armageddon::Interface::GetDeviceContext()->CSSetShaderResources(1, 1, AG_GET_RENDERER().m_Cascade.m_CascadeShadowMap[2].GetRessourceViewPtr());
            Armageddon::Interface::GetDeviceContext()->CSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());

            Armageddon::Renderer::g_CameraCBuffer.SetDynamicData(&Armageddon::Renderer::g_CameraBufferData);
            Armageddon::Renderer::g_CameraCBuffer.BindCS();

            Armageddon::Renderer::g_VolumetricBuffer.BindCS();

            Armageddon::Renderer::g_LightCBuffer.BindCS();


            Armageddon::Renderer::g_TransformCBuffer.BindCS();
            
            {
                GPUprofiler GPUvol("VolumetricInjectGPU", Armageddon::Interface::GetDeviceContext().Get(), Armageddon::Interface::GetDevice().Get());
                Armageddon::Interface::GetDeviceContext()->Dispatch(160 / 8, 90 / 2, 128 / 8);
            }

            //Clear ressources
            Armageddon::Interface::GetDeviceContext()->CSSetShaderResources(0, 1, SRVNULL);
            Armageddon::Interface::GetDeviceContext()->CSSetShaderResources(1, 1, SRVNULL);
            Armageddon::Interface::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, UAVNULL, NULL);
            Armageddon::Interface::GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
            // Volumetric Scattering (Raymarching)

            Armageddon::Interface::GetDeviceContext()->CSSetShader(AG_GET_RENDERER().m_VolumetricFog.m_VolumetricScatteringShader.GetShader(), NULL, 0);
            Armageddon::Interface::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, &AG_GET_RENDERER().m_VolumetricFog.m_VolumetricScatteringAccum.m_UAV, NULL);
            Armageddon::Interface::GetDeviceContext()->CSSetShaderResources(0, 1, AG_GET_RENDERER().m_VolumetricFog.m_VolumetricIntegration.GetRessourceViewPtr());

            Armageddon::Renderer::g_CameraCBuffer.SetDynamicData(&Armageddon::Renderer::g_CameraBufferData);
            Armageddon::Renderer::g_CameraCBuffer.BindCS();

            {
                GPUprofiler GPUvol("VolumetricScatterGPU", Armageddon::Interface::GetDeviceContext().Get(), Armageddon::Interface::GetDevice().Get());
                Armageddon::Interface::GetDeviceContext()->Dispatch(160 / 16, 90, 1);
            }
            //Clear ressources
            Armageddon::Interface::GetDeviceContext()->CSSetShaderResources(0, 1, SRVNULL);
            Armageddon::Interface::GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, UAVNULL, NULL);
            Armageddon::Interface::GetDeviceContext()->CSSetShader(nullptr, nullptr, 0);
        }
    }
    #pragma endregion

    #pragma region FrameBufferPass
    {
        Profiler FramePass("FrameBufferPass");
        //Set the viewport to the current window size & bind the RTV
        Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &Armageddon::Renderer::ViewPort);

        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &AG_GET_RENDERER().m_FrameBuffer.RenderTargetView, AG_GET_RENDERER().m_FrameBuffer.DephtStencilView.Get());
        Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(AG_GET_RENDERER().m_FrameBuffer.RenderTargetView, BackGroundColor);
        Armageddon::Interface::GetDeviceContext()->ClearDepthStencilView(AG_GET_RENDERER().m_FrameBuffer.DephtStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 1);

        Armageddon::Renderer::g_WorldCBuffer.BindVS();
        Armageddon::Renderer::g_WorldCBuffer.BindPS();

        Armageddon::Renderer::g_VolumetricBuffer.BindPS();

        auto group = g_registry.group<MeshComponent>(entt::get<TagComponent>);
        for (auto& enityID : group)
        {
            auto entity = GetEntityByID(enityID);
            auto& component = entity.GetComponent<MeshComponent>();

            Armageddon::Interface::GetDeviceContext()->PSSetSamplers(1, 1, Armageddon::Interface::GetClampSampler().GetAddressOf());
            Armageddon::Interface::GetDeviceContext()->PSSetSamplers(2, 1, Armageddon::Interface::GetCompareSampler().GetAddressOf());

            //Bind constantBuffer
            Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(component.m_mesh.GetTransform());
            Armageddon::Renderer::g_TransformCBuffer.BindPS();
            Armageddon::Renderer::g_TransformCBuffer.BindVS();

            Armageddon::Renderer::g_LightCBuffer.SetDynamicData(&Armageddon::Renderer::g_LightBufferData);
            Armageddon::Renderer::g_LightCBuffer.BindPS();
            Armageddon::Renderer::g_LightCBuffer.BindVS();

            Armageddon::Renderer::g_CameraCBuffer.SetDynamicData(&Armageddon::Renderer::g_CameraBufferData);
            Armageddon::Renderer::g_CameraCBuffer.BindPS();
            Armageddon::Renderer::g_CameraCBuffer.BindVS();

            //TODO CHECK IF SKYBOX EXISTS AND OTHER SHIT
            Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(50, 1, AG_GET_RENDERER().m_Envmap.m_convEnvMapTexture.GetRessourceViewPtr());
            Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(51, 1, AG_GET_RENDERER().m_Envmap.m_PreFilteredEnvMap.GetRessourceViewPtr());
            Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(52, 1, AG_GET_RENDERER().m_Envmap.m_BRFLutTexture.GetRessourceViewPtr());
            Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(23, 1, AG_GET_RENDERER().m_VolumetricFog.m_VolumetricScatteringAccum.GetRessourceViewPtr());

            ID3D11ShaderResourceView* temp_ressouceView[3] = { nullptr,nullptr,nullptr };//TODO NUMCASCADE
            for (size_t j = 0; j < 3; j++)
            {
                temp_ressouceView[j] = AG_GET_RENDERER().m_Cascade.m_CascadeShadowMap[j].GetRessourceView();
            }

            Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(54, 3, temp_ressouceView);


            for (auto& submesh : component.m_mesh.v_SubMeshes)
            {


                submesh.BindVertexBuffer();
                submesh.BindIndexBuffer();
                component.m_mesh.BindMaterial(submesh.m_materialIndex);
                submesh.DrawIndexed();
            }
        }

        AG_GET_RENDERER().m_Envmap.Render(&AG_GET_RENDERER().m_camera);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(23, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(50, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(51, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(52, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(53, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(54, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(55, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(56, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RTVNULL, nullptr);
    }
    #pragma endregion

    
    // TODO BETTER HANDLING OF TEXTURE DIMENSION
    // TODO BLOOM IN COMPUTE SHADER
    #pragma region BloomPass
    {
        //This is the bloom theshold
        D3D11_VIEWPORT viewport;
        ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = 1920;
        viewport.Height = 1080;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);

        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &AG_GET_RENDERER().m_bloom.m_BloomTexture.RenderTargetView,nullptr);
        Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(AG_GET_RENDERER().m_bloom.m_BloomTexture.RenderTargetView, BackGroundColor);

        Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, AG_GET_RENDERER().m_FrameBuffer.GetRessourceViewPtr());
        
        for (auto& submesh : AG_GET_RENDERER().m_quad.v_SubMeshes)
        {
            Armageddon::Interface::GetDeviceContext()->IASetInputLayout(AG_GET_RENDERER().m_bloom.vx.GetInputLayout());
            Armageddon::Interface::GetDeviceContext()->PSSetShader(AG_GET_RENDERER().m_bloom.px.GetShader(), nullptr, 0);
            Armageddon::Interface::GetDeviceContext()->VSSetShader(AG_GET_RENDERER().m_bloom.vx.GetShader(), nullptr, 0);

            submesh.BindVertexBuffer();
            submesh.BindIndexBuffer();
            submesh.DrawIndexed();
        }

        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RTVNULL, nullptr);

        AG_GET_RENDERER().m_bloom.Render();

    }
    #pragma endregion

    #pragma region CompositePass
    {
        Profiler CompositePass("CompositePass");

        Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &Armageddon::Renderer::ViewPort);
        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &AG_GET_RENDERER().m_Composite.RenderTargetView, nullptr);
        Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(AG_GET_RENDERER().m_Composite.RenderTargetView,BackGroundColor);

        Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetClampSampler().GetAddressOf());
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, AG_GET_RENDERER().m_FrameBuffer.GetRessourceViewPtr());
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, AG_GET_RENDERER().m_bloom.m_bloomUpSample[0].GetRessourceViewPtr());

        AG_GET_RENDERER().m_bloom.BloomPropety.TexelSize = DirectX::XMFLOAT2(float(1.0F / AG_GET_RENDERER().m_Composite.GetImageX()), float(1.0F / AG_GET_RENDERER().m_Composite.GetImageY()));
        AG_GET_RENDERER().m_bloom.m_BloomConstant.SetDynamicData(&AG_GET_RENDERER().m_bloom.BloomPropety);
        AG_GET_RENDERER().m_bloom.m_BloomConstant.BindPS();
        
        for (auto& submesh : AG_GET_RENDERER().m_quad.v_SubMeshes)
        {
            Armageddon::Interface::GetDeviceContext()->IASetInputLayout(AG_GET_RENDERER().FinalPassVertex.GetInputLayout());
            Armageddon::Interface::GetDeviceContext()->PSSetShader(AG_GET_RENDERER().FinalPassPixel.GetShader(), nullptr, 0);
            Armageddon::Interface::GetDeviceContext()->VSSetShader(AG_GET_RENDERER().FinalPassVertex.GetShader(), nullptr, 0);
            submesh.BindVertexBuffer();
            submesh.BindIndexBuffer();
            submesh.DrawIndexed();
        }

        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(1, 1, SRVNULL);
        Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RTVNULL, nullptr);


    }
    Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &Armageddon::Renderer::ViewPort);

    #pragma endregion
}

void Scene::OnRuntimeStart()
{
    //When the scene load (during the runtime)
    //to be call on the onInit

    auto view = g_registry.view<ScriptComponent>();
    for (auto& ent : view)
    {
        auto& entity = GetEntityByID(ent);
        auto& component = entity.GetComponent<ScriptComponent>();
        if (ScriptEngine::ClassExist(component.ClassName))
        {
            ScriptEngine::OnCreateEntity(entity, component.ClassName);
        }
    }
}


void Scene::ClearScene()
{
    //HERE CAUSE CRASH BECAUSE OF DEFAULT SHADER THAT ARE NOT IN THE SHADER MAP
    for (auto& e : EntityMap)
        e.second.Delete();
    EntityMap.clear();
    AssetManager::m_AssetMap.clear();
  //  AssetManager::m_MaterialMap.clear();
    AssetManager::v_material.clear(); // TODO: I used ? 
    AssetManager::v_texture.clear();
  //  AssetManager::v_PixelShaders.clear();
  //  AssetManager::v_VertexShaders.clear();
    g_registry.clear<TransformComponent, MeshComponent, DirectionalLightComponent, PointLightComponent, ScriptComponent, AnimationComponent>();
    Scenetime = 0.0f;
    SceneDeltaTime = 0.0f;
}

void Scene::LoadScene(const std::filesystem::path& path)
{
    ClearScene();
    Serializer m_serialzer = Serializer(this);
    m_serialzer.DeserializeScene(path);
    //TODO SCRIPT ENGINE AND OTHER SHIT 
}

void Scene::DuplicateEntity(Entity RefEntity)
{
    auto entity = CreateEntity();
    if(RefEntity.HasComponent<TagComponent>())
    {
        auto& component = RefEntity.GetComponent<TagComponent>();
        entity.AddComponent<TagComponent>(component.Tag);
    }
    if (RefEntity.HasComponent<MeshComponent>())
    {
        auto& component = RefEntity.GetComponent<MeshComponent>();
        entity.AddComponent<MeshComponent>(component.m_mesh, component.m_path);

    }
    if (RefEntity.HasComponent<PointLightComponent>())
    {
        auto& component = RefEntity.GetComponent<PointLightComponent>();

    }
    if (RefEntity.HasComponent<TransformComponent>())
    {
        auto& component = RefEntity.GetComponent<TransformComponent>();
        entity.AddComponent<TransformComponent>(component);

    }
}

void Scene::DeleteEntity(Entity& entity)
{


}

void Scene::InitScene()
{
    ScriptEngine::Init(this);
}

