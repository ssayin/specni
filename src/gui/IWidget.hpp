#pragma once

struct IWidget {
  virtual void Show() const = 0;
  virtual ~IWidget() = default;
};