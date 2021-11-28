#include "InternalsCall.h"
MonoImage* InternalsCall::InternalsImage = nullptr;

std::unordered_map<MonoType*, std::function<bool(Entity&)>> MonoComponentFunc;

void InternalsCall::LoadComponents(std::string Component)
{
	std::string data = "Armageddon."+ Component;
	
	MonoType* Atype = mono_reflection_type_from_name(data.data(), InternalsImage);
	MonoComponentFunc[Atype] = [](Entity& entity) {return entity.HasComponent<TagComponent>(); };
}

void InternalsCall::LoadInternals(MonoDomain* domain)
{
	MonoAssembly* m_assembly = mono_domain_assembly_open(domain, "C:/Users/engle/OneDrive/Documents/Armageddon/Armageddon-2.0/bin/Armageddon-Script.dll");
	if (m_assembly)
	{
		InternalsImage = mono_assembly_get_image(m_assembly);
		LoadComponents("TagComponent");
		LoadComponents("TransformComponent");
		LoadComponents("ScriptComponent");
		LoadComponents("MeshComponent");

		if (InternalsImage)
		{
			//Logs.cs
			mono_add_internal_call("Armageddon.Log::info",&InternalsCall::Internal_Log_info);
			mono_add_internal_call("Armageddon.Log::error", &InternalsCall::Internal_Log_error);
			//Input.cs
			mono_add_internal_call("Armageddon.Input::IsKeyPressed", &InternalsCall::IsKeyPressed);
			//Entity.cs
			mono_add_internal_call("Armageddon.Entity::HasComponent_Internal", &InternalsCall::Internal_HasComponent_Internal);

			mono_add_internal_call("Armageddon.TagComponent::GetTag", &InternalsCall::GetTag);
			mono_add_internal_call("Armageddon.TagComponent::SetTag", nullptr);

			mono_add_internal_call("Armageddon.TransformComponent::GetTranslation", &InternalsCall::Internal_Translation_Get);

			mono_add_internal_call("Armageddon.TransformComponent::SetTranslation", nullptr);
			mono_add_internal_call("Armageddon.TransformComponent::GetRotation", nullptr);
			mono_add_internal_call("Armageddon.TransformComponent::SetRotation", nullptr);
			mono_add_internal_call("Armageddon.TransformComponent::GetScale", nullptr);
			mono_add_internal_call("Armageddon.TransformComponent::SetScale", nullptr);


		}
	}
}

void InternalsCall::Internal_Log_info(MonoString* message)
{
	Armageddon::Log::GetLogger()->info(mono_string_to_utf8(message));

}

void InternalsCall::Internal_Log_error(MonoString* message)
{
	Armageddon::Log::GetLogger()->error(mono_string_to_utf8(message));

}

bool InternalsCall::IsKeyPressed(uint8_t Keycode)
{
	return false;
}

bool InternalsCall::Internal_HasComponent_Internal(uint64_t EntityID, void* type)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(EntityID);
	MonoType* monoType = mono_reflection_type_get_type((MonoReflectionType*)type);
	/**har component[] = "Armageddon.TagComponent";
	MonoType* Atype = mono_reflection_type_from_name(component, InternalsImage);*/
	bool Hascomponent = MonoComponentFunc[monoType](ent);
	return Hascomponent;
}

MonoString* InternalsCall::GetTag(uint64_t EntityID)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(EntityID);
	auto& component = ent.GetComponent<TagComponent>();

	return mono_string_new(ScriptEngine::GetDomain(),component.Tag.c_str());
}

void InternalsCall::Internal_Translation_Get(uint64_t ID, DirectX::XMFLOAT3* Translation)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
//	Translation = component.Translation;

}


