import sys

def orderLines(lines):
    topLines = []
    botmLines = []

    for line in lines:
        # normal value line
        if line.startswith("#---"):
            botmLines.append(line[4:].rstrip('\n'))
        # move line to top
        elif line.startswith("#--^"):
            topLines.append(line[4:].rstrip('\n'))
        # ignore special comment line or other formatted lines
        #else if line.startswith("#--#"):
        #else

    return [topLines, botmLines]


def analyzeData(lines):
    cols = 0
    
    if len(lines) > 0:
        lineparts = lines[len(lines)-1].rstrip(';').split(";")
        cols = len(lineparts) - 3 # subtract first three columns
    return cols
 

def main(lines):
    # establish a correct ordering
    [topLines, botmLines] = orderLines(lines)
    
    # output all header lines first
    for line in topLines:
        print line

    # analyze line structure: determine number of columns
    cntColumns = analyzeData(botmLines)

    # output a first data entry with zero values (for nice plots)
    print "0;" * (cntColumns+3)

    for line in botmLines:
        outLine = ""
        parts = line.rstrip(';').split(";")

        outLine += "%s;%s;%s;" % (parts[0], parts[1], parts[2]) # output first column values

        for valIdx in range(3, 3+cntColumns):
            if (valIdx < len(parts)):
                outLine += "%s;" % (parts[valIdx])
            else:
                outLine += "0;"
        
        print outLine


if __name__ == "__main__":
    with open(sys.argv[1], "r") as f:
        lines = f.readlines()
    main(lines)
