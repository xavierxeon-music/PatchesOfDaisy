#ifndef PageGateH
#define PageGateH

#include "PageAbstract.h"

class PageGate : public PageAbstract
{
public:
   PageGate(Main* main);

private:
   void render(OledDisplay* display) override;
   State forward() override;
   State back() override;

private:
   static const std::vector<Operation> operationOrder;
};

#endif // NOT PageGateH
