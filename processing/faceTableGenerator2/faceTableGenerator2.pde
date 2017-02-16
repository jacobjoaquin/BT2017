import java.lang.StringBuilder;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.File;

int ledsPerStrip = 456;
int nStrips = 3;
int nFaces = 8;
int nLeds = ledsPerStrip * nStrips;
int ledsPerBeam = 114;
int ledsPerFace = 3 * ledsPerBeam;
String fileName = "output.txt";

int g = 0;
int y = 1;
int r = 2;

int setFaceLED(int face, int index) {
        int i = 0;

        if (face == 0) {
                // [y0, g3-, r3]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, y, 0, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 3, 1, true);
                } else {
                        i = faceIndexToDisplayIndex(index, r, 3, 2, false);
                }
        } else if (face == 1) {
                // [r0, g0-, y0-]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, r, 0, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 0, 1, true);
                } else {
                        i = faceIndexToDisplayIndex(index, y, 0, 2, true);
                }
        } else if (face == 2) {
                // [y3-, g1-, r0-]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, y, 3, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 1, 1, true);
                } else {
                        i = faceIndexToDisplayIndex(index, r, 0, 2, true);
                }
        } else if (face == 3) {
                // [r3-, g2-, y3]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, r, 3, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 2, 1, true);
                } else {
                        i = faceIndexToDisplayIndex(index, y, 3, 2, false);
                }
        } else if (face == 4) {
                // [r1-, g1, y2-]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, r, 1, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 1, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, y, 2, 2, true);
                }
        } else if (face == 5) {
                // [y1, g2, r2-]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, y, 1, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 2, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, r, 2, 2, true);
                }
        } else if (face == 6) {
                // [r2, g3, y1]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, r, 2, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 3, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, y, 1, 2, false);
                }
        } else if (face == 7) {
                //     [y1-, g0, r1]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, y, 1, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, g, 0, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, r, 1, 2, false);
                }

        } else if (face == -1) {
                // DEBUG
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 1, 0, true);
                        //      i = index;
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 1, 1, 1, true);
                } else {
                        i = faceIndexToDisplayIndex(index, 2, 1, 2, true);
                }
        }

        return i;
}

int faceIndexToDisplayIndex(int index, int strip, int beam, int triangle, boolean isReverse) {
        index -= triangle * ledsPerBeam;
        index *= 1 - (isReverse ? 1 : 0) * 2;
        return strip * ledsPerStrip + ((beam + (isReverse ? 1 : 0)) * ledsPerBeam - (isReverse ? 1 : 0)) + index;
}

void setup() {
        StringBuilder list = new StringBuilder();
        list.append("const PROGMEM int faceTable[] = {\n\t");
        int datumPerLine = 16;
        int datumCounter = 0;
        for (int i = 0; i < nFaces; i++) {
                for (int j = 0; j < ledsPerFace; j++) {
                        list.append(setFaceLED(i, j));
                        list.append(",");
                        datumCounter++;
                        if (datumCounter % datumPerLine == 0) {
                          list.append("\n\t");
                        } else {
                          list.append(" ");
                        }
                }
        }
        list.delete(list.length() - 3, list.length());
        list.append("};\n");

        try {
                FileWriter writer = new FileWriter(sketchPath("") + fileName);
                PrintWriter printWriter = new PrintWriter(writer);

                printWriter.print(list.toString());
                printWriter.close();
        } catch (FileNotFoundException e) {
                println(e);
        } catch (IOException e) {
                println(e);
        } finally {
        }
        exit();
}
