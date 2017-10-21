/*!
  \file ggx_conductor_brdf.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "ggx_conductor_brdf.hpp"
// Sstandard C++ library
#include <tuple>
// Zisc
#include "zisc/error.hpp"
// Nanairo
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Data/intersection_info.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Material/shader_model.hpp"
#include "NanairoCore/Material/SurfaceModel/Surface/microfacet_ggx.hpp"
#include "NanairoCore/Sampling/sampled_direction.hpp"
#include "NanairoCore/Sampling/sampled_spectra.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
GgxConductorBrdf::GgxConductorBrdf(
    const Float roughness,
    const SampledSpectra& n,
    const SampledSpectra& eta) noexcept :
        n_{n},
        eta_{eta},
        roughness_{roughness}
{
}

/*!
  \details
  No detailed.
  */
Float GgxConductorBrdf::evalPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  const Float pdf = MicrofacetGgx::evalReflectionPdf(roughness_,
                                                     *vin,
                                                     *vout,
                                                     info->normal());
  return pdf;
}

/*!
  \details
  No detailed.
  */
SampledSpectra GgxConductorBrdf::evalRadiance(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  const auto radiance = MicrofacetGgx::evalReflectance(roughness_,
                                                       *vin,
                                                       *vout,
                                                       info->normal(),
                                                       n_,
                                                       eta_);
  return radiance;
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledSpectra, Float> GgxConductorBrdf::evalRadianceAndPdf(
    const Vector3* vin,
    const Vector3* vout,
    const WavelengthSamples& /* wavelengths */,
    const IntersectionInfo* info) const noexcept
{
  Float pdf = 0.0;
  const auto radiance = MicrofacetGgx::evalReflectance(roughness_,
                                                       *vin,
                                                       *vout,
                                                       info->normal(),
                                                       n_,
                                                       eta_,
                                                       &pdf);
  return std::make_tuple(radiance, pdf);
}

/*!
  \details
  No detailed.
  */
std::tuple<SampledDirection, SampledSpectra> GgxConductorBrdf::sample(
    const Vector3* vin,
    const WavelengthSamples& wavelengths,
    Sampler& sampler,
    const IntersectionInfo* info) const noexcept
{
  // Sample a microfacet normal
  const auto m_normal = MicrofacetGgx::sampleNormal(roughness_,
                                                    *vin,
                                                    info->shapePoint(),
                                                    sampler);
  // Get the reflection direction
  auto vout = Microfacet::calcReflectionDirection(*vin, m_normal);
  const Float cos_no = zisc::dot(info->normal(), vout.direction());

  SampledSpectra weight{wavelengths};
  if (0.0 < cos_no) {
    const Float cos_ni = -zisc::dot(info->normal(), *vin),
                cos_mi = -zisc::dot(m_normal.direction(), *vin);
    ZISC_ASSERT(0.0 <= cos_ni * cos_mi,
                "Microfacet normal isn't in the same hemisphere as normal.");

    // Evaluate fresnel term
    const auto fresnel = Fresnel::evalFresnel(n_, eta_, cos_mi);

    // Evaluate the weight
    const Float cos_mo = cos_mi;
    const Float cos_nm = zisc::dot(m_normal.direction(), info->normal());
    weight = fresnel * MicrofacetGgx::evalWeight(roughness_,
                                                 cos_ni, cos_no,
                                                 cos_mi, cos_mo,
                                                 cos_nm);
    ZISC_ASSERT(!weight.hasNegative(), "Weight contains negative.");
  }
  else {
    vout.setPdf(0.0);
  }
  return std::make_tuple(vout, weight);
}

/*!
  \details
  No detailed.
  */
bool GgxConductorBrdf::wavelengthIsSelected() const noexcept
{
  return false;
}

} // namespace nanairo
