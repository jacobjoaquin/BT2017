void debugPrintBeamBufferForwardReverse() {
  int face = 0;
  delay(1000);
  Serial.println("hello world");

  for (int i = 0; i < ledsPerFace; i++) {
    Serial.print(beamBuffer[getFaceLED(face, i)]);
    Serial.print(", ");
  }
  Serial.println();

  Serial.println("Rotating Forward");
  rotateFace(face, true);
  for (int i = 0; i < ledsPerFace; i++) {
    Serial.print(beamBuffer[getFaceLED(face, i)]);
    Serial.print(", ");
  }
  Serial.println();

  Serial.println("Rotating Reverse");
  rotateFace(face, false);
  for (int i = 0; i < ledsPerFace; i++) {
    Serial.print(beamBuffer[getFaceLED(face, i)]);
    Serial.print(", ");
  }
  Serial.println();
}
