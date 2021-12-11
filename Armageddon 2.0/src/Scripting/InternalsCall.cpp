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
			mono_add_internal_call("Armageddon.TransformComponent::SetTranslation", &InternalsCall::Internal_Translation_Set);
			//        private static extern bool TestIII(ulong EntityID, out int aaa);

			mono_add_internal_call("Armageddon.TransformComponent::GetRotation", &InternalsCall::Internal_Rotation_Get);
			mono_add_internal_call("Armageddon.TransformComponent::SetRotation", &InternalsCall::Internal_Rotation_Set);
			mono_add_internal_call("Armageddon.TransformComponent::GetScale", &InternalsCall::Internal_Scale_Get);
			mono_add_internal_call("Armageddon.TransformComponent::SetScale", &InternalsCall::Internal_Scale_Set);


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

void InternalsCall::Internal_Translation_Get(uint64_t ID, float* TranslationX, float* TranslationY, float* TranslationZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	*TranslationX = component.Translation.x;
	*TranslationY = component.Translation.y;
	*TranslationZ = component.Translation.z;

}

void InternalsCall::Internal_Translation_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	component.Translation.x = *valueX;
	component.Translation.z = *valueY;
	component.Translation.y = *valueZ;

}

void InternalsCall::Internal_Rotation_Get(uint64_t ID, float* RotX, float* RotY, float* RotZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	*RotX = component.Rotation.x;
	*RotY = component.Rotation.y;
	*RotZ = component.Rotation.z;
}

void InternalsCall::Internal_Rotation_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	component.Rotation.x = *valueX;
	component.Rotation.z = *valueY;
	component.Rotation.y = *valueZ;
}

void InternalsCall::Internal_Scale_Get(uint64_t ID, float* ScaleX, float* ScaleY, float* ScaleZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	*ScaleX = component.Scale.x;
	*ScaleY = component.Scale.y;
	*ScaleZ = component.Scale.z;
}

void InternalsCall::Internal_Scale_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ)
{
	Entity& ent = ScriptEngine::GetScene()->EntityMap.at(ID);
	auto& component = ent.GetComponent<TransformComponent>();
	component.Scale.x = *valueX;
	component.Scale.z = *valueY;
	component.Scale.y = *valueZ;
}



