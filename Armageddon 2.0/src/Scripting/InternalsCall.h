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

	//private extern static void GetTranslation(ulong EntityID, out float translationX, out float translationY, out float translationZ);
	static void Internal_Translation_Get(uint64_t ID,float* TranslationX, float* TranslationY, float* TranslationZ);
	static void Internal_Translation_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ);


	static void Internal_Rotation_Get(uint64_t ID, float* RotX, float* RotY, float* RotZ);
	static void Internal_Rotation_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ);

	static void Internal_Scale_Get(uint64_t ID, float* ScaleX, float* ScaleY, float* ScaleZ);
	static void Internal_Scale_Set(uint64_t ID, float* valueX, float* valueY, float* valueZ);
private:
	static MonoImage* InternalsImage;
};