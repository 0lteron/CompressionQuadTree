# DIGITAL IMAGE COMPRESSION FORMAT USING QUADTREES

- Ghiles MALOUM

## DESCRIPTION
This project consists of developing a tool for encoding/decoding compressed PGM image files using quadtrees.

## COMPILING THE PROJECT
To compile the project, follow these steps:

### I. LIBRARY INSTALLATION:

#### 1. INSTALLATION AS A STANDARD LIBRARY:
1. Open a terminal in the project directory.
2. Navigate to the `libqtc/` directory.
3. Run the command `make` to compile the library.
4. Run the command `sudo make install`.
5. The library will be installed in the `/usr/lib/` directory, and the header files in `/usr/include/`.

> **Note:** To uninstall the library, run the command `sudo make uninstall`.

#### 2. INSTALLATION AS A LOCAL LIBRARY:
This method is useful if you do not have administrator rights. The library will be installed in the `lib/` directory of the project and can be used anywhere as long as the library is present in the `lib/` directory.

1. Open a terminal in the project directory.
2. Navigate to the `lib/` directory.
3. Run the command `make` to compile the library.
4. Run the command `chmod +x install.sh && ./install.sh`:
  - This command will create three environment variables in the `~/.bashrc` file:
    - `Lqtc`: Linker flag to link the library.
    - `Iqtc`: Flag to include the header files.
    - `useqtc`: Flag to activate the library (for compilation and linking in one step).
  - It will also add the `lib/` directory to the `LD_LIBRARY_PATH` environment variable.

5. The library will be installed in the `lib/` directory of the project.

> **Note:** To uninstall the library, run the command `chmod +x uninstall.sh && ./uninstall.sh`.

#### 3. MANUAL LOCAL LIBRARY INSTALLATION:
This method is the same as the previous one, but here you have to manually add the library to the `LD_LIBRARY_PATH` environment variable and all the necessary flags to compile and link the library.

1. Open a terminal in the project directory.
2. Navigate to the `lib/` directory.
3. Run the command `make` to compile the library.
4. You can now use the library by adding the flag `-L/path/to/lib/ -lqtc` during compilation.

### II. COMPILING THE PROJECT:
1. Open a terminal in the project directory.
2. Navigate to the `app` directory.
3. Run the command `make` to compile the project.
4. The executable program `codec` will be created in the `bin/` directory.

> **NOTE:** In the demo provided, this method has been used. The Makefile assumes that the environment variables exist. So you'll have to run the installation script before compiling the app.

## COMMAND LINE OPTIONS
Here are the available options to use the program:

Usage: `./bin/codec [options]`

### OPTIONS
- `-u`: Decoding mode.
- `-c`: Encoding mode.
- `-i <input>`: Input file (format pgm/qtc) [required].
- `-o <output>`: Output file (format pgm/qtc). Default value: `out.pgm`.
- `-g`: Enable segmentation grid.
- `-a <number>`: Define alpha (positive). Default value: `1.6`.
- `-b <number>`: Define beta (positive). Default value: `0.8`.
- `-v`: Enable verbose mode. Default value: silent.
- `-h`: Display help message.

## USAGE EXAMPLES
- Encode an image named `input.pgm`:
  ```
  ./bin/codec -c -i "PGM/input.pgm"
  ```

- Encode an image with custom options (output name `output.qtc`, alpha and beta values, segmentation grid):
  ```
  ./bin/codec -c -i "PGM/input.pgm" -o "output.qtc" -a 2 -b 0.5 -g
  ```

- Decode an image:
  ```
  ./bin/codec -u -i "QTC/input.qtc"
  ```

- Enable verbose mode for detailed information:
  ```
  ./bin/codec -c -i "PGM/input.pgm" -v
  ```

- Display help:
  ```
  ./bin/codec -h
  ```

## TODO
 - For now, all files will be put in QTC/ or PGM/ folders, maybe remove this restriction