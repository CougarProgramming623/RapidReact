#pragma once
#include <frc2/command/CommandBase.h>
#include <frc2/command/CommandHelper.h>
#include <frc2/command/SubsystemBase.h>

class Drive : public frc2::CommandHelper<frc2::CommandBase, Drive>{
public:
    explicit Drive();

     void Initialize() override;
     void Execute() override;
    //  std::unique_ptr<frc2::Command> TransferOwnership() && override;
    //  ~Drive() { };

private:

   
};