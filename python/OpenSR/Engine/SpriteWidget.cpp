/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2013 Kosyak <ObKo@mail.ru>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <boost/python.hpp>
#include <OpenSR/SpriteWidget.h>

#include <OpenSR/Sprite.h>

#include "OpenSR/python/WidgetWrap.h"

namespace Rangers
{
namespace Python
{
struct SpriteWidgetWrap : SpriteWidget, boost::python::wrapper<SpriteWidget>
{
    SpriteWidgetWrap(): SpriteWidget()
    {
    }
    RANGERS_PYTHON_WRAP_WIDGET(SpriteWidget)
};

void exportSpriteWidget()
{
    using namespace boost::python;
    class_<SpriteWidgetWrap, bases<Widget>, boost::shared_ptr<SpriteWidgetWrap>, boost::noncopyable> c("SpriteWidget", init<>());
    c.def("sprite", &SpriteWidget::sprite)
    .def("setSprite", &SpriteWidget::setSprite);
    RANGERS_PYTHON_WRAP_WIDGET_DEF(SpriteWidget, SpriteWidgetWrap, c);
    register_ptr_to_python<boost::shared_ptr<SpriteWidget> >();
}
}
}