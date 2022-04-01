#include "ConsoleApplication.h"

#include <csignal>

#include <QCoreApplication>

void ConsoleApplication::catchSignals()
{
   signal(SIGTERM, exitSignal); // catch kill signal
   signal(SIGHUP, exitSignal);  // catch hang up signal
   signal(SIGQUIT, exitSignal); // catch quit signal
   signal(SIGINT, exitSignal);  // catch a CTRL-c signal
}

void ConsoleApplication::exitSignal(int sig)
{
   Q_UNUSED(sig);
   QCoreApplication::quit();
}
