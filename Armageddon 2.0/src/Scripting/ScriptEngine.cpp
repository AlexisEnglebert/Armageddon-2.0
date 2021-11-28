#include "ScriptEngine.h"
#include "InternalsCall.h"
Scene* ScriptEngine::ActiveScene = nullptr;
MonoDomain* ScriptEngine::Domain = nullptr;

ScriptEngine::ScriptEngine()
{

}

ScriptEngine::~ScriptEngine()
{
}

void ScriptEngine::TestPrintFunction(MonoString* string)
{
	Armageddon::Log::GetLogger()->info(mono_string_to_utf8(string));
}

void ScriptEngine::CreateScriptFile(std::string ModuleName)
{
	std::filesystem::path Path = "Assets/Scrips/"+ModuleName+".cs";
	std::string filePattern = "using System.Collections;\nusing System.Collections.Generic;\n\npublic class "+ModuleName+" : MonoBehaviour\n{\n	void Start()\n	{\n	}\n\n	void Update()\n	{\n	}\n}";
	std::ofstream fileOut(ModuleName+".cs");
	fileOut << filePattern.c_str();
}

void ScriptEngine::StartScript(const std::string& ModuleName)
{
	//m_assembly = mono_domain_assembly_open(Domain, "C:/Users/engle/OneDrive/Documents/Armageddon/Armageddon-2.0/bin/TestScript.dll");

}

void ScriptEngine::Init(Scene* scene)
{
	mono_set_dirs("../Armageddon 2.0/includes/mono/mono/lib", "../Armageddon 2.0/includes/mono/mono/etc");
	//mono_set_dirs(".", ".");
	mono_jit_init("Armageddon 2.0");
	char domain[] = "Armageddon";
	Domain = mono_domain_create_appdomain(domain, nullptr);
	ScriptEngine::ActiveScene = scene;
	InternalsCall::LoadInternals(Domain);

	if (Domain)
	{
		MonoAssembly* m_assembly = mono_domain_assembly_open(Domain, "C:/Users/engle/OneDrive/Documents/Armageddon/Armageddon-2.0/bin/TestScript.dll");
		if (m_assembly)
		{
			MonoImage* m_image = mono_assembly_get_image(m_assembly);
			if (m_image)
			{
				mono_add_internal_call("TestScript.Program::print(string)", &ScriptEngine::TestPrintFunction);
				MonoClass* TestClass = mono_class_from_name(m_image, "TestScript", "Program");
				if (TestClass)
				{
					MonoMethodDesc* methodDesc = mono_method_desc_new(".Program::Main()", false);
					if (methodDesc)
					{
						MonoMethod* mainMethod = mono_method_desc_search_in_class(methodDesc, TestClass);
						if (mainMethod)
						{
							MonoObject* obj = nullptr;
							MonoObject* test = mono_runtime_invoke(mainMethod, nullptr, nullptr, &obj);
							if (test)
							{
								int a = 10;
							}
						}
					}
				}
			}
		}
	}


}

