#!/bin/python3

import sys

class Variable:
    def __init__(self, name, type, ref, size, arg, array, default, size_type, iserialize, slot, enum):
        self.name = name
        if self.name[-1] == '_':
            self.name = self.name[:-1]
        self.type = type
        self.isRef = ref
        self.size = size
        self.arg = None
        self.array = array
        self.default = default
        self.size_type = size_type
        self.iserialize = iserialize
        self.slot = None
        self.enum = enum
        self.otherOption = None
        if not arg is None:
            tmp = arg.split("|")
            if len(tmp) == 2:
                self.otherOption = self._parseArg(tmp[1])
                arg = tmp[0]
            self.slot = slot
            self.arg = self._parseArg(arg)

    def _parseArg(self, arg):
        tmp = arg.split("::")
        arg = []
        inside = False
        for i in tmp:
            if len(i) == 0:
                break
            if i[0] == '"':
                i = i[1:]
                arg.append(i)
                inside = True
            elif i[-1] == '"':
                i = i[:-1]
                arg[-1] += "::" + i
                inside = False
            else:
                if inside:
                    arg[-1] += "::" + i
                else:
                    arg.append(i)
        if len(arg) == 2:
            comp, name = arg
            if not "." in name and name[-1] != "_" and name[-1] != ")":
                name += "_"
            elif "." in name:
                name = name.split(".")
                if name[0][-1] != "_" and name[0][-1] != ")":
                    name[0] += "_"
                name = name[0] + ("" if not self.slot else "[" + self.slot + "]") + "." + ".".join(name[1:])
            return ("entity", comp, name)
        elif len(arg) == 3:
            entityName, comp, name = arg
            if name[-1] != "_" and name[-1] != ")":
                name += "_"
            return (entityName, comp, name)
        elif len(arg) == 1:
            return ("entity", arg[0], arg[0])

    def hasArg(self):
        return not self.arg is None

    def entityName(self):
        return self.arg[0]

    def comp(self):
        return self.arg[1]

    def var(self):
        return self.arg[2] if not self.arg is None and len(self.arg) == 3 else ""

    def getName(self):
        return self.name + '_'

    def get_size(self):
        size = "sizeof({})".format(self.getName())
        if not self.size is None:
            size = str(self.size) + " * sizeof({})".format(self.getName())
        elif self._getCleanedType() == "string":
            size = "sizeof(char) * ({}.size() + 1)".format(self.getName())
        if not self.size is None and self._getCleanedType() == "string":
            size = "sizeof(char) * {}".format(str(self.size))
        if self.array:
            return "size += sizeof({})".format(self.getName())
        if not self.size_type is None:
            if self.iserialize:
                size = "{{ for (const auto& it : {}) size += it.get_size();}}".format(self.getName())
                if len(self.size_type) > 0:
                    size += " size += sizeof({})".format(self.size_type)
            else:
                size = "size += {} * {}.size()".format(size, self.getName())
                if len(self.size_type) > 0:
                    size += " + sizeof({})".format(self.size_type)
        elif self.iserialize:
            size = "size += {}.get_size()".format(self.getName())
        if size[0] != "{" and size[:5] != "size ":
            size = "size += " + size
        return size

    def getDeclaration(self):
        return self.type + " " + self.getName() + ("[" + self.array + "]" if self.array else "") + ("" if not self.default or self.default == "=" else "= " + self.default) + ";"

    def getterHeader(self):
        data = self.type
        if self.isRef or self.array:
            data = "const " + self.type + ("&" if self.array is None else "*")
        res = data + " " + self.name + "() const;"
        if self.array is None:
            return res
        data = self.type
        if self.isRef:
            data = "const " + self.type + "&"
        return res + "\n\t\t" + data + " " + self.name + "(size_t index) const;"

    def setterHeader(self, className):
        data = self.type
        if self.isRef or self.array:
            data = "const " + self.type + ("&" if self.array is None else "[" + self.array + "]")
        res = className + "& set_" + self.name + "(" + data + ");"
        if self.array is None and self.size_type is None:
            return res
        if self.size_type is None:
            data = self.type
            if self.isRef:
                data = "const " + data + "&"
            return res + "\n\t\t" + className + "& set_" + self.name + "(" + data + ", size_t index);"
        else:
            data = self._getInsideType()
            if self.isRef:
                data = "const " + data + "&"
            name = self._getInsideType().lower()
            if "::" in name:
                name = name.split("::")[1]
            return res + "\n\t\t" + className + "& add_" + name + "(" + data + ");"

    def getterCpp(self, className):
        data = self.type
        if self.isRef or self.array:
            data = "const " + self.type + ("&" if self.array is None else "*")
        res = data + " " + className + "::" + self.name + "() const { return " + self.getName() + "; }"
        if self.array is None:
            return res
        data = self.type
        if self.isRef:
            data = "const " + self.type + "&"
        return res + "\n\n" + data + " " + className + "::" + self.name + "(size_t index) const { return " + self.getName() + "[index]; }"

    def setterCpp(self, className):
        data = self.type
        if self.isRef or self.array:
            data = "const " + self.type + ("&" if self.array is None else "*")
        res = className + "& " + className + "::set_" + self.name + "(" + data + " data) { "
        if self.array is None and self.size_type is None:
            return res + self.getName() + " = data; return *this; }"
        if self.size_type is None:
            res += "for (size_t index = 0; index < " + self.array + "; ++index) " + self.getName() + "[index] = data[index]; return *this; }"
            data = self.type
            if self.isRef:
                data = "const " + data + "&"
            return res + "\n\n" + className + "& " + className + "::set_" + self.name + "(" + data + " data, size_t index) { " + self.getName() + "[index] = data; return *this; }"
        else:
            res += self.getName() + " = data; return *this; }"
            data = self._getInsideType()
            if self.isRef:
                data = "const " + data + "&"
            name = self._getInsideType().lower()
            if "::" in name:
                name = name.split("::")[1]
            return res + "\n\n" + className + "& " + className + "::add_" + name + "(" + data + " data) { " + self.getName() + ".push_back(data); return *this; }"

    def _getCleanedType(self):
        data = self.type
        if self.enum:
            return self.enum
        if "::" in data:
            data = data.split("::")[1]
        if self.iserialize:
            return "iserialize"
        if "<" in data:
            return data.split("<")[1][:-1]
        return data

    def _getInsideType(self):
        data = self.type
        if "<" in data:
            return data.split("<")[1][:-1]
        return data

    def _getter(self, data):
        if self.enum:
            return "{{ {0} _tmp = {0}(); reader.get_{0}(_tmp); {1} = static_cast<{2}>(_tmp); }}".format(self.enum, data, self.type)
        return "reader.get_" + self._getCleanedType() + "(" + data + ");"

    def reader(self):
        data = self.getName()
        if not self.size is None:
            data += ", " + str(self.size)
        if self.array:
            return "for (size_t index = 0; index < " + self.array + "; ++index) " + self._getter(data + "[index]")
        if not self.size_type is None:
            if len(self.size_type) > 0:
                return "{{\n\t\t{0} index = 0;\n\t\treader.get_{0}(index);\n\t\twhile (index-- > 0) {{\n\t\t\t{1} tmp;\n\t\t\t{2}\n\t\t\t{3}.push_back(tmp);\n\t\t}}\n\t}}".format(self.size_type, self._getInsideType(), self._getter("tmp" + (", " + str(self.size) if self.size else "")), self.getName())
            else:
                return "{{\n\t\t{0} tmp;\n\t\twhile ({1}) {{\n\t\t\t{1};\n\t\t\t{2}.push_back(tmp);\n\t\t}}\n\t}}".format(self._getInsideType(), self._getter("tmp" + (", " + str(self.size) if self.size else ""))[:-1], self.getName())
        return self._getter(data)

    def writer(self):
        data = self.getName()
        if not self.size is None:
            data += ", " + str(self.size)
        if self.array:
            return "for (size_t index = 0; index < " + self.array + "; ++index) writer.set_" + self._getCleanedType() + "(" + data + "[index]);"
        if not self.size_type is None:
            if len(self.size_type) > 0:
                return "writer.set_{0}({1}.size());\n\tfor (const auto& elem : {1}) writer.set_{2}(elem);".format(self.size_type, self.getName(), self._getCleanedType())
            else:
                return "for (const auto& elem : {0}) writer.set_{1}(elem);".format(self.getName(), self._getCleanedType())
        return "writer.set_" + self._getCleanedType() + "(" + data + ");";

    def constructorHeader(self):
        if self.default:
            return ""
        data = self.type
        if self.isRef and not self.array:
            data = "const " + self.type + "&"
        data += " " + self.name
        if self.array:
            data += "[" + self.array + "]"
        return data

    def constructorCpp(self):
        if self.default:
            return ""
        data = self.type
        if self.isRef and not self.array:
            data = "const " + self.type + "&"
        data += " " + self.name
        if self.array:
            data += "[" + self.array + "]"
        return data

    def initCpp(self):
        if self.default:
            return True, ""
        if self.array:
            return False, "for (size_t index = 0; index < " + self.array + "; ++index) " + self.getName() + "[index] = " + self.name + "[index];"
        return True, self.getName() + "(" + self.name + ")"

class Class:
    def __init__(self, ePacketType):
        self.ePacketType = ePacketType
        if "ISC" in ePacketType:
            self.recv = True
            self.name = "Isc" + "".join(ePacketType.title().split("_")[1:])
            self.filename = 'isc' + "_" + "".join(ePacketType.lower().split("_")[1:])
        else:
            self.recv = "PAKCS_" in ePacketType
            self.name = ('Cli' if self.recv else 'Srv') + "".join(ePacketType.title().split("_")[1:])
            self.filename = ('cli' if self.recv else 'srv') + "_" + "".join(ePacketType.lower().split("_")[1:])
        self.includes = []
        self.variables = []

    def addInclude(self, name):
        self.includes.append('#include <{}>'.format(name))

    def addVariable(self, *args):
        self.variables.append(Variable(*args))

    def constructorHeader(self):
        data = "\t{}();".format(self.name)
        data += "\n\t\t{}(CRoseReader reader);".format(self.name)
        if len(self.variables) and any(i.default is None for i in self.variables):
            data += "\n\tprivate:\n\t\t" + self.name + "(" + ", ".join([v.constructorHeader() for v in self.variables if len(v.constructorHeader())]) + ");"
        data += "\n\tpublic:\n\n\t\tvirtual ~{}() = default;".format(self.name)
        return data

    def constructorCpp(self):
        data = "{0}::{0}() : CRosePacket(ePacketType::{1}) {{}}".format(self.name, self.ePacketType)
        data += "\n\n{0}::{0}(CRoseReader reader) : CRosePacket(reader) {{\n".format(self.name)
        data += '\tthrow_assert(get_type() == ePacketType::{}, "Not the right packet: " << to_underlying(get_type()));\n'.format(self.ePacketType)
        for v in self.variables:
            data += "\t{}\n".format(v.reader())
        data += "}"
        if len(self.variables) and any(i.default is None for i in self.variables):
            data += "\n\n{0}::{0}(".format(self.name) + ", ".join([v.constructorCpp() for v in self.variables if len(v.constructorCpp())]) + ") : CRosePacket(ePacketType::{}), ".format(self.ePacketType)
            data += ", ".join([v.initCpp()[1] for v in self.variables if v.initCpp()[0] == True and len(v.initCpp()[1])])
            data += " {"
            data += "\n\t".join([v.initCpp()[1] for v in self.variables if v.initCpp()[0] == False])
            data += "\n}"
        data += "\n"
        return data

    def getHeader(self):
        data = '#pragma once\n\n#include "packetfactory.h"\n#include "entitycomponents.h"\n'
        data += "\n".join(self.includes)
        data += '\n\nnamespace RoseCommon {\n\n'
        data += "REGISTER_RECV_PACKET(ePacketType::{1}, {0})\n".format(self.name, self.ePacketType)
        data += "REGISTER_SEND_PACKET(ePacketType::{1}, {0})\nclass {0} : public CRosePacket {{".format(self.name, self.ePacketType)
        data += "\n\tpublic:\n\t"
        data += self.constructorHeader() + "\n\n"
        for var in self.variables:
            data += "\t\t" + var.getterHeader() + "\n"
            data += "\t\t" + var.setterHeader(self.name) + "\n"
        data += "\n\t\tstatic {} create({});".format(self.name, ", ".join(self.getArgs()))
        data += "\n\t\tstatic {0} create(uint8_t *buffer);".format(self.name)
        data += "\n\n\tprotected:\n\t\tvirtual void pack(CRoseWriter&) const override;\n"
        data += "\t\tvirtual uint16_t get_size() const override;\n"
        if len(self.variables):
            data += "\n\tprivate:\n"
            for var in self.variables:
                data += "\t\t" + var.getDeclaration() + "\n"
        data += '};\n\n'
        return data + '}'

    def getArgs(self):
        res = []
        entities = set()
        for var in self.variables:
            if var.default:
                continue
            if var.hasArg():
                if var.entityName() in entities:
                    continue
                else:
                    entities.add(var.entityName())
                    res.append("Entity " + var.entityName());
            else:
                res.append(var.constructorHeader())
        return res

    def getComps(self):
        comps = set()
        for var in self.variables:
            if var.default:
                continue
            if var.hasArg():
                comps.add((var.entityName(), var.comp()))
            if not var.otherOption is None:
                comps.add((var.otherOption[0], var.otherOption[1]))
        get = "\tconst auto {} = {}.component<{}>();"
        return "\n".join([get.format(c[0].lower() + "_" + c[1].lower().replace("::", "_"), c[0], c[1]) for c in comps])

    def expandArgs(self):
        res = []
        for var in self.variables:
            if var.default:
                continue
            if not var.array is None:
                res.append(var.getName())
                continue
            if var.hasArg():
                star = ""
                data = ""
                if not var.otherOption is None:
                    data = var.entityName().lower() + "_" + var.comp().lower().replace("::", "_") + " ? "
                if var.comp() == var.var():
                    star = "*"
                data += star + var.entityName().lower() + "_" + var.comp().lower().replace("::", "_") + ("->" + var.var() if star == "" else "")
                if not var.otherOption is None:
                    star = ""
                    if len(var.otherOption) == 2:
                        star = "*"
                    data += " : " + star + var.otherOption[0].lower() + "_" + var.otherOption[1].lower().replace("::", "_") + ("->" + var.otherOption[2] if star == "" else "")
                res.append(data)
            else:
                res.append(var.name)
        return ", ".join(res)

    def getTemps(self):
        res = []
        for var in self.variables:
            if var.array is None or not var.default is None:
                continue
            res.append(var.getDeclaration())
            data = var.name
            if var.hasArg():
                star = ""
                if var.comp() == var.var():
                    star = "*"
                data = star + var.entityName().lower() + "_" + var.comp().lower().replace("::", "_") + "->" + var.var().split(".")[0]
            res.append("for (size_t index = 0; index < " + var.array + "; ++index) " + var.getName() + "[index] = " + data + "[index]" + ("." if len(var.var().split(".")) > 1 else "") + ".".join(var.var().split(".")[1:]) + ";")
        return "\n\t".join(res)

    def getCpp(self):
        data = '#include "{}.h"\n#include "throwassert.h"\n\nnamespace RoseCommon {{\n\n'.format(self.filename)
        data += self.constructorCpp()
        for var in self.variables:
            data += "\n" + var.getterCpp(self.name) + "\n"
            data += "\n" + var.setterCpp(self.name) + "\n"
        data += "\n\nvoid {}::pack(CRoseWriter&{}) const {{\n".format(self.name, " writer" if len(self.variables) else "")
        for var in self.variables:
            data += "\t{}\n".format(var.writer())
        data += "}"
        data += "\n\nuint16_t {}::get_size() const {{\n\tuint16_t size = 0;\n".format(self.name)
        for var in self.variables:
            data += "\t{};\n".format(var.get_size())
        data += "\treturn size;\n}\n"
        data += "\n\n{} {}::create({}) {{\n".format(self.name, self.name, ", ".join(self.getArgs()))
        data += self.getComps()
        tmp = self.getTemps()
        if len(tmp):
            data += "\n\t" + tmp + "\n\n"
        else:
            data += "\n\n"
        data += "\treturn {}({});\n}}\n".format(self.name, self.expandArgs())
        data += "\n{0} {0}::create(uint8_t *buffer) {{".format(self.name)
        data += "\n\tCRoseReader reader(buffer, CRosePacket::size(buffer));\n"
        data += "\treturn {}(reader);\n}}".format(self.name)
        return data + '\n\n}'

def getInt(msg = ''):
    while True:
        try:
            return int(input(msg))
        except ValueError:
            pass

def parseLine(obj, line):
    if "include" in line:
        obj.addInclude(line.split()[1])
        return
    line = line.split()
    t, n = line[:2]
    skip = False
    ref = False
    size = None
    arg = None
    array = None
    default = None
    size_type = None
    iserialize = False
    slot = None
    enum = None
    if len(line) >= 3:
        for index, param in enumerate(line[2:]):
            if skip:
                skip = False
                continue
            if param == "&": # reference
                ref = True
            elif param[:2] == "e(": # enum e(<type>)
                enum = param[2:-1]
            elif param.isdigit(): # size of written/read buffer
                size = int(param)
            elif param[0] == "[" and param[-1] == "]": # array
                array = param[1:-1]
            elif param[0] == "=": # default value
                if len(param) == 1:
                    default = line[index + 3]
                    skip = True
                else:
                    default = param[1:]
            elif param[:2] == 's(': # vector size type s([type]) if type is omitted, then it'll be a greedy read
                if len(param) == 3:
                    size_type = ""
                else:
                    size_type = param[2:-1]
            elif param == "I": # iserialize
                iserialize = True
            elif param[0] == "{" and param[-1] == "}": # particular element in an array, works only for components
                slot = param[1:-1]
            elif "::" in param: # entity component related stuff
                arg = param
    obj.addVariable(n, t, ref, size, arg, array, default, size_type, iserialize, slot, enum)

def menu(obj):
    while True:
        print("-----Menu-----")
        print("1 - add")
        print("2 - quit")
        a = getInt("> ")
        print()
        if a == 2:
            break
        parseLine(obj, input("('include' <filename> |\n<type> <name> [&] [size]\n[[entity_name]::component_name::member_name]\n[[array_size]] [= default_value]): ").strip())

def save(obj):
    f = open("rosecommon/include/packets/{}.h".format(obj.filename), "w")
    f.write(obj.getHeader())
    print("Written header to ./rosecommon/include/packets/{}.h".format(obj.filename))
    f = open("rosecommon/src/packets/{}.cpp".format(obj.filename), "w")
    f.write(obj.getCpp())
    print("Written cpp to ./rosecommon/src/packets/{}.cpp".format(obj.filename))

if len(sys.argv) == 1:
    packet = input("ePacketType : ")
    obj = Class(packet)
    menu(obj)
    save(obj)
elif len(sys.argv) >= 2:
    for filename in sys.argv[1:]:
        with open(filename, "r") as f:
            packet = f.readline().strip()
            obj = Class(packet)
            for line in f:
                if len(line) <= 1:
                    continue
                parseLine(obj, line.strip())
        save(obj)
else:
    print("Usage: " + sys.argv[0] + " [packet_file]")
    exit(1)
