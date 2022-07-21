#include "ConsoleApplication.h"

#include <csignal>

#include <QCoreApplication>

void ConsoleApplication::catchSignals()
{
   signal(SIGTERM, exitSignal); // catch kill signal
#ifndef Q_OS_WIN32
   signal(SIGHUP, exitSignal);  // catch hang up signal
   signal(SIGQUIT, exitSignal); // catch quit signal
#endif
   signal(SIGINT, exitSignal);  // catch a CTRL-c signal
}

void ConsoleApplication::exitSignal(int sig)
{
   Q_UNUSED(sig);
   QCoreApplication::quit();
}
