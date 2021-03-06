/*
 * libsynthclone - a plugin API for `synthclone`
 * Copyright (C) 2011 Devin Anderson
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef __SYNTHCLONE_IPLUGIN_H__
#define __SYNTHCLONE_IPLUGIN_H__

#include <QtCore/QtPlugin>

#include <synthclone/participant.h>

namespace synthclone {

    /**
     * This interface is implemented by `synthclone` plugins.
     */

    class IPlugin {

    public:

        virtual
        ~IPlugin() {}

        /**
         * Gets the root Participant id.  The id is used to identify
         * Participant objects, to create child Participant ids, and to
         * save/load Participant and Component data.
         *
         * @par Valid Participant Identifiers
         * A valid Participant id is a dot-delimited string containing one or
         * more sub-id parts.  Each sub-id part must start and end with a
         * number or letter, and contain only numbers, letters, and hyphens.
         *
         * @par
         * The following ids are considered valid:
         *   - a
         *   - a.b
         *   - a.bc
         *   - a.b-c
         *   - a.b--c
         *
         * @par
         * The following ids are considered invalid:
         *   - a.
         *   - .a.b
         *   - a..b
         *   - a.b.
         *   - a.-b
         *   - a.-
         *   - -
         *
         * @par Identifier Recommendations
         * Ids should be formatted using the following sub-ids:
         *   -# A top-level domain appropriate to the plugin distributor.
         *   -# A sub-domain appropriate to the plugin distributor.
         *   -# The sub-id 'synthclone'.
         *   -# The sub-id 'plugins'.
         *   -# A sub-id containing the name of the plugin.
         *
         * @par
         * For example, a plugin named 'test' created by a developer from
         * 'example.com' would have the id
         * 'com.example.synthclone.plugins.test'.
         *
         * @par
         * Ids of plugins distributed within synthclone will take the form
         * 'com.googlecode.synthclone.plugins.[name]'.
         *
         * @par
         * Ids of the form 'com.googlecode.synthclone.*' are reserved for
         * future use.
         *
         * @returns
         *   The root Participant id.
         */

        virtual QByteArray
        getId() const = 0;

        /**
         * Gets the root Participant for this plugin.  The root Participant
         * will be registered with the application.
         *
         * @returns
         *   The root participant.
         */

        virtual Participant *
        getParticipant() = 0;

    };

}

Q_DECLARE_INTERFACE(synthclone::IPlugin,
                    "com.googlecode.synthclone.interfaces.IPlugin/0.1")

#endif
