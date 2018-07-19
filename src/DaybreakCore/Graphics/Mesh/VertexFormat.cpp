#include "stdafx.h"
#include "VertexFormat.h"
#include "Graphics/InputLayout.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
std::shared_ptr<InputLayout> vertex_ptn_t::inputLayout = std::make_shared<InputLayout>(
    std::vector<InputAttribute> {
        { InputAttribute::SemanticName::Position, 0, InputAttribute::StorageType::Float, 3 },
        { InputAttribute::SemanticName::Texture, 0, InputAttribute::StorageType::Float, 2 },
        { InputAttribute::SemanticName::Normal, 0, InputAttribute::StorageType::Float, 3 }
});
