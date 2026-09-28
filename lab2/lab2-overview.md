# Lab 2 Overview

## Project Summary

This project is a small C program scaffold for an embedded or microcontroller lab. The source file currently contains a minimal `main()` function with an infinite loop placeholder.

## Current Source

```c
/**
 * @file main.c
 * @author julialilley
 * @date 2026-09-23
 * @brief Main function
 */
int main(){

    // Add your code here and press Ctrl + Shift + B to build
    while(1) {
        
    }

    return 0;
}
```

## Notes

- The program is intentionally minimal.
- The loop is an infinite idle loop, which is common in embedded applications before adding functionality.
- The project structure includes build-related folders such as `_build`, `cmake`, and `out`.

## Project Structure

- `main.c` — main program source
- `README.md` — project documentation
- `out/` — compiled build output
- `_build/` — generated build tree
- `cmake/` — generated CMake files
- `.vscode/` — editor and project settings

## Next Step

Add the actual lab logic inside the `while(1)` loop and rebuild the project to generate the firmware output.
