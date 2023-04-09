#include <iostream>
#include <vector>
using namespace std;

const int BLOCK_SIZE = 512;
const int NUM_BLOCKS = 1024;

struct File
{
    string name;
    int size;
    int start_block;
    int end_block;
};

struct Disk
{
    vector<int> blocks;
    vector<File> files;
};

void allocate_contiguous(Disk &disk, File &file)
{
    int num_blocks = file.size / BLOCK_SIZE;
    if (file.size % BLOCK_SIZE != 0)
    {
        num_blocks++;
    }

    int start_block = -1;
    int end_block = -1;
    int consecutive_blocks = 0;
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (disk.blocks[i] == 0)
        {
            if (consecutive_blocks == 0)
            {
                start_block = i;
            }
            consecutive_blocks++;
            if (consecutive_blocks == num_blocks)
            {
                end_block = i;
                break;
            }
        }
        else
        {
            consecutive_blocks = 0;
            start_block = -1;
        }
    }

    if (start_block != -1 && end_block != -1)
    {
        file.start_block = start_block;
        file.end_block = end_block;
        for (int i = start_block; i <= end_block; i++)
        {
            disk.blocks[i] = 1;
        }
        disk.files.push_back(file);
    }
    else
    {
        cout << "Not enough contiguous blocks on the disk." << endl;
    }
}

void allocate_linked(Disk &disk, File &file)
{
    int num_blocks = file.size / BLOCK_SIZE;
    if (file.size % BLOCK_SIZE != 0)
    {
        num_blocks++;
    }

    vector<int> blocks;
    int current_block = -1;
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (disk.blocks[i] == 0)
        {
            if (current_block == -1)
            {
                current_block = i;
                blocks.push_back(current_block);
            }
            else
            {
                disk.blocks[current_block] = i;
                current_block = i;
                blocks.push_back(current_block);
            }
            num_blocks--;
            if (num_blocks == 0)
            {
                break;
            }
        }
    }

    if (num_blocks == 0)
    {
        file.start_block = blocks[0];
        file.end_block = blocks[blocks.size() - 1];
        disk.files.push_back(file);
    }
    else
    {
        cout << "Not enough linked blocks on the disk." << endl;
    }
}

void delete_file(Disk &disk, File &file)
{
    for (int i = file.start_block; i <= file.end_block; i++)
    {
        disk.blocks[i] = 0;
    }
    for (int i = 0; i < disk.files.size(); i++)
    {
        if (disk.files[i].name == file.name)
        {
            disk.files.erase(disk.files.begin() + i);
            break;
        }
    }
}

void rename_file(Disk &disk, File &file, string new_name)
{
    string old_name = file.name;
    file.name = new_name;

    for (int i = 0; i < disk.files.size(); i++)
    {
        if (disk.files[i].name == old_name)
        {
            disk.files[i].name = new_name;
            break;
        }
    }
}

// calculate fragmentation percentage on the disk (linked allocation)
double fragmentation_percentage(Disk &disk)
{
    double fragmentation_percentage = 0;
    int num_fragmented_blocks = 0;
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (disk.blocks[i] != 0)
        {
            num_fragmented_blocks++;
        }
    }
    fragmentation_percentage = (double)num_fragmented_blocks / NUM_BLOCKS * 100;
    return fragmentation_percentage;
}

// calculate no of wasted blocks on the disk
int wasted_blocks(Disk &disk)
{
    int wasted_blocks = 0;
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if (disk.blocks[i] == 0)
        {
            wasted_blocks++;
        }
    }
    return wasted_blocks;
}

void display_disk(Disk &disk)
{
    cout << "Disk blocks: ";
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        if ((disk.blocks[i + 1] == disk.blocks[i] + 1) && (disk.blocks[i] != 0))
        {
            cout << disk.blocks[i] << "->";
        }
        else if (disk.blocks[i] == 1)
        {
            cout << disk.blocks[i] << ":";
        }
        else
        {
            cout << "._.";
        }
    }
    cout << endl;
}

void list_files(Disk &disk)
{
    cout << "Files:" << endl;
    for (int i = 0; i < disk.files.size(); i++)
    {
        cout << "File ID: " << i + 1 << endl;
        cout << "Name: " << disk.files[i].name << endl;
        cout << "Size: " << disk.files[i].size << " bytes" << endl;
        cout << "Start block: " << disk.files[i].start_block << endl;
        cout << "End block: " << disk.files[i].end_block << endl;
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
        cout << "1. Create file" << endl;
        cout << "2. Create Multiple files" << endl;
        cout << "3. Delete file" << endl;
        cout << "4. Rename file" << endl;
        cout << "5. Display disk" << endl;
        cout << "6. List files" << endl;
        cout << "7. Exit" << endl;
        cout << endl;

        cout << "Enter your choice : ";
        cin >> choice;

        switch (choice)
        {
        case 1:
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
            }
            else if (allocation_method == 2)
            {
                allocate_linked(disk, file[fileCount]);
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
            for (int i = 0; i < n; i++)
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
                }
                else if (allocation_method == 2)
                {
                    allocate_linked(disk, file[fileCount]);
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