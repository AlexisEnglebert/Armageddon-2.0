#include "SubMesh.h"

SubMesh::SubMesh(std::vector<Vertex>& Vertices, std::vector<DWORD>& indices)
{
	HRESULT hr = m_VertexBuffer.Initialize(
		Armageddon::Interface::GetDevice().Get(),
		Armageddon::Interface::GetDeviceContext().Get(),
		Vertices.data(), Vertices.size());

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("Error when creating VertexBuffer [{0}]", hr);
	}

	hr = m_IndexBuffer.Init(
		Armageddon::Interface::GetDevice().Get(),
		indices.data(), indices.size());

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("Error when creating IndexBuffer [{0}]", hr);
	}
}

SubMesh::SubMesh(std::vector<Vertex>& Vertices, std::vector<DWORD>& indices, int32_t index) : m_materialIndex(index)
{
	HRESULT hr = m_VertexBuffer.Initialize(
		Armageddon::Interface::GetDevice().Get(),
		Armageddon::Interface::GetDeviceContext().Get(),
		Vertices.data(), Vertices.size());

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("Error when creating VertexBuffer [{0}]", hr);
	}

	hr = m_IndexBuffer.Init(
		Armageddon::Interface::GetDevice().Get(),
		indices.data(), indices.size());

	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("Error when creating IndexBuffer [{0}]", hr);
	}
}

void SubMesh::BindVertexBuffer()
{

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		Armageddon::Interface::GetDeviceContext()->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	

}

void SubMesh::BindIndexBuffer()
{

	Armageddon::Interface::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);


}



void SubMesh::DrawIndexed()
{
	Armageddon::Interface::GetDeviceContext()->DrawIndexed(m_IndexBuffer.GetSize(), 0, 0);

}

