#pragma once

namespace SpelSylt
{
	class CRenderQueue;
}

namespace tree
{
	class ISystem
	{
	public:

		virtual void Update(float aDT) {}
		virtual void Render(SpelSylt::CRenderQueue& aRenderQueue) {};

	private:
	};
}