#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>	
#include "Vertex.h"
#include "../Log.h"
template<class T>
class VertexBuffer
{
public:
	ID3D11Buffer* Get()const
	{
		return buffer.Get();
	}

	ID3D11Buffer* const* GetAddressOf()const
	{
		return buffer.GetAddressOf();
	}

	UINT BufferSize() const
	{
		return this->bufferSize;
	}

	const UINT* Stride() const
	{
		return &stride;
	}

	HRESULT Initialize(ID3D11Device* device, ID3D11DeviceContext* device_context, T* data, UINT numElements)
	{

		this->bufferSize = numElements;
		this->stride = sizeof(Vertex);

		D3D11_BUFFER_DESC vertex_buffer_desc;

		ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

		vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		vertex_buffer_desc.ByteWidth = sizeof(Vertex) * numElements;
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.CPUAccessFlags = 0;
		vertex_buffer_desc.MiscFlags = 0;


		D3D11_SUBRESOURCE_DATA VertexBufferData;
		ZeroMemory(&VertexBufferData, sizeof(VertexBufferData));
		VertexBufferData.pSysMem = data;
		buffer.Reset();
		HRESULT hr = device->CreateBuffer(&vertex_buffer_desc, &VertexBufferData, buffer.GetAddressOf());

		if (FAILED(hr))
		{

			Armageddon::Log::GetLogger()->error("ERROR WHEN INIT THE VERTEX BUFFFER  : {0}", hr);
			hr = device->GetDeviceRemovedReason();
			Armageddon::Log::GetLogger()->error("reson : {0}", hr);

			return hr;
		}
		Armageddon::Log::GetLogger()->error("SUCESS : {0}", hr);
		return hr;
	};




	HRESULT InitializeDynamic(ID3D11Device* device, ID3D11DeviceContext* device_context, T* data, UINT numElements)
	{

		this->bufferSize = numElements;
		this->stride = sizeof(T);

		D3D11_BUFFER_DESC vertex_buffer_desc = { 0 };

		ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

		vertex_buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		vertex_buffer_desc.ByteWidth = sizeof(T) * numElements;
		vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertex_buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertex_buffer_desc.MiscFlags = 0;
		vertex_buffer_desc.StructureByteStride = 0;




		HRESULT hr = device->CreateBuffer(&vertex_buffer_desc, nullptr, buffer.GetAddressOf());
		UINT offset = 0;
		device_context->IASetVertexBuffers(0, 1, buffer.GetAddressOf(), &stride, &offset);
		return hr;
	};


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT stride = 0;
	UINT bufferSize = 0;

};