#pragma once
#include "../Log.h"
#include "../Macros.h"
namespace Armageddon
{
	class DECL RendererAPI
	{
	public:
		enum class API
		{
			DirectX,
			Vulkan
		};

		static RendererAPI::API m_CurrentAPI;
		static void SetRenderer(RendererAPI::API api);
		static bool is_api(RendererAPI::API api);
	};
}