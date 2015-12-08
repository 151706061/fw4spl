/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VTKSIMPLENEGATO_SRENDERER_HPP__
#define __VTKSIMPLENEGATO_SRENDERER_HPP__


#include "vtkSimpleNegato/config.hpp"

#include <fwRender/IRender.hpp>

#include <fwRenderVTK/IVtkRenderWindowInteractorManager.hpp>

#include  <vtkImagePlaneWidget.h>
#include  <vtkOutlineFilter.h>
#include  <vtkRenderer.h>

namespace vtkSimpleNegato
{

/**
 * @brief    Service rendering a ::fwData::Image using VTK.
 * @class    SRenderer
 *
 * This service displays a 3D image in a scene.
 */
class VTKSIMPLENEGATO_CLASS_API SRenderer : public fwRender::IRender
{
public:
    fwCoreServiceClassDefinitionsMacro ( (SRenderer)(::fwRender::IRender) );

    /**
     * @brief    Constructor
     */
    VTKSIMPLENEGATO_API SRenderer() throw();

    /**
     * @brief    Destructor
     */
    VTKSIMPLENEGATO_API virtual ~SRenderer() throw();

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect Image::s_MODIFIED_SIG to this::s_REFRESH_SLOT
     * Connect Image::s_BUFFER_MODIFIED_SIG to this::s_REFRESH_SLOT
     */
    VTKSIMPLENEGATO_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    /**
     * @brief Starting method.
     *
     * This method is used to initialize the service.
     * Initialize VTK renderer and gui containers
     */
    VTKSIMPLENEGATO_API virtual void starting() throw(fwTools::Failed);

    /**
     * @brief Stopping method.
     *
     * Destroy VTK renderer and gui containers
     */
    VTKSIMPLENEGATO_API virtual void stopping() throw(fwTools::Failed);

    /**
     * @brief Updating method.
     *
     * This method is used to update the service.
     * Make a render if necessary
     */
    VTKSIMPLENEGATO_API virtual void updating() throw(fwTools::Failed);

    /**
     * @brief This method is used to configure the service.
     *
     * It initializes the qt container.
     */
    VTKSIMPLENEGATO_API virtual void configuring() throw(::fwTools::Failed);

    /// @brief vtk renderer
    vtkRenderer * m_render;

private:

    /// @brief VTK Interactor window manager
    ::fwRenderVTK::IVtkRenderWindowInteractorManager::sptr m_interactorManager;

    /**
     * @brief VTK pipeline initialization method.
     *
     * This method is used to initialize the VTK pipeline.
     */
    void initVTKPipeline();

    /// Slot: refresh the scene
    void refresh();
    /**
     * @brief VTK pipeline updating method.
     *
     * This method is used to update the VTK pipeline.
     */
    void updateVTKPipeline();

    /// @brief image wireframe outline.
    vtkOutlineFilter* m_outline;

    /// @brief vtk widget used in the negatoscope axial plane.
    vtkImagePlaneWidget* m_negatoAxial;

    /// @brief vtk widget used in the negatoscope sagittal plane.
    vtkImagePlaneWidget* m_negatoSagittal;

    /// @brief vtk widget used in the negatoscope frontal plane.
    vtkImagePlaneWidget* m_negatoFrontal;

    /**
     * @brief the m_bPipelineIsInit value is \b true
     * if the pipeline is initialized.
     */
    bool m_bPipelineIsInit;
};

}


#endif /* __VTKSIMPLENEGATO_SRENDERER_HPP__ */
