#include "stdafx.h"
#include "RendererEffect.h"
#include "RenderContext.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::startPass(_In_ IRenderContext& context) const
{
    onStartPass(context);
}

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::finishPass(_In_ IRenderContext& context) const
{
    onFinishPass(context);
}

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::startRenderObject(
    _In_ IRenderContext& context,
    _In_ unsigned int offset,
    _In_ unsigned int count) const
{
    onStartRenderObject(context, offset, count);
    context.drawTriangles(offset, count);
}

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::onStartPass(_In_ IRenderContext& context) const
{
}

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::onFinishPass(_In_ IRenderContext& context) const
{
}

//---------------------------------------------------------------------------------------------------------------------
void IRendererEffect::onStartRenderObject(
    _In_ IRenderContext& context,
    _In_ unsigned int offset,
    _In_ unsigned int count) const
{
}
