#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#include "Marsohod.h"

WSADATA wsd;

int main(int argc, char* argv[])
{
  _wsetlocale(LC_ALL, L"");
  int rc = WSAStartup(MAKEWORD(2, 2), &wsd);
  if (rc != 0) {
    wprintf(L"Network error.\n");
    return -1;
  }
  CMarsohod conn;
  conn.start_tcl_script(L"stp.cmd");
  Sleep(15000);
  conn.stp_connect();
  int result = conn.shift_ir(0, 1, 1);
  result = conn.shift_dr_hex("50", 0, 8, 0);
  conn.stp_disconnect();
  WSACleanup();
  return 0;
}

