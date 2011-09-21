#include <QtCore/QDebug>

#include <synthclone/error.h>

#include "participant.h"

// Static functions

void
Participant::handleError(const char *message)
{
    qWarning() << message;
}

void
Participant::handleInfo(const char *message)
{
    qDebug() << message;
}

void
Participant::ignoreMessage(const char */*message*/)
{
    // Empty
}

// Class definition

Participant::Participant(QObject *parent):
    synthclone::Participant(tr("JACK"), 0, 0, 1, "Devin Anderson",
                            tr("Sampling via the JACK Audio Connection Kit"),
                            parent),
    addSamplerAction(tr("JACK Sampler"))
{
    jack_set_error_function(&ignoreMessage);
    jack_set_info_function(&ignoreMessage);
    connect(&addSamplerAction, SIGNAL(triggered()),
            SLOT(handleSamplerAdditionRequest()));
    connect(&sampleRateChangeView, SIGNAL(closeRequest()),
            SLOT(handleSampleRateChangeViewCloseRequest()));
    connect(&sampleRateChangeView, SIGNAL(sampleRateChangeRequest()),
            SLOT(handleSampleRateChangeViewChangeRequest()));
    context = 0;
}

Participant::~Participant()
{
    // Empty
}

void
Participant::activate(synthclone::Context &context, const QVariant &/*state*/)
{
    context.addMenuAction(&addSamplerAction, synthclone::MENU_ADD_SAMPLER);
    this->context = &context;
}

void
Participant::addSampler(bool convertSampleRate)
{
    jack_set_error_function(&handleError);
    jack_set_info_function(&handleInfo);
    try {
        Sampler *sampler =
            new Sampler(tr("JACK Sampler"),
                        sessionId.isEmpty() ? 0 : sessionId.data(), this);
        QScopedPointer<Sampler> samplerPtr(sampler);
        synthclone::SampleRate serverSampleRate = sampler->getSampleRate();
        synthclone::SampleRate sessionSampleRate = context->getSampleRate();
        if (serverSampleRate != sessionSampleRate) {
            if (convertSampleRate) {
                context->setSampleRate(serverSampleRate);
                goto activateSampler;
            }
            sampleRateChangeView.setVisible(true);
        } else {
        activateSampler:
            connect(sampler, SIGNAL(fatalError(QString)),
                    SLOT(handleFatalError(QString)));
            connect(sampler, SIGNAL(sampleRateChanged()),
                    SLOT(handleJACKSampleRateChange()));
            connect(sampler,
                    SIGNAL(sessionEvent(jack_client_t *,
                                        jack_session_event_t *)),
                    SLOT(handleSessionEvent(jack_client_t *,
                                            jack_session_event_t *)));
            sampler->activate(context->getSampleChannelCount());
            const synthclone::Registration &registration =
                context->addSampler(sampler);
            connect(&registration, SIGNAL(unregistered(QObject *)),
                    SLOT(handleSamplerUnregistration(QObject *)));
            samplerPtr.take();
            sessionId.clear();
            return;
        }
    } catch (synthclone::Error &e) {
        context->reportError(e.getMessage());
    }
    jack_set_error_function(&ignoreMessage);
    jack_set_info_function(&ignoreMessage);
}

void
Participant::deactivate(synthclone::Context &context)
{
    context.removeMenuAction(&addSamplerAction);
    this->context = 0;
}

QVariant
Participant::getState(const synthclone::Sampler */*sampler*/) const
{
    QVariantMap map;
    if (! sessionId.isEmpty()) {
        map.insert("sessionId", sessionId);
    }
    return map;
}

void
Participant::handleFatalError(const QString &message)
{
    context->removeSampler();
    context->reportError(message);
}

void
Participant::handleJACKSampleRateChange()
{
    Sampler *sampler = qobject_cast<Sampler *>(sender());
    if (sampler->getSampleRate() != context->getSampleRate()) {
        context->removeSampler();

        // XXX: Do more than this.  Offer to change the sample rate of the
        // session and restart the sampler.
        context->reportError(tr("The sampler has been removed.  The JACK "
                                "server sample rate has changed."));
    }
}

void
Participant::handleSamplerAdditionRequest()
{
    addSampler(false);
}

void
Participant::handleSampleRateChangeViewChangeRequest()
{
    sampleRateChangeView.setVisible(false);
    addSampler(true);
}

void
Participant::handleSampleRateChangeViewCloseRequest()
{
    sampleRateChangeView.setVisible(false);
}

void
Participant::handleSamplerUnregistration(QObject *obj)
{
    delete qobject_cast<Sampler *>(obj);
    jack_set_error_function(&ignoreMessage);
    jack_set_info_function(&ignoreMessage);
}

void
Participant::handleSessionEvent(jack_client_t *client,
                                jack_session_event_t *event)
{
    const char *commandLine = "synthclone ${SESSION_DIR}";
    event->command_line = new char[strlen(commandLine) + 1];
    QScopedArrayPointer<char> commandLinePtr(event->command_line);
    strcpy(event->command_line, commandLine);
    sessionId = event->client_uuid;
    try {
        context->saveSession(QDir(event->session_dir));
    } catch (synthclone::Error &e) {
        event->flags = JackSessionSaveError;
        context->reportError(e.getMessage());
    }
    sessionId.clear();
    if (jack_session_reply(client, event)) {
        context->reportError(tr("failed to send session reply to JACK server"));
    } else if ((event->flags != JackSessionSaveError) &&
               (event->type == JackSessionSaveAndQuit)) {
        context->quitSession();
    }
    event->command_line = 0;
    jack_session_event_free(event);
}

void
Participant::restoreSampler(const QVariant &state)
{
    qDebug() << "Participant::restoreSampler";

    sessionId = state.toMap().value("sessionId", QByteArray()).toByteArray();
    addSampler(false);

    qDebug() << "Participant::restoreSampler - exiting";
}