with open('res', 'w') as s:
    with open('tmp', 'r') as f:
        for i, line in enumerate(f):
            a, x, w = line.split(' ')
            s.write("x[" + str(i) + "] = " + x + ";\n")
            s.write("w[" + str(i) + "] = " + w.strip() + ";\n" )
