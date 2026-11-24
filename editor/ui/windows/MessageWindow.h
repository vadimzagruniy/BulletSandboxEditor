#pragma once

#include <editor/ui/windows/ModalWindow.h>

namespace ui
{

class CMessageWindow:
  public CModalWindow 
{
public: // Types

  using TCallback = std::function<void()>;

public: // Constructor/Destructor

  CMessageWindow(std::string _Title, std::string _Message);

private: // CBaseWindow events

  void OnRenderGui() override;

public: // Interface

  void SetConfirmCallback(TCallback _OnConfirm);

  void SetCancelCallback(TCallback _OnOncancel);

private: // Events

  void OnConfirm();

  void OnCacncel();

private: // Attr

  TCallback   m_ConfirmCallback;
  TCallback   m_CancelCallback;
  std::string m_Message;
};



} // namespace ui