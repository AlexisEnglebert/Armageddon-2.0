#include "Timer.h"
std::map<std::string, float> ProfilerData::m_profilerData;

ProfilerData::ProfilerData()
{
}

void ProfilerData::Clear()
{
	ProfilerData::m_profilerData.clear();

}
//		

GPUprofiler::GPUprofiler(std::string name,ID3D11DeviceContext* ctx, ID3D11Device* device)
{
	pContext = ctx;
	this->name = name;
	D3D11_QUERY_DESC queryDescdisjoint;
	queryDescdisjoint.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
	queryDescdisjoint.MiscFlags = 0;

	D3D11_QUERY_DESC queryDescstart;
	queryDescstart.Query = D3D11_QUERY_TIMESTAMP;
	queryDescstart.MiscFlags = 0;

	D3D11_QUERY_DESC queryDescend;
	queryDescend.Query = D3D11_QUERY_TIMESTAMP;
	queryDescend.MiscFlags = 0;

	HRESULT hr = device->CreateQuery(&queryDescdisjoint, &disjointQ);
	if (FAILED(hr)) Armageddon::Log::GetLogger()->error("FAILED CREATING QUERY {0}", hr);
	hr = device->CreateQuery(&queryDescstart, &startQ);
	if (FAILED(hr)) Armageddon::Log::GetLogger()->error("FAILED CREATING QUERY {0}", hr);
	hr = device->CreateQuery(&queryDescend, &endQ);
	if (FAILED(hr)) Armageddon::Log::GetLogger()->error("FAILED CREATING QUERY {0}", hr);

	pContext->Begin(disjointQ);
	pContext->End(startQ);


}

GPUprofiler::~GPUprofiler()
{
	pContext->End(endQ);
	pContext->End(disjointQ);
	UINT64 Start,End = 0;

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT DisjointData;

	while(pContext->GetData(disjointQ,&DisjointData,sizeof(DisjointData),0) != S_OK);
	while(pContext->GetData(startQ,&Start,sizeof(Start),0) != S_OK);
	while(pContext->GetData(endQ,&End,sizeof(End),0) != S_OK);
	float Time = 0.0f;
	if (!DisjointData.Disjoint)
	{
		UINT64 Delta = End - Start;
		float Frequency = static_cast<float>(DisjointData.Frequency);
		Time = (Delta / Frequency) * 1000.0f;

	}
	ProfilerData::m_profilerData.insert(std::make_pair(name, Time));

	/*delete(disjointQ);
	delete(startQ);
	delete(endQ);
	delete(pContext);*/

}
