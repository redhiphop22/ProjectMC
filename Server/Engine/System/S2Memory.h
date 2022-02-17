#pragma once

namespace S2Memory
{
	template<class _Ty>
	_Ty Create()
	{
		return new _Ty;
	}

}; 