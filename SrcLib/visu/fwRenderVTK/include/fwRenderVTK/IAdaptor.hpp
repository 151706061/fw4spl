/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2017.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWRENDERVTK_IADAPTOR_HPP__
#define __FWRENDERVTK_IADAPTOR_HPP__

#include "fwRenderVTK/config.hpp"
#include "fwRenderVTK/SRender.hpp"

#include <fwCom/helper/SigSlotConnection.hpp>

#include <fwServices/IHasServices.hpp>
#include <fwServices/IService.hpp>

#include <string>

class vtkProp;
class vtkPropCollection;
class vtkRenderer;
class vtkRenderWindowInteractor;
class vtkTransform;

namespace fwRenderVTK
{

/**
 * @brief Base class for VTK adaptors
 */
class FWRENDERVTK_CLASS_API IAdaptor : public fwServices::IService,
                                       public fwServices::IHasServices
{
friend class SRender;
public:
    fwCoreServiceClassDefinitionsMacro( (IAdaptor)(::fwServices::IService) );
    fwCoreAllowSharedFromThis();

    typedef fwServices::IService SuperClass;

    /// Sets the assiciated render service
    FWRENDERVTK_API void setRenderService( SRender::sptr service );

    /// Returd the associated render service
    FWRENDERVTK_API SRender::sptr getRenderService() const;

    /// Sets the renderer identifier
    FWRENDERVTK_API void setRendererId(SRender::RendererIdType newID);

    /// Returns the renderer identifier
    FWRENDERVTK_API SRender::RendererIdType getRendererId() const;

    /// Returns the renderer
    FWRENDERVTK_API vtkRenderer* getRenderer();

    /// Sets the identifier of the picker used by this adaptor
    FWRENDERVTK_API void setPickerId(SRender::PickerIdType newID);

    /// Gets the identifier of the picker used by this adaptor
    FWRENDERVTK_API SRender::PickerIdType getPickerId() const;

    /**
     * @brief Get the picker
     * @param pickerId identifier of the picker in the vtk scene (if it is empty, it returns the picker associated with
     *        this adaptor
     * @return Returns the associated picker
     */
    FWRENDERVTK_API vtkAbstractPropPicker* getPicker(std::string pickerId = "");

    /// Sets the identifier of the transform used by this adaptor
    FWRENDERVTK_API void setTransformId(SRender::VtkObjectIdType newID);

    /// Returns the identifier of the transform used by this adaptor
    FWRENDERVTK_API SRender::VtkObjectIdType getTransformId() const;

    /// Returns the transform used by this adaptor
    FWRENDERVTK_API vtkTransform* getTransform();

    /// Returns the vtk object defined by 'objectId' in the vtk scene
    FWRENDERVTK_API vtkObject* getVtkObject(const SRender::VtkObjectIdType& objectId) const;

    /// Returns the render interactor
    FWRENDERVTK_API vtkRenderWindowInteractor* getInteractor();

    /**
     * @brief Returns the adaptor associated to the vtkProp.
     * @param prop vtkProp to find the associated adaptor
     * @param depth depth of reseach in sub-adaptors, if depth == 0 it only search in current prop collection
     */
    FWRENDERVTK_API IAdaptor::sptr getAssociatedAdaptor(vtkProp* prop, int depth);

    /**
     * @brief Gets all the vtkProp associated to this adaptor
     * @param[out] propc output prop collection
     * @param depth depth of reseach in sub-adaptors, if depth == 0 it only search in current prop collection,
     * if depth == -1, it recursively searchs in all sub-adaptors
     */
    FWRENDERVTK_API void getAllSubProps(vtkPropCollection* propc, int depth = -1);

    /// End-user have to call this method when a vtk structure has been modified, thus a render request will be sent.
    FWRENDERVTK_API void setVtkPipelineModified();

    /// Returns true if the service automatically triggers the rendering.
    bool getAutoRender() const
    {
        return m_autoRender;
    }

    /// Sets if the service automatically triggers the rendering.
    void setAutoRender(bool autoRender)
    {
        m_autoRender = autoRender;
    }

    /**
     * @brief   Returns the starting priority of the adaptor.
     *
     * Some adaptors may have to be started before others.
     * @return priority, the lesser will be the first to be launched and the last be to stopped (default: 0)
     */
    FWRENDERVTK_API virtual int getStartPriority();

protected:

    /**
     * @brief   constructor
     */
    FWRENDERVTK_API IAdaptor() noexcept;

    /**
     * @brief   destructor
     */
    FWRENDERVTK_API virtual ~IAdaptor() noexcept;

    /// Parse the xml configuration for renderer, picker and transform
    FWRENDERVTK_API void configureParams();

    /// Initialize the adaptor with the associated render service. (must be call in starting).
    FWRENDERVTK_API void initialize();

    /// Registers the vtkProp
    FWRENDERVTK_API void registerProp(vtkProp* prop);

    /// Unregisters all the vtkProp
    FWRENDERVTK_API void unregisterProps();

    /// Adds the vtkProp to the renderer
    FWRENDERVTK_API void addToRenderer(vtkProp* prop);

    /// Removes all the vtkProp from the renderer
    FWRENDERVTK_API void removeAllPropFromRenderer();

    /**
     * @brief Adds the vtkProp to the picking list
     * @param prop vtkProp to be pickable
     * @param pickerId picker identifier in the scene (if empty the current adaptor picker is used)
     */
    FWRENDERVTK_API void addToPicker(vtkProp* prop, std::string pickerId = "");

    /**
     * @brief Removes the vtkProp to the picking list
     * @param prop vtkProp to be pickable
     * @param pickerId picker identifier in the scene (if empty the current adaptor picker is used)
     */
    FWRENDERVTK_API void removeFromPicker(vtkProp* prop, std::string pickerId = "");

    /**
     * @brief Get all the vtkProp associated to the inProp (actors, actors2D, volumes)
     * @param[out] outPropc collection of vtkProp
     * @param[in] inProp
     */
    FWRENDERVTK_API static void getProps(vtkPropCollection* outPropc, vtkProp* inProp);

    /// notify a render request iff vtkPipeline is modified
    FWRENDERVTK_API void requestRender();

    /// state of the pipeline
    bool m_vtkPipelineModified;
    SRender::RendererIdType m_rendererId;
    SRender::PickerIdType m_pickerId;
    SRender::VtkObjectIdType m_transformId;
    SRender::wptr m_renderService;

    vtkPropCollection* m_propCollection;

    ::fwCom::helper::SigSlotConnection m_connections;

    bool m_autoRender;
};

}

#endif /*__FWRENDERVTK_IADAPTOR_HPP__*/
