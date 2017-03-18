#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Dial->installEventFilter(this);
    ui->More->installEventFilter(this);
    ui->Cancel->installEventFilter(this);
    ui->bSave->setVisible(false);
    ui->PASSWORD->setEchoMode(QLineEdit::Password);
    QLabel *per = new QLabel("by Ras0N", this);
    ui->statusBar->addPermanentWidget(per);
    InitSet();
    bSaveConfirm = ui->bSave->isChecked();
    connect(ui->bSave,SIGNAL(clicked(bool)),this,SLOT(GetSaveConfirm()));
    connect(ui->USERNAME,SIGNAL(textEdited(QString)),this,SLOT(SetChangeFlag()));
    connect(ui->PASSWORD,SIGNAL(textEdited(QString)),this,SLOT(SetChangeFlag()));
    connect(ui->Dial,SIGNAL(clicked(bool)),this,SLOT(DoPPPoeDial()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
bool MainWindow::InitSet(){
    bool bRes = false;
    FILE *pFile = NULL;
    TCHAR szCurrnetPath[MAX_PATH];
    TCHAR szFilePath[MAX_PATH];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
    _tcscpy_s(szCurrnetPath,MAX_PATH,szFilePath);
    _tcscat_s(szCurrnetPath,FILENAME_MAX, L"Phone_Book.ROP");

    TCHAR szPassWord[128] = { 0, };
    TCHAR UseName[128] = { 0, };
    if (!_tfopen_s(&pFile,szCurrnetPath, L"r"))
    {
        CHAR szFileBuff[512] = {0,};
        TCHAR szTmpBuff[512] = {0,};
        fread(&szFileBuff, 32, 4, pFile);
        mbstowcs(szTmpBuff,szFileBuff,256);
        int n = (int)(_tcschr(szTmpBuff,L'$')-szTmpBuff);
        int i ;
        for(i=0;n>0;n--)
            UseName[i] = szTmpBuff[i++];
        UseName[i]= '\0';
        TCHAR *pNBuff = _tcschr(szTmpBuff, L'$') + 1;
        n =(int)_tcslen(pNBuff);
        for (i = 0; n > 0; n--)
        {
            szPassWord[i] = pNBuff[i++];
        }
        ui->USERNAME->setText(QString::fromWCharArray(UseName));
        ui->PASSWORD->setText(QString::fromWCharArray(szPassWord));
        ui->statusBar->showMessage(tr("找到配置文件"),2000);
        bRes = true;
        fclose(pFile);
        pFile = NULL;
    }
    pFile = NULL;
    return bRes;
}
/*
bool MainWindow::FindRAS(){
    FILE *pFile = NULL;
    bool bRes = false;
    _TCHAR szCurrnetPath[MAX_PATH];
    _TCHAR szFilePath[MAX_PATH];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
    _tcscpy_s(szCurrnetPath, MAX_PATH, szFilePath);
    _tcscat_s(szCurrnetPath, FILENAME_MAX, L"Phone_Book.ROP");
    if (!_tfopen_s(&pFile, szCurrnetPath, L"r"))
        bRes = true;
    fclose(pFile);
    pFile = NULL;
    return bRes;
}
*/
/*
void MainWindow::Decode(TCHAR * const Buff, TCHAR *User, TCHAR *Pass){
    //strcpy_s
    //char tpUser[128] = {'{','S','R','U','N','3','}','\r','\n'};
    _tcscat_s(User, 64, "{SRUN3}\r\n");
    int n = _tcslen(User);
    n = (int)(_tcschr(Buff,'$') - Buff);
    int i;
    TCHAR *pUser = User + 9;
    for (i=0; n > 0; n--)
    {
        pUser[i] = Buff[i++]+4;
    }
    pUser[i] = '\0';
    //==>strcat_s(pUser,(int)(strchr(Buff, '$') - Buff),Buff);
    //strcat_s(User, 64, tpUser);
    TCHAR *pNBuff = _tcschr(Buff, L'$') + 1;
    n = _tcslen(pNBuff);
    for (i = 0; n > 0; n--)
    {
        Pass[i] = pNBuff[i++];
    }
}
*/
void MainWindow::GetPPPoEUserName(TCHAR * const Uname,TCHAR * TName){
    _tcscat_s(TName, 64, L"{SRUN3}\r\n");
    int n = (int)_tcslen(Uname);
    int i;
    TCHAR *pUser = TName + 9;
    for(i=0;n>0;n--)
        pUser[i] = Uname[i++]+4;
    pUser[i] = '\0';
}
void MainWindow::GetSaveConfirm(void)
{
    bSaveConfirm = ui->bSave->isChecked();
    if (bSaveConfirm)
        ui->statusBar->showMessage(tr("保存配置文件"),2000);
    else
        ui->statusBar->showMessage(tr("不保存配置文件"),2000);
}
bool MainWindow::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->Dial){
        if(event->type() == QEvent::Enter)
        {
            ui->statusBar->showMessage(tr("拨号"),3000);
            return true;
        }
        else
            return false;
    }
    if(target == ui->More){
        if(event->type() == QEvent::Enter){
            ui->statusBar->showMessage(tr("显示更多选项"),3000);
            return true;
        }
        else
            return false;
    }
    if(target == ui->Cancel){
        if(event->type() == QEvent::Enter){
            ui->statusBar->showMessage(tr("关闭窗口"),3000);
            return true;
        }
        else
            return false;
    }
    else
        return MainWindow::eventFilter(target, event);
}
bool MainWindow::savePhoneBook(TCHAR * const username, TCHAR * const password)
{
    FILE *pFile = NULL;
    TCHAR szCurrnetPath[MAX_PATH];
    TCHAR szFilePath[MAX_PATH];
    GetModuleFileName(NULL, szFilePath, MAX_PATH);
    (_tcsrchr(szFilePath, _T('\\')))[1] = 0;
    _tcscpy_s(szCurrnetPath, MAX_PATH, szFilePath);
    _tcscat_s(szCurrnetPath, FILENAME_MAX, L"Phone_Book.ROP");
    _tfopen_s(&pFile, szCurrnetPath, L"wb+");
    if (pFile == NULL)
    {
        ui->statusBar->showMessage(tr("写入文件失败"),2000);
        fclose(pFile);
        pFile = NULL;
        return false;
    }
    else{
        for (unsigned int i = 0; i < _tcslen(username); i++)
            fputc(username[i], pFile);
        fputc('$', pFile);
        for (unsigned int i = 0; i < _tcslen(password); i++)
            fputc(password[i], pFile);
        ui->statusBar->showMessage(tr("拨号文件已经更新"),2000);
        fclose(pFile);
        pFile=NULL;
        return true;
    }
}
void MainWindow::SetChangeFlag(){
    bChange = true;
}
void MainWindow::UpdateData(void)
{
    QString Qtuser,Qtpass;
    Qtuser = ui->USERNAME->text();
    Qtpass = ui->PASSWORD->text();
    Qtuser.toWCharArray(szUsername);
    Qtpass.toWCharArray(szPassword);
    GetPPPoEUserName(szUsername,szPPPname);
}

bool MainWindow::DoPPPoeDial(void){
    UpdateData();
    PPPoeThread *pThread = new PPPoeThread;
    if (bChange == 1)
        if(!savePhoneBook(szUsername,szPassword))
        {
            MessageBox(NULL,L"保存拨号文件失败，下次运行时还请重新输入",L"错误", MB_OKCANCEL );
        }
    ui->USERNAME->setEnabled(false);
    ui->PASSWORD->setEnabled(false);
    ui->Dial->setEnabled(false);
    ui->Cancel->setEnabled(false);
    ui->More->setEnabled(false);
    connect(this,SIGNAL(RunThread()),pThread,SLOT(setRun()));
    pThread->GetID(szPPPname,szPassword);
    pThread->start();
    emit RunThread();
    while(!pThread->GetThreadStat())
    {
        ui->statusBar->showMessage("拨号中-",500);
        waitms(500);
        ui->statusBar->showMessage("拨号中/",500);
        waitms(500);
        ui->statusBar->showMessage("拨号中-",500);
        waitms(500);
        ui->statusBar->showMessage("拨号中\\",500);
        waitms(500);
    }
    DWORD dwRes = pThread->getErrorCode();
    if(dwRes == 1)
        ui->statusBar->showMessage("发现活动连接",2000);
    else if (dwRes == 2)
        ui->statusBar->showMessage("未发现可用拨号端口",2000);
    else if (dwRes == 3)
        ui->statusBar->showMessage("无法创建拨号连接",2000);
    else if (dwRes == ERROR_SUCCESS)
        ui->statusBar->showMessage("连接已建立",2000);
    else
    {
        QString str;
        ui->statusBar->showMessage(str.sprintf("拨号错误，错误代码:%i",&dwRes),3000);
    }
    pThread->terminate();
    ui->USERNAME->setEnabled(true);
    ui->PASSWORD->setEnabled(true);
    ui->Dial->setEnabled(true);
    ui->Cancel->setEnabled(true);
    ui->More->setEnabled(true);
    return true;
}
BOOL PPPoeThread::GetPortName(TCHAR * lpBuffer) {
    DWORD dwCb = 0;
    DWORD dwRet = ERROR_SUCCESS;
    DWORD dwDevices = 0;
    LPRASDEVINFO lpRasDevInfo = NULL;
    dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);

    _tprintf(L"Find PPPoE Port . . . \n");

    if (dwRet == ERROR_BUFFER_TOO_SMALL) {
        lpRasDevInfo = (LPRASDEVINFO)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);
        if (lpRasDevInfo == NULL) {
            _tprintf(L"HeapAlloc Failed!\n");
            return false;
        }
        lpRasDevInfo[0].dwSize = sizeof(RASDEVINFO);
        dwRet = RasEnumDevices(lpRasDevInfo, &dwCb, &dwDevices);

        if (dwRet == ERROR_SUCCESS)
        {
            _tprintf(L"PPPoE Port Found!\n");
            for (DWORD i = 0; i < dwDevices; i++) {
                //wprintf(L"%s\n", lpRasDevInfo[i].szDeviceName);
                if (!_tcscmp(lpRasDevInfo[i].szDeviceType, L"pppoe"))
                {
                    _tcscpy_s(lpBuffer,RAS_MaxDeviceName + 1 ,lpRasDevInfo[i].szDeviceName);
                    _tprintf(L"Using Port %s...\n", lpRasDevInfo[i].szDeviceName);
                    break;
                }
            }
        }
        HeapFree(GetProcessHeap(), 0, lpRasDevInfo);
        lpRasDevInfo = NULL;
        return TRUE;

        if (dwDevices < 1)
            _tprintf(L"No PPPoE Port Found! Check your drivers!\n");
        return FALSE;
    }
    return FALSE; //Hope we Can't Get this command run....
}
HRASCONN PPPoeThread::DoPPP(TCHAR* UserName, TCHAR* Password) {
    RASDIALPARAMS lpParams = { 0 };
    lpParams.dwSize = sizeof(lpParams);
    _tcscpy_s(lpParams.szEntryName,128,L"Srun3 PPPoE");
    _tcscpy_s(lpParams.szUserName,128,(TCHAR*)UserName);
    _tcscpy_s(lpParams.szPassword,128,(TCHAR*)Password);
    HRASCONN dwRasconn = NULL;//Do init. else error 668
    _tprintf(L"Dialing.....\n");
    DWORD dwResult = RasDial(NULL, NULL, &lpParams, NULL, NULL, &dwRasconn);
    if (dwResult == ERROR_SUCCESS)
    {
        _tprintf(L"Dial Success!\n");
        ucErrorcode = dwResult;
        return dwRasconn;
    }
    else
    {
        _tprintf(L"Dial Error! %i", dwResult);
        ucErrorcode = dwResult;
        return 0;
    }
}
bool PPPoeThread::GetThreadStat()
{
    return FIN;
}
void PPPoeThread::run(){
    FIN = false;
    forever{
        while(!STOP)
        {
            DoPPPdial();
            STOP = true;
            FIN = true;
        }
    }
}
bool PPPoeThread::DoPPPdial(void)
{
    //Is there a PPPoE Connnection Exist?
    LPRASCONN lpRasConn = NULL;
    DWORD dwCb = 0;
    DWORD dwConnections = 0;
    DWORD dwRet = ERROR_SUCCESS;

    dwRet = RasEnumConnections(lpRasConn, &dwCb, &dwConnections);//Get lpRasConn Buffer size.
    if (dwRet == ERROR_BUFFER_TOO_SMALL) {
        lpRasConn = (LPRASCONN)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwCb);//==>Heap alloc
        if (lpRasConn == NULL)
        {
            _tprintf(L"Error ! HeapAlloc Failed!\n");
            return false;
        }
    }
    if (lpRasConn)
       lpRasConn[0].dwSize = sizeof(RASCONN);//Must contain RASCONN structure size

    if (RasEnumConnections(lpRasConn, &dwCb, &dwConnections) == ERROR_SUCCESS && lpRasConn!= NULL)
    {
        _tprintf(L"Active connection detected!If you want to reconnet,Please disconnect it first!\n");
        _tprintf(L"There are active connections:\n");
        for (DWORD i = 0; i < dwConnections; i++) {
             _tprintf(L"%s\n", lpRasConn[i].szEntryName);
        }
        ucErrorcode = 1;
        return false;
    }
    if (lpRasConn && !HeapFree(GetProcessHeap(), 0 , lpRasConn))//==>Free the Heap if HeapAlloc is called....
       {
            _tprintf(L"Heap free error!%d", GetLastError());
            return false;
       }
    //***********************Is Entry Exist?*****************************

    LPCTSTR lpszEntry = L"Srun3 PPPoE";
    RASENTRY *lpRasEnt = NULL;
    if (RasValidateEntryName(NULL, lpszEntry) != ERROR_ALREADY_EXISTS)
    {
       DWORD lpdwEntryInfoSize = NULL;//Create New Entry.
       if (RasGetEntryProperties(NULL, L"", NULL, &lpdwEntryInfoSize, NULL, NULL) == ERROR_BUFFER_TOO_SMALL)
       {
           lpRasEnt = (LPRASENTRY)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, lpdwEntryInfoSize);
           if (lpRasEnt == NULL)
           {
               _tprintf(L"Error! HeapAlloc Failed!\n");
               exit(GetLastError());
           }

           ZeroMemory(lpRasEnt, sizeof(RASENTRY));

           TCHAR szDeviceName[RAS_MaxDeviceName + 1] = { 0 };
           if (!GetPortName(szDeviceName)) {
               _tprintf(L"No Port Found! Exit!");
               ucErrorcode = 2;
               return false;
           }

        lpRasEnt->dwSize = sizeof(RASENTRY);
        lpRasEnt->dwfOptions = RASEO_SwCompression | RASEO_RemoteDefaultGateway | RASEO_PreviewUserPw | RASEO_PreviewDomain;
        _tcscpy_s(lpRasEnt->szDeviceName, szDeviceName);
        _tcscpy_s(lpRasEnt->szDeviceType, L"pppoe");
#if (WINVER >= 0x600)
        lpRasEnt->dwfNetProtocols = RASNP_Ip | RASNP_Ipv6;//IPV6 Is Enabled Default.
#else
        lpRasEnt->dwfNetProtocols = RASNP_Ip;
#endif
        if (RasSetEntryProperties(NULL, lpszEntry, lpRasEnt, lpdwEntryInfoSize, NULL, NULL) != ERROR_SUCCESS)
     {
          _tprintf(L"Cannot Create a PPPoE Connection!\n");
          ucErrorcode = 3;
          return false;
     }
        if (lpRasEnt && !HeapFree(GetProcessHeap(), NULL, lpRasEnt))//==> Free Heap
     {
       _tprintf(L"Heap Free Error!%d\n",GetLastError());
       exit(1);
     }
        }
    }
            //Begin to establish a PPPoE Connection.
            if (dwConnections < 1)
            {
                _tprintf(L"Begin to Dial\n");
                HRASCONN hCon = DoPPP(UserName, Password);
                if (hCon != 0)
                    return true;
                else
                    return false;
            }
            return false;
}
void PPPoeThread::GetID(TCHAR * const szPPPoename, TCHAR * const szPPPoePass)
{
    _tcscpy(UserName,szPPPoename);
    _tcscpy(Password,szPPPoePass);
}
void PPPoeThread::setRun()
{
    STOP = false;
}
DWORD PPPoeThread::getErrorCode()
{
    return ucErrorcode;
}
void MainWindow::waitms(DWORD ms){
    QDateTime last = QDateTime::currentDateTime();
    QDateTime now;
    while (1)
    {
        now = QDateTime::currentDateTime();
        if (last.msecsTo(now) >= ms)
        {
            break;
        }
    }
}

