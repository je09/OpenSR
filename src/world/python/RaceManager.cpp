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
#include "RaceManager.h"

#include <OpenSR/Types.h>
#include <map>


namespace Rangers
{
namespace World
{
namespace Python
{
void exportRaceManager()
{
    using namespace boost::python;

    class_<Race, boost::shared_ptr<Race> >("Race", init<>())
    .def_readwrite("id", &Race::id)
    .def_readwrite("name", &Race::name)
    .def_readwrite("icon", &Race::icon)
    .def_readwrite("invader", &Race::invader)
    .def_readwrite("relations", &Race::relations);

    class_<RaceManager, boost::noncopyable>("RaceManager", boost::python::no_init)
    .def("id", (boost::shared_ptr<Race> (RaceManager::*)(uint32_t) const)&RaceManager::race)
    .def("id", (boost::shared_ptr<Race> (RaceManager::*)(const std::string&) const)&RaceManager::race);
}
}
}
}