/*!
  \file smooth_conductor_surface.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "smooth_conductor_surface.hpp"
// Qt
#include <QJsonObject>
#include <QString>
// Zisc
#include "zisc/aligned_memory_pool.hpp"
#include "zisc/error.hpp"
// Nanairo
#include "fresnel.hpp"
#include "surface_model.hpp"
#include "NanairoCommon/keyword.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Color/spectral_distribution.hpp"
#include "NanairoCore/Material/Bxdf/specular_brdf.hpp"
#include "NanairoCore/Material/TextureModel/texture_model.hpp"
#include "NanairoCore/Utility/scene_value.hpp"
#include "NanairoCore/Utility/value.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
SmoothConductorSurface::SmoothConductorSurface(const QJsonObject& settings) noexcept
{
  initialize(settings);
}

/*!
  \details
  No detailed.
  */
auto SmoothConductorSurface::makeBxdf(
    const Point2& /* texture_coordinate */,
    const bool /* is_reverse_face */,
    const WavelengthSamples& wavelengths,
    Sampler& /* sampler */,
    MemoryPool& memory_pool) const noexcept -> ShaderPointer
{
  const auto fresnel_0deg = sample(fresnel_0deg_, wavelengths);

  using Brdf = SpecularBrdf;
  auto brdf = memory_pool.allocate<Brdf>(fresnel_0deg);
  return ShaderPointer{brdf};
}

/*!
  \details
  No detailed.
  */
SurfaceType SmoothConductorSurface::type() const noexcept
{
  return SurfaceType::SmoothConductor;
}

/*!
  \details
  No detailed.
  */
void SmoothConductorSurface::initialize(const QJsonObject& settings) noexcept
{
  const auto outer_refractive_index_settings =
      SceneValue::toString(settings, keyword::outerRefractiveIndex);
  const auto n1 = SpectralDistribution::makeSpectra(outer_refractive_index_settings);
  ZISC_ASSERT(!n1.hasValue(0.0), "The n1 contains zero value.");
  ZISC_ASSERT(!n1.hasNegative(), "The n1 contains negative value.");

  const auto inner_refractive_index_settings =
      SceneValue::toString(settings, keyword::innerRefractiveIndex);
  const auto n2 = SpectralDistribution::makeSpectra(inner_refractive_index_settings);
  ZISC_ASSERT(!n2.hasNegative(), "The n2 contains negative value.");

  const auto inner_extinction_settings =
      SceneValue::toString(settings, keyword::innerExtinction);
  const auto k2 = SpectralDistribution::makeSpectra(inner_extinction_settings);
  ZISC_ASSERT(!k2.hasNegative(), "The k2 contains negative value.");

  const auto eta = n2 / n1;
  const auto eta_k = k2 / n1;
  fresnel_0deg_ = Fresnel::evalFresnel0(eta, eta_k);
}

} // namespace nanairo
