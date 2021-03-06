/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#include "fwRuntime/BundleElement.hpp"
#include "fwRuntime/Bundle.hpp"

#include <cassert>
#include <memory>


namespace fwRuntime
{

//------------------------------------------------------------------------------

BundleElement::BundleElement() :   m_bundle( Bundle::getLoadingBundle() ), m_enable(true)
{
    // Post-condition

    SLM_ASSERT("bundle '" << m_bundle->getIdentifier() << "' not initialized", m_bundle != 0 );
}

//------------------------------------------------------------------------------

BundleElement::BundleElement( std::shared_ptr< Bundle > bundle )
    :   m_bundle( bundle ), m_enable(true)
{
    // Post-condition
    SLM_ASSERT("bundle '" << m_bundle->getIdentifier() << "' not initialized", m_bundle != 0 );
}

//------------------------------------------------------------------------------

std::shared_ptr<Bundle> BundleElement::getBundle() const
{
    return m_bundle;
}

//------------------------------------------------------------------------------

bool BundleElement::isEnable() const
{
    // Pre-condition.
    SLM_ASSERT("bundle '" << m_bundle->getIdentifier() << "' not initialized", m_bundle != 0 );
    return m_bundle->isEnable() && m_enable;
}

//------------------------------------------------------------------------------

void BundleElement::setEnable(const bool enable)
{
    m_enable = enable;
}

//------------------------------------------------------------------------------

} // namespace fwRuntime
