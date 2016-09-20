/*
**  This file is part of Vidalia, and is subject to the license terms in the
**  LICENSE file, found in the top level directory of this distribution. If
**  you did not receive the LICENSE file with this file, you may obtain it
**  from the Vidalia source package distributed by the Vidalia Project at
**  http://www.torproject.org/projects/vidalia.html. No part of Vidalia,
**  including this file, may be copied, modified, propagated, or distributed
**  except according to the terms described in the LICENSE file.
*/

/*
** \file torservice.h
** \brief Starts, stops, installs, and uninstalls a Tor service (Win32).
*/

#ifndef _TORSERVICE_P_H
#define _TORSERVICE_P_H

#include <qt_windows.h>

#define TOR_SERVICE_NAME "tor"
#define TOR_SERVICE_DISP "Tor Win32 Service"
#define TOR_SERVICE_DESC \
  TEXT("Provides an anonymous Internet communication system.")
#define TOR_SERVICE_ACCESS SERVICE_ALL_ACCESS
#define SERVICE_ERROR 8

/* NT service function prototypes. This code is adapted from Tor's
 * nt_service_load_library() in main.c. See LICENSE for details on
 * Tor's license. */
typedef BOOL (WINAPI *ChangeServiceConfig2A_fn)(
                             SC_HANDLE hService,
                             DWORD dwInfoLevel,
                             LPVOID lpInfo);
typedef BOOL (WINAPI *CloseServiceHandle_fn)(
                             SC_HANDLE hSCObject);
typedef BOOL (WINAPI *ControlService_fn)(
                             SC_HANDLE hService,
                             DWORD dwControl,
                             LPSERVICE_STATUS lpServiceStatus);
typedef SC_HANDLE (WINAPI *CreateServiceA_fn)(
                             SC_HANDLE hSCManager,
                             LPCTSTR lpServiceName,
                             LPCTSTR lpDisplayName,
                             DWORD dwDesiredAccess,
                             DWORD dwServiceType,
                             DWORD dwStartType,
                             DWORD dwErrorControl,
                             LPCTSTR lpBinaryPathName,
                             LPCTSTR lpLoadOrderGroup,
                             LPDWORD lpdwTagId,
                             LPCTSTR lpDependencies,
                             LPCTSTR lpServiceStartName,
                             LPCTSTR lpPassword);
typedef BOOL (WINAPI *DeleteService_fn)(
                             SC_HANDLE hService);
typedef SC_HANDLE (WINAPI *OpenSCManagerA_fn)(
                             LPCTSTR lpMachineName,
                             LPCTSTR lpDatabaseName,
                             DWORD dwDesiredAccess);
typedef SC_HANDLE (WINAPI *OpenServiceA_fn)(
                             SC_HANDLE hSCManager,
                             LPCTSTR lpServiceName,
                             DWORD dwDesiredAccess);
typedef BOOL (WINAPI *QueryServiceStatus_fn)(
                             SC_HANDLE hService,
                             LPSERVICE_STATUS lpServiceStatus);
typedef BOOL (WINAPI *SetServiceStatus_fn)(SERVICE_STATUS_HANDLE,
                             LPSERVICE_STATUS);
typedef BOOL (WINAPI *StartServiceA_fn)(
                             SC_HANDLE hService,
                             DWORD dwNumServiceArgs,
                             LPCTSTR* lpServiceArgVectors);

/** Table of NT service related functions. */
struct ServiceFunctions {
  bool loaded;
  ChangeServiceConfig2A_fn ChangeServiceConfig2A;
  CloseServiceHandle_fn    CloseServiceHandle;
  ControlService_fn        ControlService;
  CreateServiceA_fn        CreateServiceA;
  DeleteService_fn         DeleteService;
  OpenSCManagerA_fn        OpenSCManagerA;
  OpenServiceA_fn          OpenServiceA;
  QueryServiceStatus_fn    QueryServiceStatus;
  SetServiceStatus_fn      SetServiceStatus;
  StartServiceA_fn         StartServiceA;
};

class TorService;

class TorServicePrivate
{
public:
  TorServicePrivate(TorService* _q_ptr);
  ~TorServicePrivate();

  /** Dynamically loads NT service related functions from advapi32.dll. */
  static bool loadServiceFunctions();
  /** Opens a handle to the Tor service. Returns NULL on error. */
  SC_HANDLE openService();
  /** Opens a handle to the service control manager. Returns NULL on error. */
  static SC_HANDLE openSCM();
  /** Closes the service <b>handle</b>. */
  static void closeHandle(SC_HANDLE handle);
  /** Gets the status of the Tor service. */
  DWORD status();

  /** Handle to the service control manager. */
  SC_HANDLE _scm;
  /** List of dynamically loaded NT service functions. */
  static ServiceFunctions _service_fns;

  /** Pointer to a public class. */
  TorService* q_ptr;
};

#endif
