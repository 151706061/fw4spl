/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_STRANSFORMFROMWHEEL_HPP__
#define __VISUVTKADAPTOR_STRANSFORMFROMWHEEL_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwDataTools/helper/MedicalImageAdaptor.hpp>

#include <fwRenderVTK/IAdaptor.hpp>

namespace visuVTKAdaptor
{

/**
 * @brief Computes a volume space transform matrix from a viewport coordinate and a rotation angle.
 *
 * @section Slots Slots
 *
 * -\b updateTransform(double, double, double) : computes the transform
 * -\b updateSliceOrientation(int, int): updates slice orientation
 *
 * @section XML XML Configuration
 *
 * @code{.xml}
   <service type="::visuVTKAdaptor::STransformFromWheel" >
       <inout key="image" uid="..." />
       <inout key="transform" uid="..." />
       <config renderer="default" picker="myPicker" mode="2d" />
   </service>
 *
 * @subsection In-Out In-Out
 * - \b image [::fwData::Image]: defines the space in which the transform takes place.
 * - \b transform [::fwData::TransformationMatrix3D]: outputed image space transform.
 *
 * @subsection Configuration Configuration
 * - \b renderer (mandatory): renderer in which the image is displayed.
 * - \b picker (mandatory): used to find image voxel positions from a viewport pixel position.
 * - \b mode (optional, defaults to 2d) : determines how the position of the coordinates are computed.
 *           In 2d, the coordinates are permutated to match the correct, whereas in 3d,
 *           we do not change the picked position.
 *
 */
class VISUVTKADAPTOR_CLASS_API STransformFromWheel : public ::fwRenderVTK::IAdaptor,
                                                     public ::fwDataTools::helper::MedicalImageAdaptor
{
public:

    fwCoreServiceClassDefinitionsMacro( (STransformFromWheel)(::fwRenderVTK::IAdaptor) );

    /// Constructor.
    VISUVTKADAPTOR_API STransformFromWheel();

    /// Destructor.
    VISUVTKADAPTOR_API virtual ~STransformFromWheel();

protected:

    VISUVTKADAPTOR_API virtual void configuring() override;
    VISUVTKADAPTOR_API virtual void starting() override;
    VISUVTKADAPTOR_API virtual void updating() override;
    VISUVTKADAPTOR_API virtual void stopping() override;

    /**
     * @brief Returns proposals to connect service slots to object signals.
     *
     * Connect image::s_SLICE_TYPE_MODIFIED_SIG to this::s_UPDATE_SLICE_TYPE_SLOT.
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsMap getAutoConnections() const override;
private:

    /// Computes the transform based on the (cx, cy) wheel center and the wheel angle.
    void updateTransform(double cx, double cy, double wheelAngle);

    /// Slot: set the correct orientation to compute the right transform.
    void updateSliceOrientation(int from, int to);

    /// Interaction mode: determines how the picked point is handled (default to 2d)
    unsigned char m_interactionMode;

    /// Initial wheel position. Updated each time updateTransform() is called.
    double m_initAngle;

};

} // namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_STRANSFORMFROMWHEEL_HPP__
