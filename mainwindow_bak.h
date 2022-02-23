#ifndef MAINWINDOW_BAK_H
#define MAINWINDOW_BAK_H
#include <QStandardItemModel>
#include <QMainWindow>
#include <QMdiArea>
#include <QTextEdit>
#include <QTableView>
#include <QPointer>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QToolButton>
#include <QDebug>
#include "definition.h"
#include "sqliteworker.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpPart>
#include <QNetworkRequest>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class AboutAutolike;
class LogTableDialog;

class MainWindow_bak : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow_bak(QWidget *parent = nullptr);
    ~MainWindow_bak();
signals:
    void stopApp();
public slots:
    void createService();
    void ReplyUserKeyApiFinished(QNetworkReply *reply);
    void ReplyProxyKeyApiFinished(QNetworkReply *reply);
    void onFinish(QNetworkReply *rep);
public:
    bool openFile(const QString &fileName);
    void startCloneByUserKeyApi(QString keyUser);
    void getAPIProxy(QString key);
    void setProxy(QString proxy);
    QString getProxy();

    void startApp();

    void postRequest();


public slots:
    void alignLeft();
    void alignLt();
    void alignCenter();
    void alignRight();

protected:
    void closeEvent(QCloseEvent *event) override;

    void initDb();

private slots:
    void newFile();
    void importClone();
    void startClone();
    void open();
    void openLogClone();
    void setting();
    void settingcn();
    void settingvc();
    void settinght();
    void save();
    void stop();
    void saveAs();
    void updateRecentFileActions();
    void openRecentFile();
#ifndef QT_NO_CLIPBOARD
    void cut();
    void copy();
    void paste(bool);
#endif
    void about();
    void updateMenus();
    void updateWindowMenu();
    QTableView *createMdiChild();
    void switchLayoutDirection();

private:
    enum { MaxRecentFiles = 5 };
    QToolBar *toolBar;
    void createActions();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString &fileName);
    static bool hasRecentFiles();
    void prependToRecentFiles(const QString &fileName);
    void setRecentFilesVisible(bool visible);
    QTextEdit *activeMdiChild() const;
    QTableView *activeMdiChild1() const;
    QMdiSubWindow *findMdiChild(const QString &fileName) const;

    void loadSubWindow(QWidget *widget);

    QPointer<LogTableDialog> logTableDialog;
    QPointer<AboutAutolike> aboutAutolike;

    QMdiArea *mdiArea;

    QMenu *windowMenu;
    QAction *newAct;
    QAction *test;
    QAction *newActtt;
    QAction *newActStart;
    QAction *newActvc;
    QAction *newActvd;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *stopAct;
    QAction *recentFileActs[MaxRecentFiles];
    QAction *recentFileSeparator;
    QAction *recentFileSubMenuAct;

    void createMenus();
    void createToolBars();
    void createToolButtons();
    QAction* stopAction;

    QAction* alignLeftAction;
    QAction* alignLtAction;
    QAction* alignCenterAction;
    QAction* alignRightAction;
    QMenu* alignMenu;
    QToolBar *alignMenuT;
    QToolBar* editToolBar;
//    Customtoolbutton* alignToolButton;

    BaseConfig *baseConfig;
    Thread *m_thread;

#ifndef QT_NO_CLIPBOARD
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
#endif
    QAction *closeAct;
    QAction *closeAllAct;
    QAction *tileAct;
    QAction *cascadeAct;
    QAction *nextAct;
    QAction *previousAct;
    QAction *windowMenuSeparatorAct;
    int m_cloneContinue;
    QNetworkAccessManager *m_managerKeyUser;
    QNetworkAccessManager *m_managerKeyProxy;
    bool m_checkUserAPI = false;
    QString m_proxy = "";

};
#endif // MAINWINDOW_BAK_H
