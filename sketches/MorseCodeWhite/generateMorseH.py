#!/usr/bin/python

quote = 'HELLO WORLD DISORIENT PRESENTS BEDOUIN TECH 2017 CREATE PARTICIPATE LOVE DISORIENT LOVES YOU'

dur = {'.': 1, '-': 3, 'letter': 3, 'space': 7, 'none': 0}
morse = {
    'A': '.-', 'B': '-...', 'C': '-.-.', 'D': '-..', 'E': '.', 'F': '..-.',
    'G': '--.', 'H': '....', 'I': '..', 'J': '.---', 'K': '-.-', 'L': '.-..',
    'M': '--', 'N': '-.', 'O': '---', 'P': '.--.', 'Q': '--.-', 'R': '.-.',
    'S': '...', 'T': '-', 'U': '..-', 'V': '...-', 'W': '.--', 'X': '-..-',
    'Y': '-.--', 'Z': '--..', '0': '-----', '1': '.----', '2': '..---',
    '3': '...--', '4': '....-', '5': '.....', '6': '-....', '7': '--...',
    '8': '---..', '9': '----.'}

data_table = {
'.': '1',
'-': '2',
' ': '0'
}

encoded = ''

# Process each character in quote
for c in quote:
    # Letter
    if c in morse:
        for m in morse[c]:
            encoded += m;

        encoded += ' ';

    # Space
    elif c == ' ':
        encoded += ' '

# encoded += ' '

# Generate data
data = []
for c in encoded:
    if c == ' ':
        data.append(0)
        data.append(0)
    elif  c == '.':
        data.append(1)
        data.append(0)
    elif c == '-':
        data.append(2)
        data.append(2)
        data.append(2)
        data.append(0)

# Generate output
output = '// File Generated with generateMorseH.py\n'
output += '// ' + quote + '\n'
output += 'const uint8_t encoded[] PROGMEM = {'
output += ', '.join(map(str, data));
output += '};\n'
output += 'const int encodedLength = ' + str(len(data)) + ';\n'

with open('encoded.h', 'w') as f:
    f.write(output)

# Print results
print("'" + encoded + "'")
print(len(encoded))
print(output)
print(encoded)
