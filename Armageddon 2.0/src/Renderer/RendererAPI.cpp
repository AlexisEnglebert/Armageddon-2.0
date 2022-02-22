#include "RendererAPI.h"
Armageddon::RendererAPI::API Armageddon::RendererAPI::m_CurrentAPI = Armageddon::RendererAPI::API::DirectX;

void Armageddon::RendererAPI::SetRenderer(Armageddon::RendererAPI::API api)
{
	Armageddon::RendererAPI::m_CurrentAPI = api;
}