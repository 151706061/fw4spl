/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "gui/action/SStarter.hpp"

#include <fwCore/base.hpp>

#include <fwGui/dialog/MessageDialog.hpp>

#include <fwRuntime/Extension.hpp>
#include <fwRuntime/helper.hpp>

#include <fwServices/macros.hpp>
#include <fwServices/op/Get.hpp>

#include <fwTools/fwID.hpp>

namespace gui
{
namespace action
{

fwServicesRegisterMacro( ::fwGui::IActionSrv, ::gui::action::SStarter, ::fwData::Object );

//-----------------------------------------------------------------------------

SStarter::SStarter() noexcept
{
}

//-----------------------------------------------------------------------------

SStarter::~SStarter() noexcept
{
}

//-----------------------------------------------------------------------------

void SStarter::starting()
{
    this->actionServiceStarting();
}

//-----------------------------------------------------------------------------

void SStarter::stopping()
{
    std::vector< ::fwServices::IService::SharedFutureType > futures;

    for( VectPairIDActionType::value_type serviceUid :  m_uuidServices)
    {
        bool srv_exists = ::fwTools::fwID::exist(serviceUid.first );
        if (srv_exists &&  (m_idStartedSrvSet.find(serviceUid.first) != m_idStartedSrvSet.end()) )
        {
            ::fwServices::IService::sptr service = ::fwServices::get( serviceUid.first );
            if (service->isStarted())
            {
                futures.push_back(service->stop());
            }
        }
    }

    ::boost::wait_for_all(futures.begin(), futures.end());

    this->actionServiceStopping();
}

//-----------------------------------------------------------------------------

void SStarter::info(std::ostream& _sstream )
{
    _sstream << "Starter Action" << std::endl;
}

//-----------------------------------------------------------------------------

void SStarter::updating()
{
    for(size_t i = 0; i < m_uuidServices.size(); i++)
    {
        ActionType action = m_uuidServices.at(i).second;
        IDSrvType uid     = m_uuidServices.at(i).first;
        bool srv_exists   = ::fwTools::fwID::exist(uid);

        // Manage special action
        if ( action == START_IF_EXISTS )
        {
            if ( srv_exists )
            {
                action = START;
            }
            else
            {
                action = DO_NOTHING;
            }
        }
        else if( action == STOP_IF_EXISTS )
        {
            if ( srv_exists )
            {
                action = STOP;
            }
            else
            {
                action = DO_NOTHING;
            }
        }

        if( action != DO_NOTHING)
        {
            ::fwGui::LockAction lock(this->getSptr());

            ::fwServices::IService::sptr service = ::fwServices::get( uid );
            SLM_ASSERT("service not found", service);
            switch ( action )
            {
                case START_OR_STOP:
                {
                    if(service->isStopped())
                    {
                        service->start();
                        service->update();
                        m_idStartedSrvSet.insert(uid);
                    }
                    else
                    {
                        service->stop();
                        m_idStartedSrvSet.erase(uid);
                    }
                    break;
                }
                case START:
                {
                    if(service->isStopped())
                    {
                        service->start();
                        m_idStartedSrvSet.insert(uid);
                    }
                    else
                    {
                        OSLM_WARN("Service " << service->getID() << " is not stopped");
                    }
                    service->update();
                    break;
                }
                case STOP:
                {
                    if(service->isStarted())
                    {
                        service->stop();
                    }
                    else
                    {
                        OSLM_WARN("Service " << service->getID() << " is not started");
                    }
                    break;
                }
                default:
                {
                    OSLM_FATAL("There is no action ("<< action
                                                     <<") type corresponding to the action id requested for " << uid <<
                               ".");
                    break;
                }
            }
        }
        else
        {
            ::fwGui::dialog::MessageDialog::showMessageDialog(
                "Service unavailable",
                "The service is unavailable.",
                ::fwGui::dialog::IMessageDialog::WARNING);

            OSLM_INFO("Do nothing for Service " << m_uuidServices.at(i).first);
        }
    }
}

//-----------------------------------------------------------------------------

void SStarter::configuring()
{
    this->initialize();

    for(ConfigurationType actionCfg :  m_configuration->getElements() )
    {
        OSLM_INFO( "SStarter " << actionCfg->getName());

        std::string actionType = actionCfg->getName();
        ActionType action;
        if ( actionType == "start" )
        {
            action = START;
        }
        else if ( actionType == "stop" )
        {
            action = STOP;
        }
        else if ( actionType == "start_or_stop" )
        {
            action = START_OR_STOP;
        }
        else if ( actionType == "start_if_exists" )
        {
            action = START_IF_EXISTS;
        }
        else if ( actionType == "stop_if_exists" )
        {
            action = STOP_IF_EXISTS;
        }
        else
        {
            OSLM_WARN("The \"actionType\":" << actionType <<" is not managed by SStarter");
            continue;
        }
        SLM_ASSERT("Attribute uid missing", actionCfg->hasAttribute("uid"));
        IDSrvType uuid = actionCfg->getExistingAttributeValue("uid");

        m_uuidServices.push_back( std::make_pair(uuid, action) );
    }
}

//-----------------------------------------------------------------------------

} // namespace action
} // namespace gui

