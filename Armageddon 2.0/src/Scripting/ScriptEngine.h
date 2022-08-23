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



/*
typedef struct
{
	MonoDomain* AppDomain = nullptr;
	MonoAssembly* m_Assembly = nullptr;
	MonoImage* m_Image = nullptr;
}ScriptClass_t;*/



class ScriptClass //store all Class
{
public:
	ScriptClass() = default;
	ScriptClass(const std::string& c_namespace, const std::string& c_name, MonoImage* image);	
	void InvokeMethod(MonoObject* Instance, MonoMethod* Method, void** params = nullptr);
	MonoMethod* GetMethod(const std::string methodName, int paramC);
	MonoObject* Instanciate();
private:
	std::string class_namespace;
	std::string class_name;
	MonoClass* m_MonoClass = nullptr;
};


class ScriptInstance // of the current entity
{
public:
	ScriptInstance(ScriptClass* scriptClass, Entity* entt);
	ScriptInstance() = default;
	ScriptInstance(const ScriptInstance& instance) {};
	void InvokeOnCreate();
	void InvokeOnUpdate();
private:
	ScriptClass* m_scriptClass;
	MonoObject* m_Instance;
	MonoMethod* m_OnCreate;
	MonoMethod* m_OnUpdate;
};

class ScriptEngine
{
public:
	ScriptEngine();
	~ScriptEngine();
	static DECL void Init(Scene* scene);
	static DECL void Shutdown();

	static MonoAssembly* LoadAssembly(const std::filesystem::path& AssemblyPath);
	static void LoadClasses(MonoAssembly* assembly);
	static DECL bool ClassExist(const std::string& classname);
	static DECL void OnCreateEntity(Entity& entity, const std::string& className);
	static DECL void OnUpdateEntity(Entity& entity);
	static DECL MonoObject* InstanciateClass(MonoClass* monoclass);
	//static std::unordered_map <uint64_t, ScriptInstance> EntityClass; //Keep track of entity and ScriptInstance
	static std::unordered_map <std::string, ScriptClass> EntityClass; //Keep track of entity and ScriptInstance
	static std::unordered_map <uint64_t, ScriptInstance> EntityInstances; //Keep track of entity and ScriptInstance

	static MonoDomain* Domain;
	static MonoMethod* EntityConstructor;
	static Scene* ActiveScene;

};