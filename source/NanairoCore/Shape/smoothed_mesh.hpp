/*!
  \file smoothed_mesh.hpp
  \author Sho Ikeda

  Copyright (c) 2015-2016 Sho Ikeda
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
  */

#ifndef NANAIRO_SMOOTHED_MESH_HPP
#define NANAIRO_SMOOTHED_MESH_HPP

// Standard C++ library
#include <array>
#include <tuple>
// Nanairo
#include "shape.hpp"
#include "triangle_mesh.hpp"
#include "NanairoCore/nanairo_core_config.hpp"
#include "NanairoCore/Geometry/point.hpp"
#include "NanairoCore/Geometry/vector.hpp"
#include "NanairoCore/Sampling/sampled_point.hpp"

namespace nanairo {

// Forward declaration
class Aabb;
class IntersectionInfo;
class Ray;
class Sampler;

//! \addtogroup Core
//! \{

/*!
  \details
  Please see "Ray Tracing of Quadratic Parametric Surface.pdf"
  */
class SmoothedMesh : public TriangleMesh
{
 public:
  //! Initialize smoothed mesh
  SmoothedMesh(const Point3& vertex1,
               const Point3& vertex2,
               const Point3& vertex3,
               const Vector3& normal1,
               const Vector3& normal2,
               const Vector3& normal3) noexcept;


  //! Return the bounding box
  Aabb boundingBox() const noexcept override;

  //! Calculate the curve coefficients
  std::array<Float, 10> calcCurveCoefficients(const Ray& ray) const noexcept;

  //! Return the cost of a ray-patch intersection test
  Float getTraversalCost() const noexcept override;

  //! Return the normal
  Vector3 normal(const double u, const double v) const noexcept;

  //! Return the normal1
  Vector3 normal1() const noexcept;

  //! Return the normal2
  Vector3 normal2() const noexcept;

  //! Return the normal3
  Vector3 normal3() const noexcept;

  //! Return the surface point
  Point3 point(const double u, const double v) const noexcept;

  //! Solve the pencil equation
  Float solvePencil(const std::array<Float, 10>& coefficients) const noexcept;

  //! Test ray-mesh intersection
  bool testIntersection(const Ray& ray,
                        IntersectionInfo* intersection) const noexcept override;

  //! Sample a point randomly on the surface of the mesh
  std::tuple<SampledPoint, Vector3, Point2> samplePoint(
      Sampler& sampler) const noexcept override;

  //! Apply affine transformation
  void transform(const Matrix4x4& matrix) noexcept override;

  //! Return the vertex1
  Vector3 vertex1() const noexcept;

  //! Return the vertex2
  Vector3 vertex2() const noexcept;

  //! Return the vertex3
  const Vector3& vertex3() const noexcept;

 private:
  //! The method of smoothing
  enum class SmoothingMethod : uint
  {
    kResultant,
    kPencil,
    kNoSmoothed
  };

  //! Calculate the control points of the surface
  void calcControlPoints(const Point3& vertex1,
                         const Point3& vertex2,
                         const Point3& vertex3,
                         const Vector3& normal1,
                         const Vector3& normal2,
                         const Vector3& normal3) noexcept;

  //! Calculate the ray plane
  std::tuple<Vector3, Float> calcRayPlane(const Ray& ray,
                                          const Vector3& c) const noexcept;

  //! Calculate the v using resultant method
  std::tuple<std::array<Float, 2>, uint> calcResultantV(
      const std::array<Float, 10>& coefficients,
      const Float u) const noexcept;

  //! Initialize
  void initialize(const Point3& vertex1,
                  const Point3& vertex2,
                  const Point3& vertex3,
                  const Vector3& normal1,
                  const Vector3& normal2,
                  const Vector3& normal3) noexcept;

  //! Return the used smoothing method
  static constexpr SmoothingMethod smoothingMethod() noexcept;

  //! Solve the resultant equation
  std::tuple<std::array<Float, 4>, uint> solveResultant(
      const std::array<Float, 10>& coefficients) const noexcept;

  //! Test ray-mesh intersection
  bool testIntersectionUsingPencil(const Ray& ray,
                                   const std::array<Float, 10>& coefficients,
                                   IntersectionInfo* intersection) const noexcept;

  //! Test ray-mesh intersection
  bool testIntersectionUsingResultant(const Ray& ray,
                                      const std::array<Float, 10>& coefficients,
                                      IntersectionInfo* intersection) const noexcept;

  //! Test ray-mesh intersection
  bool testIntersectionWithoutSmoothing(
      const Ray& ray,
      IntersectionInfo* intersection) const noexcept;

  //! Test line-surface intersection
  bool testPencilLineIntersection(const Ray& ray,
                                  const std::array<Float, 10>& coefficients,
                                  const Float x,
                                  IntersectionInfo* intersection) const noexcept;

  //! Test line-surface intersection
  bool testPencilLineIntersection(const Ray& ray,
                                  const std::array<Float, 10>& coefficients,
                                  const Float alpha,
                                  const Float beta,
                                  const Float gamma,
                                  IntersectionInfo* intersection) const noexcept;

  //! Test ray-surface intersection
  bool testRaySurfaceIntersection(const Ray& ray,
                                  const Float u,
                                  const Float v,
                                  IntersectionInfo* intersection) const noexcept;

  Vector3 c_[6]; //!< Control points
};

//! \} Core

} // namespace nanairo

#include "smoothed_mesh-inl.hpp"

#endif // NANAIRO_SMOOTHED_MESH_HPP
