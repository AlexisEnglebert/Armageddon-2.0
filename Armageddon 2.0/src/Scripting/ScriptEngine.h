#pragma once
#include "../Log.h"
#include "../Macros.h"
#include "../Scene/Entity.h"
#include "../Scene/Scene.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <fstream>
#include <filesystem>
class ScriptEngine
{
public:
	ScriptEngine();
	~ScriptEngine();
	static void TestPrintFunction(MonoString* string);
	static DECL void CreateScriptFile(std::string ModuleName);
	static DECL void StartScript(const std::string& ModuleName);	
	static DECL void Init(Scene* scene);
	static DECL Scene* GetScene() { return ActiveScene; };
	static MonoDomain* GetDomain() { return Domain; };
private:
	static MonoDomain* Domain;
	MonoAssembly* m_assembly;
	MonoImage* m_image;
	static Scene* ActiveScene;
};