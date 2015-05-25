/*****************************************************************************
 * Copyright: 2013 Michael Zanetti <michael_zanetti@gmx.net>                 *
 *                                                                           *
 * This file is part of ubuntu-authenticator                                 *
 *                                                                           *
 * This prject is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation, either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This project is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.     *
 *                                                                           *
 ****************************************************************************/

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.ListItems 1.0
import Ubuntu.Components.Popups 1.0

MainView {

    applicationName: "guh.guru"

    width: units.gu(40)
    height: units.gu(68)

    useDeprecatedToolbar: false

    Component.onCompleted: {
        Theme.name = "Ubuntu.Components.Themes.SuruDark"
    }

    PageStack {
        id: pageStack

        Component.onCompleted: pageStack.push(mainPage)
    }

    Component {
        id: mainPage
        Page {
            title: "Gear Up your Phone"
        }
    }
}
