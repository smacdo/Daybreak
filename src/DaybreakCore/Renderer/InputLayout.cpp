#include "stdafx.h"
#include "InputLayout.h"
#include "Graphics/InputLayoutDescription.h"
#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
InputLayout::InputLayout(const InputLayoutDescription& description)
    : m_layoutDescription(description)
{
}

//---------------------------------------------------------------------------------------------------------------------
InputLayout::~InputLayout() = default;

//---------------------------------------------------------------------------------------------------------------------
const InputLayoutDescription& InputLayout::layoutDescription() const noexcept
{
    return m_layoutDescription;
}
