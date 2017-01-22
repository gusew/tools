# exampletask

## Intended purpose 

This tool is a dummy workflow task implementation that takes data from a file as input and processes this data in a configurable manner.
Currently, the processing is implemented in the following ways:

1. the number of lines in the input file can be reduces halfway through by skipping each second line
2. the number of input lines is kept unchanged
3. the number of files in the input file is doubled for the output file by repeating each line once

## Tool properties

* imitates processing of data with three modes
* assumes that the specified input file contains data in a line-wise format
* can reduce, keep constant, or increase the number of lines in the input file when writing to the output file
* data processing occurs instantly without artificial delaying or computations

## Usage

	exampletask (r|k|i) <input file> <output file>
		<input file> as path to file which shall be read
		<output file> as path to file which shall be written with results
		choose between modes:
			'r' = reduce number of lines by skipping,
			'k' = keep number of lines unchanged, or
			'i' = inflate number of lines by repeating

## Customization

This task implementation can be extended with some artificial data processing computations that could consume cpu power.
Introducing some delays or operations on the input data should be placed inside the `while`-loop in `main`.
Furthermore, other fractions of lines to reduce or inflate could be used by adjusting the condition `cnt % 2 == 0` for the reduce-part, or creating a loop-structure around the `fhout.writeLine` statement.
