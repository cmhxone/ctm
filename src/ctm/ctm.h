#pragma once

#ifndef _CTM_CTM_CTM_H_
#define _CTM_CTM_CTM_H_

namespace ctm {
class CTM {
  public:
    CTM();
    virtual ~CTM() = default;

    const CTM &operator=(const CTM &) = delete;
    CTM(const CTM &) = delete;

  protected:
  private:
};
} // namespace ctm

#endif