/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGui/IActionSrv.hpp"
#include "fwGui/IMenuItemCallback.hpp"
#include "fwGui/IMenuSrv.hpp"
#include "fwGui/registry/worker.hpp"

#include <fwCore/base.hpp>
#include <fwServices/Base.hpp>
#include <fwTools/fwID.hpp>

#include <fwThread/Worker.hpp>
#include <fwThread/Worker.hxx>

namespace fwGui
{

IMenuSrv::IMenuSrv() : m_hideActions (false)
{
}

//-----------------------------------------------------------------------------

IMenuSrv::~IMenuSrv()
{
}

//-----------------------------------------------------------------------------

void IMenuSrv::initialize()
{
    m_registrar = ::fwGui::registrar::MenuRegistrar::New(this->getID());
    // find ViewRegistryManager configuration
    std::vector < ConfigurationType > vectRegistrar = m_configuration->find("registry");
    SLM_ASSERT("Registry section is mandatory.", !vectRegistrar.empty() );

    if(!vectRegistrar.empty())
    {
        m_registrarConfig = vectRegistrar.at(0);
        m_registrar->initialize(m_registrarConfig);
    }

    // find gui configuration
    std::vector < ConfigurationType > vectGui = m_configuration->find("gui");
    SLM_ASSERT("Gui section is mandatory.", !vectGui.empty() );
    if(!vectGui.empty())
    {
        // find LayoutManager configuration
        std::vector < ConfigurationType > vectLayoutMng = vectGui.at(0)->find("layout");
        SLM_ASSERT("Layout section is mandatory.", !vectLayoutMng.empty() );
        if(!vectLayoutMng.empty())
        {
            m_layoutConfig = vectLayoutMng.at(0);
            this->initializeLayoutManager(m_layoutConfig);

            if (m_layoutConfig->hasAttribute("hideAction"))
            {
                std::string hideActions = m_layoutConfig->getAttributeValue("hideActions");
                SLM_ASSERT("'hideActions' attribute value must be 'true' or 'false'",
                           hideActions == "true" || hideActions == "false");
                m_hideActions = (hideActions == "true");
            }
        }
    }
}

//-----------------------------------------------------------------------------

void IMenuSrv::create()
{
    ::fwGui::container::fwMenu::sptr menu = m_registrar->getParent();
    std::vector< ::fwGui::IMenuItemCallback::sptr > callbacks = m_registrar->getCallbacks();

    SLM_ASSERT("Parent menu is unknown.", menu);
    m_layoutManager->setCallbacks(callbacks);

    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
        {
            m_layoutManager->createLayout(menu);
        })).wait();

    m_registrar->manage(m_layoutManager->getMenuItems());
    m_registrar->manage(m_layoutManager->getMenus());
}

//-----------------------------------------------------------------------------

void IMenuSrv::destroy()
{
    m_registrar->unmanage();
    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
        {
            m_layoutManager->destroyLayout();
        })).wait();
}

//-----------------------------------------------------------------------------

void IMenuSrv::actionServiceStopping(std::string actionSrvSID)
{
    ::fwGui::container::fwMenuItem::sptr menuItem = m_registrar->getFwMenuItem(actionSrvSID,
                                                                               m_layoutManager->getMenuItems());

    if (m_hideActions)
    {
        ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
            {
                m_layoutManager->menuItemSetVisible(menuItem, false);
            })).wait();
    }
    else
    {
        ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
            {
                m_layoutManager->menuItemSetEnabled(menuItem, false);
            })).wait();
    }
}

//-----------------------------------------------------------------------------

void IMenuSrv::actionServiceStarting(std::string actionSrvSID)
{
    ::fwGui::container::fwMenuItem::sptr menuItem = m_registrar->getFwMenuItem(actionSrvSID,
                                                                               m_layoutManager->getMenuItems());

    if (m_hideActions)
    {
        ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
            {
                m_layoutManager->menuItemSetVisible(menuItem, true);
            })).wait();
    }
    else
    {
        ::fwServices::IService::sptr service = ::fwServices::get( actionSrvSID );
        ::fwGui::IActionSrv::sptr actionSrv  = ::fwGui::IActionSrv::dynamicCast(service);

        ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
            {
                m_layoutManager->menuItemSetEnabled(menuItem, actionSrv->getIsExecutable());
                m_layoutManager->menuItemSetChecked(menuItem, actionSrv->getIsActive());
                m_layoutManager->menuItemSetVisible(menuItem, actionSrv->isVisible());
            })).wait();
    }
}

//-----------------------------------------------------------------------------

void IMenuSrv::actionServiceSetActive(std::string actionSrvSID, bool isActive)
{
    ::fwGui::container::fwMenuItem::sptr menuItem = m_registrar->getFwMenuItem(actionSrvSID,
                                                                               m_layoutManager->getMenuItems());

    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >( [&]
        {
            m_layoutManager->menuItemSetChecked(menuItem, isActive);
        })).wait();
}

//-----------------------------------------------------------------------------

void IMenuSrv::actionServiceSetExecutable(std::string actionSrvSID, bool isExecutable)
{
    ::fwGui::container::fwMenuItem::sptr menuItem = m_registrar->getFwMenuItem(actionSrvSID,
                                                                               m_layoutManager->getMenuItems());

    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
        {
            m_layoutManager->menuItemSetEnabled(menuItem, isExecutable);
        })).wait();
}

//-----------------------------------------------------------------------------

void IMenuSrv::actionServiceSetVisible(std::string actionSrvSID, bool isVisible)
{
    ::fwGui::container::fwMenuItem::sptr menuItem = m_registrar->getFwMenuItem(actionSrvSID,
                                                                               m_layoutManager->getMenuItems());

    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
        {
            m_layoutManager->menuItemSetVisible(menuItem, isVisible);
        })).wait();
}

//-----------------------------------------------------------------------------

void IMenuSrv::initializeLayoutManager(ConfigurationType layoutConfig)
{
    OSLM_ASSERT("Bad configuration name "<<layoutConfig->getName()<< ", must be layout",
                layoutConfig->getName() == "layout");

    ::fwGui::GuiBaseObject::sptr guiObj = ::fwGui::factory::New(
        ::fwGui::layoutManager::IMenuLayoutManager::REGISTRY_KEY);
    m_layoutManager = ::fwGui::layoutManager::IMenuLayoutManager::dynamicCast(guiObj);
    OSLM_ASSERT("ClassFactoryRegistry failed for class "<< ::fwGui::layoutManager::IMenuLayoutManager::REGISTRY_KEY,
                m_layoutManager);

    m_layoutManager->initialize(layoutConfig);
}

//-----------------------------------------------------------------------------

}
