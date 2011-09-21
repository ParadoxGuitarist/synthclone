#ifndef __ZONEVIEWLET_H__
#define __ZONEVIEWLET_H__

#include <QtGui/QMainWindow>
#include <QtGui/QStandardItemModel>
#include <QtGui/QTableView>

#include <synthclone/types.h>
#include <synthclone/zone.h>

#include "contextmenueventfilter.h"
#include "menuviewlet.h"
#include "standarditem.h"
#include "zonetablecolumn.h"
#include "zonetabledelegate.h"
#include "zonetablemodel.h"

class ZoneViewlet: public QObject {

    Q_OBJECT

public:

    explicit
    ZoneViewlet(QMainWindow *mainWindow, QObject *parent=0);

    ~ZoneViewlet();

    MenuViewlet *
    getMenuViewlet();

public slots:

    void
    addZone(int index);

    void
    moveZone(int fromIndex, int toIndex);

    void
    removeZone(int index);

    void
    setAftertouch(int index, synthclone::MIDIData aftertouch);

    void
    setAftertouchPropertyVisible(bool visible);

    void
    setApplyEffectsEnabled(bool enabled);

    void
    setBuildTargetsEnabled(bool enabled);

    void
    setChannel(int index, synthclone::MIDIData channel);

    void
    setChannelPropertyVisible(bool visible);

    void
    setClearSelectionEnabled(bool enabled);

    void
    setControlPropertyVisible(synthclone::MIDIData control, bool visible);

    void
    setControlValue(int index, synthclone::MIDIData control,
                    synthclone::MIDIData value);

    void
    setCopyEnabled(bool enabled);

    void
    setCutEnabled(bool enabled);

    void
    setDeleteEnabled(bool enabled);

    void
    setDrySampleAcquired(int index, bool acquired);

    void
    setDrySamplePropertyVisible(bool visible);

    void
    setDrySampleStale(int index, bool stale);

    void
    setInsertEnabled(bool enabled);

    void
    setInvertSelectionEnabled(bool enabled);

    void
    setNote(int index, synthclone::MIDIData note);

    void
    setNotePropertyVisible(bool visible);

    void
    setPasteEnabled(bool enabled);

    void
    setPlayDrySampleEnabled(bool enabled);

    void
    setPlayWetSampleEnabled(bool enabled);

    void
    setReleaseTime(int index, synthclone::SampleTime releaseTime);

    void
    setReleaseTimePropertyVisible(bool visible);

    void
    setRemoveEffectJobEnabled(bool enabled);

    void
    setRemoveSamplerJobEnabled(bool enabled);

    void
    setSampleEnabled(bool enabled);

    void
    setSampleTime(int index, synthclone::SampleTime sampleTime);

    void
    setSampleTimePropertyVisible(bool visible);

    void
    setSelectAllEnabled(bool enabled);

    void
    setSelected(int index, bool selected);

    void
    setStatus(int index, synthclone::Zone::Status status);

    void
    setStatusPropertyVisible(bool visible);

    void
    setVelocity(int index, synthclone::MIDIData velocity);

    void
    setVelocityPropertyVisible(bool visible);

    void
    setWetSampleAcquired(int index, bool acquired);

    void
    setWetSamplePropertyVisible(bool visible);

    void
    setWetSampleStale(int index, bool stale);

signals:

    void
    aftertouchChangeRequest(int index, synthclone::MIDIData aftertouch);

    void
    aftertouchPropertyVisibilityChangeRequest(bool visible);

    void
    aftertouchPropertySortRequest(bool ascending);

    void
    applyEffectsRequest();

    void
    buildTargetsRequest();

    void
    channelChangeRequest(int index, synthclone::MIDIData channel);

    void
    channelPropertyVisibilityChangeRequest(bool visible);

    void
    channelPropertySortRequest(bool ascending);

    void
    clearSelectionRequest();

    void
    controlPropertyVisibilityChangeRequest(synthclone::MIDIData control,
                                           bool visible);

    void
    controlPropertySortRequest(synthclone::MIDIData control, bool ascending);

    void
    controlValueChangeRequest(int index, synthclone::MIDIData control,
                              synthclone::MIDIData value);

    void
    copyRequest();

    void
    cutRequest();

    void
    deleteRequest();

    void
    drySamplePropertyVisibilityChangeRequest(bool visible);

    void
    drySamplePropertySortRequest(bool ascending);

    void
    insertRequest();

    void
    invertSelectionRequest();

    void
    noteChangeRequest(int index, synthclone::MIDIData note);

    void
    notePropertyVisibilityChangeRequest(bool visible);

    void
    notePropertySortRequest(bool ascending);

    void
    pasteRequest();

    void
    playDrySampleRequest();

    void
    playWetSampleRequest();

    void
    releaseTimeChangeRequest(int index, synthclone::SampleTime releaseTime);

    void
    releaseTimePropertyVisibilityChangeRequest(bool visible);

    void
    releaseTimePropertySortRequest(bool ascending);

    void
    removeEffectJobRequest();

    void
    removeSamplerJobRequest();

    void
    sampleRequest();

    void
    sampleTimeChangeRequest(int index, synthclone::SampleTime sampleTime);

    void
    sampleTimePropertyVisibilityChangeRequest(bool visible);

    void
    sampleTimePropertySortRequest(bool ascending);

    void
    selectAllRequest();

    void
    selectionChangeRequest(int index, bool selected);

    void
    statusPropertyVisibilityChangeRequest(bool visible);

    void
    statusPropertySortRequest(bool ascending);

    void
    velocityChangeRequest(int index, synthclone::MIDIData velocity);

    void
    velocityPropertyVisibilityChangeRequest(bool visible);

    void
    velocityPropertySortRequest(bool ascending);

    void
    wetSamplePropertyVisibilityChangeRequest(bool visible);

    void
    wetSamplePropertySortRequest(bool ascending);

private slots:

    void
    handleColumnShowAction();

    void
    handleContextMenuRequest(int x, int y);

    void
    handleSelectionChange(const QItemSelection &selectedItems,
                          const QItemSelection &deselectedItems);

    void
    handleSortRequest(int column, bool ascending);

private:

    void
    disableRow(int index);

    void
    enableRow(int index);

    QStandardItem *
    getModelItem(int row, int column);

    void
    initializeColumnShowAction(QWidget *widget, int column,
                               const QString &actionId);

    void
    initializeControlColumnShowActions(const QString &menuId,
                                       synthclone::MIDIData start,
                                       synthclone::MIDIData end);

    void
    setColumnVisible(int column, bool visible);

    void
    setItemFlags(QStandardItem *item, Qt::ItemFlags flags);

    void
    setModelData(int row, int column, const QVariant &value,
                 int role=Qt::EditRole);

    QAction *applyEffectsAction;
    QAction *buildTargetsAction;
    QAction *clearSelectionAction;
    ContextMenuEventFilter contextMenuEventFilter;
    QAction *coreColumnShowActions[ZONETABLECOLUMN_BASE_TOTAL];
    QAction *copyAction;
    QAction *cutAction;
    QAction *deleteAction;
    bool emitZoneSelectRequest;
    QAction *insertAction;
    QAction *invertSelectionAction;
    StandardItem *itemPrototype;
    MenuViewlet *menuViewlet;
    QAction *pasteAction;
    QAction *playDrySampleAction;
    QAction *playWetSampleAction;
    QAction *removeEffectJobAction;
    QAction *removeSamplerJobAction;
    QAction *sampleAction;
    QAction *selectAllAction;
    ZoneTableDelegate tableDelegate;
    ZoneTableModel tableModel;
    QItemSelectionModel tableSelectionModel;
    QTableView *tableView;
    QMenu *zonesMenu;

};

#endif