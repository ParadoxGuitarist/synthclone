#ifndef __SESSIONLOADVIEW_H__
#define __SESSIONLOADVIEW_H__

#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDoubleValidator>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>

#include <synthclone/types.h>

#include "dialogview.h"

class SessionLoadView: public DialogView {

    Q_OBJECT

public:

    enum Status {
        STATUS_ERROR,
        STATUS_INFO,
        STATUS_VALID
    };

    explicit
    SessionLoadView(QObject *parent=0);

    ~SessionLoadView();

    void
    addRecentSession(const QString &path);

    void
    clearRecentSessions();

    void
    setCreationEnabled(bool enabled);

    void
    setCreationMessage(const QString &message);

    void
    setCreationName(const QString &name);

    void
    setCreationPath(const QString &path);

    void
    setCreationStatus(Status status);

    void
    setOpenEnabled(bool enabled);

    void
    setOpenMessage(const QString &message);

    void
    setOpenPath(const QString &path);

    void
    setOpenStatus(Status status);

public slots:

    void
    setVisible(bool visible);

signals:

    void
    creationDirectoryBrowseRequest(const QString &path, const QString &name);

    void
    creationDirectoryChanged(const QString &path, const QString &name);

    void
    creationRequest(const QString &path, const QString &name,
                    synthclone::SampleRate sampleRate,
                    synthclone::SampleChannelCount count);

    void
    error(const QString &message);

    void
    openDirectoryBrowseRequest(const QString &path);

    void
    openDirectoryChanged(const QString &path);

    void
    openRequest(const QString &path);

private slots:

    void
    handleCreationDirectoryLookup();

    void
    handleOpenButtonClick();

    void
    handleOpenDirectoryLookup();

    void
    handleRecentSessionsCurrentTextChange(const QString &path);

    void
    handleTabChange(int tab);

    void
    updateCreationDirectory();

    void
    updateOpenDirectory();

private:

    QString
    getResource(Status status);

    QPushButton *openButton;
    QPushButton *quitButton;
    QListWidget *recentSessions;
    QSpinBox *sampleChannelCount;
    QComboBox *sampleRate;
    QDoubleValidator sampleRateValidator;
    QLineEdit *sessionCreationDirectory;
    QPushButton *sessionCreationDirectoryLookupButton;
    bool sessionCreationEnabled;
    QLineEdit *sessionCreationName;
    QLabel *sessionCreationStatusIcon;
    QLabel *sessionCreationStatusMessage;
    QLineEdit *sessionOpenDirectory;
    QPushButton *sessionOpenDirectoryLookupButton;
    bool sessionOpenEnabled;
    QLabel *sessionOpenStatusIcon;
    QLabel *sessionOpenStatusMessage;
    QTabWidget *tabWidget;

};

#endif