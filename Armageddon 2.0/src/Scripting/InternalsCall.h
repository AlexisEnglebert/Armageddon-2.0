#pragma once
#include "../Log.h"
#include "../Macros.h"
#include "ScriptEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
class InternalsCall
{
public:
	static void LoadComponents(std::string Component);
	static void LoadInternals(MonoDomain* domain);
	static void Internal_Log_info(MonoString* message);
	static void Internal_Log_error(MonoString* message);
	static bool IsKeyPressed(uint8_t Keycode);
	static bool Internal_HasComponent_Internal(uint64_t EntityID, void* type);
	static MonoString* GetTag(uint64_t EntityID);
	static void Internal_Translation_Get(uint64_t ID, DirectX::XMFLOAT3* Translation);
private:
	static MonoImage* InternalsImage;
};