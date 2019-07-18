#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopServices>
#include <QDir>
#include <QProcess>
#include <QUrl>
#include <QDebug>
#include <QFileDialog>
#include <QFileIconProvider>
#include <QTemporaryFile>
#include <QString>
#include <QFileDialog>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QPainter>
#include <QSysInfo>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QHostAddress>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHeaderView>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 初始化布局、button及其数据
    init();
    // 注册钩子
//    setHook();

    // 设置button响应
    for (int i = 1; i <= BUTTON_SIZE; i++) {
        button[i]->installEventFilter(this);                                            // 加入事件过滤器.
        button[i]->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);                     // 上面图标,下面文字
        setButtonText(i);
        connect(button[i], SIGNAL(clicked()), this, SLOT(onClick_Button()));            // button数组响应
    }
    connect(editFileAction, SIGNAL(triggered()), this, SLOT(onClick_editFileAction()));     // 编辑文件按钮响应
    connect(editDirAction, SIGNAL(triggered()), this, SLOT(onClick_editDirAction()));       // 编辑文件按钮响应
    connect(clearAction, SIGNAL(triggered()), this, SLOT(onClick_clearAction()));           // 清除按钮响应

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化
void MainWindow::init()
{
    mainWidget = new QWidget();             // 新建主窗体
    wholeLayout = new QGridLayout();        // 主窗口的布局
    tabLayout = new QGridLayout();          // 键盘菜单的布局
    // 添加button到布局
    for (int i = 1; i <= BUTTON_SIZE; i++) {
        button[i] = new QToolButton();
        button[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        if (i <= 10) {
            wholeLayout->addWidget(button[i], 0, i - 1);
        }else if (i > 10 && i <= BUTTON_SIZE) {
            if (i > 10 && i <= 20) {
                tabLayout->addWidget(button[i], 0, i - 11);     // 放在第1行
            }else if (i > 20 && i <= 30) {
                tabLayout->addWidget(button[i], 1, i - 21);     // 放在第2行
            }else if (i > 30 && i <= BUTTON_SIZE) {
                tabLayout->addWidget(button[i], 2, i - 31);     // 放在第3行
            }
        }

    }
    wholeLayout->addWidget(ui->tabWidget, 1, 0, 1, 10);// 添加tabWidget到布局
    // 设置伸缩比例2:8
    wholeLayout->setRowStretch(0, 2);
    wholeLayout->setRowStretch(1, 8);
    ui->tab->setLayout(tabLayout);// 将键盘布局放到tabWidget
    mainWidget->setLayout(wholeLayout);
    this->setCentralWidget(mainWidget);

    BUTTON_NUM = 1;                                 // 初始化button的序号为1,即第一个button

    fileForButton = new QString[BUTTON_SIZE + 1];   // 每个button对应的文件或目录

    // 设置button右键菜单
    contextMenu = new QMenu;
    editFileAction = new QAction("编辑文件");
    editDirAction = new QAction("编辑文件夹");
    clearAction = new QAction("清除");
    contextMenu->addAction(editFileAction);
    contextMenu->addAction(editDirAction);
    contextMenu->addAction(clearAction);

    // 初始化界面皮肤
    QFile file(":/qss/Qss2.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);
    // 初始化窗口背景
    mainWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/image/Grass2.jpg").scaled(width(), height())));
    mainWidget->setPalette(palette);
    ui->mainToolBar->hide();// hide toolbar

    // 初始化tableWidget
    sysInfoTable = new QTableWidget(15, 2);
    sysInfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);// 不可编辑
    sysInfoTable->setHorizontalHeaderItem(0, new QTableWidgetItem("项目"));
    sysInfoTable->setHorizontalHeaderItem(1, new QTableWidgetItem("值"));
    sysInfoTable->verticalHeader()->setVisible(false);                      // 隐藏行表头
    sysInfoTable->setShowGrid(false);                                       // 隐藏表格线
    sysInfoTable->setSelectionMode(QAbstractItemView::ExtendedSelection);   // 可选中多个目标
    sysInfoTable->setColumnWidth(0, 200);
    sysInfoTable->setColumnWidth(1, 400);
    tabLayout = new QGridLayout();
    tabLayout->addWidget(sysInfoTable);
    ui->tab_Info->setLayout(tabLayout);
    initSystemInfoTable();

}

void MainWindow::setButtonText(int n)
{
    if (n >= 1 && n <= 10) {
        button[n]->setText("F" + QString::number(n));
    }else if (n > 10 && n <= BUTTON_SIZE) { // A-Z <==> 65-90
        switch (n) {
        case 11:
            button[n]->setText("Q");
            break;
        case 12:
            button[n]->setText("W");
            break;
        case 13:
            button[n]->setText("E");
            break;
        case 14:
            button[n]->setText("R");
            break;
        case 15:
            button[n]->setText("T");
            break;
        case 16:
            button[n]->setText("Y");
            break;
        case 17:
            button[n]->setText("U");
            break;
        case 18:
            button[n]->setText("I");
            break;
        case 19:
            button[n]->setText("O");
            break;
        case 20:
            button[n]->setText("P");
            break;
        case 21:
            button[n]->setText("A");
            break;
        case 22:
            button[n]->setText("S");
            break;
        case 23:
            button[n]->setText("D");
            break;
        case 24:
            button[n]->setText("F");
            break;
        case 25:
            button[n]->setText("G");
            break;
        case 26:
            button[n]->setText("H");
            break;
        case 27:
            button[n]->setText("J");
            break;
        case 28:
            button[n]->setText("K");
            break;
        case 29:
            button[n]->setText("L");
            break;
        case 30:
            button[n]->setText(";");
            break;
        case 31:
            button[n]->setText("Z");
            break;
        case 32:
            button[n]->setText("X");
            break;
        case 33:
            button[n]->setText("C");
            break;
        case 34:
            button[n]->setText("V");
            break;
        case 35:
            button[n]->setText("B");
            break;
        case 36:
            button[n]->setText("N");
            break;
        case 37:
            button[n]->setText("M");
            break;
        case 38:
            button[n]->setText(",");
            break;
        case 39:
            button[n]->setText(".");
            break;
        case 40:
            button[n]->setText("/");
            break;
        }
    }
}

// 获取本地用户名
QString MainWindow::getNativeUserName()
{
    QString nativeUserName = "";

    nativeUserName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    nativeUserName = nativeUserName.section("/", -1, -1);
    return nativeUserName;
}

// 获取本地IP
QString MainWindow::getNativecIp()
{
    QString localIPAddress = "";
    QList<QHostAddress> listAddress = QNetworkInterface::allAddresses();

    for (int j = 0; j < listAddress.size(); j++) {
        if (!listAddress.at(j).isNull()
            && listAddress.at(j).protocol() == QAbstractSocket::IPv4Protocol
            && listAddress.at(j) != QHostAddress::LocalHost
            && listAddress.at(j).toString().startsWith("192")) {
            localIPAddress = listAddress.at(j).toString();
            break;
        }
    }
    return localIPAddress;
}

//外网的获取方法，通过爬网页来获取外网IP
QString MainWindow::getHtml(QString url)//网页源代码
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;

    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return QString(responseData);
}

// 获取公网IP
QString MainWindow::getPublicIp()
{
    QString strIp;
    QString webCode = getHtml("http://whois.pconline.com.cn/");

    if (!webCode.isEmpty()) {
        QString web = webCode.replace(" ", "");
        web = web.replace("\r", "");
        web = web.replace("\n", "");
        QStringList list = web.split("<br/>");
        QString tar = list[3];
        qDebug() << "tar :" << tar;
        QStringList ip = tar.split("=");
        qDebug() << "ip :" << ip;
        strIp = ip[1];
        qDebug() << "strIp :" << strIp;
    }else
        strIp = "无法获取公网ip";
    return strIp;
}

// 获取本地MAC地址
QString MainWindow::getMac()
{
    QList<QNetworkInterface> NetList;               //网卡链表
    int NetCount = 0;                               //网卡个数
    int Neti = 0;
    QNetworkInterface thisNet;                      //所要使用的网卡

    NetList = QNetworkInterface::allInterfaces();   //获取所有网卡信息
    NetCount = NetList.count();                     //统计网卡个数
    for (Neti = 0; Neti < NetCount; Neti++) {       //遍历所有网卡
        if (NetList[Neti].isValid()) {          //判断该网卡是否是合法
            thisNet = NetList[Neti];        //将该网卡置为当前网卡
            break;
        }
    }
    return thisNet.hardwareAddress();
}

// 获取计算机CPU信息
QString MainWindow::getCpuInfo()
{
    QSettings *cpu = new QSettings("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", QSettings::NativeFormat);
    QString cpuInfo = cpu->value("ProcessorNameString").toString();

    delete cpu;
    return cpuInfo;
}

// 获取计算机显卡信息
QString MainWindow::getDCardInfo()
{
    QString dcard;
    QSettings *DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\services\\nvlddmkm\\Device0", QSettings::NativeFormat);
    QString type = DCard->value("Device Description").toString();

    qDebug() << "type: " << type;

    delete DCard;

    QString dType = type;
    dType = dType.trimmed();
    if (!dType.isEmpty())
        dcard = dType;
    qDebug() << "dType: " << dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\igfx\\Device0", QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType = dType.trimmed();
    if (!dType.isEmpty())
        dcard = dcard + "\n" + dType;

    DCard = new QSettings("HKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet001\\Services\\amdkmdap\\Device0", QSettings::NativeFormat);
    type = DCard->value("Device Description").toString();
    delete DCard;

    dType = type;
    dType = dType.trimmed();
    if (!dType.isEmpty())
        dcard = dcard + "\n" + dType;

    dcard = dcard.trimmed();
    return dcard;
}

// TODO 获取本地系统信息
void MainWindow::initSystemInfoTable()
{
    sysInfoTable->setItem(0, 0, new QTableWidgetItem("计算机名称"));
    sysInfoTable->setItem(0, 1, new QTableWidgetItem(QSysInfo::machineHostName()));
    sysInfoTable->setItem(1, 0, new QTableWidgetItem("本地用户名"));
    sysInfoTable->setItem(1, 1, new QTableWidgetItem(getNativeUserName()));
    sysInfoTable->setItem(2, 0, new QTableWidgetItem("本机IP地址"));
    sysInfoTable->setItem(2, 1, new QTableWidgetItem(getNativecIp()));
    sysInfoTable->setItem(3, 0, new QTableWidgetItem("公网IP"));
    sysInfoTable->setItem(3, 1, new QTableWidgetItem(getPublicIp()));
    sysInfoTable->setItem(4, 0, new QTableWidgetItem("计算机MAC地址"));
    sysInfoTable->setItem(4, 1, new QTableWidgetItem(getMac()));
    sysInfoTable->setItem(5, 0, new QTableWidgetItem("计算机CPU信息"));
    sysInfoTable->setItem(5, 1, new QTableWidgetItem(getCpuInfo()));

    int windowsVer;
    windowsVer = QSysInfo::windowsVersion();
    sysInfoTable->setItem(6, 0, new QTableWidgetItem("Windows Version"));
    sysInfoTable->setItem(6, 1, new QTableWidgetItem(QString::number(windowsVer)));
    QString s;
    s = QSysInfo::buildAbi();
    sysInfoTable->setItem(7, 0, new QTableWidgetItem("Build Abi"));
    sysInfoTable->setItem(7, 1, new QTableWidgetItem(s));
    s = QSysInfo::buildCpuArchitecture();
    sysInfoTable->setItem(8, 0, new QTableWidgetItem("Build Cpu Architecture"));
    sysInfoTable->setItem(8, 1, new QTableWidgetItem(s));
    s = QSysInfo::currentCpuArchitecture();
    sysInfoTable->setItem(9, 0, new QTableWidgetItem("Current Cpu Architecture"));
    sysInfoTable->setItem(9, 1, new QTableWidgetItem(s));
    s = QSysInfo::kernelType();
    sysInfoTable->setItem(10, 0, new QTableWidgetItem("Kernel Type"));
    sysInfoTable->setItem(10, 1, new QTableWidgetItem(s));
    s = QSysInfo::kernelVersion();
    sysInfoTable->setItem(11, 0, new QTableWidgetItem("Kernel Version"));
    sysInfoTable->setItem(11, 1, new QTableWidgetItem(s));
    s = QSysInfo::prettyProductName();
    sysInfoTable->setItem(12, 0, new QTableWidgetItem("Pretty Product Name"));
    sysInfoTable->setItem(12, 1, new QTableWidgetItem(s));
    s = QSysInfo::productType();
    sysInfoTable->setItem(13, 0, new QTableWidgetItem("Product Type"));
    sysInfoTable->setItem(13, 1, new QTableWidgetItem(s));
    s = QSysInfo::productVersion();
    sysInfoTable->setItem(14, 0, new QTableWidgetItem("Product Version"));
    sysInfoTable->setItem(14, 1, new QTableWidgetItem(s));

}

// 点击按钮响应
void MainWindow::onClick_Button()
{
    QToolButton *selectedButton = qobject_cast<QToolButton*>(sender());// 获取发送信号的控件类型

    // 判断控件为哪个button
    for (int i = 1; i <= BUTTON_SIZE; i++) {
        if (selectedButton == button[i]) {
            BUTTON_NUM = i;
            break;
        }
    }
    if (fileForButton[BUTTON_NUM] != "") {
        QDesktopServices::openUrl(QUrl(QString("file:").append(fileForButton[BUTTON_NUM]), QUrl::TolerantMode));
    }
    qDebug() << "clicked; " << " BUTTON_NUM——" << BUTTON_NUM << " file——" + fileForButton[BUTTON_NUM];
}

// 事件过滤器
bool MainWindow::eventFilter(QObject *o, QEvent *event)
{

    for (int i = 1; i <= BUTTON_SIZE; i++) {
        if (o == button[i]) {
            if (event->type() == QEvent::ContextMenu) {
                BUTTON_NUM = i;
                qDebug() << BUTTON_NUM;
                contextMenu->exec(QCursor::pos());
                return true;
            }
        }
    }

    return QObject::eventFilter(o, event);
}

// 添加文件响应
void MainWindow::onClick_editFileAction()
{
    // TODO BUTTON_NUM为0，以及直接清除
    fileForButton[BUTTON_NUM] = QFileDialog::getOpenFileName(this, tr("Select File"), "/", tr("All Files (*.*)"));// 文件选择对话框
    qDebug() << "num: " << BUTTON_NUM;
    qDebug() << fileForButton[BUTTON_NUM];
    setFileIconAndText(fileForButton[BUTTON_NUM]);

}

// 添加文件夹响应
void MainWindow::onClick_editDirAction()
{
    fileForButton[BUTTON_NUM] = QFileDialog::getExistingDirectory(this, tr("Select File"), "./");// 文件选择对话框
    qDebug() << "num: " << BUTTON_NUM;
    qDebug() << fileForButton[BUTTON_NUM];
    setFileIconAndText(fileForButton[BUTTON_NUM]);

}

// 清除响应
void MainWindow::onClick_clearAction()
{
    // TODO
    fileForButton[BUTTON_NUM] = "";
    setButtonText(BUTTON_NUM);
    button[BUTTON_NUM]->setIcon(QIcon());
    // 卸载钩子
//    unHook();
}

// 获取文件图标
void MainWindow::setFileIconAndText(QString fileName)
{
    QFileInfo tmpFileInfo(fileName);
    QFileIconProvider icon_provider;
    QIcon buttonIcon = icon_provider.icon(tmpFileInfo);

    if (fileForButton[BUTTON_NUM] != "") {
        QString s = tmpFileInfo.suffix();// 文件后缀名
        button[BUTTON_NUM]->setIconSize(QSize(25, 25));
        button[BUTTON_NUM]->setIcon(buttonIcon);
        qDebug() << s;
        s = tmpFileInfo.fileName();
        if (s.length() > 6) {
            s = s.left(5);
        }
        // TODO
        button[BUTTON_NUM]->setText(s.append("..."));// 设置当前按钮显示的文字
        qDebug() << s;
    }
    qDebug() << button[BUTTON_NUM]->text();
}

// 鼠标钩子
//LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam){
//    qDebug()<<"mouse";
//    return 1;
//}

//void MainWindow::setHook(){
//    mouseHook = SetWindowsHookExA(WH_MOUSE_LL,mouseProc,GetModuleHandleA(NULL),0);
//}

//void MainWindow::unHook(){
//    UnhookWindowsHookEx(mouseHook);
//}


//bool MainWindow::nativeEventFilter(const QByteArray &eventType, void *message, long *)
//{

//    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
//        qDebug()<<"ok???";
//        MSG* pMsg = reinterpret_cast<MSG*>(message);
//        if (pMsg->message == WM_MOUSEMOVE) {
//            QPoint cursorPoint = QCursor::pos();// 获取全局坐标
//            qDebug() << cursorPoint;
//        }
//    }
//    qDebug() << "MainWindow windows system message WM_COPYDATA";
//    return false;
//}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    int keyCode = e->key();

    if ((keyCode >= Qt::Key_F1 && keyCode <= Qt::Key_F10) ||
        (keyCode >= Qt::Key_A && keyCode <= Qt::Key_Z) ||
        keyCode == Qt::Key_Semicolon || keyCode == Qt::Key_Comma ||
        keyCode == Qt::Key_Period || keyCode == Qt::Key_Slash) {
        switch (e->key()) {
        case Qt::Key_F1:
            BUTTON_NUM = 1;
            break;
        case Qt::Key_F2:
            BUTTON_NUM = 2;
            break;
        case Qt::Key_F3:
            BUTTON_NUM = 3;
            break;
        case Qt::Key_F4:
            BUTTON_NUM = 4;
            break;
        case Qt::Key_F5:
            BUTTON_NUM = 5;
            break;
        case Qt::Key_F6:
            BUTTON_NUM = 6;
            break;
        case Qt::Key_F7:
            BUTTON_NUM = 7;
            break;
        case Qt::Key_F8:
            BUTTON_NUM = 8;
            break;
        case Qt::Key_F9:
            BUTTON_NUM = 9;
            break;
        case Qt::Key_F10:
            BUTTON_NUM = 10;
            break;
        case Qt::Key_Q:
            BUTTON_NUM = 11;
            break;
        case Qt::Key_W:
            BUTTON_NUM = 12;
            break;
        case Qt::Key_E:
            BUTTON_NUM = 13;
            break;
        case Qt::Key_R:
            BUTTON_NUM = 14;
            break;
        case Qt::Key_T:
            BUTTON_NUM = 15;
            break;
        case Qt::Key_Y:
            BUTTON_NUM = 16;
            break;
        case Qt::Key_U:
            BUTTON_NUM = 17;
            break;
        case Qt::Key_I:
            BUTTON_NUM = 18;
            break;
        case Qt::Key_O:
            BUTTON_NUM = 19;
            break;
        case Qt::Key_P:
            BUTTON_NUM = 20;
            break;
        case Qt::Key_A:
            BUTTON_NUM = 21;
            break;
        case Qt::Key_S:
            BUTTON_NUM = 22;
            break;
        case Qt::Key_D:
            BUTTON_NUM = 23;
            break;
        case Qt::Key_F:
            BUTTON_NUM = 24;
            break;
        case Qt::Key_G:
            BUTTON_NUM = 25;
            break;
        case Qt::Key_H:
            BUTTON_NUM = 26;
            break;
        case Qt::Key_J:
            BUTTON_NUM = 27;
            break;
        case Qt::Key_K:
            BUTTON_NUM = 28;
            break;
        case Qt::Key_L:
            BUTTON_NUM = 29;
            break;
        case Qt::Key_Semicolon:
            BUTTON_NUM = 30;
            qDebug() << ";";
            break;
        case Qt::Key_Z:
            BUTTON_NUM = 31;
            break;
        case Qt::Key_X:
            BUTTON_NUM = 32;
            break;
        case Qt::Key_C:
            BUTTON_NUM = 33;
            break;
        case Qt::Key_V:
            BUTTON_NUM = 34;
            break;
        case Qt::Key_B:
            BUTTON_NUM = 35;
            break;
        case Qt::Key_N:
            BUTTON_NUM = 36;
            break;
        case Qt::Key_M:
            BUTTON_NUM = 37;
            break;
        case Qt::Key_Comma:
            BUTTON_NUM = 38;
            qDebug() << ",";
            break;
        case Qt::Key_Period:
            BUTTON_NUM = 39;
            qDebug() << ".";
            break;
        case Qt::Key_Slash:
            BUTTON_NUM = 40;
            qDebug() << "/";
            break;
        }
        onClick_Button();
    }

}

void MainWindow::enterEvent(QEvent *e)
{
    rc = QApplication::desktop()->geometry();
    qDebug() << "rc: " << rc;
    rect = this->geometry();
    qDebug() << "rectR: " << rect.right();
    if (rect.right() > rc.right() - 10) {
        this->move(rc.right() - rect.width(), rect.top());
    }
}

void MainWindow::leaveEvent(QEvent *e)
{
    rc = QApplication::desktop()->geometry();
    qDebug() << "rc: " << rc;
    rect = this->geometry();
    qDebug() << "rectR: " << rect.right();
    if (rect.right() > rc.right() - 10) {
        this->move(rc.right() - 10, rect.top());
    }
}

// 关闭程序
void MainWindow::on_action_Quit_triggered()
{
    this->close();
}

// 打开开始菜单文件夹
void MainWindow::on_action_StartMenu_triggered()
{
    QString startMenuPath = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs";

    QDesktopServices::openUrl(QUrl(QString("file:").append(startMenuPath), QUrl::TolerantMode));
}

// 打开任务管理器
void MainWindow::on_action_Taskmgr_triggered()
{
    ShellExecuteA(nullptr, "OPEN", "Taskmgr", nullptr, nullptr, SW_SHOWNORMAL);
}

// 打开控制面板
void MainWindow::on_action_ControlPanel_triggered()
{
    ShellExecuteA(nullptr, "OPEN", "rundll32.exe", "shell32.dll Control_RunDLL", nullptr, SW_SHOW);
}

void MainWindow::on_action_NightSkin_triggered()
{
    // 夜间模式
    QFile file(":/qss/Qss2.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);
}

void MainWindow::on_action_LightSkin_triggered()
{
    // TODO 日间模式
    QFile file(":/qss/Qss1.qss");

    file.open(QFile::ReadOnly);
    QString styleSheet = QString::fromLatin1(file.readAll());
    qApp->setStyleSheet(styleSheet);
    mainWidget->setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/image/Grass.jpg")));
}
