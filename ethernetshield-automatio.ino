#include <SPI.h>
#include <string.h>
#include <Ethernet.h>
#include <Servo.h>

// INICIALIZAR O SERVO
Servo myservo;

// CONFIGURAÇÃO DO MAC, IP E PORTA PARA ETHERNET SHIELD
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC DA PLACA ETHERNET
byte ip[] = { 192, 168, 0 , 10}; // IP DA PLACA ETHERNET
EthernetServer server(8090); //PORTA DA PLACA ETHERNET

// INICIALIZAR AS LEITURAS DO STATUS
String readString = String(30); //
String statusRele;
String statusServo;

//PINO DIGITAL UTILIZADO PELO SERVO MOTOR
int servo = 9;

//PINO DIGITAL UTILIZADO PELO MÓDULO RELÉ
const int rele = 2;

void setup() {
  
  //SETUP ETHERNET
  Ethernet.begin(mac, ip);

  //SETUP SERVO
  myservo.attach(7); //pino de dados do servo conectado ao pino 9
  myservo.write(0);


  //SETUP RELÉ
  pinMode(rele, OUTPUT); //DEFINE O PINO DO RELÉ COMO SAÍDA
  digitalWrite(rele, HIGH); //MÓDULO RELÉ INICIA DESLIGADO

}

void loop() {

  EthernetClient client = server.available();
  
  if(client) {
    while(client.connected()) {

      if(client.available()) {

        char c = client.read();

        if(readString.length() < 100) {
          readString += (c);
        }

        if(c == '\n') {

          if(readString.indexOf("rele") > 0) {
            digitalWrite(rele, !digitalRead(rele));
          }

          if(readString.indexOf("servo") > 0) {
            digitalWrite(servo, !digitalRead(servo));
            myservo.write(180);
            delay(5000);
            myservo.write(0);
          }

          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("");

          client.println("<!DOCTYPE html>");
          client.println("<html lang=\"pt-br\">");
          client.println("<head>");
          client.println("<meta charset=\"UTF-8\">");
          client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
          client.println("<title>Projeto Arduino - Aquário</title>");
          client.println("</head>");
          client.println("<body>");
          client.println("<h1>Projeto Arduino Uno - Aquário</h1>");
          client.println("<form action=\"rele\" method=\"get\">");
          if(digitalRead(rele)) {
            statusRele = "Desligado";
          } else {
            statusRele = "Ligado";
          }

          client.println("<button type=\"submit\" style=\"width: 200px;\">Relé - "+ statusRele +"</button>");
          client.println("</form>");

          client.println("<form action=\"servo\" method=\"get\">");
          if(digitalRead(servo)) {
            statusServo = "Desligado";
          } else {
            statusServo = "Ligado";
          }

          client.println("<button type=\"submit\" style=\"width: 200px;\">Relé - "+ statusServo +"</button>");
          client.println("</form>");
          client.println("</body>");
          client.println("</html>");
          client.println("");
          client.println("");

          readString = "";

        }
      }
    }
  }

}