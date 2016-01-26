/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __VISUVTKADAPTOR_RESECTIONDB_HPP__
#define __VISUVTKADAPTOR_RESECTIONDB_HPP__

#include "visuVTKAdaptor/config.hpp"
#include "visuVTKAdaptor/MeshFactory.hpp"

#include <fwRenderVTK/IVtkAdaptorService.hpp>

namespace visuVTKAdaptor
{

class VISUVTKADAPTOR_CLASS_API ResectionDB : public ::fwRenderVTK::IVtkAdaptorService
{

public:
    fwCoreServiceClassDefinitionsMacro ( (ResectionDB)(::fwRenderVTK::IVtkAdaptorService) );

    VISUVTKADAPTOR_API ResectionDB() throw();

    VISUVTKADAPTOR_API virtual ~ResectionDB() throw();

    void setClippingPlanes(::fwRenderVTK::SRender::VtkObjectIdType id)
    {
        m_clippingPlanes = id;
    }

    void setSharpEdgeAngle(double angle)
    {
        m_sharpEdgeAngle = angle;
    }

    /**
     * @brief Returns proposals to connect service slots to associated object signals,
     * this method is used for obj/srv auto connection
     *
     * Connect ResectionDB::s_MODIFIED_SIG to this::s_UPDATE_SLOT
     * Connect ResectionDB::s_SAFE_PART_ADDED_SIG to this::s_UPDATE_SLOT
     * Connect ResectionDB::s_RESECTION_ADDED_SIG to this::s_UPDATE_SLOT
     */
    VISUVTKADAPTOR_API virtual KeyConnectionsType getObjSrvConnections() const;

protected:

    VISUVTKADAPTOR_API void doStart() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doConfigure() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doSwap() throw(fwTools::Failed);
    // redraw all (stop then restart sub services)
    VISUVTKADAPTOR_API void doUpdate() throw(fwTools::Failed);
    VISUVTKADAPTOR_API void doStop() throw(fwTools::Failed);

private:
    ::fwRenderVTK::SRender::VtkObjectIdType m_clippingPlanes;
    double m_sharpEdgeAngle;
};


} //namespace visuVTKAdaptor

#endif // __VISUVTKADAPTOR_RESECTIONDB_HPP__
