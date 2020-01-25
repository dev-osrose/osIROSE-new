#!/bin/python3
enum = {}

previousValue = None

def parse(line):
    global previousValue
    line = line.strip()
    if len(line) == 0:
        return None, None
    id = line.find("//")
    if id != -1:
        line = line[:id]
    line = line.strip()
    line = line[:-1]
    line = line.split("=")
    line[0] = line[0].strip()
    if len(line[0]) == 0:
        return None, None
    if len(line) < 2:
        if previousValue is None:
            print("error parsing line", line)
            exit(-1)
        previousValue += 1
        line.append("%x" % previousValue)
    else:
        line[1] = line[1].strip()
        try:
            previousValue = int(line[1], 16)
        except ValueError:
            return line[0], enum[line[1]]
    return line[0], int(line[1], 16)

with open("rosecommon/include/epackettype.h", "r") as f:
    while "enum class ePacketType : uint16_t {" not in f.readline():
        pass
    for line in f:
        if "};" in line:
            break
        name, value = parse(line)
        if value:
            enum[name] = value
while True:
    value = input("Enter the hex value of the packet: ")
    for name, v in enum.items():
        if int(value, 16) == v:
            if "PAKCS" in name:
                print("Sent by client :", name)
            else:
                print("Sent by server :", name)
