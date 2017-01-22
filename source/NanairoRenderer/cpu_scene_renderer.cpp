/*!
  \file cpu_scene_renderer.cpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#include "cpu_scene_renderer.hpp"
// Standard C++ library
#include <cstddef>
// Qt
#include <QLocale>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
// Zisc
#include "zisc/error.hpp"
#include "zisc/unit.hpp"
#include "zisc/utility.hpp"
// Nanairo
#include "NanairoCore/scene.hpp"
#include "NanairoCore/system.hpp"
#include "NanairoCore/Color/hdr_image.hpp"
#include "NanairoCore/RenderingMethod/rendering_method.hpp"
#include "NanairoCore/Sampling/wavelength_sampler.hpp"
#include "NanairoCore/ToneMappingOperator/tone_mapping_operator.hpp"
#include "NanairoCore/Utility/unique_pointer.hpp"

namespace nanairo {

/*!
  \details
  No detailed.
  */
CpuSceneRenderer::CpuSceneRenderer() noexcept
{
}

/*!
  */
CpuSceneRenderer::~CpuSceneRenderer() noexcept
{
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::convertSpectraToHdr(const quint64 cycle) noexcept
{
  auto& film = scene_->film();
  auto& spectra_image = film.spectraBuffer();
  spectra_image.toHdrImage(*system_, cycle, hdr_image_.get());
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::handleCameraEvent() noexcept
{
  auto& camera_event = cameraEvent();
  auto& camera = scene_->camera();
  auto& matrix = cameraMatrix();
  // Transform camera
  if (camera_event.isHorizontalTranslationEventOccured()) {
    const auto value = camera_event.flushHorizontalTranslationEvent();
    matrix = matrix * camera.translateHorizontally(value);
  }
  if (camera_event.isVerticalTranslationEventOccured()) {
    const auto value = camera_event.flushVerticalTranslationEvent();
    matrix = matrix * camera.translateVertically(value);
  }
  if (camera_event.isRotationEventOccured()) {
    const auto value = camera_event.flushRotationEvent();
    matrix = matrix * camera.rotate(value);
  }
  cameraMatrix() = matrix;

  // Initialize memory
  auto& film = scene_->film();
  film.spectraBuffer().clear();
  renderingMethod().clear();
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::initializeRenderer(const QJsonObject& settings) noexcept
{
  // System initialization
  system_ = new System{settings};
  scene_ = new Scene{*system_, settings};

  // Rendering method
  const auto& world = scene_->world();
  const auto color_settings = SceneValue::toObject(settings, keyword::color);
  // Sampler
  wavelength_sampler_ = new WavelengthSampler{world, color_settings};
  // Method
  const auto method_settings = SceneValue::toObject(settings,
                                                    keyword::renderingMethod);
  rendering_method_ = RenderingMethod::makeMethod(*system_, method_settings);
  // ToneMapping
  tone_mapping_operator_ = ToneMappingOperator::makeOperator(*system_,
                                                             color_settings);

  // Images
  const auto& camera = scene_->camera();
  // HDR image
  hdr_image_ = new HdrImage{camera.widthResolution(), camera.heightResolution()};
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::render(const quint64 /* cycle */) noexcept
{
  auto& sampler = system_->globalSampler();
  const auto& wavelength_sampler = wavelengthSampler();
  auto& method = renderingMethod();
  const auto sampled_wavelengths = wavelength_sampler(sampler);
  method.render(*system_, *scene_, sampled_wavelengths);
}

/*!
  */
inline
const RenderingMethod& CpuSceneRenderer::renderingMethod() const noexcept
{
  return *rendering_method_;
}

/*!
  */
inline
RenderingMethod& CpuSceneRenderer::renderingMethod() noexcept
{
  return *rendering_method_;
}

/*!
  \details
  No detailed.
  */
void CpuSceneRenderer::toneMap() noexcept
{
  auto& ldr_image = renderedImage();
  tone_mapping_operator_->map(*system_, *hdr_image_, &ldr_image);
}

/*!
  */
inline
const WavelengthSampler& CpuSceneRenderer::wavelengthSampler() const noexcept
{
  return *wavelength_sampler_;
}

/*!
  */
inline
WavelengthSampler& CpuSceneRenderer::wavelengthSampler() noexcept
{
  return *wavelength_sampler_;
}


} // namespace nanairo
