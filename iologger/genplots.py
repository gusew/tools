import sys

def analyzeData(lines):
    if len(lines) > 1:
        lineparts = lines[1].rstrip(';\n').split(";")
        colCnt = len(lineparts) - 3 # subtract time and cycle columns
        colsUse = [False] * (colCnt) # start with no relevant columns
        
        for line in lines[2:]:
            parts = line.rstrip(';').split(";")

            # index iterates over columns in colsUse, valIdx iterates over column values in parts
            index = 0
            for valIdx in range(3, 3+colCnt):
                if not colsUse[index] and (int(parts[valIdx]) > 0):
                    colsUse[index] = True
                index += 1
        
        # filter some not needed columns with specified captions
        captions = lines[0].rstrip(';\n').split(';')
        index = 0
        for caption in captions[3:]:
            if caption.find("/lib/x86") >= 0 or caption.find("stdin") >= 0 or caption.find("stdout") >= 0:
                colsUse[index] = False
            index += 1

        return colsUse
    else:
        return [False]

def genPlotLines(colsUse, template):
    # generate remaining gnuplot script for I/O file plots
    outPlotLines = []
    for colIdx in range(0, len(colsUse)):
        if colsUse[colIdx]: # plot data if values != 0 in columns
            realIdx = colIdx + 4
            line = template.replace("***IO_COL_N***", str(realIdx))
            styleNo = 3 # for data on reads
            if realIdx % 3 == 2:
                styleNo = 4 # for data on writes
            elif realIdx % 3 == 0:
                styleNo = 5 # for data on positions

            line = line.replace("***IO_COL_STYLE***", str(styleNo))

            outPlotLines.append(line)

    # remove line trailer at last line
    if len(outPlotLines) > 0:
        outPlotLines[-1] = outPlotLines[-1].rstrip(", \\")

    return outPlotLines


def main(template, datalines, datafilename):
    # find columns which contain values != 0
    colsUse = analyzeData(datalines)

    # remove suffix from data-filename, if present
    fileprefix = datafilename.rsplit('.', 1)[0]

    # replace all template markers in plot-template and extract template part for I/O plots
    foundIoMarker = False
    ioPlotTemplate = ""
    outPlotFile = []
    for line in template:
        if foundIoMarker:
            if line.startswith("#***END_IO_PLOT_TEMPLATE"):
                foundIoMarker = False
            else:
                ioPlotTemplate = line.rstrip('\n')
        else:
            if line.startswith("#***START_IO_PLOT_TEMPLATE"):
                foundIoMarker = True
            else:
                line = line.replace("***FILEPREFIX***", fileprefix)
                line = line.replace("***FILENAME***", datafilename)
                line = line.rstrip('\n')
                outPlotFile.append(line)


    # generate masks only for reads, for writes, for positions, and for reads and writes merged
    colsUseReads = [False] * len(colsUse)
    colsUseWrites = [False] * len(colsUse)
    colsUsePosition = [False] * len(colsUse)
    colsUseMerged = [False] * len(colsUse)

    for colIdx in range(0, len(colsUse)):
        realIdx = colIdx + 4

        if realIdx % 3 == 2:
            colsUseReads[colIdx] = False # hide write column
            colsUseWrites[colIdx] = colsUse[colIdx]
            colsUsePosition[colIdx] = False
            colsUseMerged[colIdx] = colsUse[colIdx]

        elif realIdx % 3 == 0:
            colsUseReads[colIdx] = False # hide position column
            colsUseWrites[colIdx] = False 
            colsUsePosition[colIdx] = colsUse[colIdx]
            colsUseMerged[colIdx] = False

        else:
            colsUseReads[colIdx] = colsUse[colIdx]
            colsUseWrites[colIdx] = False # hide read column
            colsUsePosition[colIdx] = False
            colsUseMerged[colIdx] = colsUse[colIdx]


    # generate plot script for all cases
    outPlotReads = genPlotLines(colsUseReads, ioPlotTemplate)
    outPlotWrites = genPlotLines(colsUseWrites, ioPlotTemplate)
    outPlotPositions = genPlotLines(colsUsePosition, ioPlotTemplate)
    # generate gnuplot script for both reads and writes
    outPlotMerged = genPlotLines(colsUseMerged, ioPlotTemplate)
    
    # if there is no data for an empty plot, avoid gnuplot error
    dummyForEmptyPlots = "\tfile using 0 t \"NO DATA AVAILABLE\""

    # replace occurences of special template markers in generated output script
    for line in outPlotFile:
        if line.find("***PLOT_FILE_MERGED***") >= 0:
            for plotline in outPlotMerged:
                print plotline
            if len(outPlotMerged) == 0:
                print dummyForEmptyPlots
        elif line.find("***PLOT_FILE_READS***") >= 0:
            for plotline in outPlotReads:
                print plotline
            if len(outPlotReads) == 0:
                print dummyForEmptyPlots
        elif line.find("***PLOT_FILE_WRITES***") >= 0:
            for plotline in outPlotWrites:
                print plotline
            if len(outPlotWrites) == 0:
                print dummyForEmptyPlots
        elif line.find("***PLOT_FILE_POSITIONS***") >= 0:
            for plotline in outPlotPositions:
                print plotline
            if len(outPlotPositions) == 0:
                print dummyForEmptyPlots
        else:
            print line


if __name__ == "__main__":
    # check number of arguments
    if len(sys.argv) < 3:
        print "Usage: %s <gnuplot-template-merge> <data-file>" % (sys.argv[0])
        exit()
    
    with open(sys.argv[1], "r") as f1:
        template = f1.readlines()

    with open(sys.argv[2], "r") as f2:
        data = f2.readlines()

    main(template, data, sys.argv[2])

