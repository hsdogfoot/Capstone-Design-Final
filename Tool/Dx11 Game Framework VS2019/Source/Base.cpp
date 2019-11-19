#include "stdafx.h"
#include "Base.h"

Base::Base()
	: mRefCount(1)
{
}

uint32_t Base::Release()
{
	if (--mRefCount == 0)
	{
		destroy();
		delete this;

		return 0;
	}

	return mRefCount;
}