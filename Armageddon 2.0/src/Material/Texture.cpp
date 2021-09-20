#include "Texture.h"
#include "../Renderer/Renderer.h"
	
Texture::Texture(std::filesystem::path path)
{
	this->Create(path);
}

void Texture::Create(const std::filesystem::path& path)
{
	if(TextureRessource != nullptr)
		TextureRessource->Release();
	if(TextureRessourceView != nullptr)
		TextureRessourceView->Release();
	TexturePath = path;
	HRESULT hr = DirectX::CreateWICTextureFromFile(Armageddon::Interface::GetDevice().Get(), path.c_str(), TextureRessource.GetAddressOf(), TextureRessourceView.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR Can not load : {0}", hr);
	}
}

void Texture::CreateCubeMap(const std::filesystem::path& path)
{
	/*
	Premièrement on doit load l'image HDR dans une Texture 2D
	Pour après faire un rendu offscreen de cette image sur chaque face d'un
	cube et récuperer le tout sous une "Texture cube" 
	*/

	/*-----------------------------------------------------------------------------*/
	//								Load the HDR File							  //
	/*----------------------------------------------------------------------------*/
	DirectX::TexMetadata metadata;
	DirectX::ScratchImage image;
	DirectX::LoadFromHDRFile(path.c_str(), &metadata, image);
	Armageddon::Log::GetLogger()->info("tex width : {0}  Height: {1} Format {2}  Dimension {3} Misc flags {4} Misc flags 2 {5} Mips level {6}", metadata.width, metadata.height, metadata.format, metadata.dimension
		, metadata.miscFlags, metadata.miscFlags2, metadata.mipLevels);
	/*-----------------------------------------------------------------------------*/
	//								Create the shader							  //
	/*----------------------------------------------------------------------------*/

	//Pour le moment c'est comme ça mais après faudrais que ça soit automatique

	Armageddon::VertexShaders VertexShader;
	Armageddon::PixelShaders  PixelShader;
	VertexShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\EquirectangularToCubemapVertex.cso");
	PixelShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\EquirectangularToCubemapPixel.cso");


	/*LA BAS ET LE HAUT SONT MAL GENERES ILS SONT 90° ROTATIONE !!!!!!!*/


	DirectX::XMMATRIX captureProjection = DirectX::XMMatrixPerspectiveFovLH((90.0f / 360.0f) * DirectX::XM_2PI, 1.0f, 0.1f, 10000.0f);;

	DirectX::XMFLOAT3 Eyepos1 = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f); // rien
	DirectX::XMFLOAT3 Eyepos2 = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f); // 180 deuxiemme
	DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); //90 debut					-- ICI CA CLOCHE   DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f); //-90 debut				-- ICI CA CLOCHE  DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos5 = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f); //90 deuxiemme
	DirectX::XMFLOAT3 Eyepos6 = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f); //-90 deuxiemme

	DirectX::XMMATRIX CameraRotationMatrix1 = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
	DirectX::XMMATRIX CameraRotationMatrix2 = DirectX::XMMatrixRotationRollPitchYaw(0, (180 / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix3 = DirectX::XMMatrixRotationRollPitchYaw((90.0f / 360.0f) * DirectX::XM_2PI, 0, (-90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix4 = DirectX::XMMatrixRotationRollPitchYaw((-90.0f / 360.0f) * DirectX::XM_2PI, 0,(90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix5 = DirectX::XMMatrixRotationRollPitchYaw(0, (90.0f / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix6 = DirectX::XMMatrixRotationRollPitchYaw(0, (-90.0f / 360.0f) * DirectX::XM_2PI, 0);

	DirectX::XMVECTOR vecPos1 = DirectX::XMLoadFloat3(&Eyepos1);
	DirectX::XMVECTOR vecPos2 = DirectX::XMLoadFloat3(&Eyepos2);
	DirectX::XMVECTOR vecPos3 = DirectX::XMLoadFloat3(&Eyepos3);
	DirectX::XMVECTOR vecPos4 = DirectX::XMLoadFloat3(&Eyepos4);
	DirectX::XMVECTOR vecPos5 = DirectX::XMLoadFloat3(&Eyepos5);
	DirectX::XMVECTOR vecPos6 = DirectX::XMLoadFloat3(&Eyepos6);

	DirectX::XMVECTOR CamTarget1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix1);
	CamTarget1 += vecPos1;
	DirectX::XMVECTOR CamTarget2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix2);
	CamTarget2 += vecPos2;
	DirectX::XMVECTOR CamTarget3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix3);
	CamTarget3 += vecPos3;
	DirectX::XMVECTOR CamTarget4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix4);
	CamTarget4 += vecPos4;
	DirectX::XMVECTOR CamTarget5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix5);
	CamTarget5 += vecPos5;
	DirectX::XMVECTOR CamTarget6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix6);
	CamTarget6 += vecPos6;

	DirectX::XMVECTOR UpDir1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix1);
	DirectX::XMVECTOR UpDir2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix2);
	DirectX::XMVECTOR UpDir4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix4);
	DirectX::XMVECTOR UpDir3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix3);
	DirectX::XMVECTOR UpDir5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix5);
	DirectX::XMVECTOR UpDir6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix6);

	DirectX::XMMATRIX captureViews[] =
	{
		 DirectX::XMMatrixLookAtLH(vecPos1, CamTarget1,UpDir1),
		 DirectX::XMMatrixLookAtLH(vecPos2, CamTarget2,UpDir2),
		 DirectX::XMMatrixLookAtLH(vecPos3, CamTarget3,UpDir3),
		 DirectX::XMMatrixLookAtLH(vecPos4, CamTarget4,UpDir4),
		 DirectX::XMMatrixLookAtLH(vecPos5, CamTarget5,UpDir5),
		 DirectX::XMMatrixLookAtLH(vecPos6, CamTarget6,UpDir6),

	};



	/*-----------------------------------------------------------------------------*/
	//								Create the 2D Texture					 	  //
	/*----------------------------------------------------------------------------*/

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = metadata.width;
	desc.Height = metadata.height;
	desc.MipLevels = metadata.mipLevels;
	desc.ArraySize = metadata.arraySize;
	desc.Format = metadata.format;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = metadata.miscFlags;

	ID3D11Texture2D* pTexture = NULL;


	D3D11_SUBRESOURCE_DATA Subressource;
	Subressource.pSysMem = image.GetPixels();
	Subressource.SysMemPitch = image.GetImages()->rowPitch;
	Subressource.SysMemSlicePitch = image.GetImages()->slicePitch;

	HRESULT hr = Armageddon::Interface::GetDevice()->CreateTexture2D(&desc, &Subressource, &pTexture);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR CREATING TEXTURE 2D {0}", hr);
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC ViewDesc;
	ViewDesc.Format = metadata.format;
	ViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	ViewDesc.TextureCube.MipLevels = 1;
	ViewDesc.TextureCube.MostDetailedMip = 0;

	ID3D11ShaderResourceView* ressourceview;
	hr = Armageddon::Interface::GetDevice()->CreateShaderResourceView(pTexture, &ViewDesc, TextureRessourceView.GetAddressOf());
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CHREATING SHADER RESOURCE VIEW [{0}]", hr);
	}
	TextureRessource = pTexture;
	
	
	
	/*-----------------------------------------------------------------------------*/
	//							Create the CubeMap texture					 	  //
	/*----------------------------------------------------------------------------*/
	
	D3D11_TEXTURE2D_DESC Texdesc;
	Texdesc.Width = 512;
	Texdesc.Height = 512;
	Texdesc.MipLevels = 0;
	Texdesc.ArraySize = 6;
	Texdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Texdesc.SampleDesc.Count = 1;
	Texdesc.SampleDesc.Quality = 0;
	Texdesc.Usage = D3D11_USAGE_DEFAULT;
	Texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Texdesc.CPUAccessFlags = 0;
	Texdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;;

	ID3D11Texture2D* pTextureCubeMap = NULL;

	 hr = Armageddon::Interface::GetDevice()->CreateTexture2D(&Texdesc, nullptr, &pTextureCubeMap);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR CREATING TEXTURE 2D FOR HDR {0}", hr);
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC RessourceViewDesc;
	RessourceViewDesc.Format = Texdesc.Format;
	RessourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RessourceViewDesc.TextureCube.MipLevels = 1;
	RessourceViewDesc.TextureCube.MostDetailedMip = 0;

	ID3D11ShaderResourceView* Cubemapressourceview;
	hr = Armageddon::Interface::GetDevice()->CreateShaderResourceView(pTextureCubeMap, &RessourceViewDesc, &Cubemapressourceview);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CHREATING SHADER RESOURCE VIEW [{0}]", hr);
	}




	/*On vas rendre notre texture sur les 6 faces d'un cube*/
	ID3D11RenderTargetView* RenderTargetView[6];
	for (UINT i = 0; i < 6; i++)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = Texdesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipSlice = 0;
		desc.Texture2DArray.FirstArraySlice = i;
		desc.Texture2DArray.ArraySize = 1;
		Armageddon::Interface::GetDevice()->CreateRenderTargetView(pTextureCubeMap, &desc, &RenderTargetView[i]);
	}


	
	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, TextureRessourceView.GetAddressOf());
	Armageddon::Interface::GetDeviceContext()->VSSetShader(VertexShader.GetShader(), NULL, 0);
	Armageddon::Interface::GetDeviceContext()->PSSetShader(PixelShader.GetShader(), NULL, 0);

	/*On doit set le viewport à la taille de la texture qu'on veut (512X512)*/

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 512;
	viewport.Height = 512;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);
	Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (UINT i = 0; i < 6; i++)
	{
		TransFormBuffer Buffer;
		float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };
		Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(RenderTargetView[i], BackGroundColor);
		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView[i], nullptr);
		Buffer.ViewMat = captureViews[i];
		Buffer.ProjectionMat = captureProjection;
		Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(&Buffer);
		Armageddon::Renderer::g_TransformCBuffer.BindVS();

		Armageddon::Interface::GetDeviceContext()->Draw(14, 0);

	}

	Armageddon::Interface::GetDeviceContext()->GenerateMips(Cubemapressourceview);
	ID3D11RenderTargetView* RDTVNULL = nullptr;

	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);

	for (auto& RenderTarget : RenderTargetView)
	{
		RenderTarget->Release();
	}

	TextureRessourceView = Cubemapressourceview; //on set le CubeMap au ressource view
	pTexture->Release();
}

void Texture::CreateIrradiancedMap(ID3D11ShaderResourceView** envmapRessource)
{
	/*-----------------------------------------------------------------------------*/
	//								Create the shader							  //
	/*----------------------------------------------------------------------------*/

//Pour le moment c'est comme ça mais après faudrais que ça soit automatique

	Armageddon::VertexShaders VertexShader;
	Armageddon::PixelShaders  PixelShader;
	VertexShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\CubemapConvolutionVertex.cso");
	PixelShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\CubemapConvolutionPixel.cso");




	DirectX::XMMATRIX captureProjection = DirectX::XMMatrixPerspectiveFovLH((90.0f / 360.0f) * DirectX::XM_2PI, 1.0f, 0.1f, 10000.0f);;

	DirectX::XMFLOAT3 Eyepos1 = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f); // rien
	DirectX::XMFLOAT3 Eyepos2 = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f); // 180 deuxiemme
	DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); //90 debut					-- ICI CA CLOCHE   DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f); //-90 debut				-- ICI CA CLOCHE  DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos5 = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f); //90 deuxiemme
	DirectX::XMFLOAT3 Eyepos6 = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f); //-90 deuxiemme

	DirectX::XMMATRIX CameraRotationMatrix1 = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
	DirectX::XMMATRIX CameraRotationMatrix2 = DirectX::XMMatrixRotationRollPitchYaw(0, (180 / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix3 = DirectX::XMMatrixRotationRollPitchYaw((90.0f / 360.0f) * DirectX::XM_2PI, 0, (-90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix4 = DirectX::XMMatrixRotationRollPitchYaw((-90.0f / 360.0f) * DirectX::XM_2PI, 0, (90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix5 = DirectX::XMMatrixRotationRollPitchYaw(0, (90.0f / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix6 = DirectX::XMMatrixRotationRollPitchYaw(0, (-90.0f / 360.0f) * DirectX::XM_2PI, 0);

	DirectX::XMVECTOR vecPos1 = DirectX::XMLoadFloat3(&Eyepos1);
	DirectX::XMVECTOR vecPos2 = DirectX::XMLoadFloat3(&Eyepos2);
	DirectX::XMVECTOR vecPos3 = DirectX::XMLoadFloat3(&Eyepos3);
	DirectX::XMVECTOR vecPos4 = DirectX::XMLoadFloat3(&Eyepos4);
	DirectX::XMVECTOR vecPos5 = DirectX::XMLoadFloat3(&Eyepos5);
	DirectX::XMVECTOR vecPos6 = DirectX::XMLoadFloat3(&Eyepos6);

	DirectX::XMVECTOR CamTarget1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix1);
	CamTarget1 += vecPos1;
	DirectX::XMVECTOR CamTarget2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix2);
	CamTarget2 += vecPos2;
	DirectX::XMVECTOR CamTarget3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix3);
	CamTarget3 += vecPos3;
	DirectX::XMVECTOR CamTarget4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix4);
	CamTarget4 += vecPos4;
	DirectX::XMVECTOR CamTarget5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix5);
	CamTarget5 += vecPos5;
	DirectX::XMVECTOR CamTarget6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix6);
	CamTarget6 += vecPos6;

	DirectX::XMVECTOR UpDir1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix1);
	DirectX::XMVECTOR UpDir2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix2);
	DirectX::XMVECTOR UpDir4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix4);
	DirectX::XMVECTOR UpDir3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix3);
	DirectX::XMVECTOR UpDir5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix5);
	DirectX::XMVECTOR UpDir6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix6);

	DirectX::XMMATRIX captureViews[] =
	{
		 DirectX::XMMatrixLookAtLH(vecPos1, CamTarget1,UpDir1),
		 DirectX::XMMatrixLookAtLH(vecPos2, CamTarget2,UpDir2),
		 DirectX::XMMatrixLookAtLH(vecPos3, CamTarget3,UpDir3),
		 DirectX::XMMatrixLookAtLH(vecPos4, CamTarget4,UpDir4),
		 DirectX::XMMatrixLookAtLH(vecPos5, CamTarget5,UpDir5),
		 DirectX::XMMatrixLookAtLH(vecPos6, CamTarget6,UpDir6),

	};


	/*-----------------------------------------------------------------------------*/
	//							Create the CubeMap texture					 	  //
	/*----------------------------------------------------------------------------*/

	D3D11_TEXTURE2D_DESC Texdesc;
	Texdesc.Width = 512;
	Texdesc.Height = 512;
	Texdesc.MipLevels = 0;
	Texdesc.ArraySize = 6;
	Texdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Texdesc.SampleDesc.Count = 1;
	Texdesc.SampleDesc.Quality = 0;
	Texdesc.Usage = D3D11_USAGE_DEFAULT;
	Texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Texdesc.CPUAccessFlags = 0;
	Texdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;;

	ID3D11Texture2D* pTextureCubeMap = NULL;

	HRESULT hr = Armageddon::Interface::GetDevice()->CreateTexture2D(&Texdesc, nullptr, &pTextureCubeMap);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR CREATING TEXTURE 2D FOR HDR {0}", hr);
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC RessourceViewDesc;
	RessourceViewDesc.Format = Texdesc.Format;
	RessourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RessourceViewDesc.TextureCube.MipLevels = 1;
	RessourceViewDesc.TextureCube.MostDetailedMip = 0;

	ID3D11ShaderResourceView* Cubemapressourceview;
	hr = Armageddon::Interface::GetDevice()->CreateShaderResourceView(pTextureCubeMap, &RessourceViewDesc, &Cubemapressourceview);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CHREATING SHADER RESOURCE VIEW [{0}]", hr);
	}

	/*On vas rendre notre texture sur les 6 faces d'un cube*/
	ID3D11RenderTargetView* RenderTargetView[6];
	for (UINT i = 0; i < 6; i++)
	{
		D3D11_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = Texdesc.Format;
		desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipSlice = 0;
		desc.Texture2DArray.FirstArraySlice = i;
		desc.Texture2DArray.ArraySize = 1;
		Armageddon::Interface::GetDevice()->CreateRenderTargetView(pTextureCubeMap, &desc, &RenderTargetView[i]);
	}

	Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetSamplerState().GetAddressOf());
	Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, envmapRessource);
	Armageddon::Interface::GetDeviceContext()->VSSetShader(VertexShader.GetShader(), NULL, 0);
	Armageddon::Interface::GetDeviceContext()->PSSetShader(PixelShader.GetShader(), NULL, 0);

	/*On doit set le viewport à la taille de la texture qu'on veut (512X512)*/

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 512;
	viewport.Height = 512;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);
	Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	for (UINT i = 0; i < 6; i++)
	{
		TransFormBuffer Buffer;
		float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };
		Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(RenderTargetView[i], BackGroundColor);
		Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView[i], nullptr);
		Buffer.ViewMat = captureViews[i];
		Buffer.ProjectionMat = captureProjection;
		Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(&Buffer);
		Armageddon::Renderer::g_TransformCBuffer.BindVS();

		Armageddon::Interface::GetDeviceContext()->Draw(14, 0);

	}

	Armageddon::Interface::GetDeviceContext()->GenerateMips(Cubemapressourceview);
	ID3D11RenderTargetView* RDTVNULL = nullptr;

	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);

	for (auto& RenderTarget : RenderTargetView)
	{
		RenderTarget->Release();
	}

	TextureRessourceView = Cubemapressourceview; //on set le CubeMap au ressource view
}

void Texture::CreatePreFilteredMap(ID3D11ShaderResourceView** envmapRessource)
{

	Armageddon::VertexShaders VertexShader;
	Armageddon::PixelShaders  PixelShader;
	VertexShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\PreFilteHDRVertex.cso");
	PixelShader.Init(Armageddon::Interface::GetDevice(), L"..\\bin\\Debug-x64\\Armageddon 2.0\\PreFilteHDRPixel.cso");

	DirectX::XMMATRIX captureProjection = DirectX::XMMatrixPerspectiveFovLH((90.0f / 360.0f) * DirectX::XM_2PI, 1.0f, 0.1f, 10000.0f);;

	DirectX::XMFLOAT3 Eyepos1 = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f); // rien
	DirectX::XMFLOAT3 Eyepos2 = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f); // 180 deuxiemme
	DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f); //90 debut					-- ICI CA CLOCHE   DirectX::XMFLOAT3 Eyepos3 = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f); //-90 debut				-- ICI CA CLOCHE  DirectX::XMFLOAT3 Eyepos4 = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);
	DirectX::XMFLOAT3 Eyepos5 = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f); //90 deuxiemme
	DirectX::XMFLOAT3 Eyepos6 = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f); //-90 deuxiemme

	DirectX::XMMATRIX CameraRotationMatrix1 = DirectX::XMMatrixRotationRollPitchYaw(0, 0, 0);
	DirectX::XMMATRIX CameraRotationMatrix2 = DirectX::XMMatrixRotationRollPitchYaw(0, (180 / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix3 = DirectX::XMMatrixRotationRollPitchYaw((90.0f / 360.0f) * DirectX::XM_2PI, 0, (-90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix4 = DirectX::XMMatrixRotationRollPitchYaw((-90.0f / 360.0f) * DirectX::XM_2PI, 0, (90.0f / 360.0f) * DirectX::XM_2PI);
	DirectX::XMMATRIX CameraRotationMatrix5 = DirectX::XMMatrixRotationRollPitchYaw(0, (90.0f / 360.0f) * DirectX::XM_2PI, 0);
	DirectX::XMMATRIX CameraRotationMatrix6 = DirectX::XMMatrixRotationRollPitchYaw(0, (-90.0f / 360.0f) * DirectX::XM_2PI, 0);

	DirectX::XMVECTOR vecPos1 = DirectX::XMLoadFloat3(&Eyepos1);
	DirectX::XMVECTOR vecPos2 = DirectX::XMLoadFloat3(&Eyepos2);
	DirectX::XMVECTOR vecPos3 = DirectX::XMLoadFloat3(&Eyepos3);
	DirectX::XMVECTOR vecPos4 = DirectX::XMLoadFloat3(&Eyepos4);
	DirectX::XMVECTOR vecPos5 = DirectX::XMLoadFloat3(&Eyepos5);
	DirectX::XMVECTOR vecPos6 = DirectX::XMLoadFloat3(&Eyepos6);

	DirectX::XMVECTOR CamTarget1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix1);
	CamTarget1 += vecPos1;
	DirectX::XMVECTOR CamTarget2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix2);
	CamTarget2 += vecPos2;
	DirectX::XMVECTOR CamTarget3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix3);
	CamTarget3 += vecPos3;
	DirectX::XMVECTOR CamTarget4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix4);
	CamTarget4 += vecPos4;
	DirectX::XMVECTOR CamTarget5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix5);
	CamTarget5 += vecPos5;
	DirectX::XMVECTOR CamTarget6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), CameraRotationMatrix6);
	CamTarget6 += vecPos6;

	DirectX::XMVECTOR UpDir1 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix1);
	DirectX::XMVECTOR UpDir2 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix2);
	DirectX::XMVECTOR UpDir4 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix4);
	DirectX::XMVECTOR UpDir3 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix3);
	DirectX::XMVECTOR UpDir5 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix5);
	DirectX::XMVECTOR UpDir6 = DirectX::XMVector3TransformCoord(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), CameraRotationMatrix6);

	DirectX::XMMATRIX captureViews[] =
	{
		 DirectX::XMMatrixLookAtLH(vecPos1, CamTarget1,UpDir1),
		 DirectX::XMMatrixLookAtLH(vecPos2, CamTarget2,UpDir2),
		 DirectX::XMMatrixLookAtLH(vecPos3, CamTarget3,UpDir3),
		 DirectX::XMMatrixLookAtLH(vecPos4, CamTarget4,UpDir4),
		 DirectX::XMMatrixLookAtLH(vecPos5, CamTarget5,UpDir5),
		 DirectX::XMMatrixLookAtLH(vecPos6, CamTarget6,UpDir6),

	};


	D3D11_TEXTURE2D_DESC Texdesc;
	Texdesc.Width = 128;
	Texdesc.Height = 128;
	Texdesc.MipLevels = 0;
	Texdesc.ArraySize = 6;
	Texdesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	Texdesc.SampleDesc.Count = 1;
	Texdesc.SampleDesc.Quality = 0;
	Texdesc.Usage = D3D11_USAGE_DEFAULT;
	Texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	Texdesc.CPUAccessFlags = 0;
	Texdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;;

	ID3D11Texture2D* pTextureCubeMap = NULL;

	HRESULT hr = Armageddon::Interface::GetDevice()->CreateTexture2D(&Texdesc, nullptr, &pTextureCubeMap);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR CREATING TEXTURE 2D FOR HDR {0}", hr);
	}


	D3D11_SHADER_RESOURCE_VIEW_DESC RessourceViewDesc;
	RessourceViewDesc.Format = Texdesc.Format;
	RessourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	RessourceViewDesc.TextureCube.MipLevels = -1;
	RessourceViewDesc.TextureCube.MostDetailedMip = 0;

	ID3D11ShaderResourceView* Cubemapressourceview;
	hr = Armageddon::Interface::GetDevice()->CreateShaderResourceView(pTextureCubeMap, &RessourceViewDesc, &Cubemapressourceview);
	if (FAILED(hr))
	{
		Armageddon::Log::GetLogger()->error("ERROR WHEN CHREATING SHADER RESOURCE VIEW [{0}]", hr);
	}

	const int MipLevels = 5;
	ID3D11RenderTargetView* RenderTargetView[(MipLevels * 6)];
	for (int mip = 0; mip < MipLevels; ++mip)
	{
		for (int i = 0; i < 6; ++i)
		{
			D3D11_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = Texdesc.Format;
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			desc.Texture2DArray.MipSlice = mip;
			desc.Texture2DArray.FirstArraySlice = i;
			desc.Texture2DArray.ArraySize = 1;
			Armageddon::Interface::GetDevice()->CreateRenderTargetView(pTextureCubeMap, &desc, &RenderTargetView[mip*6+i]);
		}
	}


	for (int i = 0; i < MipLevels; i++)
	{
		int mipWidth  = 128 * std::pow(0.5, i);
		int mipHeight = 128 * std::pow(0.5, i);


		D3D11_VIEWPORT viewport;
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = mipWidth;
		viewport.Height = mipHeight;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		Armageddon::Interface::GetDeviceContext()->RSSetViewports(1, &viewport);
		
		Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		Armageddon::Interface::GetDeviceContext()->PSSetSamplers(0, 1, Armageddon::Interface::GetTrilinearSampler().GetAddressOf());
		Armageddon::Interface::GetDeviceContext()->PSSetShaderResources(0, 1, envmapRessource);
		Armageddon::Interface::GetDeviceContext()->VSSetShader(VertexShader.GetShader(), NULL, 0);
		Armageddon::Interface::GetDeviceContext()->PSSetShader(PixelShader.GetShader(), NULL, 0);


		float roughness = (float)i / (float)(MipLevels - 1);

		for (UINT j = 0; j < 6; j++)
		{
			TransFormBuffer Buffer;
			RoughnessBuffer RougnessB;
			float BackGroundColor[] = { 0.1f,0.1f,0.1f,1.0f };
			Armageddon::Interface::GetDeviceContext()->ClearRenderTargetView(RenderTargetView[i * 6 + j], BackGroundColor);
			Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RenderTargetView[i * 6 + j], nullptr);
			Buffer.ViewMat = captureViews[j];
			Buffer.ProjectionMat = captureProjection;
			Armageddon::Renderer::g_TransformCBuffer.SetDynamicData(&Buffer);
			Armageddon::Renderer::g_TransformCBuffer.BindVS();

			RougnessB.Rougness = roughness;
			Armageddon::Renderer::g_RoughnessCBuffer.SetDynamicData(&RougnessB);
			Armageddon::Renderer::g_RoughnessCBuffer.BindPS();

			Armageddon::Interface::GetDeviceContext()->Draw(14, 0);

		}
	}
	Armageddon::Interface::GetDeviceContext()->GenerateMips(Cubemapressourceview);

	Armageddon::Interface::GetDeviceContext()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11RenderTargetView* RDTVNULL = nullptr;
	Armageddon::Interface::GetDeviceContext()->OMSetRenderTargets(1, &RDTVNULL, nullptr);

	for (auto& RenderTarget : RenderTargetView)
	{
		RenderTarget->Release();
	}
	TextureRessourceView = Cubemapressourceview; //on set le CubeMap au ressource view

}

