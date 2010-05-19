/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2010.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */
/* ***** BEGIN CONTRIBUTORS BLOCK *****
 * Contributors:
 *  - Jean-Baptiste.Fasquel (LISA Laboratory, Angers University, France)
 * ****** END CONTRIBUTORS BLOCK ****** */

#ifndef _VISUVTKADAPTOR_SLICEFOLLOWERCAMERA_HPP_
#define _VISUVTKADAPTOR_SLICEFOLLOWERCAMERA_HPP_

#include <vector>

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/IImagesAdaptor.hpp"

namespace visuVTKAdaptor
{


class VISUVTKADAPTOR_CLASS_API SliceFollowerCamera: public ::visuVTKAdaptor::IImagesAdaptor
{
public:
    fwCoreServiceClassDefinitionsMacro ( (SliceFollowerCamera)(::fwRenderVTK::IVtkAdaptorService::Baseclass) ) ;

    VISUVTKADAPTOR_API SliceFollowerCamera() throw();

    VISUVTKADAPTOR_API virtual ~SliceFollowerCamera() throw();

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

    /**
     * @brief Configuration managment.
     *
     * Management of two kind of configurations. Case where follower is <adaptor /> XML elements of the generic scene
     * and case where negato is directly attached to image object.
     */
    VISUVTKADAPTOR_API void configuring() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doUpdate(::fwServices::ObjectMsg::csptr msg) throw(fwTools::Failed);


    void initializeCamera();
    void updateCamera(double distance = -1., double size=-1);

    vtkCamera* m_camera;
};




} //namespace visuVTKAdaptor

#endif // _VISUVTKADAPTOR_SLICEFOLLOWERCAMERA_HPP_
