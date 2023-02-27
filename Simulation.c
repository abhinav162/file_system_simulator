#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DISK_SIZE 1024
#define BLOCK_SIZE 8
#define MAX_FILES 128

struct file
{
    char name[16];
    int size;
    int start_block;
};

int disk[DISK_SIZE / BLOCK_SIZE]; // simulated disk
struct file files[MAX_FILES];     // array of files
int num_files = 0;                // number of files on disk

void init_disk()
{
    // initialize disk to all zeros
    memset(disk, 0, DISK_SIZE);
}

int allocate_blocks(int num_blocks)
{
    // find first block of contiguous free space
    int start_block = -1;
    int count = 0;
    for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; i++)
    {
        if (disk[i] == 0)
        {
            if (start_block == -1)
            {
                start_block = i;
            }
            count++;
            if (count == num_blocks)
            {
                // found enough contiguous free space
                return start_block;
            }
        }
        else
        {
            // reset counters
            start_block = -1;
            count = 0;
        }
    }
    // not enough contiguous free space found
    return -1;
}

int allocate_file(char *name, int size)
{
    // allocate space for file using contiguous allocation
    int num_blocks = (size + BLOCK_SIZE - 1) / BLOCK_SIZE + 1;
    int start_block = allocate_blocks(num_blocks);
    if (start_block == -1)
    {
        // not enough contiguous free space
        return -1;
    }
    // mark blocks as used by the file
    for (int i = start_block; i < start_block + num_blocks; i++)
    {
        disk[i] = 1;
    }
    // add file to files array
    strcpy(files[num_files].name, name);
    files[num_files].size = size;
    files[num_files].start_block = start_block;
    num_files++;
    return 0;
}

int find_file(char *name)
{
    // find file in files array
    for (int i = 0; i < num_files; i++)
    {
        if (strcmp(files[i].name, name) == 0)
        {
            return i;
        }
    }
    // file not found
    return -1;
}

int delete_file(char *name)
{
    // find file in files array
    int index = find_file(name);
    if (index == -1)
    {
        // file not found
        return -1;
    }
    // free blocks used by file
    int start_block = files[index].start_block;
    int num_blocks = (files[index].size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = start_block; i < start_block + num_blocks; i++)
    {
        disk[i] = 0;
    }
    // remove file from files array
    for (int i = index; i < num_files - 1; i++)
    {
        files[i] = files[i + 1];
    }
    num_files--;
    return 0;
}

int rename_file(char *old_name, char *new_name)
{
    // find file in files array
    int index = find_file(old_name);
    if (index == -1)
    {
        // file not found
        return -1;
    }
    // rename file
    strcpy(files[index].name, new_name);
    return 0;
}

int move_file(char *name, int new_size)
{
    // find file in files array
    int index = find_file(name);
    if (index == -1)
    {
        // file not found
        return -1;
    }
    // free old blocks used by file
    int start_block = files[index].start_block;
    int num_blocks = (files[index].size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = start_block; i < start_block + num_blocks; i++)
    {
        disk[i] = 0;
    }
    // allocate new blocks for file
    int new_num_blocks = (new_size + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int new_start_block = allocate_blocks(new_num_blocks);
    if (new_start_block == -1)
    {
        // not enough contiguous free space
        return -1;
    }
    // mark new blocks as used by the file
    for (int i = new_start_block; i < new_start_block + new_num_blocks; i++)
    {
        disk[i] = 1;
    }
    // update file in files array
    files[index].size = new_size;
    files[index].start_block = new_start_block;
    return 0;
}

void print_disk()
{
    // print disk in 16-byte rows
    for (int i = 0; i < DISK_SIZE / 16; i++)
    {
        printf("%04x: ", i);
    }
    printf("\n");
}

int main()
{
    srand(time(NULL));
    init_disk();
    // allocate_file("file1", 100);
    // allocate_file("file2", 200);
    // allocate_file("file3", 300);
    // allocate_file("file4", 400);
    // allocate_file("file5", 500);

    // print_disk();
    // delete_file("file3");
    // print_disk();
    // rename_file("file4", "file4a");
    // print_disk();
    // move_file("file5", 600);
    // print_disk();
    // return 0;

    // allocate some files
    allocate_file("file1", 100);
    allocate_file("file2", 200);
    allocate_file("file3", 300);

    // print contents of disk
    printf("Disk contents:\n");
    // for (int i = 0; i < DISK_SIZE / BLOCK_SIZE; i++)
    // {
    //     printf("%d ", disk[i]);
    // }
    // printf("\n\n");
    print_disk();

    // print list of files
    printf("Files on disk:\n");
    for (int i = 0; i < num_files; i++)
    {
        printf("%s (%d bytes, starting at block %d)\n", files[i].name, files[i].size, files[i].start_block);
    }
    printf("\n");

    // delete a file
    printf("Deleting file1...\n");
    delete_file("file1");

    // print list of files again
    printf("Files on disk:\n");
    for (int i = 0; i < num_files; i++)
    {
        printf("%s (%d bytes, starting at block %d)\n", files[i].name, files[i].size, files[i].start_block);
    }
    printf("\n");

    // rename a file
    printf("Renaming file2 to new_file2...\n");
    rename_file("file2", "new_file2");

    // print list of files again
    printf("Files on disk:\n");
    for (int i = 0; i < num_files; i++)
    {
        printf("%s (%d bytes, starting at block %d)\n", files[i].name, files[i].size, files[i].start_block);
    }
    printf("\n");

    // move a file to a new location
    printf("Moving new_file2 to new location with size 400...\n");
    move_file("new_file2", 400);

    // print list of files again
    printf("Files on disk:\n");
    for (int i = 0; i < num_files; i++)
    {
        printf("%s (%d bytes, starting at block %d)\n", files[i].name, files[i].size, files[i].start_block);
    }
    printf("\n");

    return 0;
}