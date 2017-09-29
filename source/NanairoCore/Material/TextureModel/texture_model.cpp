/*!
  \file texture_model.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "texture_model.hpp"
// Standard C++ library
#include <memory>
// Zisc
#include "zisc/algorithm.hpp"
#include "zisc/error.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "checkerboard_texture.hpp"
#include "image_texture.hpp"
#include "unicolor_texture.hpp"
#include "value_texture.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Setting/setting_node_base.hpp"
#include "NanairoCore/Setting/texture_setting_node.hpp"

namespace nanairo {

/*!
  */
TextureModel::~TextureModel() noexcept
{
}

/*!
  \details
  No detailed.
  */
std::unique_ptr<TextureModel> TextureModel::makeTexture(
    const System& system,
    const SettingNodeBase* settings) noexcept
{
  const auto texture_settings = castNode<TextureSettingNode>(settings);

  std::unique_ptr<TextureModel> texture;
  switch (texture_settings->textureType()) {
    case TextureType::kValue: {
      texture = std::make_unique<ValueTexture>(system, settings);
      break;
    }
    case TextureType::kUnicolor: {
      texture = std::make_unique<UnicolorTexture>(system, settings);
      break;
    }
    case TextureType::kCheckerboard: {
      texture = std::make_unique<CheckerboardTexture>(system, settings);
      break;
    }
    case TextureType::kImage: {
      texture = std::make_unique<ImageTexture>(system, settings);
      break;
    }
    default: {
      zisc::raiseError("TextureError: Unsupported type is specified.");
      break;
    }
  }
  return texture;
}

} // namespace nanairo
