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

int setFaceLED(int face, int index) {
        int i = 0;
        if (face == 0) {
                // [r2, g3, -y1]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 2, 2, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 3, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 1, 1, 2, true);
                }
        } else if (face == 1) {
                // [-y1, g0, r1]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 1, 1, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 0, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 2, 1, 2, false);
                }
        } else if (face == 2) {
                // [-r1, g1, -y2]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 2, 1, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 1, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 1, 2, 2, true);
                }
        } else if (face == 3) {
                // [y2, g2, -r2]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 1, 2, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 2, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 2, 2, 2, true);
                }
        } else if (face == 4) {
                // [-y3, g2, r3]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 1, 3, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 2, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 2, 3, 2, false);
                }
        } else if (face == 5) {
                // [-r3, g3, -y0]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 2, 3, 0, true);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 3, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 1, 0, 2, true);
                }
        } else if (face == 6) {
                // [y0, g0, -r0]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 1, 0, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 0, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 2, 0, 2, true);
                }
        } else if (face == 7) {
                //     [r0, g1, y3]
                if (index < ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 2, 0, 0, false);
                } else if (index < 2 * ledsPerBeam) {
                        i = faceIndexToDisplayIndex(index, 0, 1, 1, false);
                } else {
                        i = faceIndexToDisplayIndex(index, 1, 3, 2, false);
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
        list.append("int faceTable[] = {");

        for (int i = 0; i < nFaces; i++) {
                for (int j = 0; j < ledsPerFace; j++) {
                        list.append(setFaceLED(i, j));
                        list.append(", ");
                }
        }
        list.delete(list.length() - 2, list.length());
        list.append("};\n");

        try {
                FileWriter writer = new FileWriter("/Users/jacobjoaquin/Documents/Projects/BT2017/BT2017/processing/faceTableGenerator/output.txt");
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
