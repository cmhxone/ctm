#pragma once

#ifndef _CTM_CTI_CLIENT_H_
#define _CTM_CTI_CLIENT_H_

namespace ctm {
class CTIClient {
  public:
    CTIClient();
    virtual ~CTIClient() = default;

    const CTIClient &operator=(const CTIClient &) = delete;
    CTIClient(const CTIClient &) = delete;

  protected:
  private:
};
} // namespace ctm

#endif