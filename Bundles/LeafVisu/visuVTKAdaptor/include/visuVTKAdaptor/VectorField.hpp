/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_VECTORFIELD_HPP__
#define __VISUVTKADAPTOR_VECTORFIELD_HPP__

#include "visuVTKAdaptor/config.hpp"

#include <fwData/Image.hpp>

#include <fwRenderVTK/IVtkAdaptorService.hpp>
#include <fwComEd/helper/MedicalImageAdaptor.hpp>

#include <vtkSmartPointer.h>

class vtkPolyDataAlgorithm;
class vtkImageData;
class vtkGlyph3D;

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API VectorField : public ::fwRenderVTK::IVtkAdaptorService
{

public:

    fwCoreServiceClassDefinitionsMacro ( (VectorField)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API VectorField() throw();

    VISUVTKADAPTOR_API virtual ~VectorField() throw();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect Image::s_BUFFER_MODIFIED_SIG to this::s_UPDATE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    VISUVTKADAPTOR_API void doStart() throw(::fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(::fwTools::Failed);

    VISUVTKADAPTOR_API void doUpdate() throw(::fwTools::Failed);
    VISUVTKADAPTOR_API void configuring() throw(::fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(::fwTools::Failed);

    virtual void buildPipeline();
    virtual void destroyPipeline();

    vtkSmartPointer<vtkPolyDataAlgorithm> m_arrowSource;
    vtkSmartPointer<vtkImageData> m_imageData;
    vtkSmartPointer<vtkGlyph3D> glyphFilter;

};

} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_VECTORFIELD_HPP__
