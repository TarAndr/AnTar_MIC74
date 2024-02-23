
void showPORT() {
  char sBuffer[80];
  Serial.println("\n*************\n");
  sprintf(sBuffer,"\nGPIOS: %d - %d\n", mic.readPortMode(), mic.portRead());
  Serial.print(sBuffer);
  Serial.println(mic.readPortMode(), BIN);
  Serial.println(mic.portRead(), BIN);
}
