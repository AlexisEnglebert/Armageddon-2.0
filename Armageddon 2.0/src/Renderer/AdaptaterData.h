#pragma once
#if __linux__
//Pass pour l'instant
#else
#include <dxgi.h>
#include <d3d11.h>
#include <wrl\client.h>
#endif
#include <vector>
#include "../Macros.h"
#include "../Log.h"

#pragma comment(lib, "dxgi.lib")



	class DECL Adpatater
	{
	public: 
		Adpatater(IDXGIAdapter* ptrAdaptater);

		IDXGIAdapter* ptrAdaptater = nullptr;
		DXGI_ADAPTER_DESC Description;

	};

	class DECL AdaptaterReader
	{
	public: 
		static std::vector<Adpatater> GetAdaptaterData(); // get all adaptaters

	private:
		static std::vector<Adpatater> adaptaters;		// list de tout les adaptaters

	};

