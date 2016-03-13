#pragma once

namespace Networking
{
  class NetworkComponent : public Component
  {
  public:
    NetworkComponent();
    ~NetworkComponent();

    virtual void Initialize() override;

  private:
  };
}
