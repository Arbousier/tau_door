/*
  Web Server 1 Relay
  based on "Web Server" Examples by "David A. Mellis" and "Tom Igoe"
*/

#include <SPI.h>
#include <Ethernet.h>

#define HTTP_VERB             "GET"
#define LINE_BUFFER_SIZE      255
#define REQUEST_BUFFER_SIZE   96

#define TIMER_PORTE_AUTO      500

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,250);

// Initialize the Ethernet server library
// with the IP address and port you want to use 
// (port 80 is default for HTTP):

EthernetServer server(80);

int switchOneState = LOW;

const int switchOpen = 7;

void setup()
{
  // Initialize the relays
  pinMode(switchOpen, OUTPUT);

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
}

// Extract /XYZ from a request line :
// GET /XYZ HTTP/1.1

void get_page(char *line, char *request, char *verb)
{
  int i = 0;
  int verb_and_space_len = strlen(verb) + 1; // add the space between the verb and the request
  for (; line[i + verb_and_space_len] && ( line[i + verb_and_space_len] != ' ' ) && (i < REQUEST_BUFFER_SIZE); i++)
    request[i] = line[i + verb_and_space_len]; 
  request[i] = 0;
}

void open_door()
{
  digitalWrite(switchOpen, HIGH);
  delay(TIMER_PORTE_AUTO);
  digitalWrite(switchOpen, LOW);
  // open door
}

void loop()
{
  char lineBuffer[LINE_BUFFER_SIZE];
  int  lineBufferIndex = 0;
  char requestBuffer[REQUEST_BUFFER_SIZE];

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
                    // add a meta refresh tag, so the browser pulls again every 5 seconds:
          client.println("<meta http-equiv=\"refresh\" content=\"5\">");
          client.println("<body>");
          
          client.println("<h1>HELLO</h1>");
          get_page(lineBuffer, requestBuffer, HTTP_VERB);
          
          if (strcmp(requestBuffer, "/openthedoor") == 0) {
            open_door();
          } else {
            client.println("<a href='/openthedoor'>open</a> <br />");
          }
          client.println("</body></html>");
          // reinit
          lineBufferIndex = 0;
          
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          lineBuffer[lineBufferIndex++] = 0;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          if (lineBufferIndex <= LINE_BUFFER_SIZE - 1)
            lineBuffer[lineBufferIndex++] = c;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}
