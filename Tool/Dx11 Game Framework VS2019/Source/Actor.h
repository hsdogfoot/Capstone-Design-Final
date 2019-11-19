#pragma once
#include "GameObject.h"

class Renderer;
class Transform;
class ColorEffect;
class Pattern;

class Actor : public GameObject
{
public:
	static Actor* Create(const WCHAR* prefabName);
	bool Clone(Actor** outInstance);

	void SetRenderer(Renderer* renderer);
	void SetTransform(Transform* transform);
	void SetColorEffect(ColorEffect* colorEffect);
	void SetPattern(Pattern* pattern);

	void ReadyRender() const;
	void Render() const;
	void AfterRender() const;

	inline Renderer* GetRenderer() const;
	inline Transform* GetTransform() const;
	inline ColorEffect* GetColorEffect() const;
	inline Pattern* GetPattern() const;

	virtual void PreRelease() override;

protected:
	Actor();
	explicit Actor(const WCHAR* prefabName);
	virtual ~Actor() override = default;

	virtual void destroy() override;

protected:
	Renderer* mRenderer;
	Transform* mTransform;
	ColorEffect* mColorEffect;
	Pattern* mPattern;

public:
	WCHAR* PrefabName = nullptr;
};

inline Renderer* Actor::GetRenderer() const
{
	return mRenderer;
}

inline Transform* Actor::GetTransform() const
{
	return mTransform;
}

inline ColorEffect* Actor::GetColorEffect() const
{
	return mColorEffect;
}

inline Pattern* Actor::GetPattern() const
{
	return mPattern;
}
