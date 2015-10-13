/*
    OpenSR - opensource multi-genre game based upon "Space Rangers 2: Dominators"
    Copyright (C) 2015 Kosyak <ObKo@mail.ru>

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

#include "OpenSR/QMLHelper.h"

#include <OpenSR/Engine.h>
#include <OpenSR/ResourceManager.h>
#include <QIODevice>

namespace OpenSR
{
namespace QML
{
QMLHelper::QMLHelper(QObject* parent): QObject(parent)
{

}

QVariant QMLHelper::questInfo(const QUrl& url)
{
    QIODevice *d = qobject_cast<Engine*>(qApp)->resources()->getIODevice(url);
    if (!d || !d->isOpen())
        return QVariant();

    QVariant result = convertQuestInfoToJS(QM::readQuestInfo(d));

    delete d;

    return result;
}

QVariant QMLHelper::convertQuestInfoToJS(const QM::QuestInfo& info)
{
    QVariantMap map;

    map["races"] = info.races;
    map["doneImmediately"] = info.doneImmediately;
    map["planetRaces"] = info.planetRaces;
    map["playerTypes"] = info.playerTypes;
    map["playerRaces"] = info.playerRaces;
    map["relation"] = info.relation;
    map["difficulty"] = info.difficulty;
    map["winnerText"] = info.winnerText;
    map["descriptionText"] = info.descriptionText;

    return map;
}
}
}

