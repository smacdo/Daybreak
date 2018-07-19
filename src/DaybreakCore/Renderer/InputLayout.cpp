#include "stdafx.h"
#include "InputLayout.h"
#include "Graphics/InputLayoutDescription.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
InputLayout::InputLayout(_In_ std::shared_ptr<const InputLayoutDescription> description)
    : m_layoutDescription(description)
{
    CHECK_NOT_NULL(description);
}

//---------------------------------------------------------------------------------------------------------------------
InputLayout::~InputLayout() = default;

//---------------------------------------------------------------------------------------------------------------------
std::shared_ptr<const InputLayoutDescription> InputLayout::layoutDescription() const noexcept
{
    return m_layoutDescription;
}

//---------------------------------------------------------------------------------------------------------------------
const InputLayoutDescription& InputLayout::layoutDescriptionRef() const noexcept
{
    return *m_layoutDescription.get();
}
