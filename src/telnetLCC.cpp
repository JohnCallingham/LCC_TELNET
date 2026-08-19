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
    TelnetLCC::TelnetMenuCommand command1;
    command1.commandShort = "?";
    command1.commandLong = "help";
    command1.description = "Show this help message";
    command1.argument = 0; // Not used.
    command1.handler = showMenuCommands;
    TelnetLCC::registerTelnetMenuCommand(command1);

    TelnetLCC::TelnetMenuCommand command2;
    command2.commandShort = "q";
    command2.commandLong = "quit";
    command2.description = "Disconnect from the Telnet session";
    command2.argument = 0; // Not used.
    command2.handler = disconnectTelnet;
    TelnetLCC::registerTelnetMenuCommand(command2);

    TelnetLCC::TelnetMenuCommand command3;
    command3.commandShort = "cl";
    command3.commandLong = "clear";
    command3.description = "Clear the screen";
    command3.argument = 0; // Not used.
    command3.handler = clearTelnet;
    TelnetLCC::registerTelnetMenuCommand(command3);
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
      if (input.equalsIgnoreCase(command.commandLong) || input.equalsIgnoreCase(command.commandShort)) {
        command.handler(command.argument);
        telnet.print("> ");
        return;
      }
    }
  
    // No matching command found.
    telnet.println("Unknown command. Type 'help' or '?' for available commands.");
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

    telnet.println("\n(Use CTRL+] + q  to disconnect.)");
    telnet.println("\nMenu options are available by typing 'help' or '?' and pressing enter.");
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

  void showMenuCommands(int i) {
    telnet.println("Available commands;-");
    for (const auto& command : telnetMenuCommands) {
      telnet.println("  " + command.commandLong + " or " + command.commandShort + " - " + command.description);
    }
  }

  void disconnectTelnet(int i) {
    telnet.println("Disconnecting from Telnet session...");
    telnet.disconnectClient();
  }

  void clearTelnet(int i) {
    // telnet.print("\033[1;31mThis is bold red text\033[0m");
    telnet.print("\033[2J");
  }
}
