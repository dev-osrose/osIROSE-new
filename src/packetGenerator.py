class Class:
    class Variable:
        def __init__(self, name, type, simple):
            self.name = name
            if self.name[-1] == '_':
                self.name = self.name[:-1]
            self.type = type
            self.simple = simple
            self.getterName = self.name + "()"
            self.complexPack = "//TODO : implement setter for " + self.name + "_"

        def getterHeader(self):
            return self.type + " &" + self.getterName + ";"

        def getterCpp(self, className):
            data = self.type + " &"
            data += className + "::" + self.getterName + " {\n"
            data += "\treturn " + self.name + "_;\n}"
            return data

        def getName(self):
            return self.name + '_'

        def getSetter(self):
            if self.simple:
                return "*this << " + self.name + '_;'
            return self.complexPack

        def getConstructor(self):
            return self.type + " " + self.name

        def getCppConstructor(self):
            return self.name + '_(' + self.name + ')'

        def getGetter(self):
            if self.simple:
                return "*this >> " + self.name + '_;'
            return self.complexPack

        def __str__(self):
            return self.type + ' ' + self.name + '_;'

    def __init__(self, ePacketType):
        self.ePacketType = ePacketType
        self.recv = "PAKCS_" in ePacketType
        self.name = ('Cli' if self.recv else 'Srv') + "".join(ePacketType.title().split("_")[1:])
        self.variables = []
        self.filename = ('cli' if self.recv else 'srv') + "_" + "".join(ePacketType.lower().split("_")[1:])

    def addVariable(self, name, type, simple):
        self.variables.append(Class.Variable(name, type, simple))
        return self.variables[-1]

    def getHeaderConstructor(self):
        constructor = "\t{}(".format(self.name)
        if self.recv:
            constructor += "uint8_t buffer[MAX_PACKET_SIZE]"
        elif len(self.variables):
                constructor += ");\n\n\t\t{}({}".format(self.name, ", ".join([v.getConstructor() for v in self.variables]))
        constructor += ");\n\n\t\tvirtual ~{}() = default;\n\n".format(self.name)
        return constructor

    def getCppConstructor(self):
        constructor = "{0}::{0}(".format(self.name)
        if self.recv:
            constructor += "uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {\n"
            constructor += '\tif (type() != ePacketType::{})\n\t\tthrow std::runtime_error("Not the right packet!");\n'.format(self.ePacketType)
            if len(self.variables):
                for var in self.variables:
                    constructor += "\t{}\n".format(var.getGetter())
        else:
            constructor += ") : CRosePacket(ePacketType::{}) {{\n".format(self.ePacketType)
            if len(self.variables):
                constructor += "}}\n\n{0}::{0}({1}".format(self.name, ", ".join([v.getConstructor() for v in self.variables]))
                constructor += ") : CRosePacket(ePacketType::{}), {} {{".format(self.ePacketType, ", ".join([v.getCppConstructor() for v in self.variables]))
        return constructor + "\n}"

    def getHeader(self):
        data = '#pragma once\n\n#include "packetfactory.h"\n\nnamespace RoseCommon {\n\n'
        if self.recv:
            data += "class {0} : public CRosePacket, public RegisterRecvPacket<ePacketType::{1}, {0}> {{".format(self.name, self.ePacketType)
        else:
            data += "REGISTER_SEND_PACKET(ePacketType::{1}, {0})\nclass {0} : public CRosePacket {{".format(self.name, self.ePacketType)
        data += "\n\tpublic:\n\t"
        data += self.getHeaderConstructor()
        if not self.recv:
            data += "\t\tvirtual void pack();\n\n"
        for var in self.variables:
            data += "\t\t" + var.getterHeader() + "\n"
        if len(self.variables):
            data += "\n\tprivate:\n"
            for var in self.variables:
                data += "\t\t" + str(var) + "\n"
        return data + '};\n\n}'
    
    def getCpp(self):
        data = '#include "{}.h"\n\nnamespace RoseCommon {{\n\n'.format(self.filename)
        data += self.getCppConstructor()
        for var in self.variables:
            data += "\n\n" + var.getterCpp(self.name)
        if self.recv:
            return data + '\n\n}'
        data += "\n\nvoid {}::pack() {{\n".format(self.name)
        for var in self.variables:
            data += "\t{}\n".format(var.getSetter())
        data += "}"
        return data + '\n\n}'

def getInt(msg = ''):
    while True:
        try:
            return int(input(msg))
        except ValueError:
            pass

def packGetComponents(name, components):
    get = "auto {{}} = {}.component<{{}}>();".format(name)
    data = ""
    for c in components:
        tmp = c[0].lower() + c[1:]
        data += "\t{}\n".format(get.format(tmp, c))
    return data[1:]

def entity(var, functions):
    components = set()
    variables = []
    while True:
        print("-----Packing entity-----")
        print("1 - add simple variable/array")
        print("2 - add ISerialize variable")
        print("3 - add ISerialize array")
        print("4 - return")
        a = getInt("> ")
        print()
        if a == 4:
            break
        component, name = [x for x in input("Definition (<Component>::<name>) : ").split(":") if x][:2]
        components.add(component)
        if name[-1] != "_":
            name += "_"
        if a == 1:
            variables.append(("simple var", name, component))
        elif a == 2:
            variables.append(("serialize var", name, component))
        elif a == 3:
            variables.append(("serialize arr", name, component))
    data = packGetComponents(var.getName(), components) + "\n"
    for t, n, c in variables:
        tmp = c[0].lower() + c[1:] + "->" + n
        data += "\t{}\n".format(functions[t].format(tmp))
    var.complexPack = data

def getEntity(var):
    functions = {
            "simple var" : "*this >> {};",
            "serialize var" : "*this >> static_cast<ISerialize&>({});",
            "serialize arr" : "for (auto &it : {}) *this >> static_cast<ISerialize&>(it);"
            }
    entity(var, functions)

def packEntity(var):
    functions = {
            "simple var" : "*this << {};",
            "serialize var" : "*this << static_cast<ISerialize&>({});",
            "serialize arr" : "for (auto &it : {}) *this << static_cast<ISerialize&>(it);"
            }
    entity(var, functions)

def menu(obj):
    while True:
        print("-----Menu-----")
        print("1 - add simple type")
        print("2 - add complex type")
        print("3 - quit")
        a = getInt("> ")
        print()
        if a == 3:
            break
        t, n = input("Definition (<type> <name>) : ").split()[:2]
        if a == 1:
            obj.addVariable(n, t, True)
        elif a == 2:
            var = obj.addVariable(n, t, False)
            if not obj.recv and "Entity" in t:
                packEntity(var)
            elif "Entity" in t:
                getEntity(var)
            else:
                print("Error while generating the packet, abording")
                exit(1)

print("Welcome to the packet generator")
packet = input("ePacketType : ")
obj = Class(packet)
menu(obj)
with open("rosecommon/include/packets/{}.h".format(obj.filename), "w") as f:
    f.write(obj.getHeader())
    print("header file written at location rosecommon/include/packets/{}.h".format(obj.filename))
with open("rosecommon/src/packets/{}.cpp".format(obj.filename), "w") as f:
    f.write(obj.getCpp())
    print("Cpp file written at location rosecommon/src/packets/{}.cpp".format(obj.filename))

from os import walk
files = []
for (dirpath, dirnames, filenames) in walk("rosecommon/include/packets/"):
    files.extend(filenames)
    break
files = filter(lambda f: ".h" in f, files)

with open("rosecommon/include/packets.h", "w") as f:
    f.write("#pragma once\n\n")
    for tmp in files:
        f.write('#include "{}"\n'.format(tmp))
    print("Added include to rosecommon/include/packets.h")
