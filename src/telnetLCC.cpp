#include "telnetLCC.h"

namespace TelnetLCC {
  ESPTelnet telnet; // Create an instance of the ESPTelnet class
  NodeID nodeid;
  String model = "";
  String swVersion = "";
  std::vector<TelnetMenuCommand> telnetMenuCommands;

  void initialiseTelnet() {
    telnet.begin();
    telnet.onConnect(onTelnetConnect);
    telnet.onDisconnect(onTelnetDisconnect);
    telnet.onInputReceived(onTelnetInputReceived);

    /**
     * Add the fixed menu commands.
     */

    // Create a TelnetMenuCommand object and set its common method.
    TelnetLCC::TelnetMenuCommand command;
    command.argument = 0; // Not used.

    // Register a menu command to show help information.
    command.commandShort = "?";
    command.description = " Show this help message";
    command.handler = showMenuCommands;
    TelnetLCC::registerTelnetMenuCommand(command);

    // Register a menu command to allow Telnet session disconnection.
    command.commandShort = "q";
    command.description = " Disconnect from the Telnet session";
    command.handler = disconnectTelnet;
    TelnetLCC::registerTelnetMenuCommand(command);

    // Register a menu command to clear the Telnet client screen.
    command.commandShort = "cl";
    command.description = "Clear the screen";
    command.handler = clearScreen;
    TelnetLCC::registerTelnetMenuCommand(command);
  
    // Register a menu command to show the events.
    command.commandShort = "ev";
    command.description = "Show the events";
    command.handler = showEvents;
    TelnetLCC::registerTelnetMenuCommand(command);

    // Register a menu command to show the Preference values.
    command.commandShort = "p";
    command.description = " Show the Preference values";
    command.handler = showPreferenceValues;
    TelnetLCC::registerTelnetMenuCommand(command);

    // Register a command to restart the processor.
    command.commandShort = "r";
    command.description = " Restart the processor";
    command.handler = restartProcessor;
    TelnetLCC::registerTelnetMenuCommand(command);
  }

  void setNodeID(NodeID id) {
    nodeid = id;
  }

  void setModel(String m) {
    model = m;
  }

  void setSWVersion(String v) {
    swVersion = v;
  }

  void onTelnetInputReceived(String input) {
    // Handle the received input here
    Serial.printf("\n%6ld [onTelnetInputReceived] Received input: %s", millis(), input.c_str());

    // Check for no input.
    if (input.length() == 0) {
      telnet.print("> ");
      return;
    } 

    // Find a matching command.
    for (const auto& command : telnetMenuCommands) {
      if (input.equalsIgnoreCase(command.commandShort)) {
        // Found the matching command, so call its handler function.
        command.handler(command.commandShort, command.argument);
        telnet.print("> ");
        return;
      }
    }
  
    // No matching command found.
    telnet.println("Unknown command. Type '?' for available commands.");
    telnet.print("> ");
    return;
  }

  void onTelnetConnect(String ip) {
    Serial.printf("\n%6ld [onTelnetConnect] Telnet connection from %s", millis(), ip.c_str());
    
    telnet.println("\nWelcome " + telnet.getIP());
    telnet.println("\nThis is an LCC node with the following configuration;-");
    telnet.println("\n            Model: " + model);
    char charNodeID[30] = "";
    sprintf(charNodeID, "%02X.%02X.%02X.%02X.%02X.%02X", nodeid.val[0], nodeid.val[1], nodeid.val[2], nodeid.val[3], nodeid.val[4], nodeid.val[5]);
    telnet.println("          Node ID: " + String(charNodeID));
    telnet.println(" Software version: " + swVersion);
    telnet.println(" Compilation date: " + String(__DATE__));
    telnet.println(" Compilation time: " + String(__TIME__));

    // telnet.println("\n(Use CTRL+] + q  to disconnect.)");
    telnet.println("\nMenu options are available by typing '?' and pressing enter.");
    telnet.print("> ");
  }

  void onTelnetDisconnect(String ip) {
    Serial.printf("\n%6ld [onTelnetDisconnect] Telnet connection from %s closed", millis(), ip.c_str());
  }

  void loop() {
    telnet.loop();
  }

  void logMessageCallbackFunction(const char* format, ...) {
    char logMessageBuffer[200];

    // Format the log message using the provided format and arguments.
    va_list argptr;
    va_start(argptr, format);
    vsnprintf(logMessageBuffer, sizeof(logMessageBuffer), format, argptr);
    va_end(argptr);

    // Telnet needs a carriage return before the line feed to display correctly.
    telnet.print("\r");
    telnet.print(logMessageBuffer);

    // Serial monitor doesn't need the carriage return.
    Serial.print(logMessageBuffer);
  }

  void registerTelnetMenuCommand(TelnetMenuCommand command) {
    telnetMenuCommands.push_back(command);
  }

  void showMenuCommands(String commandShort, int i) {
    telnet.println("Available commands;-");
    for (const auto& command : telnetMenuCommands) {
      telnet.println("  " + command.commandShort + " - " + command.description);
    }
  }

  void disconnectTelnet(String commandShort, int i) {
    telnet.println("Disconnecting from Telnet session...");
    telnet.disconnectClient();
  }

  void clearScreen(String commandShort, int i) {
    telnet.print("\033[2J");
  }

  void showEvents(String commandShort, int i) {
    telnet.printf("Num events: %d\r\n", OpenLcb.numEvents);
    telnet.printf("Index C/P Event                    C/P Event + 1\r\n");

    for (uint16_t i = 0; i < OpenLcb.numEvents; i++) {
      // Print index at the start of a line.
      if ((i % 2) == 0) {
        telnet.printf(" %02X   ", i);
      }

      // Print flags.
      printFlags(OpenLcb.events[i].flags);

      telnet.print("  ");

      // Print eventID.
      printEventID(OpenLcb.events[i].eid);

      // Print spaces or CR,LF.
      if (i % 2 == 0) {
        telnet.print("  ");
      } else {
        telnet.println();
      }
    }

    telnet.print("\r\n");
  }

  void printEventID(EventID eventID) {
    for (int i = 0; i < 8; i++) {
      if (i > 0) telnet.print(".");
      telnet.printf("%02X", eventID.val[i]);
    }
  }

  void printFlags(uint16_t flags) {
    switch (flags) {
      case 0x20:
        telnet.print(" C");
        break;
      case 0x40:
        telnet.print(" P");
        break;
      default:
        telnet.printf("%02X", flags);
        break;
    }
  }

  void showPreferenceValues(String commandShort, int i) {
    telnet.print("Preferences values;-\r\n");

    telnet.print("         Node ID: ");
    printNodeID(ConfigurationPreferences::getNodeID(NodeID(0,0,0,0,0,0)));
    telnet.println();

    telnet.printf("       WiFi SSID: %s", ConfigurationPreferences::getWiFiSSID());
    telnet.println();

    telnet.printf("  Hub IP Address: %s", ConfigurationPreferences::getHubIPAddress().c_str());
    telnet.println();

    telnet.printf(" Node IP Address: %s", ConfigurationPreferences::getNodeIPAddress().c_str());
    telnet.print("\r\n");
  }

  void printNodeID(NodeID nodeID) {
    for (int i = 0; i < 6; i++) {
      if (i > 0) telnet.print(".");
      telnet.printf("%02X", nodeID.val[i]);
    }
  }

  void restartProcessor(String commandShort, int i) {
    // Disconnect telnet first.
    telnet.println("Disconnecting from Telnet session...");
    telnet.disconnectClient();

    ESP.restart();
  }
}
