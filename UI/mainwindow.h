#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "Config/interactconfig.h"
#include "Config/cloneconfig.h"
#include "Config/proxyconfig.h"
#include <QMdiSubWindow>
#include <QFileDialog>
#include <QMainWindow>
#include "Config/viewclone.h"
#include "Config/checkpoint.h"

#include "CommonInforClone.h"
#include "tokendialog.h"
#include "viewlog.h"
#include "processdownload.h"
#include <QLabel>
#include <QTextEdit>
#include <worker/sqliteworker.h>
#include <QMessageBox>
#include "UI/Config/statusclone.h"
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <QGraphicsScene>
#include <QPoint>
#include "Dialog/infoversion.h"
#include "UI/definition.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList accountFile();
    void reloadTable();
    void onChangeReloadTable(QString cloneId);
    void showTable();

    InteractConfig* getInteractConfig() const;
    CloneConfig* getCloneConfig() const;
    ProxyConfig* getProxyConfig() const;
    ViewClone* getViewClone() const;

    void setStatusInfo(QString message);
    void setCloneInfo(int totalClone, int liveClone, int checkpointClone);

    ProcessDownload* getProcessDownload();

    void initialize();

public slots:
    void closeInteractConfigMenu();
    void closeProxyConfigMenu();
    void closeCloneConfigMenu();
    void closeViewCloneMenu();
    void closeCheckPoint();
    void setToken(QString token, bool x);
    void onAppRunningChanged(bool appRunning);
    void onServiceRunningChanged(bool serviceRunning);

protected:
    QMdiSubWindow* loadSubWindow(QWidget *widget);
    void closeEvent(QCloseEvent *event) override;

private slots:

    void on_actionConfigClone_triggered();

    void on_actionConfigProxy_triggered();

    void on_actionRunAllAccount_triggered();

    void on_actionStopAllAccount_triggered();

    void on_actionConfigInteract_triggered();

    void on_actionImport_account_triggered();

    void on_actionUpdate_account_triggered();

    void on_actionTile_triggered();

    void on_actionCascade_triggered();

    void on_actionToken_triggered();
    void on_actionViewClone_triggered();

    void on_actionView_log_triggered();

    void on_actionCheckPoint_triggered();

    void on_action_F_care_Info_triggered();

public slots:

    void showStatusClone(int ordinalNumberOfClone);
    void doAction();
    void doToken();

private:
    Ui::MainWindow *ui;
    InteractConfig *interactConfig;
    CloneConfig *cloneConfig;
    ProxyConfig *proxyConfig;
    ViewClone *viewClone;
    CheckPoint *checkPoint;

    void getTextFromFile();

    QMdiSubWindow * windowClone;
    QMdiSubWindow * windowProxy;
    QMdiSubWindow * windowInteract;
    QMdiSubWindow * windowViewClone;
    QMdiSubWindow * windowCheckPoint;

    QStringList list;
    TokenDialog * tokenDialog;
    InfoVersion *infoVersion;
    ViewLog *viewLog;

    QLabel* tokenLable;

    QTextEdit *sizeWeightChromeTab;
    QTextEdit *sizeHeightChromeTab;
    QLabel *weightChromeTab;
    QLabel *heightChromeTab;
    QLabel *cloneInfo;
    QLabel *statusInfo;
    QPushButton *action;
    QPixmap pixmapPlay;
    QPushButton *labelPlayStop;
    QPixmap pixmapStop;
    QPushButton *labelToken;
    QPushButton *token;
    QPixmap pixmapToken;
    QPixmap pixmapTokenDisplay;
    ProcessDownload* processDownload = nullptr;

    QLabel *versionInfo;

};
#endif // MAINWINDOW_H
