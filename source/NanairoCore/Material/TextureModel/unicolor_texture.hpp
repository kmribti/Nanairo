/*!
  \file unicolor_texture.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNICOLOR_TEXTURE_HPP
#define NANAIRO_UNICOLOR_TEXTURE_HPP

// Standard C++ library
#include <cstddef>
#include <memory>
// Nanairo
#include "texture_model.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"

// Forward declaration
class QJsonObject;

namespace nanairo {

// Forward declaration
class SampledSpectra;
class System;
class WavelengthSamples;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class UnicolorTexture : public TextureModel
{
 public:
  //! Create a unicolor texture
  UnicolorTexture(const System& system, const QJsonObject& settings) noexcept;


  //! Evaluate a float value at the coordinate
  Float floatValue(const Point2& coordinate) const noexcept override;

  //! Evaluate a emissive spectra at the texture coordinate
  SampledSpectra emissiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate a reflective spectra at the texture coordinate
  SampledSpectra reflectiveValue(
      const Point2& coordinate,
      const WavelengthSamples& wavelengths) const noexcept override;

  //! Evaluate the reflective value by the wavelength at the texture coordinate
  Float reflectiveValue(const Point2& coordinate,
                        const uint16 wavelength) const noexcept override;

  //! Return the unicolor texture type
  TextureType type() const noexcept override;

 private:
  //! Initialize
  void initialize(const System& system, const QJsonObject& settings) noexcept;


  std::unique_ptr<SpectralDistribution> emissive_value_;
  std::unique_ptr<SpectralDistribution> reflective_value_;
  Float float_value_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_UNICOLOR_TEXTURE_HPP
