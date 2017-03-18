#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifndef _UNICODE
#define _UNICODE
#endif

#include <QMainWindow>
#include <QStatusBar>
#include <QTextCodec>
#include <QThread>
#include <Qtime>
#include <process.h>
#include <tchar.h>
#include <Windows.h>
#include <WinUser.h>
#include <string.h>
#include <Ras.h>
#include <RasDlg.h>
#include <raserror.h>
#include <locale.h>
#include <ctype.h>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

#pragma comment(lib, "rasapi32.lib")
#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "User32.lib")


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool PPPoEinit();
public slots:
    void GetSaveConfirm(void);
    void SetChangeFlag(void);
    bool DoPPPoeDial(void);
signals:
    void RunThread();
private:
    Ui::MainWindow *ui;
//  bool FindRAS();
    bool InitSet();
    bool bSaveConfirm;
    void GetPPPoEUserName(TCHAR * const Uname,TCHAR * TName);
    TCHAR szUsername[256]= {0,};
    TCHAR szPPPname[256] = {0,};
    TCHAR szPassword[256] = {0,};
    bool savePhoneBook(TCHAR *const username,TCHAR * const password);
    bool bChange = false;
    void UpdateData(void);
    void waitms(DWORD ms);
protected:
    bool eventFilter(QObject *target, QEvent *event);
};
class PPPoeThread:public QThread
{
    Q_OBJECT
public:
    PPPoeThread(){
        FIN = FALSE;
        STOP = TRUE;
    }
    //~PPPoeThread();
    bool DoPPPdial(void);
    bool GetThreadStat(void);
    void GetID(TCHAR * const szPPPoename,TCHAR * const szPPPoePass);
    DWORD getErrorCode(void);

public slots:
    void setRun();
//    void start();
//    void terminate();

private:
    BOOL GetPortName(TCHAR * lpBuffer);
    HRASCONN DoPPP(TCHAR* UserName, TCHAR* Password);
    TCHAR UserName[256];
    TCHAR Password[256];
    bool FIN;
    bool STOP;
    DWORD ucErrorcode;
protected:
    virtual void run();
};

#endif // MAINWINDOW_H
