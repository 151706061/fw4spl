/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __SCENE2D_ADAPTOR_SGRID2D_HPP__
#define __SCENE2D_ADAPTOR_SGRID2D_HPP__

#include "scene2D/config.hpp"

#include <fwRenderQt/IAdaptor.hpp>

namespace scene2D
{
namespace adaptor
{
/**
 * @brief Render grid on the scene2d.
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service uid="grid1" type="::scene2D::adaptor::SGrid2D">
    <config xMin="-1100" xMax="750" yMin="-0.7" yMax="1.7" xSpacing="100" ySpacing="0.1"
        color="lightGray" xAxis="xAxis" yAxis="yAxis" zValue="1"/>
   </service>
   @endcode
 *
 * @subsection Configuration Configuration:
 * - \b config (mandatory): contains the adaptor configuration
 *    - \b xAxis (optional): x axis associated to the adaptor
 *    - \b yAxis (optional): y axis associated to the adaptor
 *    - \b zValue (optional, default=0): z value of the layer
 *    - \b opacity (optional, default=1.0): adaptor opacity (float)
 *    - \b color (optional, default black): color of the axis
 *    - \b xMin (optional): Set the minimum x value of the grid.
 *    - \b xMax (optional): Set the maximum x value of the grid.
 *    - \b yMin (optional): Set the minimum y value of the grid.
 *    - \b yMax (optional): Set the maximum y value of the grid.
 *    - \b xSpacing (optional, default value: 10): Set the grid spacing (space between 2 consecutive lines) in x.
 *    - \b ySpacing (optional, default value: 10): Set the grid spacing (space between 2 consecutive lines) in y.
 */
class SCENE2D_CLASS_API SGrid2D : public ::fwRenderQt::IAdaptor
{

public:
    fwCoreServiceClassDefinitionsMacro( (SGrid2D)(::fwRenderQt::IAdaptor) );

    /// Constructor, set the x and y spacing to 10
    SCENE2D_API SGrid2D() noexcept;

    /// Basic destructor, do nothing
    SCENE2D_API virtual ~SGrid2D() noexcept;

protected:

    SCENE2D_API void configuring();

    /// Initialize the layer, set the pen style to DashLine and call the draw() function.
    SCENE2D_API void starting();

    SCENE2D_API void updating();

    /// Clean the lines vector and remove the layer from the scene.
    SCENE2D_API void stopping();

    /// Manage the given events
    SCENE2D_API void processInteraction( ::fwRenderQt::data::Event& _event );

private:

    /// Calculate the x/y start/end values, create the lines, set'em m_pen, push'em back in
    /// the m_lines vector, add'em to the layer, set the layer position and zValue and add it
    /// to the scene.
    void draw();

    /// Returns x-axis starting value
    float getXStartVal();

    /// Returns x-axis ending value
    float getXEndVal();

    /// Returns y-axis starting value
    float getYStartVal();

    /// Returns y-axis ending value
    float getYEndVal();

    /// Bounds of the grid and spacing values for each axis.
    float m_xMin, m_xMax, m_yMin, m_yMax, m_xSpacing, m_ySpacing;

    /// The pen.
    QPen m_pen;

    /// A vector containing QGraphicsItems representing the lines of the grid.
    std::vector<QGraphicsItem*> m_lines;

    /// The layer.
    QGraphicsItemGroup* m_layer;

};

} // namespace adaptor
} // namespace scene2D

#endif // __SCENE2D_ADAPTOR_SGRID2D_HPP__

