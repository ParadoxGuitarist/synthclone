#include <cassert>

#include "context.h"
#include "session.h"
#include "signalmap.h"

// Session signals

static const char *ADDING_EFFECT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingEffect(const synthclone::Effect *, int)));
static const char *EFFECT_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectAdded(const synthclone::Effect *, int)));
static const char *EFFECT_MOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectMoved(const synthclone::Effect *, int, int)));
static const char *EFFECT_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectRemoved(const synthclone::Effect *, int)));
static const char *MOVING_EFFECT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(movingEffect(const synthclone::Effect *, int, int)));
static const char *REMOVING_EFFECT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingEffect(const synthclone::Effect *, int)));

static const char *ADDING_SAMPLER_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingSampler(const synthclone::Sampler *)));
static const char *REMOVING_SAMPLER_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingSampler(const synthclone::Sampler *)));
static const char *SAMPLER_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(samplerAdded(const synthclone::Sampler *)));
static const char *SAMPLER_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(samplerRemoved(const synthclone::Sampler *)));

static const char *ADDING_TARGET_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingTarget(const synthclone::Target *, int)));
static const char *TARGET_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetAdded(const synthclone::Target *, int)));
static const char *TARGET_MOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetMoved(const synthclone::Target *, int, int)));
static const char *TARGET_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetRemoved(const synthclone::Target *, int)));
static const char *MOVING_TARGET_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(movingTarget(const synthclone::Target *, int, int)));
static const char *REMOVING_TARGET_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingTarget(const synthclone::Target *, int)));

static const char *ADDING_EFFECT_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingEffectJob(const synthclone::EffectJob *, int)));
static const char *EFFECT_JOB_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectJobAdded(const synthclone::EffectJob *, int)));
static const char *EFFECT_JOB_MOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectJobMoved(const synthclone::EffectJob *, int, int)));
static const char *EFFECT_JOB_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(effectJobRemoved(const synthclone::EffectJob *, int)));
static const char *MOVING_EFFECT_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(movingEffectJob(const synthclone::EffectJob *, int, int)));
static const char *REMOVING_EFFECT_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingEffectJob(const synthclone::EffectJob *, int)));

static const char *ADDING_SAMPLER_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingSamplerJob(const synthclone::SamplerJob *, int)));
static const char *SAMPLER_JOB_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(samplerJobAdded(const synthclone::SamplerJob *, int)));
static const char *SAMPLER_JOB_MOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(samplerJobMoved(const synthclone::SamplerJob *, int, int)));
static const char *SAMPLER_JOB_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(samplerJobRemoved(const synthclone::SamplerJob *, int)));
static const char *MOVING_SAMPLER_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(movingSamplerJob(const synthclone::SamplerJob *, int, int)));
static const char *REMOVING_SAMPLER_JOB_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingSamplerJob(const synthclone::SamplerJob *, int)));

static const char *SELECTED_EFFECT_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(selectedEffectChanged(const synthclone::Effect *, int)));
static const char *SELECTED_TARGET_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(selectedTargetChanged(const synthclone::Target *, int)));
static const char *CURRENT_EFFECT_JOB_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(currentEffectJobChanged(const synthclone::EffectJob *)));
static const char *CURRENT_SAMPLER_JOB_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(currentSamplerJobChanged(const synthclone::SamplerJob *)));
static const char *FOCUSED_COMPONENT_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(focusedComponentChanged(const synthclone::Component *)));

static const char *ADDING_ZONE_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingZone(const synthclone::Zone *, int)));
static const char *MOVING_ZONE_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(movingZone(const synthclone::Zone *, int, int)));
static const char *REMOVING_ZONE_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingZone(const synthclone::Zone *, int)));
static const char *ZONE_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(zoneAdded(const synthclone::Zone *, int)));
static const char *ZONE_MOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(zoneMoved(const synthclone::Zone *, int, int)));
static const char *ZONE_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(zoneRemoved(const synthclone::Zone *, int)));

static const char *ZONE_SELECTION_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(zoneSelectionChanged(const synthclone::Zone *, bool)));

static const char *ADDING_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuAction(synthclone::Menu,
                             const synthclone::MenuAction *,
                             const QStringList &)));
static const char *ADDING_EFFECT_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuAction(const synthclone::Effect *,
                             const synthclone::MenuAction *,
                             const QStringList &)));
static const char *ADDING_SAMPLER_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuAction(const synthclone::Sampler *,
                             const synthclone::MenuAction *,
                             const QStringList &)));
static const char *ADDING_TARGET_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuAction(const synthclone::Target *,
                             const synthclone::MenuAction *,
                             const QStringList &)));

static const char *ADDING_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuSeparator(synthclone::Menu,
                                const synthclone::MenuSeparator *,
                                const QStringList &)));
static const char *ADDING_EFFECT_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuSeparator(const synthclone::Effect *,
                                const synthclone::MenuSeparator *,
                                const QStringList &)));
static const char *ADDING_SAMPLER_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuSeparator(const synthclone::Sampler *,
                                const synthclone::MenuSeparator *,
                                const QStringList &)));
static const char *ADDING_TARGET_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingMenuSeparator(const synthclone::Target *,
                                const synthclone::MenuSeparator *,
                                const QStringList &)));

static const char *MENU_ACTION_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionAdded(synthclone::Menu,
                            const synthclone::MenuAction *,
                            const QStringList &)));
static const char *EFFECT_MENU_ACTION_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionAdded(const synthclone::Effect *,
                            const synthclone::MenuAction *,
                            const QStringList &)));
static const char *SAMPLER_MENU_ACTION_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionAdded(const synthclone::Sampler *,
                            const synthclone::MenuAction *,
                            const QStringList &)));
static const char *TARGET_MENU_ACTION_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionAdded(const synthclone::Target *,
                            const synthclone::MenuAction *,
                            const QStringList &)));

static const char *MENU_ACTION_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionRemoved(synthclone::Menu,
                              const synthclone::MenuAction *,
                              const QStringList &)));
static const char *EFFECT_MENU_ACTION_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionRemoved(const synthclone::Effect *,
                              const synthclone::MenuAction *,
                              const QStringList &)));
static const char *SAMPLER_MENU_ACTION_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionRemoved(const synthclone::Sampler *,
                              const synthclone::MenuAction *,
                              const QStringList &)));
static const char *TARGET_MENU_ACTION_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuActionRemoved(const synthclone::Target *,
                              const synthclone::MenuAction *,
                              const QStringList &)));

static const char *MENU_SEPARATOR_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorAdded(synthclone::Menu,
                               const synthclone::MenuSeparator *,
                               const QStringList &)));
static const char *EFFECT_MENU_SEPARATOR_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorAdded(const synthclone::Effect *,
                               const synthclone::MenuSeparator *,
                               const QStringList &)));
static const char *SAMPLER_MENU_SEPARATOR_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorAdded(const synthclone::Sampler *,
                               const synthclone::MenuSeparator *,
                               const QStringList &)));
static const char *TARGET_MENU_SEPARATOR_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorAdded(const synthclone::Target *,
                               const synthclone::MenuSeparator *,
                               const QStringList &)));

static const char *MENU_SEPARATOR_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorRemoved(synthclone::Menu,
                                 const synthclone::MenuSeparator *,
                                 const QStringList &)));
static const char *EFFECT_MENU_SEPARATOR_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorRemoved(const synthclone::Effect *,
                                 const synthclone::MenuSeparator *,
                                 const QStringList &)));
static const char *SAMPLER_MENU_SEPARATOR_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorRemoved(const synthclone::Sampler *,
                                 const synthclone::MenuSeparator *,
                                 const QStringList &)));
static const char *TARGET_MENU_SEPARATOR_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(menuSeparatorRemoved(const synthclone::Target *,
                                 const synthclone::MenuSeparator *,
                                 const QStringList &)));

static const char *REMOVING_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuAction(synthclone::Menu,
                               const synthclone::MenuAction *,
                               const QStringList &)));
static const char *REMOVING_EFFECT_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuAction(const synthclone::Effect *,
                               const synthclone::MenuAction *,
                               const QStringList &)));
static const char *REMOVING_SAMPLER_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuAction(const synthclone::Sampler *,
                               const synthclone::MenuAction *,
                               const QStringList &)));
static const char *REMOVING_TARGET_MENU_ACTION_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuAction(const synthclone::Target *,
                               const synthclone::MenuAction *,
                               const QStringList &)));

static const char *REMOVING_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuSeparator(synthclone::Menu,
                                  const synthclone::MenuSeparator *,
                                  const QStringList &)));
static const char *REMOVING_EFFECT_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuSeparator(const synthclone::Effect *,
                                  const synthclone::MenuSeparator *,
                                  const QStringList &)));
static const char *REMOVING_SAMPLER_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuSeparator(const synthclone::Sampler *,
                                  const synthclone::MenuSeparator *,
                                  const QStringList &)));
static const char *REMOVING_TARGET_MENU_SEPARATOR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingMenuSeparator(const synthclone::Target *,
                                  const synthclone::MenuSeparator *,
                                  const QStringList &)));

static const char *BUILDING_TARGETS_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(buildingTargets()));
static const char *SAVING_TARGET_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(savingTarget(const synthclone::Target *)));
static const char *TARGET_BUILDING_COMPLETED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetBuildingCompleted()));
static const char *TARGET_SAVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetSaved(const synthclone::Target *)));
static const char *TARGET_SAVE_ERROR_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetSaveError(const synthclone::Target *, const QString &)));
static const char *TARGET_VALIDATION_COMPLETED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(targetValidationCompleted(const synthclone::Target *)));
static const char *VALIDATING_TARGET_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(validatingTarget(const synthclone::Target *)));

static const char *ACTIVATING_PARTICIPANT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(activatingParticipant(const synthclone::Participant *,
                                  const synthclone::Participant *,
                                  const QByteArray &)));
static const char *ADDING_PARTICIPANT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(addingParticipant(const synthclone::Participant *,
                              const synthclone::Participant *,
                              const QByteArray &)));
static const char *DEACTIVATING_PARTICIPANT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(deactivatingParticipant(const synthclone::Participant *,
                                    const synthclone::Participant *,
                                    const QByteArray &)));
static const char *PARTICIPANT_ACTIVATED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(participantActivated(const synthclone::Participant *,
                                 const synthclone::Participant *,
                                 const QByteArray &)));
static const char *PARTICIPANT_ADDED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(participantAdded(const synthclone::Participant *,
                             const synthclone::Participant *,
                             const QByteArray &)));
static const char *PARTICIPANT_DEACTIVATED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(participantDeactivated(const synthclone::Participant *,
                                   const synthclone::Participant *,
                                   const QByteArray &)));
static const char *PARTICIPANT_REMOVED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(participantRemoved(const synthclone::Participant *,
                               const synthclone::Participant *,
                               const QByteArray &)));
static const char *REMOVING_PARTICIPANT_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(removingParticipant(const synthclone::Participant *,
                                const synthclone::Participant *,
                                const QByteArray &)));

static const char *AFTERTOUCH_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(aftertouchPropertyVisibilityChanged(bool)));
static const char *CHANNEL_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(channelPropertyVisibilityChanged(bool)));
static const char *CONTROL_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(controlPropertyVisibilityChanged(synthclone::MIDIData, bool)));
static const char *DRY_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(drySamplePropertyVisibilityChanged(bool)));
static const char *NOTE_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(notePropertyVisibilityChanged(bool)));
static const char *RELEASE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(releaseTimePropertyVisibilityChanged(bool)));
static const char *SAMPLE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(sampleTimePropertyVisibilityChanged(bool)));
static const char *STATUS_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(statusPropertyVisibilityChanged(bool)));
static const char *VELOCITY_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(velocityPropertyVisibilityChanged(bool)));
static const char *WET_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(wetSamplePropertyVisibilityChanged(bool)));

static const char *SAMPLE_CHANNEL_COUNT_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(sampleChannelCountChanged(synthclone::SampleChannelCount)));
static const char *SAMPLE_RATE_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(sampleRateChanged(synthclone::SampleRate)));

static const char *SESSION_STATE_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(sessionStateChanged(synthclone::SessionState, const QDir *)));

static const char *STATE_CHANGED_SIGNAL =
    QMetaObject::normalizedSignature
    (SIGNAL(stateChanged(synthclone::SessionState, const QDir *)));

static SignalMap sessionSignalMap =
    SignalMap() <<

    SignalPair(QLatin1String(ADDING_EFFECT_SIGNAL),
               ADDING_EFFECT_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_ADDED_SIGNAL),
               EFFECT_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_MOVED_SIGNAL),
               EFFECT_MOVED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_REMOVED_SIGNAL),
               EFFECT_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(MOVING_EFFECT_SIGNAL),
               MOVING_EFFECT_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_EFFECT_SIGNAL),
               REMOVING_EFFECT_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_SAMPLER_SIGNAL),
               ADDING_SAMPLER_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_ADDED_SIGNAL),
               SAMPLER_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_REMOVED_SIGNAL),
               SAMPLER_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_SAMPLER_SIGNAL),
               REMOVING_SAMPLER_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_TARGET_SIGNAL),
               ADDING_TARGET_SIGNAL) <<
    SignalPair(QLatin1String(MOVING_TARGET_SIGNAL),
               MOVING_TARGET_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_TARGET_SIGNAL),
               REMOVING_TARGET_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_ADDED_SIGNAL),
               TARGET_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_MOVED_SIGNAL),
               TARGET_MOVED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_REMOVED_SIGNAL),
               TARGET_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_EFFECT_JOB_SIGNAL),
               ADDING_EFFECT_JOB_SIGNAL) <<
    SignalPair(QLatin1String(MOVING_EFFECT_JOB_SIGNAL),
               MOVING_EFFECT_JOB_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_EFFECT_JOB_SIGNAL),
               REMOVING_EFFECT_JOB_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_JOB_ADDED_SIGNAL),
               EFFECT_JOB_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_JOB_MOVED_SIGNAL),
               EFFECT_JOB_MOVED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_JOB_REMOVED_SIGNAL),
               EFFECT_JOB_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_SAMPLER_JOB_SIGNAL),
               ADDING_SAMPLER_JOB_SIGNAL) <<
    SignalPair(QLatin1String(MOVING_SAMPLER_JOB_SIGNAL),
               MOVING_SAMPLER_JOB_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_SAMPLER_JOB_SIGNAL),
               REMOVING_SAMPLER_JOB_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_JOB_ADDED_SIGNAL),
               SAMPLER_JOB_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_JOB_MOVED_SIGNAL),
               SAMPLER_JOB_MOVED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_JOB_REMOVED_SIGNAL),
               SAMPLER_JOB_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(SELECTED_EFFECT_CHANGED_SIGNAL),
               SELECTED_EFFECT_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(SELECTED_TARGET_CHANGED_SIGNAL),
               SELECTED_TARGET_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(CURRENT_EFFECT_JOB_CHANGED_SIGNAL),
               CURRENT_EFFECT_JOB_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(CURRENT_SAMPLER_JOB_CHANGED_SIGNAL),
               CURRENT_SAMPLER_JOB_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(FOCUSED_COMPONENT_CHANGED_SIGNAL),
               FOCUSED_COMPONENT_CHANGED_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_ZONE_SIGNAL),
               ADDING_ZONE_SIGNAL) <<
    SignalPair(QLatin1String(MOVING_ZONE_SIGNAL),
               MOVING_ZONE_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_ZONE_SIGNAL),
               REMOVING_ZONE_SIGNAL) <<
    SignalPair(QLatin1String(ZONE_ADDED_SIGNAL),
               ZONE_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(ZONE_MOVED_SIGNAL),
               ZONE_MOVED_SIGNAL) <<
    SignalPair(QLatin1String(ZONE_REMOVED_SIGNAL),
               ZONE_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(ZONE_SELECTION_CHANGED_SIGNAL),
               ZONE_SELECTION_CHANGED_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_MENU_ACTION_SIGNAL),
               ADDING_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_EFFECT_MENU_ACTION_SIGNAL),
               ADDING_EFFECT_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_SAMPLER_MENU_ACTION_SIGNAL),
               ADDING_SAMPLER_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_TARGET_MENU_ACTION_SIGNAL),
               ADDING_TARGET_MENU_ACTION_SIGNAL) <<

    SignalPair(QLatin1String(ADDING_MENU_SEPARATOR_SIGNAL),
               ADDING_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_EFFECT_MENU_SEPARATOR_SIGNAL),
               ADDING_EFFECT_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_SAMPLER_MENU_SEPARATOR_SIGNAL),
               ADDING_SAMPLER_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_TARGET_MENU_SEPARATOR_SIGNAL),
               ADDING_TARGET_MENU_SEPARATOR_SIGNAL) <<

    SignalPair(QLatin1String(MENU_ACTION_ADDED_SIGNAL),
               MENU_ACTION_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_MENU_ACTION_ADDED_SIGNAL),
               EFFECT_MENU_ACTION_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_MENU_ACTION_ADDED_SIGNAL),
               SAMPLER_MENU_ACTION_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_MENU_ACTION_ADDED_SIGNAL),
               TARGET_MENU_ACTION_ADDED_SIGNAL) <<

    SignalPair(QLatin1String(MENU_ACTION_REMOVED_SIGNAL),
               MENU_ACTION_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_MENU_ACTION_REMOVED_SIGNAL),
               EFFECT_MENU_ACTION_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_MENU_ACTION_REMOVED_SIGNAL),
               SAMPLER_MENU_ACTION_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_MENU_ACTION_REMOVED_SIGNAL),
               TARGET_MENU_ACTION_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(MENU_SEPARATOR_ADDED_SIGNAL),
               MENU_SEPARATOR_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_MENU_SEPARATOR_ADDED_SIGNAL),
               EFFECT_MENU_SEPARATOR_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_MENU_SEPARATOR_ADDED_SIGNAL),
               SAMPLER_MENU_SEPARATOR_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_MENU_SEPARATOR_ADDED_SIGNAL),
               TARGET_MENU_SEPARATOR_ADDED_SIGNAL) <<

    SignalPair(QLatin1String(MENU_SEPARATOR_REMOVED_SIGNAL),
               MENU_SEPARATOR_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(EFFECT_MENU_SEPARATOR_REMOVED_SIGNAL),
               EFFECT_MENU_SEPARATOR_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLER_MENU_SEPARATOR_REMOVED_SIGNAL),
               SAMPLER_MENU_SEPARATOR_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_MENU_SEPARATOR_REMOVED_SIGNAL),
               TARGET_MENU_SEPARATOR_REMOVED_SIGNAL) <<

    SignalPair(QLatin1String(REMOVING_MENU_ACTION_SIGNAL),
               REMOVING_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_EFFECT_MENU_ACTION_SIGNAL),
               REMOVING_EFFECT_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_SAMPLER_MENU_ACTION_SIGNAL),
               REMOVING_SAMPLER_MENU_ACTION_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_TARGET_MENU_ACTION_SIGNAL),
               REMOVING_TARGET_MENU_ACTION_SIGNAL) <<

    SignalPair(QLatin1String(REMOVING_MENU_SEPARATOR_SIGNAL),
               REMOVING_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_EFFECT_MENU_SEPARATOR_SIGNAL),
               REMOVING_EFFECT_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_SAMPLER_MENU_SEPARATOR_SIGNAL),
               REMOVING_SAMPLER_MENU_SEPARATOR_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_TARGET_MENU_SEPARATOR_SIGNAL),
               REMOVING_TARGET_MENU_SEPARATOR_SIGNAL) <<

    SignalPair(QLatin1String(BUILDING_TARGETS_SIGNAL),
               BUILDING_TARGETS_SIGNAL) <<
    SignalPair(QLatin1String(SAVING_TARGET_SIGNAL),
               SAVING_TARGET_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_BUILDING_COMPLETED_SIGNAL),
               TARGET_BUILDING_COMPLETED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_SAVED_SIGNAL),
               TARGET_SAVED_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_SAVE_ERROR_SIGNAL),
               TARGET_SAVE_ERROR_SIGNAL) <<
    SignalPair(QLatin1String(TARGET_VALIDATION_COMPLETED_SIGNAL),
               TARGET_VALIDATION_COMPLETED_SIGNAL) <<
    SignalPair(QLatin1String(VALIDATING_TARGET_SIGNAL),
               VALIDATING_TARGET_SIGNAL) <<

    SignalPair(QLatin1String(ACTIVATING_PARTICIPANT_SIGNAL),
               ACTIVATING_PARTICIPANT_SIGNAL) <<
    SignalPair(QLatin1String(ADDING_PARTICIPANT_SIGNAL),
               ADDING_PARTICIPANT_SIGNAL) <<
    SignalPair(QLatin1String(DEACTIVATING_PARTICIPANT_SIGNAL),
               DEACTIVATING_PARTICIPANT_SIGNAL) <<
    SignalPair(QLatin1String(PARTICIPANT_ACTIVATED_SIGNAL),
               PARTICIPANT_ACTIVATED_SIGNAL) <<
    SignalPair(QLatin1String(PARTICIPANT_ADDED_SIGNAL),
               PARTICIPANT_ADDED_SIGNAL) <<
    SignalPair(QLatin1String(PARTICIPANT_DEACTIVATED_SIGNAL),
               PARTICIPANT_DEACTIVATED_SIGNAL) <<
    SignalPair(QLatin1String(PARTICIPANT_REMOVED_SIGNAL),
               PARTICIPANT_REMOVED_SIGNAL) <<
    SignalPair(QLatin1String(REMOVING_PARTICIPANT_SIGNAL),
               REMOVING_PARTICIPANT_SIGNAL) <<

    SignalPair(QLatin1String(AFTERTOUCH_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               AFTERTOUCH_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(CHANNEL_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               CHANNEL_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(CONTROL_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               CONTROL_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(DRY_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               DRY_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(NOTE_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               NOTE_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(RELEASE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               RELEASE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               SAMPLE_TIME_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(STATUS_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               STATUS_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(VELOCITY_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               VELOCITY_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(WET_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL),
               WET_SAMPLE_PROPERTY_VISIBILITY_CHANGED_SIGNAL) <<

    SignalPair(QLatin1String(SAMPLE_CHANNEL_COUNT_CHANGED_SIGNAL),
               SAMPLE_CHANNEL_COUNT_CHANGED_SIGNAL) <<
    SignalPair(QLatin1String(SAMPLE_RATE_CHANGED_SIGNAL),
               SAMPLE_RATE_CHANGED_SIGNAL) <<

    SignalPair(QLatin1String(STATE_CHANGED_SIGNAL),
               SESSION_STATE_CHANGED_SIGNAL);

// And now, back to our regularly scheduled class definition.

Context::Context(synthclone::Participant &participant, Session &session,
                 QObject *parent):
    synthclone::Context(parent),
    participant(participant),
    session(session)
{
    sampler = 0;
}

Context::~Context()
{
    int i;

    for (i = participants.count() - 1; i >= 0; i--) {
        session.removeParticipant(participants[i]);
    }
    if (sampler) {
        session.removeSampler();
    }
    for (i = effects.count() - 1; i >= 0; i--) {
        session.removeEffect(effects[i]);
    }
    for (i = targets.count() - 1; i >= 0; i--) {
        session.removeTarget(targets[i]);
    }

    // Removing components created in this context will remove all of the menu
    // items associated with those components; however, there may also be menu
    // items associated with components that weren't created in this context.

    for (i = menuActions.count() - 1; i >= 0; i--) {
        session.removeMenuAction(menuActions[i]);
    }
    for (i = menuSeparators.count() - 1; i >= 0; i--) {
        session.removeMenuSeparator(menuSeparators[i]);
    }

    assert(! effects.count());
    assert(! menuActions.count());
    assert(! menuSeparators.count());
    assert(! participants.count());
    assert(! sampler);
    assert(! targets.count());
}

void
Context::abortCurrentSamplerJob()
{
    session.abortCurrentSamplerJob();
}

void
Context::activateParticipant(const synthclone::Participant *participant)
{
    session.activateParticipant(participant);
}

const synthclone::Registration &
Context::addEffect(synthclone::Effect *effect, int index)
{
    const synthclone::Registration &registration =
        session.addEffect(effect, &participant, index);
    effects.append(effect);
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleEffectRemoval(QObject *)));
    return registration;
}

const synthclone::EffectJob *
Context::addEffectJob(synthclone::Zone *zone)
{
    return session.addEffectJob(zone);
}

template<typename T>
const synthclone::Registration &
Context::addMenuAction(synthclone::MenuAction *action, T rootMenuItem,
                       const QStringList &subMenus)
{
    const synthclone::Registration &registration =
        session.addMenuAction(action, rootMenuItem, subMenus);
    menuActions.append(action);
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleMenuActionRemoval(QObject *)));
    return registration;
}

const synthclone::Registration &
Context::addMenuAction(synthclone::MenuAction *action, synthclone::Menu menu,
                       const QStringList &subMenus)
{
    return addMenuAction<synthclone::Menu>(action, menu, subMenus);
}

const synthclone::Registration &
Context::addMenuAction(synthclone::MenuAction *action,
                       const synthclone::Effect *effect,
                       const QStringList &subMenus)
{
    return addMenuAction<const synthclone::Effect *>(action, effect, subMenus);
}

const synthclone::Registration &
Context::addMenuAction(synthclone::MenuAction *action,
                       const synthclone::Sampler *sampler,
                       const QStringList &subMenus)
{
    return addMenuAction<const synthclone::Sampler *>(action, sampler,
                                                      subMenus);
}

const synthclone::Registration &
Context::addMenuAction(synthclone::MenuAction *action,
                       const synthclone::Target *target,
                       const QStringList &subMenus)
{
    return addMenuAction<const synthclone::Target *>(action, target, subMenus);
}

template<typename T>
const synthclone::Registration &
Context::addMenuSeparator(synthclone::MenuSeparator *separator, T rootMenuItem,
                          const QStringList &subMenus)
{
    const synthclone::Registration &registration =
        session.addMenuSeparator(separator, rootMenuItem, subMenus);
    menuSeparators.append(separator);
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleMenuSeparatorRemoval(QObject *)));
    return registration;
}

const synthclone::Registration &
Context::addMenuSeparator(synthclone::MenuSeparator *separator,
                          synthclone::Menu menu, const QStringList &subMenus)
{
    return addMenuSeparator<synthclone::Menu>(separator, menu, subMenus);
}

const synthclone::Registration &
Context::addMenuSeparator(synthclone::MenuSeparator *separator,
                          const synthclone::Effect *effect,
                          const QStringList &subMenus)
{
    return addMenuSeparator<const synthclone::Effect *>(separator, effect,
                                                        subMenus);
}

const synthclone::Registration &
Context::addMenuSeparator(synthclone::MenuSeparator *separator,
                          const synthclone::Sampler *sampler,
                          const QStringList &subMenus)
{
    return addMenuSeparator<const synthclone::Sampler *>(separator, sampler,
                                                         subMenus);
}

const synthclone::Registration &
Context::addMenuSeparator(synthclone::MenuSeparator *separator,
                          const synthclone::Target *target,
                          const QStringList &subMenus)
{
    return addMenuSeparator<const synthclone::Target *>(separator, target,
                                                        subMenus);
}

const synthclone::Registration &
Context::addParticipant(synthclone::Participant *participant,
                        const QByteArray &subId)
{
    const synthclone::Registration &registration =
        session.addParticipant(participant, &(this->participant), subId);
    participants.append(participant);
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleParticipantRemoval(QObject *)));
    return registration;
}

const synthclone::Registration &
Context::addSampler(synthclone::Sampler *sampler)
{
    const synthclone::Registration &registration =
        session.addSampler(sampler, &participant);
    this->sampler = sampler;
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleSamplerRemoval(QObject *)));
    return registration;
}

const synthclone::SamplerJob *
Context::addSamplerJob(synthclone::SamplerJob::Type type,
                       synthclone::Zone *zone)
{
    return session.addSamplerJob(type, zone);
}

const synthclone::Registration &
Context::addTarget(synthclone::Target *target, int index)
{
    const synthclone::Registration &registration =
        session.addTarget(target, &participant, index);
    targets.append(target);
    connect(&registration, SIGNAL(unregistered(QObject *)),
            SLOT(handleTargetRemoval(QObject *)));
    return registration;
}

synthclone::Zone *
Context::addZone(int index)
{
    return session.addZone(index);
}

void
Context::buildTargets()
{
    session.buildTargets();
}

void
Context::connectNotify(const char *signal)
{
    QObject::connectNotify(signal);
    if (receivers(signal) == 1) {
        QLatin1String strSignal(signal);
        if (sessionSignalMap.contains(strSignal)) {
            connect(&session, signal, sessionSignalMap[strSignal]);
        }
    }
}

void
Context::createSession(const QDir &directory, synthclone::SampleRate sampleRate,
                       synthclone::SampleChannelCount count)
{
    Session::create(directory, sampleRate, count);
}

void
Context::deactivateParticipant(const synthclone::Participant *participant)
{
    session.deactivateParticipant(participant);
}

void
Context::disconnectNotify(const char *signal)
{
    QObject::disconnectNotify(signal);
    if (! receivers(signal)) {
        QLatin1String strSignal(signal);
        if (sessionSignalMap.contains(strSignal)) {
            disconnect(&session, signal, this, sessionSignalMap[strSignal]);
        }
    }
}

const synthclone::EffectJob *
Context::getCurrentEffectJob() const
{
    return session.getCurrentEffectJob();
}

const synthclone::SamplerJob *
Context::getCurrentSamplerJob() const
{
    return session.getCurrentSamplerJob();
}

const synthclone::Effect *
Context::getEffect(int index) const
{
    return session.getEffect(index);
}

int
Context::getEffectCount() const
{
    return session.getEffectCount();
}

int
Context::getEffectIndex(const synthclone::Effect *effect) const
{
    return session.getEffectIndex(effect);
}

const synthclone::EffectJob *
Context::getEffectJob(int index) const
{
    return session.getEffectJob(index);
}

int
Context::getEffectJobCount() const
{
    return session.getEffectJobCount();
}

int
Context::getEffectJobIndex(const synthclone::EffectJob *job) const
{
    return session.getEffectJobIndex(job);
}

const synthclone::Component *
Context::getFocusedComponent() const
{
    return session.getFocusedComponent();
}

int
Context::getMajorVersion() const
{
    return session.getMajorVersion();
}

int
Context::getMinorVersion() const
{
    return session.getMinorVersion();
}

const synthclone::Participant *
Context::getParticipant(const QByteArray &id) const
{
    return session.getParticipant(id);
}

const synthclone::Participant *
Context::getParticipant(int index, const synthclone::Participant *parent) const
{
    return session.getParticipant(index, parent);
}

int
Context::getParticipantCount(const synthclone::Participant *parent) const
{
    return session.getParticipantCount(parent);
}

QByteArray
Context::getParticipantId(const synthclone::Participant *participant) const
{
    return session.getParticipantId(participant);
}

const synthclone::Participant *
Context::getParticipantParent(const synthclone::Participant *participant) const
{
    return session.getParticipantParent(participant);
}

int
Context::getRevision() const
{
    return session.getRevision();
}

synthclone::SampleChannelCount
Context::getSampleChannelCount() const
{
    return session.getSampleChannelCount();
}

const synthclone::Sampler *
Context::getSampler() const
{
    return session.getSampler();
}

synthclone::SampleRate
Context::getSampleRate() const
{
    return session.getSampleRate();
}

const synthclone::SamplerJob *
Context::getSamplerJob(int index) const
{
    return session.getSamplerJob(index);
}

int
Context::getSamplerJobCount() const
{
    return session.getSamplerJobCount();
}

int
Context::getSamplerJobIndex(const synthclone::SamplerJob *job) const
{
    return session.getSamplerJobIndex(job);
}

const synthclone::Effect *
Context::getSelectedEffect() const
{
    return session.getSelectedEffect();
}

const synthclone::Target *
Context::getSelectedTarget() const
{
    return session.getSelectedTarget();
}

synthclone::Zone *
Context::getSelectedZone(int index)
{
    return session.getSelectedZone(index);
}

int
Context::getSelectedZoneCount() const
{
    return session.getSelectedZoneCount();
}

const QDir *
Context::getSessionDirectory() const
{
    return session.getDirectory();
}

synthclone::SessionState
Context::getSessionState() const
{
    return session.getState();
}

const synthclone::Target *
Context::getTarget(int index) const
{
    return session.getTarget(index);
}

int
Context::getTargetCount() const
{
    return session.getTargetCount();
}

int
Context::getTargetIndex(const synthclone::Target *target) const
{
    return session.getTargetIndex(target);
}

synthclone::Zone *
Context::getZone(int index)
{
    return session.getZone(index);
}

int
Context::getZoneCount() const
{
    return session.getZoneCount();
}

int
Context::getZoneIndex(const synthclone::Zone *zone) const
{
    return session.getZoneIndex(zone);
}

void
Context::handleEffectRemoval(QObject *obj)
{
    synthclone::Effect *effect = qobject_cast<synthclone::Effect *>(obj);
    assert(effect);
    bool removed = effects.removeOne(effect);
    assert(removed);
}

void
Context::handleMenuActionRemoval(QObject *obj)
{
    synthclone::MenuAction *action =
        qobject_cast<synthclone::MenuAction *>(obj);
    assert(action);
    bool removed = menuActions.removeOne(action);
    assert(removed);
}

void
Context::handleMenuSeparatorRemoval(QObject *obj)
{
    synthclone::MenuSeparator *separator =
        qobject_cast<synthclone::MenuSeparator *>(obj);
    assert(separator);
    bool removed = menuSeparators.removeOne(separator);
    assert(removed);
}

void
Context::handleParticipantRemoval(QObject *obj)
{
    synthclone::Participant *participant =
        qobject_cast<synthclone::Participant *>(obj);
    assert(participant);
    bool removed = participants.removeOne(participant);
    assert(removed);
}

void
Context::handleSamplerRemoval(QObject *obj)
{
    synthclone::Sampler *sampler = qobject_cast<synthclone::Sampler *>(obj);
    assert(sampler);
    assert(this->sampler == sampler);
    this->sampler = 0;
}

void
Context::handleTargetRemoval(QObject *obj)
{
    synthclone::Target *target = qobject_cast<synthclone::Target *>(obj);
    assert(target);
    bool removed = targets.removeOne(target);
    assert(removed);
}

bool
Context::isAftertouchPropertyVisible() const
{
    return session.isAftertouchPropertyVisible();
}

bool
Context::isChannelPropertyVisible() const
{
    return session.isChannelPropertyVisible();
}

bool
Context::isControlPropertyVisible(synthclone::MIDIData control) const
{
    return session.isControlPropertyVisible(control);
}

bool
Context::isDrySamplePropertyVisible() const
{
    return session.isDrySamplePropertyVisible();
}

bool
Context::isNotePropertyVisible() const
{
    return session.isNotePropertyVisible();
}

bool
Context::
isParticipantActivated(const synthclone::Participant *participant) const
{
    return session.isParticipantActivated(participant);
}

bool
Context::isReleaseTimePropertyVisible() const
{
    return session.isReleaseTimePropertyVisible();
}

bool
Context::isSampleTimePropertyVisible() const
{
    return session.isSampleTimePropertyVisible();
}

bool
Context::isSessionDirectory(const QDir &directory) const
{
    return Session::isDirectory(directory);
}

bool
Context::isStatusPropertyVisible() const
{
    return session.isStatusPropertyVisible();
}

bool
Context::isVelocityPropertyVisible() const
{
    return session.isVelocityPropertyVisible();
}

bool
Context::isWetSamplePropertyVisible() const
{
    return session.isWetSamplePropertyVisible();
}

bool
Context::isZoneSelected(const synthclone::Zone *zone) const
{
    return session.isZoneSelected(zone);
}

bool
Context::isZoneSelected(int index) const
{
    return session.isZoneSelected(index);
}

void
Context::loadSession(const QDir &directory)
{
    session.load(directory);
}

void
Context::moveEffect(int fromIndex, int toIndex)
{
    session.moveEffect(fromIndex, toIndex);
}

void
Context::moveEffectJob(int fromIndex, int toIndex)
{
    session.moveEffectJob(fromIndex, toIndex);
}

void
Context::moveSamplerJob(int fromIndex, int toIndex)
{
    session.moveSamplerJob(fromIndex, toIndex);
}

void
Context::moveTarget(int fromIndex, int toIndex)
{
    session.moveTarget(fromIndex, toIndex);
}

void
Context::moveZone(int fromIndex, int toIndex)
{
    session.moveZone(fromIndex, toIndex);
}

void
Context::quitSession()
{
    session.quit();
}

void
Context::removeEffect(const synthclone::Effect *effect)
{
    session.removeEffect(effect);
}

void
Context::removeEffect(int index)
{
    session.removeEffect(index);
}

void
Context::removeEffectJob(const synthclone::EffectJob *job)
{
    session.removeEffectJob(job);
}

void
Context::removeEffectJob(int index)
{
    session.removeEffectJob(index);
}

void
Context::removeMenuAction(const synthclone::MenuAction *action)
{
    session.removeMenuAction(action);
}

void
Context::removeMenuSeparator(const synthclone::MenuSeparator *separator)
{
    session.removeMenuSeparator(separator);
}

void
Context::removeParticipant(const synthclone::Participant *participant)
{
    session.removeParticipant(participant);
}

void
Context::removeSampler()
{
    session.removeSampler();
}

void
Context::removeSamplerJob(const synthclone::SamplerJob *job)
{
    session.removeSamplerJob(job);
}

void
Context::removeSamplerJob(int index)
{
    session.removeSamplerJob(index);
}

void
Context::removeTarget(const synthclone::Target *target)
{
    session.removeTarget(target);
}

void
Context::removeTarget(int index)
{
    session.removeTarget(index);
}

void
Context::removeZone(synthclone::Zone *zone)
{
    session.removeZone(zone);
}

void
Context::removeZone(int index)
{
    session.removeZone(index);
}

void
Context::reportError(const QString &message)
{
    session.reportError(message);
}

void
Context::saveSession()
{
    session.save();
}

void
Context::saveSession(const QDir &directory)
{
    session.save(directory);
}

void
Context::setAftertouchPropertyVisible(bool visible)
{
    session.setAftertouchPropertyVisible(visible);
}

void
Context::setChannelPropertyVisible(bool visible)
{
    session.setChannelPropertyVisible(visible);
}

void
Context::setControlPropertyVisible(synthclone::MIDIData control, bool visible)
{
    session.setControlPropertyVisible(control, visible);
}

void
Context::setDrySamplePropertyVisible(bool visible)
{
    session.setDrySamplePropertyVisible(visible);
}

void
Context::setFocusedComponent(const synthclone::Component *component)
{
    session.setFocusedComponent(component);
}

void
Context::setNotePropertyVisible(bool visible)
{
    session.setNotePropertyVisible(visible);
}

void
Context::setReleaseTimePropertyVisible(bool visible)
{
    session.setReleaseTimePropertyVisible(visible);
}

void
Context::setSampleChannelCount(synthclone::SampleChannelCount count)
{
    session.setSampleChannelCount(count);
}

void
Context::setSampleRate(synthclone::SampleRate sampleRate)
{
    session.setSampleRate(sampleRate);
}

void
Context::setSampleTimePropertyVisible(bool visible)
{
    session.setSampleTimePropertyVisible(visible);
}

void
Context::setSelectedEffect(const synthclone::Effect *effect)
{
    session.setSelectedEffect(effect);
}

void
Context::setSelectedEffect(int index)
{
    session.setSelectedEffect(index);
}

void
Context::setSelectedTarget(const synthclone::Target *target)
{
    session.setSelectedTarget(target);
}

void
Context::setSelectedTarget(int index)
{
    session.setSelectedTarget(index);
}

void
Context::setSessionModified()
{
    session.setModified();
}

void
Context::setStatusPropertyVisible(bool visible)
{
    session.setStatusPropertyVisible(visible);
}

void
Context::setVelocityPropertyVisible(bool visible)
{
    session.setVelocityPropertyVisible(visible);
}

void
Context::setWetSamplePropertyVisible(bool visible)
{
    session.setWetSamplePropertyVisible(visible);
}

void
Context::setZoneSelected(const synthclone::Zone *zone, bool selected)
{
    session.setZoneSelected(zone, selected);
}

void
Context::setZoneSelected(int index, bool selected)
{
    session.setZoneSelected(index, selected);
}

void
Context::sortZones(const synthclone::ZoneComparer &comparer, bool ascending)
{
    session.sortZones(comparer, ascending);
}

void
Context::unloadSession()
{
    session.unload();
}