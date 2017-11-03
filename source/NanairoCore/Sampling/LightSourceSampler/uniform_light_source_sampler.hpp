/*!
  \file uniform_light_source_sampler.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2017 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_UNIFORM_LIGHT_SOURCE_SAMPLER_HPP
#define NANAIRO_UNIFORM_LIGHT_SOURCE_SAMPLER_HPP

// Standard C++ library
#include <vector>
// Nanairo
#include "light_source_sampler.hpp"
#include "NanairoCore/Data/light_source_info.hpp"

namespace nanairo {

// Forward declaration
class IntersectionInfo;
class Object;
class Sampler;
class World;

//! \addtogroup Core
//! \{

/*!
  \details
  No detailed.
  */
class UniformLightSourceSampler : public LightSourceSampler
{
 public:
  //! Create a light source sampler
  UniformLightSourceSampler(const World& world) noexcept;


  //! Return the light source info by the light source
  LightSourceInfo getInfo(
      const IntersectionInfo& info,
      const Object* light_source) const noexcept override;

  //! Sample a light source
  LightSourceInfo sample(Sampler& sampler) const noexcept override;

  //! Sample a light source
  LightSourceInfo sample(
      const IntersectionInfo& info,
      Sampler& sampler) const noexcept override;

 private:
  //! Return the light source info by the light source
  LightSourceInfo getInfo(const Object* light_source) const noexcept;

  //! Initialize
  void initialize(const World& world) noexcept;

  //! Return the light source list
  const std::vector<const Object*>& lightSourceList() const noexcept;

  //! Sample a light source
  LightSourceInfo sampleInfo(Sampler& sampler) const noexcept;

  //! Return the weight per light
  Float weightPerLight() const noexcept;


  const std::vector<const Object*>* light_source_list_;
  Float weight_per_light_;
};

//! \} Core

} // namespace nanairo

#endif // NANAIRO_UNIFORM_LIGHT_SOURCE_SAMPLER_HPP