//
// Created by luke on 12-10-22.
//

#ifndef WOR_WORLD_KINEMATICA_SLIDER_H
#define WOR_WORLD_KINEMATICA_SLIDER_H

#include "Config.hpp"
#include "Widgets.hpp"

#include <functional>
#include <string>

namespace Application
{
    template<typename ActionFunction>
    Slider* makeSlider(Window* aParent, int32_t minValue, int32_t maxValue, const ActionFunction anActionFunction)
    {
        Slider* slider = new Slider(aParent,
                                    DEFAULT_ID,
                                    0,
                                    minValue,
                                    maxValue,
                                    wxDefaultPosition,
                                    wxDefaultSize,
                                    wxSL_HORIZONTAL | wxSL_VALUE_LABEL);
        slider->Bind(wxEVT_SCROLL_THUMBRELEASE, anActionFunction);
        return slider;
    }
}// namespace Application

#endif//WOR_WORLD_KINEMATICA_SLIDER_H
