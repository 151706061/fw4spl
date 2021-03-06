/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWDATATOOLS_MESH_HPP__
#define __FWDATATOOLS_MESH_HPP__

#include "fwDataTools/config.hpp"
#include "fwDataTools/Vector.hxx"
#include <fwDataTools/helper/Mesh.hpp>

#include <fwCore/base.hpp>

#include <fwData/Mesh.hpp>
#include <fwData/TransformationMatrix3D.hpp>

namespace fwDataTools
{

/**
 * @brief   This helper generates a mesh using specified structure (quad or triangle).
 */
class Mesh
{

public:

    typedef std::map<Point, ::fwData::Mesh::Id> PointsMapType;
    typedef std::array<double, 3> PositionType;
    typedef std::array<size_t, 3> OrganizationType;

    /**
     * @brief Initialize 'rand' seed
     */
    FWDATATOOLS_API static void initRand();

    /**
     * @brief Generate cell normals for the mesh.
     *
     * @param[out]  mesh fwData::Mesh structure to fill with cell normals.
     */
    FWDATATOOLS_API static void generateCellNormals(::fwData::Mesh::sptr mesh);

    /**
     * @brief Generate point normals for the mesh.
     *
     * @param[out]  mesh fwData::Mesh structure to fill with cell normals.
     */
    FWDATATOOLS_API static void generatePointNormals(::fwData::Mesh::sptr mesh);

    /**
     * @brief Shake Array of Normals.
     *
     * Array must have 3 float components, 1 dimension. Otherwise, shakeNormals
     * will do nothing.
     *
     * @param[out]  array mesh's array containing normals.
     */
    FWDATATOOLS_API static void shakeNormals(::fwData::Array::sptr array);

    /**
     * @brief Shake point Normals.
     *
     * @param[out]  mesh fwData::Mesh structure to shake normals.
     */
    FWDATATOOLS_API static void shakePointNormals(::fwData::Mesh::sptr mesh);

    /**
     * @brief Shake cell Normals.
     *
     * @param[out]  mesh fwData::Mesh structure to shake normals.
     */
    FWDATATOOLS_API static void shakeCellNormals(::fwData::Mesh::sptr mesh);

    /**
     * @brief Shake points of the mesh.
     *
     * @param[out]  mesh fwData::Mesh structure to shake.
     */
    FWDATATOOLS_API static void shakePoint(::fwData::Mesh::sptr mesh);

    /**
     * @brief Colorize mesh (vertex point color).
     *
     * @param[in]  mesh fwData::Mesh mesh structure to colorize.
     */
    FWDATATOOLS_API static void colorizeMeshPoints(::fwData::Mesh::sptr mesh);

    /**
     * @brief Colorize mesh (cell color).
     *
     * @param[in]  mesh fwData::Mesh mesh structure to colorize.
     */
    FWDATATOOLS_API static void colorizeMeshCells(::fwData::Mesh::sptr mesh);

    /**
     * @brief
     *
     * @param[in]  mesh fwData::Mesh mesh structure to find cell type.
     * @param[in]  cell CellTypes to find in mesh.
     */
    FWDATATOOLS_API static bool hasUniqueCellType(::fwData::Mesh::sptr mesh, ::fwData::Mesh::CellTypes cell);

    /// Apply a transformation 4x4 from an input mesh to an output mesh
    FWDATATOOLS_API static void transform(fwData::Mesh::csptr inMesh, ::fwData::Mesh::sptr outMesh,
                                          ::fwData::TransformationMatrix3D::csptr t );

    /// Apply a transformation 4x4 on a mesh
    FWDATATOOLS_API static void transform( ::fwData::Mesh::sptr mesh, ::fwData::TransformationMatrix3D::csptr t );

    /**
     * @brief Colorize the mesh points with the given color
     * @pre point color array must be allocated
     * @pre mesh must only contain triangle
     */
    FWDATATOOLS_API static void colorizeMeshPoints(const ::fwData::Mesh::sptr& mesh, const std::uint8_t colorR,
                                                   const std::uint8_t colorG, const std::uint8_t colorB,
                                                   const std::uint8_t colorA = 255);

    /**
     * @brief Colorize the mesh points associated to the triangle in _vectorNumTriangle
     * @pre point color array must be allocated
     * @pre mesh must only contain triangle
     */
    FWDATATOOLS_API static void colorizeMeshPoints(const ::fwData::Mesh::sptr& _mesh,
                                                   const std::vector<size_t>& _vectorNumTriangle,
                                                   const std::uint8_t _colorR, const std::uint8_t colorG,
                                                   const std::uint8_t colorB, const std::uint8_t _colorA = 255);

    /**
     * @brief Colorize the mesh cells with the given rgb color
     * @pre cell color array must be allocated
     * @pre mesh must only contain triangle
     */
    FWDATATOOLS_API static void colorizeMeshCells(const::fwData::Mesh::sptr& mesh, const std::uint8_t colorR,
                                                  const std::uint8_t colorG, const std::uint8_t colorB,
                                                  const std::uint8_t _colorA = 255);
    /**
     * @brief Colorize the cells in vectorNumTriangle
     * @pre cell color array must be allocated
     * @pre mesh must only contain triangle
     */
    FWDATATOOLS_API static void colorizeMeshCells(const::fwData::Mesh::sptr& mesh,
                                                  const std::vector<size_t>& triangleIndexVector,
                                                  const std::uint8_t colorR,
                                                  const std::uint8_t colorG, const std::uint8_t colorB,
                                                  const std::uint8_t _colorA = 255);

};

} // namespace fwDataTools

#endif // __FWDATATOOLS_MESH_HPP__
