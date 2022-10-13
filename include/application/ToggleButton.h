//
// Created by luke on 12-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_TOGGLEBUTTON_H
#define WOR_WORLD_KINEMATICA_TOGGLEBUTTON_H

#include "Config.hpp"
#include "Widgets.hpp"

#include <functional>
#include <string>

namespace Application
{
    /**
     * Utility function that creates a ToggleButton with aLabel that binds the given ActionFunction with the
	 * wxEVT_COMMAND_TOGGLEBUTTON_CLICKED of the button.
     *
     * @tparam ActionFunction
     * @param aParent
     * @param aLabel
     * @param anActionFunction
     * @return
     */
    template<typename ActionFunction>
    ToggleButton* makeToggleButton(Window* aParent, const std::string& aLabel, const ActionFunction anActionFunction)
    {
        ToggleButton* button = new ToggleButton(aParent, DEFAULT_ID, WXSTRING(aLabel));
        button->Bind(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, anActionFunction);
        return button;
    }
}// namespace Application

#endif//WOR_WORLD_KINEMATICA_TOGGLEBUTTON_H
