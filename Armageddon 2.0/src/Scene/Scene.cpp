#include "Scene.h"
#include "Entity.h"
#include "Serializer.h"

Entity Scene::CreateEntity()
{
    Entity ent = { g_registry.create(), this };
    v_Entity.push_back(ent); 
    return ent;
}

Entity& Scene::GetEntityByID(entt::entity ID)
{
    for (auto& ent : v_Entity)
    {
        if (ID == ent.GetHandle())
            return ent;
    }
}

void Scene::ClearScene()
{
    for(auto& ent : v_Entity)
     g_registry.destroy(ent.GetHandle());

    v_Entity.clear();

}

void Scene::LoadScene(const std::filesystem::path& path)
{
    //Serializer m_serialzer = Serializer(this);
    ClearScene();
   // m_serialzer.DeserializeScene(path);
}

void Scene::DuplicateEntity(Entity& RefEntity)
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
    if (RefEntity.HasComponent<LightComponent>())
    {
        auto& component = RefEntity.GetComponent<LightComponent>();

    }
    if (RefEntity.HasComponent<TransformComponent>())
    {
        auto& component = RefEntity.GetComponent<TransformComponent>();
        entity.AddComponent<TransformComponent>(component);

    }
}

