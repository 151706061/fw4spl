/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2016.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUI_BUILDER_ISLIDEVIEWBUILDER_HPP__
#define __FWGUI_BUILDER_ISLIDEVIEWBUILDER_HPP__

#include "fwGui/GuiBaseObject.hpp"
#include "fwGui/container/fwContainer.hpp"
#include "fwGui/config.hpp"

#include <fwRuntime/ConfigurationElement.hpp>

namespace fwGui
{

namespace builder
{

/**
 * @brief   Defines the interface class for the slide view builder.
 *
 * Example of configuration
 * @code{.xml}
    <slideView align="top" size="200" opacity="1.0">
        <styleSheet>color: blue; background-color: yellow</styleSheet>
    </slideView>
   @endcode
 *      - \b align (optional, default=left) : defines the widget side ('left', 'right', 'top' or 'bottom')
 *      - \b size (optional, default=200) : defines the widget width or height (width if side is 'left' or 'right' and
 *           height if side is 'top' or 'bottom')
 *      - \b opacity (optional, default=1.0) : defines the widget opacity
 *      - \b styleSheet (optional) : defines the style of the widget (see Qt style sheets
 *           http://doc.qt.io/qt-5/stylesheet-examples.html )
 */
class FWGUI_CLASS_API ISlideViewBuilder : public ::fwGui::GuiBaseObject
{
public:
    fwCoreNonInstanciableClassDefinitionsMacro( (ISlideViewBuilder)(::fwGui::GuiBaseObject) )

    typedef std::string RegistryKeyType;

    FWGUI_API const static RegistryKeyType REGISTRY_KEY;

    enum Aligment
    {
        TOP,
        BOTTOM,
        RIGHT,
        LEFT
    };


    /// Constructor. Do nothing.
    FWGUI_API ISlideViewBuilder();

    /// Destructor. Do nothing.
    FWGUI_API virtual ~ISlideViewBuilder();

    /**
     * @brief Returns the builded tool bar.
     */
    FWGUI_API virtual ::fwGui::container::fwContainer::sptr getContainer() const;

    /**
     * @brief Initialize the tool bar.
     */
    FWGUI_API virtual void initialize( ::fwRuntime::ConfigurationElement::sptr configuration);

    /**
     * @brief Instantiate layout with parent toolBar.
     * @pre LayoutManager must be initialized before.
     * @pre parent toolBar must be instanced.
     */
    FWGUI_API virtual void createContainer( ::fwGui::container::fwContainer::sptr parent ) = 0;

    /**
     * @brief Destroy local layout with sub containers.
     * @pre services using this sub containers must be stopped before.
     */
    FWGUI_API virtual void destroyContainer() = 0;

protected:

    ::fwGui::container::fwContainer::sptr m_container;

    int m_size;
    double m_opacity;
    Aligment m_aligment;
    std::string m_styleSheet;
};

} // namespace builder
} // namespace fwGui

#endif /*__FWGUI_BUILDER_ISLIDEVIEWBUILDER_HPP__*/
