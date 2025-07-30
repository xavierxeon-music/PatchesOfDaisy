#ifndef PageRampH
#define PageRampH

#include "PageAbstract.h"

class PageRamp : public PageAbstract
{
public:
   PageRamp(Main* main);

private:
   void render(OledDisplay* display) override;
   State forward() override;
   State back() override;

private:
   static const std::vector<Operation> operationOrder;
};

#endif // NOT PageRampH
