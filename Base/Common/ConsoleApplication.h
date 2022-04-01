#ifndef ConsoleApplicationH
#define ConsoleApplicationH

class ConsoleApplication
{
public:
   static void catchSignals();

private:
   static void exitSignal(int sig);
};

#endif // NOT ConsoleApplicationH
