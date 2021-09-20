#pragma once
#include <wrl/client.h>
#include <d3d11.h>

template<class t>
class IndexBuffer
{
public:

	ID3D11Buffer* Get()
	{
		return buffer.Get();
	}
	ID3D11Buffer* const* GetAdressOf() const
	{
		return buffer.GetAddressOf();
	}
	UINT GetSize()
	{
		return this->buffer_size;
	}
	HRESULT Init(ID3D11Device* device, DWORD* data, UINT n_indices)
	{

		D3D11_SUBRESOURCE_DATA Indice_buffer_data;

		this->buffer_size = n_indices;
		D3D11_BUFFER_DESC Indice_buffer_desc;
		ZeroMemory(&Indice_buffer_desc, sizeof(Indice_buffer_desc));

		Indice_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		Indice_buffer_desc.ByteWidth = sizeof(DWORD) * n_indices;
		Indice_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		Indice_buffer_desc.CPUAccessFlags = 0;
		Indice_buffer_desc.MiscFlags = 0;



		ZeroMemory(&Indice_buffer_data, sizeof(Indice_buffer_data));
		Indice_buffer_data.pSysMem = data;

		HRESULT hr = device->CreateBuffer(&Indice_buffer_desc, &Indice_buffer_data, buffer.GetAddressOf());

		return hr;
	};

private:

	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT buffer_size;
};