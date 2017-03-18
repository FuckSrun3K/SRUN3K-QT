#include "mainwindow.h"
#include <QApplication>
BOOL GetAccess() {
    HANDLE hToken;
    TOKEN_PRIVILEGES tpk;
    LUID Luid;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        _tprintf(L"ACCESS DENYED!(%d)\n", GetLastError());
        return FALSE;
    }
    {
        if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &Luid))
        {
            _tprintf(L"Lookup Privilege Error!(%d)\n", GetLastError());
            return FALSE;
        }
        tpk.PrivilegeCount = 1;
        tpk.Privileges[0].Luid = Luid;
        tpk.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        if (!AdjustTokenPrivileges(hToken, FALSE, &tpk, sizeof(tpk), (PTOKEN_PRIVILEGES)NULL, 0))
            _tprintf(L"Connot Get Access!\n");
    }
    return TRUE;
}

int main(int argc, char *argv[])
{
    if (!GetAccess())
    {
        _tprintf(L"Unable to get access.Is you running as Administrator?\n");
        exit(1);
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
