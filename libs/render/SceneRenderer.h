#pragma once

#include "irender.h"
#include "iscenegraph.h"

namespace render
{

/**
 * Simple implementation of a scene::Walker passing visited nodes
 * on to the attached RenderableCollector. Suitable for use in the
 * scene::Graph::foreach*() methods.
 */
class SceneRenderer :
	public scene::Graph::Walker
{
private:
	// The collector which is sorting our renderables
	RenderableCollector& _collector;

	// The view we're using for culling
	const VolumeTest& _volume;

public:
	SceneRenderer(RenderableCollector& collector, const VolumeTest& volume) :
		_collector(collector),
		_volume(volume)
	{}

	// Render function, instructs the Renderable object to submit its geometry
	// to the contained RenderableCollector.
	void render(const Renderable& renderable) const
	{
	    switch(_collector.getStyle())
	    {
	    case RenderableCollector::eFullMaterials:
			renderable.renderSolid(_collector, _volume);
			break;
	    case RenderableCollector::eWireframeOnly:
			renderable.renderWireframe(_collector, _volume);
			break;
	    }
	}

	RenderableCallback getRenderableCallback()
	{
		return boost::bind(&RenderHighlighted::render, this, _1);
	}

	// scene::Graph::Walker implementation, tells each node to submit its OpenGLRenderables
	bool visit(const scene::INodePtr& node)
	{
		_collector.PushState();

		// greebo: Fix for primitive nodes: as we don't traverse the scenegraph nodes
		// top-down anymore, we need to set the shader state of our parent entity ourselves.
		// Otherwise we're in for NULL-states when rendering worldspawn brushes.
		scene::INodePtr parent = node->getParent();

		Entity* entity = Node_getEntity(parent);

		if (entity != NULL)
		{
			_collector.SetState(node->getRenderEntity()->getWireShader(), RenderableCollector::eWireframeOnly);
		}

		node->viewChanged();
		
		if (node->isHighlighted() || (parent != NULL && parent->isHighlighted()))
		{
			if (GlobalSelectionSystem().Mode() != SelectionSystem::eComponent)
			{
				_collector.Highlight(RenderableCollector::eFace);
			}
			else
			{
				node->renderComponents(_collector, _volume);
			}

			_collector.Highlight(RenderableCollector::ePrimitive);
		}

		render(*node);

		_collector.PopState();

		return true;
	}
};

} // namespace render
