#ifndef TELNET_LCC_H
#define TELNET_LCC_H

#include <vector>
#include <ESPTelnet.h>
#include "NodeID.h"
#include "OpenLcbCore.h"

namespace TelnetLCC {
  struct TelnetMenuCommand {
    String commandShort;
    String commandLong;
    String description;
    int argument; // Used to specify the object value, e.g. toti 1.
    void (*handler)(int);
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
  void showMenuCommands(int i);
  void disconnectTelnet(int i);
  void clearScreen(int i);
  void showEvents(int i);
  void printEventID(EventID eventID);
  void printFlags(uint16_t flags);
}

#endif
