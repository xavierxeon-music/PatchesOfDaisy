#ifndef PageOverviewH
#define PageOverviewH

#include "PageAbstract.h"

#include <Blocks/RandomChain.h>

class PageOverview : public PageAbstract
{
public:
   PageOverview(Main* main);

private:
   enum class Operation
   {
      Gate1,
      Gate2,
      Gate3,
      Gate4,
      Ramp1,
      Ramp2
   };

private:
   void render(OledDisplay* display) override;
   State forward() override;
   State back() override;
   void alterChainType() const;

private:
   static const std::vector<Operation> operationOrder;
   Operation operation;
};

#endif // NOT PageOverviewH
