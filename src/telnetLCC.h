#ifndef TELNET_LCC_H
#define TELNET_LCC_H

#include <vector>
#include <ESPTelnet.h>
#include "NodeID.h"

namespace TelnetLCC {
  struct TelnetMenuCommand {
    String commandShort;
    String commandLong;
    String description;
    void (*handler)();
  };

  void initialiseTelnet();
  void setNodeID(NodeID id);
  void setModel(String m);
  void setSWVersion(String v);
  void onTelnetConnect(String ip);
  void onTelnetDisconnect(String ip);
  void onTelnetInputReceived(String input);
  void loop();
  void logMessageCallbackFunction(const char* format, ...);
  void registerTelnetMenuCommand(struct TelnetMenuCommand command);
  void showMenuCommands();
  void disconnectTelnet();
}

#endif
