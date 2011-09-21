#include <cassert>

#include <QtGui/QHeaderView>

#include <synthclone/util.h>

#include "participanttreecolumn.h"
#include "participantview.h"

ParticipantView::ParticipantView(QObject *parent):
    DialogView(":/synthclone/participantview.ui", parent)
{
    closeButton = synthclone::getChild<QPushButton>(dialog, "closeButton");
    connect(closeButton, SIGNAL(clicked()), SIGNAL(closeRequest()));

    treeView = synthclone::getChild<QTreeView>(dialog, "treeView");
    treeView->setModel(&treeModel);
    treeView->setSelectionBehavior(QAbstractItemView::SelectRows);
    treeView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(treeView, SIGNAL(activated(QModelIndex)),
            SLOT(handleTreeViewActivation(QModelIndex)));

    treeModel.setColumnCount(PARTICIPANTTREECOLUMN_TOTAL);
    treeModel.setRowCount(0);
    treeModel.setHeaderData(PARTICIPANTTREECOLUMN_AUTHOR, Qt::Horizontal,
                            tr("Author"));
    treeModel.setHeaderData(PARTICIPANTTREECOLUMN_NAME, Qt::Horizontal,
                            tr("Name"));
    treeModel.setHeaderData(PARTICIPANTTREECOLUMN_STATE, Qt::Horizontal,
                            tr("State"));
    treeModel.setHeaderData(PARTICIPANTTREECOLUMN_SUMMARY, Qt::Horizontal,
                            tr("Summary"));
    treeModel.setHeaderData(PARTICIPANTTREECOLUMN_VERSION, Qt::Horizontal,
                            tr("Version"));
}

ParticipantView::~ParticipantView()
{
    for (int i = children.count() - 1; i >= 0; i--) {
        remove(children[i]);
    }
}

void
ParticipantView::add(ParticipantViewlet *viewlet)
{
    assert(viewlet);
    children.append(viewlet);
    treeModel.appendRow(viewlet->items);
    registerViewlet(viewlet);
}

ParticipantViewlet *
ParticipantView::createParticipantViewlet()
{
    return new ParticipantViewlet(treeView, this);
}

void
ParticipantView::destroyParticipantViewlet(ParticipantViewlet *viewlet)
{
    viewlet->deleteLater();
}

ParticipantViewlet *
ParticipantView::getChild(int index)
{
    assert((index >= 0) && (index < children.count()));
    return children[index];
}

int
ParticipantView::getChildCount() const
{
    return children.count();
}

void
ParticipantView::handleTreeViewActivation(const QModelIndex &index)
{
    assert(index.isValid());
    QModelIndex parentIndex = index.parent();
    int row = index.row();
    QModelIndex stateIndex =
        treeModel.index(row, PARTICIPANTTREECOLUMN_STATE, parentIndex);
    ParticipantViewlet *viewlet =
        itemViewletMap.value(treeModel.itemFromIndex(stateIndex), 0);
    assert(viewlet);
    bool active = treeModel.data(stateIndex, Qt::UserRole).toBool();
    viewlet->emitActivationChangeRequest(! active);
}

void
ParticipantView::registerViewlet(ParticipantViewlet *viewlet)
{
    assert(! viewlet->getChildCount());
    itemViewletMap.insert(viewlet->items[PARTICIPANTTREECOLUMN_STATE], viewlet);
    connect(viewlet, SIGNAL(childAdded(ParticipantViewlet *)),
            SLOT(registerViewlet(ParticipantViewlet *)));
    connect(viewlet, SIGNAL(childRemoved(ParticipantViewlet *)),
            SLOT(unregisterViewlet(ParticipantViewlet *)));
}

void
ParticipantView::remove(ParticipantViewlet *viewlet)
{
    assert(viewlet);
    assert(! viewlet->getChildCount());
    int index = children.indexOf(viewlet);
    assert(index != -1);
    unregisterViewlet(viewlet);
    treeModel.removeRow(viewlet->items[0]->row());
    children.removeAt(index);
}

void
ParticipantView::setVisible(bool visible)
{
    if (visible) {
        treeView->header()->resizeSections(QHeaderView::ResizeToContents);
    }
    View::setVisible(visible);
}

void
ParticipantView::unregisterViewlet(ParticipantViewlet *viewlet)
{
    disconnect(viewlet, SIGNAL(childAdded(ParticipantViewlet *)),
               this, SLOT(registerViewlet(ParticipantViewlet *)));
    disconnect(viewlet, SIGNAL(childRemoved(ParticipantViewlet *)),
               this, SLOT(unregisterViewlet(ParticipantViewlet *)));
    bool removed =
        itemViewletMap.remove(viewlet->items[PARTICIPANTTREECOLUMN_STATE]);
    assert(removed);
}