#ifndef TELNET_LCC_H
#define TELNET_LCC_H

#include <vector>
#include <ESPTelnet.h>
#include "NodeID.h"
#include "OpenLcbCore.h"
#include "configurationPreferences.h"

namespace TelnetLCC {
  struct TelnetMenuCommand {
    String commandShort;
    String description;
    int argument; // Used to specify the object value, e.g. toti 1.
    void (*handler)(String, int);
  };

  void initialiseTelnet();
  void setNodeID(NodeID id);
  void setModel(String m);
  void setSWVersion(String v);
  void onTelnetConnect(String ip);
  void onTelnetDisconnect(String ip);
  void onTelnetInputReceived(String input);
  void loop();
  void registerTelnetMenuCommand(struct TelnetMenuCommand command);

  /**
   * Command handlers.
   */
  void showMenuCommands(String commandShort, int i);
  void disconnectTelnet(String commandShort, int i);
  void clearScreen(String commandShort, int i);
  void showEvents(String commandShort, int i);
  void displayLogMessages(String commandShort, int i);
  void restartProcessor(String commandShort, int i);

  /**
    * Helper functions.
    */
  void logMessageCallbackFunction(const char* format, ...);
  void displayMessageAtPosition(int row, int col, String message);
  void printEventID(EventID eventID);
  void printFlags(uint16_t flags);
  void showPreferenceValues(String commandShort, int i);
  void printNodeID(NodeID nodeID);
  void clearScreen();

}

#endif
