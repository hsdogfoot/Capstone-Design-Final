#pragma once

class Base abstract
{
public:
	Base();
	virtual ~Base() = default;

	inline uint32_t AddRefCount();
	inline uint32_t	GetRefCount() const;

	uint32_t Release();

protected:
	virtual void destroy() = 0;

protected:
	uint32_t mRefCount;
};

inline uint32_t Base::AddRefCount()
{
	return ++mRefCount;
}

inline uint32_t Base::GetRefCount() const
{
	return mRefCount;
}