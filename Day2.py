opcodes = {
    1: lambda a, b: a + b,
    2: lambda a, b: a * b
}

def intcode(inst):
    for i in range(0, len(inst), 4):
        try:
            if inst[i] == 99: break
            if opcodes.get(inst[i]) == None: raise KeyError()
            inst[inst[i + 3]] = opcodes.get(inst[i])(inst[inst[i + 1]], inst[inst[i + 2]])
        except KeyError:
            print(i, "Unknown OpCode:", inst[i], "inst[", inst[i+1], "]=", inst[inst[i + 1]], "inst[", inst[i+2], "]=", inst[inst[i + 2]], inst[i + 3]) #debug
    return inst

inp = input()
inst = inp.split(',')
for i, s in enumerate(inst): inst[i] = int(s)
out = inst.copy()
out[1] = 12
out[2] = 2
print(intcode(out))

for noun in range(0,99):     #yay for brute force
    for verb in range(0,99):
        print("Trying noun=", noun, " verb=", verb)
        out = inst.copy()
        out[1] = noun
        out[2] = verb
        out = intcode(out)
        if out[0] == 19690720:
            print(noun)
            print(verb)
            print(100*noun+verb)
            exit(1)



