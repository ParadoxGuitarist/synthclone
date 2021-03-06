/*
 * libsynthclone_zonegenerator - Zone generation plugin for `synthclone`
 * Copyright (C) 2011 Devin Anderson
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 * Ave, Cambridge, MA 02139, USA.
 */

#include <cassert>

#include "data.h"

Data::Data(QObject *parent):
    QObject(parent)
{
    reset();
}

Data::~Data()
{
    // Empty
}

synthclone::MIDIData
Data::getAftertouchLayers() const
{
    return aftertouchLayers;
}

synthclone::MIDIData
Data::getChannelPressureLayers() const
{
    return channelPressureLayers;
}

synthclone::MIDIData
Data::getFirstNote() const
{
    return firstNote;
}

synthclone::MIDIData
Data::getLastNote() const
{
    return lastNote;
}

synthclone::MIDIData
Data::getMIDIChannel() const
{
    return midiChannel;
}

synthclone::SampleTime
Data::getReleaseTime() const
{
    return releaseTime;
}

synthclone::SampleTime
Data::getSampleTime() const
{
    return sampleTime;
}

synthclone::MIDIData
Data::getTotalNotes() const
{
    return totalNotes;
}

synthclone::MIDIData
Data::getVelocityLayers() const
{
    return velocityLayers;
}

void
Data::reset()
{
    // Simple defaults
    aftertouchLayers = 0;
    channelPressureLayers = 0;
    firstNote = 0;
    lastNote = 0x7f;
    midiChannel = 1;
    releaseTime = 1.0;
    sampleTime = 5.0;
    totalNotes = 128;
    velocityLayers = 8;
}

void
Data::setAftertouchLayers(synthclone::MIDIData layers)
{
    assert(layers <= 0x80);
    if (aftertouchLayers != layers) {
        aftertouchLayers = layers;
        emit aftertouchLayersChanged(layers);
    }
}

void
Data::setChannelPressureLayers(synthclone::MIDIData layers)
{
    assert(layers <= 0x80);
    if (channelPressureLayers != layers) {
        channelPressureLayers = layers;
        emit channelPressureLayersChanged(layers);
    }
}

void
Data::setFirstNote(synthclone::MIDIData firstNote)
{
    assert(firstNote < 0x80);
    if (this->firstNote != firstNote) {
        this->firstNote = firstNote;
        emit firstNoteChanged(firstNote);
        if (lastNote < firstNote) {
            setLastNote(firstNote);
        }
        updateTotalNotes();
    }
}

void
Data::setLastNote(synthclone::MIDIData lastNote)
{
    assert(lastNote < 0x80);
    if (this->lastNote != lastNote) {
        this->lastNote = lastNote;
        emit lastNoteChanged(lastNote);
        if (firstNote > lastNote) {
            setFirstNote(lastNote);
        }
        updateTotalNotes();
    }
}

void
Data::setMIDIChannel(synthclone::MIDIData midiChannel)
{
    assert((midiChannel >= 1) && (midiChannel <= 16));
    if (this->midiChannel != midiChannel) {
        this->midiChannel = midiChannel;
        emit midiChannelChanged(midiChannel);
    }
}

void
Data::setReleaseTime(synthclone::SampleTime releaseTime)
{
    assert(releaseTime >= 0.0);
    if (this->releaseTime != releaseTime) {
        this->releaseTime = releaseTime;
        emit releaseTimeChanged(releaseTime);
    }
}

void
Data::setSampleTime(synthclone::SampleTime sampleTime)
{
    assert(sampleTime >= 0.0);
    if (this->sampleTime != sampleTime) {
        this->sampleTime = sampleTime;
        emit sampleTimeChanged(sampleTime);
    }
}

void
Data::setTotalNotes(synthclone::MIDIData totalNotes)
{
    assert((totalNotes > 0) && (totalNotes <= 0x80));
    if (this->totalNotes != totalNotes) {
        this->totalNotes = totalNotes;
        emit totalNotesChanged(totalNotes);
        synthclone::MIDIData currentMaxTotal = (lastNote - firstNote) + 1;
        if (totalNotes > currentMaxTotal) {
            synthclone::MIDIData lastNoteMaxSteps = 0x7f - lastNote;
            synthclone::MIDIData steps = totalNotes - currentMaxTotal;
            if (lastNoteMaxSteps > steps) {
                setLastNote(lastNote + steps);
            } else {
                setLastNote(0x7f);
                setFirstNote(firstNote - (steps - lastNoteMaxSteps));
            }
        }
    }
}

void
Data::setVelocityLayers(synthclone::MIDIData layers)
{
    assert((layers > 0) && (layers < 0x80));
    if (velocityLayers != layers) {
        velocityLayers = layers;
        emit velocityLayersChanged(layers);
    }
}

void
Data::updateTotalNotes()
{
    synthclone::MIDIData maximumNotes = (lastNote - firstNote) + 1;
    if (totalNotes > maximumNotes) {
        setTotalNotes(maximumNotes);
    }
}
