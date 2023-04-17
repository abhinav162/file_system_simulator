#include <iostream>
#include <vector>
using namespace std;

//The following code is a C++ program to simulate the Disk Operating System (DOS). It is used to manage and store data on a storage device, such as a hard drive. 

const int BLOCK_SIZE = 512;  //The size of each block in bytes
const int NUM_BLOCKS = 1024; //The total number of blocks available in the disk

//A File structure is used to represent data stored on the disk.
struct File {
    string name;           //The file's name
    int size;              //The file's size in bytes
    int start_block;       //The starting block of the file
    int end_block;         //The ending block of the file
    string type;           //The file's type
};

//A Disk is represented using a vector of integers, each integer representing a single block on the disk, with a value of 1 if the block is used and 0 if the block is free.
struct Disk {
    vector<int> blocks;     //A vector of integers representing the blocks on the disk
    vector<File> files;     //A vector of File structures representing the files stored in the disk
};

/* allocate_contiguous() function is used for allocating a file stored on the disk using contiguous allocation. 
It takes two arguments - a reference to the Disk data structure, and a File structure representing the file to be allocated. */
void allocate_contiguous(Disk &disk, File &file) {
    //Calculate the number of blocks needed for the file
    int num_blocks = file.size / BLOCK_SIZE;
    if (file.size % BLOCK_SIZE != 0) {
        num_blocks++;
    }

    //Search through the blocks to find a contiguous sequence of free blocks to store the file
    int start_block = -1;
    int end_block = -1;
    int consecutive_blocks = 0;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (disk.blocks[i] == 0) {   //Check whether the block is free or not
            if (consecutive_blocks == 0) {    //Check if this is the first free block found
                start_block = i;
            }
            consecutive_blocks++;
            if (consecutive_blocks == num_blocks) {    //Check if enough blocks have been found
                end_block = i;
                break;
            }
        } else {    //If a block is not free then reset the variables
            consecutive_blocks = 0;
            start_block = -1;
        }
    }

    //If enough free blocks were found then allocating them to the file
    if (start_block != -1 && end_block != -1) {
        file.start_block = start_block;
        file.end_block = end_block;
        for (int i = start_block; i <= end_block; i++) {
            disk.blocks[i] = 1;
        }
        disk.files.push_back(file);
    } else {    //Otherwise display an error message
        cout << "Not enough contiguous blocks on the disk." << endl;
    }
}

/* allocate_linked() function is used for allocating a file stored on the disk using linked allocation. 
It takes two arguments - a reference to the Disk data structure, and a File structure representing the file to be allocated. */
void allocate_linked(Disk &disk, File &file) {
    //Calculate the number of blocks needed for the file
    int num_blocks = file.size / BLOCK_SIZE;
    if (file.size % BLOCK_SIZE != 0) {
        num_blocks++;
    }

    //Search through the blocks to find a sequence of free blocks to store the file
    vector<int> blocks;
    int current_block = -1;
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (disk.blocks[i] == 0) {   //Check whether the block is free or not
            if (current_block == -1) {    //Check if this is the first free block found
                current_block = i;
                blocks.push_back(current_block);
            } else {    //Link the current free block to the last free block
                disk.blocks[current_block] = i;
                current_block = i;
                blocks.push_back(current_block);
            }
            num_blocks--;
            if (num_blocks == 0) {   //Check if enough free blocks were found
                break;
            }
        }
    }

    //If enough free blocks were found then allocating them to the file
    if (num_blocks == 0) {
        file.start_block = blocks[0];
        file.end_block = blocks[blocks.size() - 1];
        for (int i = file.start_block; i <= file.end_block; i++) {
            disk.blocks[i] = i + 1;
        }
        disk.files.push_back(file);
    } else {    //Otherwise display an error message
        cout << "Not enough linked blocks on the disk." << endl;
    }
}

/* delete_file() function is used for deleting a file stored on the disk. 
It takes two arguments - a reference to the Disk data structure, and a File structure representing the file to be deleted. */
// take file id and check if file.type is Linked or Contiguous and then delete it accordingly and deallocate the blocks
void delete_file(Disk& disk, File file) {
    int start = file.start_block;
    int end = file.end_block;
    for (int i = start; i <= end; i++) {
        disk.blocks[i] = 0; // deallocate the blocks
    }
    file.start_block = -1;
    file.end_block = -1;
    // remove the file from the files vector
    for (int i = 0; i < disk.files.size(); i++) {
        if (disk.files[i].name == file.name) {
            disk.files.erase(disk.files.begin() + i);
            break;
        }
    }
}

/* rename_file() function is used for renaming a file stored on the disk. 
It takes three arguments - a reference to the Disk data structure, a File structure representing the file to be renamed, and a string representing the new name of the file. */
void rename_file(Disk &disk, File &file, string new_name) {
    //Store the old name of the file
    string old_name = file.name;
    file.name = new_name;

    //Rename the file in the vector of files in the disk
    for (int i = 0; i < disk.files.size(); i++) {
        if (disk.files[i].name == old_name) {
            disk.files[i].name = new_name;
            break;
        }
    }
}

/* fragmentation_percentage() function is used for calculating the fragmentation percentage on the disk (linked allocation). 
It takes one argument - a reference to the Disk data structure. */
double fragmentation_percentage(Disk &disk) {
    double fragmentation_percentage = 0;
    int num_fragmented_blocks = 0;
    //Calculate the number of fragmented blocks on the disk and convert it to a percentage
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (disk.blocks[i] != 0) {
            num_fragmented_blocks++;
        }
    }
    fragmentation_percentage = (double)num_fragmented_blocks / NUM_BLOCKS * 100;
    return fragmentation_percentage;
}

/* wasted_blocks() function is used for calculating the number of wasted blocks on the disk. 
It takes one argument - a reference to the Disk data structure. */
int wasted_blocks(Disk &disk) {
    int wasted_blocks = 0;
    //Calculate the number of wasted blocks on the disk
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (disk.blocks[i] == 0) {
            wasted_blocks++;
        }
    }
    return wasted_blocks;
}

void display_disk(Disk &disk) // displays the disk blocks
{
    cout << "Disk blocks: ";
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if(disk.blocks[i] == 0)
        {
            cout << "._.";
        }
        else if ((disk.blocks[i + 1] == disk.blocks[i] + 1))
        {
            cout << disk.blocks[i] << "->"  ;
        }
        else if ((disk.blocks[i + 1] == 0 || disk.blocks[i + 1] == 1) && (disk.blocks[i] > 1))
        {
            cout << disk.blocks[i] ;
        }
        else if (disk.blocks[i] == 1)
        {
            cout  << ":1:";
        }
        else
        {
            cout << "._.";
        }
    }
    cout << endl;
}

void list_files(Disk &disk) // lists the files in the disk
{
    if(disk.files.size() == 0)
    {
        cout << "No files in disk." << endl;
        return;
    }

    cout << "Files in Disk:" << endl;
    for (int i = 0; i < disk.files.size(); i++)
    {
        cout << "+ File ID: " << i + 1 << endl;
        cout << "  Name: " << disk.files[i].name << endl;
        cout << "  Size: " << disk.files[i].size << " bytes" << endl;
        cout << "  Start block: " << disk.files[i].start_block << endl;
        cout << "  End block: " << disk.files[i].end_block << endl;
        cout << endl;
    }
}

int main()
{
    Disk disk;
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        disk.blocks.push_back(0);
    }

    int n;
    vector<File> file;

    // create one file
    int fileID;
    int fileCount = 0;
    string new_name;

    int choice;
    while (1)
    {
        cout << "<==Options============================>" << endl;
        cout << "1. Create file" << endl;
        cout << "2. Create Multiple files" << endl;
        cout << "3. Delete file" << endl;
        cout << "4. Rename file" << endl;
        cout << "5. Display disk" << endl;
        cout << "6. List files" << endl;
        cout << "7. Exit" << endl;
        cout << "<======== Enter your choice : ========> \n: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            file.push_back(File()); // create a new file
            cout << "Enter file name : ";
            cin >> file[fileCount].name;
            cout << "Enter file size : ";
            cin >> file[fileCount].size;
            file[fileCount].start_block = -1;
            file[fileCount].end_block = -1;
            cout << "Choose allocation method: " << endl;
            cout << "1. Contiguous" << endl;
            cout << "2. Linked" << endl;
            int allocation_method;
            cin >> allocation_method;
            if (allocation_method == 1)  // allocate the file on the disk
            {
                allocate_contiguous(disk, file[fileCount]); // allocate the file contiguous manner
                file[fileCount].type = "Contiguous";
            }
            else if (allocation_method == 2)
            {
                allocate_linked(disk, file[fileCount]); // allocate the file in Linked manner
                file[fileCount].type = "Linked";
            }
            else
            {
                cout << "Invalid allocation method" << endl;
            }
            display_disk(disk);
            list_files(disk);
            cout << fragmentation_percentage(disk) << "%, Fragmentation" << endl;
            cout << wasted_blocks(disk) << " Wasted Blocks\n" << endl;
            fileCount++;
            break;
        case 2:
            cout << "How many files you wants to create: ";
            cin >> n;
            for (int i = 0; i < n; i++) // Create n number of files
            {
                file.push_back(File());
                cout << "Enter file name : ";
                cin >> file[fileCount].name;
                cout << "Enter file size : ";
                cin >> file[fileCount].size;
                file[fileCount].start_block = -1;
                file[fileCount].end_block = -1;
                cout << "Choose allocation method: " << endl;
                cout << "1. Contiguous" << endl;
                cout << "2. Linked" << endl;
                int allocation_method;
                cin >> allocation_method;
                if (allocation_method == 1)
                {
                    allocate_contiguous(disk, file[fileCount]);
                    file[fileCount].type = "Contiguous";
                }
                else if (allocation_method == 2)
                {
                    allocate_linked(disk, file[fileCount]);
                    file[fileCount].type = "Linked";
                }
                else
                {
                    cout << "Invalid allocation method" << endl;
                }
                fileCount++;
            }
            display_disk(disk);
            list_files(disk);
            cout << fragmentation_percentage(disk) << "%, Fragmentation" << endl;
            cout << wasted_blocks(disk) << " Wasted Blocks" << endl;
            break;
        case 3:
            cout << "Which file you want to delete? :" << endl;
            list_files(disk);
            cin >> fileID;
            delete_file(disk, file[fileID - 1]);
            cout << "files deleted successfully" << endl;
            cout << "Files in the disk after deletion : " << endl;
            list_files(disk);
            break;
        case 4:
            list_files(disk);
            cout << "Enter ID of the file to rename : " << endl;
            cin >> fileID;
            cout << "Enter new file name : ";
            cin >> new_name;
            rename_file(disk, file[fileID - 1], new_name);
            cout << "Files in the disk after rename : " << endl;
            list_files(disk);
            break;
        case 5:
            display_disk(disk);
            cout << fragmentation_percentage(disk) << "%, Fragmentation" << endl;
            cout << wasted_blocks(disk) << " Wasted Blocks" << endl;
            break;
        case 6:
            list_files(disk);
            break;
        case 7:
            exit(0);
            break;
        default:
            cout << "Invalid choice";
            break;
        }
    }
    return 0;
}