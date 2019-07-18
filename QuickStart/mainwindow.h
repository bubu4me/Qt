#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAbstractNativeEventFilter>
#include <QApplication>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QMainWindow>
#include <QToolButton>
#include <QtWin>
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
//        , public QAbstractNativeEventFilter
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onClick_Button();
    void onClick_editFileAction();
    void onClick_editDirAction();
    void onClick_clearAction();

    void on_action_Quit_triggered();

    void on_action_StartMenu_triggered();

    void on_action_Taskmgr_triggered();

    void on_action_ControlPanel_triggered();

    void on_action_NightSkin_triggered();

    void on_action_LightSkin_triggered();

private:
    Ui::MainWindow *ui;

    int BUTTON_NUM; // button编号
    static const int BUTTON_SIZE = 40;// button数组长度
    QToolButton *button[BUTTON_SIZE+1]; // button指针数组
    QString *fileForButton;

    // 右键菜单
    QMenu *contextMenu;
    QAction *editFileAction;
    QAction *editDirAction;
    QAction *clearAction;

    // UI布局
    QWidget *mainWidget;
    QHBoxLayout *buttonLayout;
    QGridLayout *wholeLayout;
    QGridLayout *tabLayout;
    QTableWidget *sysInfoTable;

    // 窗口隐藏
    QRect rc;
    QRect rect;

    // 自定义函数列表
    QString getNativeUserName();
    QString getNativecIp();
    QString getPublicIp();
    QString getMac();
    QString getHtml(QString url);
    QString getCpuInfo();
    QString getDCardInfo();
    void init();
    void setButtonText(int n); // n为button编号
    void setFileIconAndText(QString fileName);// 获取文件图标、类型
    void initSystemInfoTable();

    // 重载函数列表:
    bool eventFilter(QObject *o, QEvent *e);
    void keyPressEvent(QKeyEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

    // 鼠标钩子
    HHOOK mouseHook = nullptr;
    void setHook();
    void unHook();

//    virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *);



};

#endif // MAINWINDOW_H
