#include "ScriptEngine.h"
#include "InternalsCall.h"
#include "../Scene/Entity.h"
#include "../Scene/Scene.h"

MonoDomain* ScriptEngine::Domain = nullptr;
Scene* ScriptEngine::ActiveScene = nullptr;
MonoMethod* ScriptEngine::EntityConstructor = nullptr;
std::unordered_map <uint64_t, ScriptInstance> ScriptEngine::EntityInstances; //Keep track of entity and ScriptInstance
std::unordered_map <std::string, ScriptClass> ScriptEngine::EntityClass; //Keep track of entity and ScriptInstance

ScriptEngine::ScriptEngine()
{

}

ScriptEngine::~ScriptEngine()
{
}

/*
void ScriptEngine::CreateScriptFile(std::string ModuleName)
{
	std::filesystem::path Path = "Assets/Scrips/"+ModuleName+".cs";
	std::string filePattern = "using System.Collections;\nusing System.Collections.Generic;\n\npublic class "+ModuleName+" : MonoBehaviour\n{\n	void Start()\n	{\n	}\n\n	void Update()\n	{\n	}\n}";
	std::ofstream fileOut(ModuleName+".cs");
	fileOut << filePattern.c_str();
}*/


void DECL ScriptEngine::Init(Scene* scene)
{
	//TODO MOVE IN RIGHT FOLDER
	mono_set_dirs("Assembly/mono/lib", "Assembly/mono/etc");
	mono_jit_init("Armageddon 2.0");
	char domain[] = "Armageddon";

	Domain = mono_domain_create_appdomain(domain, nullptr);
	
	ActiveScene = scene; //TODO VIRER CETTE MERDE

	//Load the core library
	InternalsCall::LoadInternals(Domain,scene);

	//Load the user library
	MonoAssembly* m_userAssembly = LoadAssembly("Assembly/ScriptingTest.dll");
	if (!m_userAssembly)
		Armageddon::Log::GetLogger()->error("Failed to load User Script");

	LoadClasses(m_userAssembly);

	MonoClass* monoclass = mono_class_from_name(InternalsCall::InternalsImage, "Armageddon", "Entity");
	if(!monoclass)
		Armageddon::Log::GetLogger()->error("Failed to load Entity Class");

	ScriptEngine::EntityConstructor = mono_class_get_method_from_name(monoclass, ".ctor", 1);
	if (ScriptEngine::EntityConstructor == nullptr)
	{
		Armageddon::Log::GetLogger()->error("ERROR WHILE GETTING ENTITY CONSTRUCTOR");
	}

}

DECL void ScriptEngine::Shutdown()
{
	Domain = nullptr;
	EntityConstructor = nullptr;
	ActiveScene = nullptr;
	EntityClass.clear();
	EntityInstances.clear();
}

MonoAssembly* ScriptEngine::LoadAssembly(const std::filesystem::path& AssemblyPath)
{
	 
	MonoAssembly* m_assembly = mono_domain_assembly_open(Domain, AssemblyPath.string().c_str());
	return m_assembly;
}

void ScriptEngine::LoadClasses(MonoAssembly* assembly)
{
	EntityClass.clear();
	MonoImage* image = mono_assembly_get_image(assembly);
	const MonoTableInfo* tableinfo = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
	int32_t rows = mono_table_info_get_rows(tableinfo);

	for (size_t i = 0; i < rows; i++)
	{
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(tableinfo, i, cols, MONO_TYPEDEF_SIZE);

		const char* c_namespace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* c_name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
		std::string name = "";
		if (strlen(c_namespace) != 0)
			name = fmt::format("{0}.{1}", c_namespace, c_name);
		else
			name = c_name;

		//We only need class not all module or object
	//	MonoClass* monoClass = mono_class_from_name(image, c_namespace, c_name);
		//bool isclass = mono_class_is_subclass_of(MonoClass,)
		Armageddon::Log::GetLogger()->info(name.c_str());
		if (name != "<Module>")
		{
			EntityClass[name] = ScriptClass(c_namespace, c_name, image);
		}

	}
}

bool ScriptEngine::ClassExist(const std::string& classname)
{
	return EntityClass.find(classname) != EntityClass.cend();
}

DECL void ScriptEngine::OnCreateEntity(Entity& entity, const std::string& className)
{
	ScriptInstance instance = ScriptInstance(&ScriptEngine::EntityClass[className],&entity);
	EntityInstances[(int64_t)entity.GetHandle()] = instance;
	instance.InvokeOnCreate();
}

DECL void ScriptEngine::OnUpdateEntity(Entity& entity)
{
	ScriptInstance* instance = &EntityInstances[(int64_t)entity.GetHandle()];
	instance->InvokeOnUpdate();
}

DECL MonoObject* ScriptEngine::InstanciateClass(MonoClass* monoclass)
{
	MonoObject* obj = mono_object_new(ScriptEngine::Domain, monoclass);
	mono_runtime_object_init(obj);
	return obj;
}

ScriptClass::ScriptClass(const std::string& c_namespace, const std::string& c_name, MonoImage* image) : class_namespace(c_namespace), class_name(c_name)
{
	m_MonoClass = mono_class_from_name(image, c_namespace.c_str(), c_name.c_str());
}

void ScriptClass::InvokeMethod(MonoObject* Instance, MonoMethod* Method, void** params)
{
	mono_runtime_invoke(Method, Instance, params, nullptr);
}

MonoMethod* ScriptClass::GetMethod(const std::string methodName, int paramC)
{
	return mono_class_get_method_from_name(m_MonoClass, methodName.c_str(), paramC);
}

MonoObject* ScriptClass::Instanciate()
{
	return ScriptEngine::InstanciateClass(m_MonoClass);
}

ScriptInstance::ScriptInstance(ScriptClass* scriptClass,Entity* entt) : m_scriptClass(scriptClass)
{
	m_Instance = scriptClass->Instanciate();
	m_OnCreate = scriptClass->GetMethod("OnInit",0);
	m_OnUpdate = scriptClass->GetMethod("OnUpdate",0);
	Armageddon::Log::GetLogger()->trace("{0}", (void*)ScriptEngine::EntityConstructor);
	int id = (int)entt->GetHandle();
	void* param = &id;
	scriptClass->InvokeMethod(m_Instance, ScriptEngine::EntityConstructor, &param);
	

	
}

void ScriptInstance::InvokeOnCreate()
{
	m_scriptClass->InvokeMethod(m_Instance, m_OnCreate);
}

void ScriptInstance::InvokeOnUpdate()
{
	m_scriptClass->InvokeMethod(m_Instance, m_OnUpdate);

}
