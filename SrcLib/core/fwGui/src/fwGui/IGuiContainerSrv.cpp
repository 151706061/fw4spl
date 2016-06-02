/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwGui/IGuiContainerSrv.hpp"
#include "fwGui/registry/worker.hpp"

#include <fwCom/Slot.hpp>
#include <fwCom/Slot.hxx>
#include <fwCom/Slots.hpp>
#include <fwCom/Slots.hxx>

#include <fwCore/base.hpp>
#include <fwServices/macros.hpp>
#include <fwTools/fwID.hpp>

namespace fwGui
{

const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_SET_ENABLED_SLOT = "setEnabled";
const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_ENABLE_SLOT      = "enable";
const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_DISABLE_SLOT     = "disable";
const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_SET_VISIBLE_SLOT = "setVisible";
const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_SHOW_SLOT        = "show";
const ::fwCom::Slots::SlotKeyType IGuiContainerSrv::s_HIDE_SLOT        = "hide";

//-----------------------------------------------------------------------------

IGuiContainerSrv::IGuiContainerSrv()
    : m_viewLayoutManagerIsCreated (false),
      m_hasToolBar(false)
{
    newSlot(s_SET_ENABLED_SLOT, &IGuiContainerSrv::setEnabled, this);
    newSlot(s_ENABLE_SLOT, &IGuiContainerSrv::enable, this);
    newSlot(s_DISABLE_SLOT, &IGuiContainerSrv::disable, this);
    newSlot(s_SET_VISIBLE_SLOT, &IGuiContainerSrv::setVisible, this);
    newSlot(s_SHOW_SLOT, &IGuiContainerSrv::show, this);
    newSlot(s_HIDE_SLOT,&IGuiContainerSrv::hide, this);
}

//-----------------------------------------------------------------------------

IGuiContainerSrv::~IGuiContainerSrv()
{
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::initialize()
{
    SLM_ASSERT("Service hasn't configuration", m_configuration);

    // Create view registrar
    m_viewRegistrar = ::fwGui::registrar::ViewRegistrar::New( this->getID() );
    // find ViewRegistrar configuration
    std::vector < ConfigurationType > vectViewMng = m_configuration->find("registry");
    if ( !vectViewMng.empty() )
    {
        m_viewRegistrarConfig = vectViewMng.at(0);
        m_viewRegistrar->initialize(m_viewRegistrarConfig);
    }

    // Create initializeLayoutManager
    // find gui configuration
    std::vector < ConfigurationType > vectGui = m_configuration->find("gui");
    if( !vectGui.empty() )
    {
        SLM_ASSERT("No <registry> tag is allowed in the <gui> section", vectGui.at(0)->find("registry").empty());

        // find view LayoutManager configuration
        std::vector < ConfigurationType > vectLayoutMng = vectGui.at(0)->find("layout");
        if( !vectLayoutMng.empty() )
        {
            m_viewLayoutConfig = vectLayoutMng.at(0);
            this->initializeLayoutManager( m_viewLayoutConfig );
            m_viewLayoutManagerIsCreated = true;
        }

        // find toolBarBuilder configuration
        std::vector < ConfigurationType > vectTBBuilder = vectGui.at(0)->find("toolBar");
        if(!vectTBBuilder.empty())
        {
            m_toolBarConfig = vectTBBuilder.at(0);
            this->initializeToolBarBuilder(m_toolBarConfig);

            m_hasToolBar = true;
        }
    }
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::create()
{
    ::fwGui::container::fwContainer::sptr parent = m_viewRegistrar->getParent();
    SLM_ASSERT("Parent container is unknown.", parent);

    ::fwGui::registry::worker::get()->postTask< void >(::boost::function< void() >([this, &parent]
        {
            SLM_ASSERT("ViewRegistrar must be initialized.", m_viewRegistrar);

            ::fwGui::GuiBaseObject::sptr guiObj =
                ::fwGui::factory::New(::fwGui::builder::IContainerBuilder::REGISTRY_KEY);
            m_containerBuilder = ::fwGui::builder::IContainerBuilder::dynamicCast(guiObj);

            OSLM_ASSERT("ClassFactoryRegistry failed for class "<< ::fwGui::builder::IContainerBuilder::REGISTRY_KEY,
                        m_containerBuilder);
            m_containerBuilder->createContainer(parent);

            ::fwGui::container::fwContainer::sptr container = m_containerBuilder->getContainer();

            if ( m_viewLayoutManagerIsCreated )
            {
                if (m_hasToolBar)
                {
                    ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
                    {
                        m_toolBarBuilder->createToolBar(parent);
                    })).wait();

                    m_viewRegistrar->manageToolBar(m_toolBarBuilder->getToolBar());
                }

                ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
                {
                    m_viewLayoutManager->createLayout(container);
                })).wait();

                m_viewRegistrar->manage(m_viewLayoutManager->getSubViews());
            }
        })).wait();
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::destroy()
{
    SLM_ASSERT("ViewRegistrar must be initialized.", m_viewRegistrar);

    if ( m_viewLayoutManagerIsCreated )
    {
        if (m_hasToolBar)
        {
            m_viewRegistrar->unmanageToolBar();
            SLM_ASSERT("ToolBarBuilder must be initialized.", m_toolBarBuilder);

            ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
                {
                    m_toolBarBuilder->destroyToolBar();
                })).wait();
        }

        m_viewRegistrar->unmanage();
        SLM_ASSERT("ViewLayoutManager must be initialized.", m_viewLayoutManager);

        ::fwGui::registry::worker::get()->postTask<void>(::boost::function< void() >([&]
            {
                m_viewLayoutManager->destroyLayout();
            })).wait();
    }

    m_containerBuilder->destroyContainer();
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::initializeLayoutManager(ConfigurationType layoutConfig)
{
    OSLM_ASSERT("Bad configuration name "<<layoutConfig->getName()<< ", must be layout",
                layoutConfig->getName() == "layout");
    SLM_ASSERT("<layout> tag must have type attribute", layoutConfig->hasAttribute("type"));
    const std::string layoutManagerClassName = layoutConfig->getAttributeValue("type");

    ::fwGui::GuiBaseObject::sptr guiObj = ::fwGui::factory::New(layoutManagerClassName);
    m_viewLayoutManager                 = ::fwGui::layoutManager::IViewLayoutManager::dynamicCast(guiObj);
    OSLM_ASSERT("ClassFactoryRegistry failed for class "<< layoutManagerClassName, m_viewLayoutManager);

    m_viewLayoutManager->initialize(layoutConfig);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::initializeToolBarBuilder(ConfigurationType toolBarConfig)
{
    OSLM_ASSERT("Bad configuration name "<<toolBarConfig->getName()<< ", must be toolBar",
                toolBarConfig->getName() == "toolBar");

    ::fwGui::GuiBaseObject::sptr guiObj = ::fwGui::factory::New(::fwGui::builder::IToolBarBuilder::REGISTRY_KEY);
    m_toolBarBuilder                    = ::fwGui::builder::IToolBarBuilder::dynamicCast(guiObj);
    OSLM_ASSERT("ClassFactoryRegistry failed for class "<< ::fwGui::builder::IToolBarBuilder::REGISTRY_KEY,
                m_toolBarBuilder);

    m_toolBarBuilder->initialize(toolBarConfig);
}

//-----------------------------------------------------------------------------

::fwGui::container::fwContainer::sptr IGuiContainerSrv::getContainer()
{
    return m_containerBuilder->getContainer();
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::setParent(std::string wid)
{
    ::fwGui::registry::worker::get()->postTask< void >(::boost::function< void() >([this, &wid]
        {
            m_viewRegistrar->setParent(wid);
            ::fwGui::container::fwContainer::sptr parent = m_viewRegistrar->getParent();
            SLM_ASSERT("Parent container is unknown.", parent);
            m_containerBuilder->setParent(parent);
        } ));
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::setEnabled(bool isEnabled)
{
    ::fwGui::container::fwContainer::sptr container = m_viewRegistrar->getParent();
    container->setEnabled(isEnabled);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::enable()
{
    this->setEnabled(true);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::disable()
{
    this->setEnabled(false);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::setVisible(bool isVisible)
{
    ::fwGui::container::fwContainer::sptr container = m_viewRegistrar->getParent();
    container->setVisible(isVisible);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::show()
{
    this->setVisible(true);
}

//-----------------------------------------------------------------------------

void IGuiContainerSrv::hide()
{
    this->setVisible(false);
}

//-----------------------------------------------------------------------------

}
