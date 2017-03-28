class Variable:
    def __init__(self, name, type, isSimple):
        self.name = name
        if self.name[-1] == '_':
            self.name = self.name[:-1]
        self.type = type
        self.isSimple = isSimple
        self.unpackComplex = "TODO : implement unpack " + self.getName()
        self.packComplex = "TODO : implement pack " + self.getName()
    
    def getName(self):
        return self.name + '_'

    def getDeclaration(self):
        return self.type + " " + self.getName() + ";"

    def getterHeader(self, type = "", const = False):
        const2 = ""
        if type == "" or const:
            const2 = "const"
        if const:
            const = "const "
        else:
            const = ""
        return const + self.type + " " + type + self.name + "()" + (" " + const2 if len(const2) else "") + ";"

    def getterCpp(self, className, type = "", const = False):
        const2 = ""
        if type == "" or const:
            const2 = "const"
        if const:
            const = "const "
        else:
            const = ""
        return const + self.type + " " + type + className + "::" + self.name + "()" + (" " + const2 if len(const2) else "") + " {\n\treturn " + self.getName() + ";\n}"

    def constructorHeader(self, type = "", const = False):
        if const:
            const = "const "
        else:
            const = ""
        return const + self.type + " " + type + self.name

    def constructorInitCpp(self):
        return self.getName() + "(" + self.name + ")"

    def unpack(self):
        if self.isSimple:
            return "*this >> " + self.getName() + ";"
        return self.unpackComplex

    def pack(self):
        if self.isSimple:
            return "*this << " + self.getName() + ";"
        return self.packComplex

class Class:
    def __init__(self, ePacketType):
        self.ePacketType = ePacketType
        self.recv = "PAKCS_" in ePacketType
        self.name = ('Cli' if self.recv else 'Srv') + "".join(ePacketType.title().split("_")[1:])
        self.variables = []
        self.filename = ('cli' if self.recv else 'srv') + "_" + "".join(ePacketType.lower().split("_")[1:])
        self.includes = []

    def addInclude(self, name):
        self.includes.append('#include <{}>'.format(name))

    def addVariable(self, name, type, simple, byRef):
        self.variables.append((Variable(name, type, simple), byRef))
        return self.variables[-1][0]

    def getHeaderConstructor(self):
        data = "\t{0}();".format(self.name)
        if self.recv:
            data += "\n\t\t{}(uint8_t buffer[MAX_PACKET_SIZE]);".format(self.name)
        if len(self.variables):
            data += "\n\t\t" + self.name + "(" + ", ".join([v.constructorHeader("&" if b else "", b) for v, b in self.variables]) + ");"
        data += "\n\n\t\tvirtual ~{}() = default;".format(self.name)
        return data

    def getCppConstructor(self):
        data = "{0}::{0}() : CRosePacket(ePacketType::{1}) {{}}".format(self.name, self.ePacketType)
        if self.recv:
            data += "\n\n{0}::{0}(uint8_t buffer[MAX_PACKET_SIZE]) : CRosePacket(buffer) {{\n".format(self.name)
            data += '\tthrow_assert(type() == ePacketType::{}, "Not the right packet: " << to_underlying(type()));\n'.format(self.ePacketType)
            if len(self.variables):
                for v, b in self.variables:
                    data += "\t{}\n".format(v.unpack())
            data += "}"
        if len(self.variables):
            data += "\n\n{0}::{0}(".format(self.name) + ", ".join([v.constructorHeader("&" if b else "", b) for v, b in self.variables]) + ") : CRosePacket(ePacketType::{}), ".format(self.ePacketType)
            data += ", ".join([v.constructorInitCpp() for v, b in self.variables])
            data += " {}"
        data += "\n"
        return data

    def getHeader(self):
        data = '#pragma once\n\n#include "packetfactory.h"\n#include "entityComponents.h"\n'
        data += "\n".join(self.includes)
        data += '\n\nnamespace RoseCommon {\n\n'
        if self.recv:
            data += "REGISTER_RECV_PACKET(ePacketType::{1}, {0})\n".format(self.name, self.ePacketType)
            data += "class {0} : public CRosePacket {{".format(self.name, self.ePacketType)
        else:
            data += "REGISTER_SEND_PACKET(ePacketType::{1}, {0})\nclass {0} : public CRosePacket {{".format(self.name, self.ePacketType)
        data += "\n\tpublic:\n\t"
        data += self.getHeaderConstructor() + "\n\n"
        for var, b in self.variables:
            if b:
                data += "\t\t" + var.getterHeader("&", False) + "\n"
            data += "\t\t" + var.getterHeader("&" if b else "", b) + "\n"
        data += "\n\tprotected:\n\t\tvirtual void pack() override;\n"
        if len(self.variables):
            data += "\n\tprivate:\n"
            for var, b in self.variables:
                data += "\t\t" + var.getDeclaration() + "\n"
        return data + '};\n\n}'
    
    def getCpp(self):
        data = '#include "{}.h"\n#include "throwassert.h"\n\nnamespace RoseCommon {{\n\n'.format(self.filename)
        data += self.getCppConstructor()
        for var, b in self.variables:
            if b:
                data += "\n" + var.getterCpp(self.name, "&", False) + "\n"
            data += "\n" + var.getterCpp(self.name, "&" if b else "", b) + "\n"
        data += "\n\nvoid {}::pack() {{\n".format(self.name)
        for var, b in self.variables:
            data += "\t{}\n".format(var.pack())
        data += "}"
        return data + '\n\n}'

def getInt(msg = ''):
    while True:
        try:
            return int(input(msg))
        except ValueError:
            pass

def getComponents(name, components):
    get = "auto {{}} = {}.component<{{}}>();".format(name)
    data = ""
    for c in components:
        tmp = c[0].lower() + c[1:]
        data += "\t{}\n".format(get.format(tmp, c))
    return data[1:]

def entity(var):
    unpackFunctions = {
            "simple var" : "*this >> {};",
            "serialize var" : "*this >> static_cast<ISerialize&>({});",
            "serialize arr" : "for (auto &it : {}) {{\n\t\t*this >> static_cast<ISerialize&>(it);\n\t}}",
            "special" : "TODO : implement {}"
            }
    packFunctions = {
            "simple var" : "*this << {};",
            "serialize var" : "*this << static_cast<ISerialize&>({});",
            "serialize arr" : "for (auto &it : {}) {{\n\t\t*this << static_cast<ISerialize&>(it);\n\t}}",
            "special" : "TODO : implement {}"
            }
    components = set()
    variables = []
    toImplement = False
    while True:
        print("-----Packing entity-----")
        print("1 - add simple variable/array")
        print("2 - add ISerialize variable")
        print("3 - add ISerialize array")
        print("4 - add special variable/array")
        print("5 - return")
        a = getInt("> ")
        print()
        if a == 5:
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
        elif a == 4:
            variables.append(("special", name, component))
            toImplement = True
    packData = getComponents(var.getName(), components) + "\n"
    unpackData = getComponents(var.getName(), components) + "\n"
    for t, n, c in variables:
        tmp = c[0].lower() + c[1:] + "->" + n
        packData += "\t{}\n".format(packFunctions[t].format(tmp))
        unpackData += "\t{}\n".format(unpackFunctions[t].format(tmp))
    var.unpackComplex = unpackData
    var.packComplex = packData
    return toImplement

def menu(obj):
    toImplement = False
    while True:
        print("-----Menu-----")
        print("1 - add simple type")
        print("2 - add complex type")
        print("3 - add include")
        print("4 - quit")
        a = getInt("> ")
        print()
        if a == 4:
            break
        elif a == 3:
            obj.addInclude(input("Name of the file : "))
            continue
        t, n = input("Definition (<type> <name>) : ").split()[:2]
        byRef = input("Pass by reference (y/n)? ")
        if byRef == "y":
            print("Passing by reference")
            byRef = True
        else:
            print("Passing by value")
            byRef = False
        if a == 1:
            obj.addVariable(n, t, True, byRef)
        elif a == 2:
            var = obj.addVariable(n, t, False, byRef)
            toImplement = True
            if "Entity" in t:
                toImplement = entity(var)
    return toImplement

print("Welcome to the packet generator")
print("""How to use:
        - enter the ePacketType name of the packet you will use
        - you will then presented with a menu to choose between 3 options
        - the first one is really simple, enter your type and name just has you would in the header file. The trailing _ will be added if need be
        - the second one is the same as the first one, but the script won't implement the cpp part for you (except if it's an Entity, see below). If you add an Entity, the script will ask you some more information
        - the last option is if you need a special include. In this case please write the name of the file that would be enclosed between <> (or "")
        
        - If you add an Entity, the script will ask you to specify which variables you want to extract from it. To do that you have multiple options
        - first one is a CRosePacket parsable type
        - second one is a ISerialize variable
        - third one is a ISerialize array
        - the last option won't implement the cpp part for you (except include the component if need be)

        """)
packet = input("ePacketType : ")
obj = Class(packet)
toImplement = menu(obj)
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
if toImplement:
    print("You have some implementation left in {}.cpp (it'll cause compilation errors if you don't)".format(obj.filename))
