# Template Expander

- Implement template expander as per [problem statement](https://drive.google.com/file/d/1V6SgufnUOOTG_SIXi02BYTy_EqUTU9g4/view).

- This is a very bare bones implementation
- There is little error chekcing for path-name and such
- This is not very cross platform friendly
- There are probably a few memory leaks in write_file
  - Inside the switch statement that conditionally replaces substrings

## Build
- I have tested this using GCC
- The program outputs to the "output/" folder
  - It expects that there will be an output folder else it will fail

### Windows
`build.bat`

### Linux or Mac
`build.sh`