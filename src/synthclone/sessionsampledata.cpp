/*
 * synthclone - Synthesizer-cloning software
 * Copyright (C) 2011-2013 Devin Anderson
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

#include <synthclone/util.h>
#include <synthclone/sampleinputstream.h>
#include <synthclone/sampleoutputstream.h>

#include "samplerateconverter.h"
#include "sessionsampledata.h"
#include "util.h"

SessionSampleData::SessionSampleData(QObject *parent):
    QObject(parent)
{
    sampleChannelCount = 2;
    sampleDirectory = 0;
    sampleRate = synthclone::SAMPLE_RATE_NOT_SET;
}

SessionSampleData::~SessionSampleData()
{
    if (sampleDirectory) {
        delete sampleDirectory;
    }
}

synthclone::SampleChannelCount
SessionSampleData::getSampleChannelCount() const
{
    return sampleChannelCount;
}

const QDir *
SessionSampleData::getSampleDirectory() const
{
    return sampleDirectory;
}

synthclone::SampleRate
SessionSampleData::getSampleRate() const
{
    return sampleRate;
}

void
SessionSampleData::setSampleChannelCount(synthclone::SampleChannelCount count)
{
    CONFIRM(count > 0, tr("sample channel count cannot be 0"));
    if (this->sampleChannelCount != count) {
        this->sampleChannelCount = count;
        emit sampleChannelCountChanged(count);
    }
}

void
SessionSampleData::setSampleDirectory(const QDir *directory)
{
    QDir *oldDirectory = sampleDirectory;
    if (oldDirectory != directory) {
        if (oldDirectory) {
            if (directory) {
                if (oldDirectory->absolutePath() == directory->absolutePath()) {
                    return;
                }
            }
            delete oldDirectory;
        }
        sampleDirectory = directory ? new QDir(*directory) : 0;
        emit sampleDirectoryChanged(sampleDirectory);
    }
}

void
SessionSampleData::setSampleRate(synthclone::SampleRate sampleRate)
{
    CONFIRM((sampleRate == synthclone::SAMPLE_RATE_NOT_SET) ||
            ((sampleRate >= synthclone::SAMPLE_RATE_MINIMUM) &&
             (sampleRate <= synthclone::SAMPLE_RATE_MAXIMUM)),
            tr("'%1': invalid sample rate").arg(sampleRate));

    if (this->sampleRate != sampleRate) {
        this->sampleRate = sampleRate;
        emit sampleRateChanged(sampleRate);
    }
}

synthclone::Sample *
SessionSampleData::updateSample(synthclone::Sample &sample, bool forceCopy,
                                QObject *parent)
{
    if (! sampleDirectory) {
        // The session is being unloaded.
        return 0;
    }

    synthclone::SampleInputStream inputStream(sample);

    ChannelConvertAlgorithm channelConvertAlgorithm;
    synthclone::SampleChannelCount inputChannels = inputStream.getChannels();
    if (sampleChannelCount == inputChannels) {
        channelConvertAlgorithm = CHANNELCONVERTALGORITHM_NONE;
    } else if (sampleChannelCount == 1) {
        channelConvertAlgorithm = CHANNELCONVERTALGORITHM_TO_MONO;
    } else if (inputChannels == 1) {
        channelConvertAlgorithm = CHANNELCONVERTALGORITHM_FROM_MONO;
    } else {
        // How does one convert a multi-channel sample to a different channel
        // count that isn't mono?  I'm open to ideas.
        return 0;
    }

    // If the sample rate isn't set, then set it to the sample rate of the new
    // sample.
    synthclone::SampleRate inputSampleRate = inputStream.getSampleRate();
    if (sampleRate == synthclone::SAMPLE_RATE_NOT_SET) {
        setSampleRate(inputSampleRate);
    }

    bool sampleConversionRequired = inputSampleRate != sampleRate;
    QString newPath = createUniqueFile(sampleDirectory);
    if ((channelConvertAlgorithm == CHANNELCONVERTALGORITHM_NONE) &&
        (! sampleConversionRequired) && (! forceCopy)) {
        if (sampleDirectory) {
            if (QFileInfo(sample.getPath()).absolutePath() ==
                sampleDirectory->absolutePath()) {
                // Nothing needs to be done.
                return &sample;
            }
        }
    }

    // At this point, either some sort of conversion is required, the sample is
    // being moved from outside the sample directory into the sample directory,
    // or a forced copy was requested.

    float *channelBuffer;

    // For some reason, the empty QScopedArrayPointer constructor is not
    // available on the Mac OSX platform.
    QScopedArrayPointer<float> channelBufferPtr(static_cast<float *>(0));

    float *convertBuffer = new float[sampleChannelCount * 512];
    QScopedArrayPointer<float> convertBufferPtr(convertBuffer);
    SampleRateConverter *converter;
    QScopedPointer<SampleRateConverter> converterPtr;
    float *inputBuffer = new float[inputChannels * 512];
    QScopedArrayPointer<float> inputBufferPtr(inputBuffer);
    if (! sampleConversionRequired) {
        channelBuffer = convertBuffer;
        converter = 0;
    } else {
        if (channelConvertAlgorithm != CHANNELCONVERTALGORITHM_NONE) {
            channelBuffer = new float[sampleChannelCount];
            channelBufferPtr.reset(channelBuffer);
        } else {
            channelBuffer = inputBuffer;
        }
        double ratio = static_cast<double>(sampleRate) / inputSampleRate;
        converter = new SampleRateConverter(sampleChannelCount, ratio, this);
        converterPtr.reset(converter);
    }

    // Create the new sample object.
    synthclone::Sample *outputSample = new synthclone::Sample(newPath, parent);
    QScopedPointer<synthclone::Sample> outputSamplePtr(outputSample);
    synthclone::SampleOutputStream
        outputStream(*outputSample, sampleRate, sampleChannelCount);

    // Convert.
    synthclone::SampleFrameCount framesRead;
    long outputFramesUsed;
    do {

        // Get data from input stream.
        framesRead = inputStream.read(inputBuffer, 512);

        // Channel conversion.
        switch (channelConvertAlgorithm) {
        case CHANNELCONVERTALGORITHM_TO_MONO:
            for (int i = 0; i < framesRead; i++) {
                float n = 0.0;
                for (int j = 0; j < inputChannels; j++) {
                    n += inputBuffer[(i * inputChannels) + j];
                }
                channelBuffer[i] = n / inputChannels;
            }
            break;
        case CHANNELCONVERTALGORITHM_FROM_MONO:
            for (int i = 0; i < framesRead; i++) {
                float n = inputBuffer[i];
                for (int j = 0; j < sampleChannelCount; j++) {
                    channelBuffer[(i * sampleChannelCount) + j] = n;
                }
            }
            break;
        case 0:
            // There's no channel conversion.  Above, we assign the
            // 'channelBuffer' to 'inputBuffer' when there's no channel
            // conversion.  So, the data is already in 'channelBuffer'.
            ;
        }

        // Sample rate conversion.
        if (sampleConversionRequired) {
            long inputFramesUsed =
                converter->convert(channelBuffer, static_cast<long>(framesRead),
                                   convertBuffer, 512, outputFramesUsed,
                                   ! framesRead);
            if (inputFramesUsed != framesRead) {
                inputStream.seek(inputFramesUsed - framesRead,
                                 synthclone::SampleStream::OFFSET_CURRENT);
            }
        } else {
            // If sample rate conversion isn't required, then the data to write
            // is already in 'convertBuffer', as 'convertBuffer' and
            // 'channelBuffer' point to the same memory area.
            outputFramesUsed = framesRead;
        }

        // Write data to output stream.
        if (outputFramesUsed) {
            outputStream.write(convertBuffer,
                               static_cast<synthclone::SampleFrameCount>
                               (outputFramesUsed));
        }

    } while (framesRead);

    // Finish up sample rate conversion.
    if (sampleConversionRequired && outputFramesUsed) {
        for (;;) {
            converter->convert(channelBuffer, 0, convertBuffer, 512,
                               outputFramesUsed, true);
            if (! outputFramesUsed) {
                break;
            }
            outputStream.write(convertBuffer,
                               static_cast<synthclone::SampleFrameCount>
                               (outputFramesUsed));
        }
    }

    // Cleanup.
    outputStream.close();
    return outputSamplePtr.take();
}
