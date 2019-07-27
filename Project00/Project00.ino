/*
 * Project00 - A.D.A. - Controle de Dispositivos
 * Intended to be run on an ESP8266
 */
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
 
String html_1 = R"=====(
<!DOCTYPE html>
<html>
 <head>
 <meta name='viewport' content='width=device-width, initial-scale=1.0'/>
 <title>Felipe Marques</title>
 <meta charset='utf-8'>
 
 <script type="text/javascript" src="http://code.jquery.com/jquery-1.7.1.js"></script>
 
 <style>
  #checkboxes { line-height: 0; }
  h2 {text-align:center; }
  #checkboxes input[type="checkbox"] { margin: 0px; padding: 0px; cursor: pointer;}
 </style>
 
 <script type="text/javascript">//<![CDATA[
 
    $(window).load(function(){
 
        $(function(){
   
            var target = $('#checkboxes');
 
            var  x, y, checkbox;
            for(x = 0; x < 8; x++)
            {
                for(y = 0; y < 32; y++)
                {
                    checkbox = $('<input></input>', {
                        'type': 'checkbox',
                        'data-x': x,
                        'data-y': x,
                        'onclick': "submit();",
                        'name': +x+'-'+y,
                    });
           
                    target.append(checkbox);
                }
            }
   
            target.width(checkbox.outerWidth() * 32);
       
                //alert('checkbox changed (' + x + ', ' + y + '): ' + checked);
 
 
          });
 
    });
 </script>
 
</head>
<body>
<b>A.D.A. - Controle de Dispositivos -- NodeMCU + FC16</b>
 <form method="get">
  <div id="checkboxes">
    </form>
</div>
)=====";
 
String html_4 = R"=====(    
</body>
</html>
)=====";
 
#include <ESP8266WiFi.h>
#include <FC16.h>
 
const int csPin = D8;                       // CS pin used to connect FC16
const int displayCount = 4;                 // Number of displays; usually 4 or 8

FC16 display = FC16(csPin, displayCount);

// change these values to match your network
char ssid[] = "ssid";       //  your network SSID (name)
char pass[] = "pw";          //  your network password
 
WiFiServer server(80);
 
 
String request = "";
int LED_Pin = D1;

int matriz[8][32];
 
 
void setup()
{
   
    display.begin();            // turn on display
    display.setIntensity(8);    // set medium brightness
    display.clearDisplay();     // turn all LED off    
 
    Serial.begin(115200);
    Serial.println();
    Serial.println("Serial started at 115200");
    Serial.println("Project00 - A.D.A. - Controle de Dispositivos");
    Serial.println();
 
    // Connect to a WiFi network
    Serial.print(F("Connecting to "));  Serial.println(ssid);
    WiFi.begin(ssid, pass);
 
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
 
    Serial.println("");
    Serial.println(F("[CONNECTED]"));
    Serial.print("[IP ");              
    Serial.print(WiFi.localIP());
    Serial.println("]");
 
    // start a server
    server.begin();
    Serial.println("Server started");

    int i, j;
    for(i = 0; i < 8; i++){
      for(j = 0; j < 32; j++){
        matriz[i][j] = 0;
      }  
    }
 
} // void setup()
 
 
 
void loop()
{
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
    Serial.print("request: "); Serial.println(request);
 
    String oChamado = request;
    if (oChamado != NULL && oChamado != "GET / HTTP/1.1" && oChamado != "GET /favicon.ico HTTP/1.1") {
        int linha = request[6] - '0';
        //Serial.print(linha);
        int coluna = oChamado[8]-'0';
        //Serial.print(coluna);
        int coluna2;

        if(oChamado[9] != '=') {
            coluna2 = oChamado[9] - '0';
            coluna = (coluna*10) + coluna2;
        }
        if(matriz[linha][coluna] == 0){
          display.setLed(linha, coluna, true);
          matriz[linha][coluna] = 1;
        }
        else{
          display.setLed(linha, coluna, false);
          matriz[linha][coluna] = 0;
        }
    }
   
    //else
    //{
 
        client.flush();
        client.print( header );
        client.print( html_1 );    
        client.print( html_4 );
        delay(5);
    //}
 
 
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
} // void loop()
