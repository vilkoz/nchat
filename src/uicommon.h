// uicommon.h
//
// Copyright (c) 2019 Kristofer Berggren
// All rights reserved.
//
// nchat is distributed under the MIT license, see LICENSE for details.

#pragma once

#include <map>
#include <mutex>
#include <stack>
#include <string>
#include <vector>

#include "config.h"
#include "ui.h"

class Config;
class Contact;
struct Message;
class Protocol;

class UiCommon : public Ui
{
public:
  explicit UiCommon(const std::string& p_Name);
  virtual ~UiCommon();
  
  virtual void Init();
  virtual std::string GetName();
  virtual void Cleanup();
  virtual void AddProtocol(Protocol* p_Protocol);
  virtual void RemoveProtocol(Protocol* p_Protocol);
  virtual void UpdateChat(Chat p_Chats);
  virtual void UpdateChats(std::vector<Chat> p_Chats);
  virtual void UpdateMessages(std::vector<Message> p_Messages, bool p_ClearChat = false);
  virtual void Run();

protected:
  virtual std::map<std::string, std::string> GetPrivateConfig() = 0;
  virtual void PrivateInit() = 0;
  virtual void RedrawContactWin() = 0;
  virtual void SetupWin() = 0;
  virtual void CleanupWin() = 0;
  
  void RequestRedraw(char p_WinId);
  
  void RedrawInputWin();
  void RedrawOutputWin();

  void NextPage(int p_Offset);
  void MoveInputCursor(int p_Key);
  void NextChat(int p_Offset);
  void Backspace();
  void Delete();
  void Send();
  void NextUnread();
  void Exit();
  void SetCurrentChat(const std::string& p_Chat);

  void InputBuf(wint_t ch);
  void ToggleEmoji();  
  
protected:
  std::string m_Name = "undefined";
  
  WINDOW* m_InWin = NULL;
  WINDOW* m_OutWin = NULL;

  Config m_Config;

  const char m_InputWinId  = 1 << 0;
  const char m_OutputWinId = 1 << 1;
  const char m_ContactWinId = 1 << 2;
  
  size_t m_ScreenWidth = 0;
  size_t m_ScreenHeight = 0;

  size_t m_OutHeight = 0;
  size_t m_OutWidth = 0;

  size_t m_InHeight = 0;
  size_t m_InWidth = 0;
  
  bool m_Running = false;

  int m_KeyNextChat = 0;
  int m_KeyPrevChat = 0;
  int m_KeyNextPage = 0;
  int m_KeyPrevPage = 0;
  int m_KeyCursUp = 0;
  int m_KeyCursDown = 0;
  int m_KeyCursLeft = 0;
  int m_KeyCursRight = 0;
  int m_KeyBackspace = 0;
  int m_KeyDelete = 0;
  int m_KeyLinebreak = 0;
  int m_KeySend = 0;
  int m_KeyNextUnread = 0;
  int m_KeyExit = 0;
  int m_KeyToggleEmoji = 0;
  
  bool m_HighlightBold = true;
  bool m_ShowEmoji = true;

  int m_Sockets[2] = {-1, -1};

  std::map<std::string, Protocol*> m_Protocols;
  std::map<std::string, Chat> m_Chats;
  std::map<std::string, std::map<std::int64_t, Message>> m_Messages;
  std::map<std::string, std::wstring> m_Input;
  std::map<std::string, size_t> m_InputCursorPos;
  std::map<std::string, std::stack<std::int64_t>> m_ShowMsgIdBefore;
  std::map<std::string, std::int64_t> m_LowestMsgIdShown;
  std::string m_CurrentChat;

  std::map<std::string, std::vector<std::wstring>> m_InputLines;
  std::map<std::string, size_t> m_InputCursorX;
  std::map<std::string, size_t> m_InputCursorY;

  std::mutex m_Lock;
};
