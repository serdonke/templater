# Template Expander

- Implement template expander as per [problem statement](https://drive.google.com/file/d/1V6SgufnUOOTG_SIXi02BYTy_EqUTU9g4/view).

- This is a very bare bones implementation
- There is little error chekcing for path-name and such
- This is not very cross platform friendly
- There are probably a few memory leaks in write_file
  - template_string leaks asw, welp might fix.
  - Inside the switch statement that conditionally replaces substrings
- The error management is barebones asw, in case of a fatal error, the program ungracefully exits
- I intended to add the functionality for having an arbitrary number of entries but I have not, it is simple tho, one needs to check if current count exceeds total entires in the internal Student array and then realloc accordingly
  - In case if there are more than 512 entries in `students.csv`, the behaviour is unexpected.

## Build
- I have tested this using GCC locally on my Windows machine, but seems like replit is using Clang. 
- The program outputs to the "output/" folder
  - It expects that there will be an output folder else it will fail

### Windows
`build.bat`

### Linux or Mac
`build.sh`