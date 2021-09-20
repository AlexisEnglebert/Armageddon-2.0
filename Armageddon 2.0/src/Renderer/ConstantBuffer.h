#pragma once
#include "../Macros.h"
#include "Interface.h"
#include "../Log.h"
#include <d3d11.h>

template <class T>
class ConstantBuffer
{
public:
	ConstantBuffer<T>() {};
	HRESULT Create(D3D11_USAGE Usage, UINT slot)
	{
		D3D11_BUFFER_DESC cbDesc;
		cbDesc.Usage = Usage;
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbDesc.MiscFlags = 0;
		cbDesc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
		cbDesc.StructureByteStride = 0;
		HRESULT hr = Armageddon::Interface::GetDevice()->CreateBuffer(&cbDesc, 0, mConstantBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("FAILED CREATING CONSTANT BUFFER");
		}

		this->m_slot = slot;

		return hr;
	};
	 void SetDynamicData(T* data) 
	{
		D3D11_MAPPED_SUBRESOURCE mRessource;
		HRESULT hr = Armageddon::Interface::GetDeviceContext()->Map(mConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mRessource);
		if (FAILED(hr))
		{
			Armageddon::Log::GetLogger()->error("ERROR MAPPING CONSTANT BUFFER  : [{0}]", hr);
		}
		CopyMemory(mRessource.pData, data, sizeof(T)); // copie les data qu'on envoie
		Armageddon::Interface::GetDeviceContext()->Unmap(mConstantBuffer.Get(), 0);
	};
	 void BindVS() {Armageddon::Interface::GetDeviceContext()->VSSetConstantBuffers(this->m_slot, 1, this->mConstantBuffer.GetAddressOf());};
	 void BindPS() {Armageddon::Interface::GetDeviceContext()->PSSetConstantBuffers(this->m_slot, 1, this->mConstantBuffer.GetAddressOf());};
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> mConstantBuffer;
	T m_data;
	UINT m_slot;
};


