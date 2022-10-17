#include "../Base.h"

long __stdcall Base::Hooks::hkDrawIndexedPrimitive(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	if (pDevice == NULL)
	{
		return Base::Data::oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
	}

	return Base::Data::oDrawIndexedPrimitive(pDevice, Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

long __stdcall Base::Hooks::hkDrawIndexedPrimitiveUp(LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	if (pDevice == NULL)
	{
		return Base::Data::oDrawIndexedPrimitiveUp(pDevice, Type, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
	}

	return Base::Data::oDrawIndexedPrimitiveUp(pDevice, Type, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}