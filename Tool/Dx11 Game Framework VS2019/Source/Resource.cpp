#include "stdafx.h"
#include "Resource.h"

Resource::Resource()
	: Base()
{
}

Resource* Resource::Clone()
{
	return this;
}

void Resource::ReadyRender() const
{
}
