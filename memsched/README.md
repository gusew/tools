# memsched

## Intended purpose 

The tool memsched is intended for evaluating the allocation of main memory of a system. 
It iteratively allocates main memory in blocks with a certain block size over multiple `calloc`-calls.
Each of the allocated blocks is referenced in a special array that needs extra memory for. 
Main memory for this special array with references is allocated in an initial step.
Each block cell is set to `1` and all blocks are summed up before all allocated memory is freed and the program is terminated.

## Tool properties

* simple design for single purpose
* allocation is performed blockwise
* status of allocation process is periodically output
* each block is set to the numerical value one with `memset`

## Usage

	memsched <mem to allocate in KB>

## Customization

You can change the number of bytes that are allocated by a single `calloc`-call in each allocation step with the variable `allocSteps`.
If the allocation should be performed over a longer period of time, you can add a `sleep` statement at the bottom of the `while` loop in `main()`.
Some source code fragments exist for more silly operations to each allocated memory block in the functions `annealBlocks` and `sumOneBlock`.
These functions as well as the `srandom` call were created in order to prevent an intelligent compiler from optimizing all calloc-calls to a single one.
