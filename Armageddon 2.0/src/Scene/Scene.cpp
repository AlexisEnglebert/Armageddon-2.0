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

void Scene::DeleteEntity(Entity& entity)
{
    Armageddon::Log::GetLogger()->info("DELETEING ENTITY");

    for (UINT i = 0; i < v_Entity.size(); i++)
    {
        if (v_Entity[i].GetHandle() == entity.GetHandle())
        {
            g_registry.destroy(v_Entity[i].GetHandle());
            Armageddon::Log::GetLogger()->info("ENTITY FOUND");
            Armageddon::Log::GetLogger()->info(""+i);
            auto a = (v_Entity.begin() + i);
            v_Entity.erase(v_Entity.begin()+i);

        }
    }
    Armageddon::Log::GetLogger()->info("DELETEING ALL COMPONENTS");


}

