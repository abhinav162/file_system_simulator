# C++ Program for File System Simulation

This program simulates a file system on a disk with support for two allocation schemes: contiguous allocation and linked allocation.

## File structure
The File struct represents a file in the file system and has the following members:

* `name`: string representing the name of the file
* `size`: integer representing the size of the file in bytes
* `start_block`: integer representing the index of the first block allocated to the file
* `end_block`: integer representing the index of the last block allocated to the file

## Disk structure
The Disk struct represents the disk and has the following members:

* `blocks`: vector of integers representing the status of each block on the disk (0 for free and 1 for allocated)
* `files`: vector of File structs representing the files stored on the disk

## Functions
The program defines several functions to allocate and manage files on the disk:

* `allocate_contiguous()`: allocates a file using contiguous allocation
* `allocate_linked()`: allocates a file using linked allocation
* `delete_file()`: deletes a file from the disk
* `rename_file()`: renames a file on the disk
* `fragmentation_percentage()`: calculates the percentage of fragmented blocks on the disk (linked allocation only)
* `wasted_blocks()`: calculates the number of wasted blocks on the disk
* `display_disk()`: displays the current status of each block on the disk
* `list_files()`: lists all files stored on the disk, along with their names, sizes, and allocated blocks.


## Variables

- `fileID` : File ID
- `fileCount` : File count
- `new_name` : New file name
- `choice` : User choice
- `n` : Number of files to create
- `allocation_method` : Allocation method
- `file_name` : File name
- `file_size` : File size
- `file` : File object
- `disk` : Disk object

## How to run

- Compile the program using the following command:

- For Linux
    
    ```bash
    g++ simulation.cpp -o simulation
    ```

- For Windows

    ```bash
    g++ simulation.cpp -o simulation.exe
    ```

- Run the program using the following command:
    
    ```bash
    ./simulation
    ```

## How to use

### Create a file

- Select the option `1` to create a file.
- Enter the file name.
- Enter the file size.
- Select the allocation method.
- The file will be created and displayed on the screen.

### Create multiple files

- Select the option `2` to create multiple files.
- Enter the number of files to create.
- Enter the file name.
- Enter the file size.
- Select the allocation method.
- The files will be created and displayed on the screen.

### Delete a file

- Select the option `3` to delete a file.
- Enter the file ID.
- The file will be deleted and the disk will be displayed on the screen.

### Rename a file

- Select the option `4` to rename a file.
- Enter the file ID.
- Enter the new file name.
- The file will be renamed and the disk will be displayed on the screen.

### Display disk

- Select the option `5` to display the disk.
- The disk will be displayed on the screen.

### List files

- Select the option `6` to list all files.
- All files will be listed on the screen.

### Exit

- Select the option `7` to exit the program.
