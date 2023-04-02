#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

const int NUM_BLOCKS = 1024;
const int BLOCK_SIZE = 32; // in KB

void createFile(bool *allocated, int numBlocks, vector<int> &fileStarts, vector<string> &fileNames, string fileName, int fileSize)
{
    int numRequiredBlocks = (fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int startBlock = -1;
    for (int i = 0; i <= numBlocks - numRequiredBlocks; i++)
    {
        bool fits = true;
        for (int j = 0; j < numRequiredBlocks; j++)
        {
            if (allocated[i + j])
            {
                fits = false;
                break;
            }
        }
        if (fits)
        {
            startBlock = i;
            for (int j = 0; j < numRequiredBlocks; j++)
            {
                allocated[i + j] = true;
            }
            break;
        }
    }
    if (startBlock != -1)
    {
        fileStarts.push_back(startBlock);
        fileNames.push_back(fileName);
        cout << "Allocated " << numRequiredBlocks << " blocks for file " << fileName << " starting at block " << startBlock << endl;
    }
    else
    {
        cout << "Not enough contiguous free blocks to allocate for file " << fileName << endl;
    }
}

// function to search and delete a file using its name and print the updated blocks after deletion of the file
void deleteFile(bool *allocated, int numBlocks, vector<int> &fileStarts, vector<string> &fileNames, string fileName)
{
    int startBlock = -1;
    for (int i = 0; i < fileNames.size(); i++)
    {
        if (fileNames[i] == fileName)
        {
            startBlock = fileStarts[i];
            break;
        }
    }
    if (startBlock != -1)
    {
        for (int i = startBlock; i < numBlocks; i++)
        {
            if (allocated[i])
            {
                allocated[i] = false;
            }
            else
            {
                break;
            }
        }
        fileStarts.erase(fileStarts.begin() + startBlock);
        fileNames.erase(fileNames.begin() + startBlock);
        cout << "File " << fileName << " deleted" << endl;
    }
    else
    {
        cout << "File " << fileName << " not found" << endl;
    }
}

// function to rename a file using its name and print the updated blocks after renaming of the file
void renameFile(bool *allocated, int numBlocks, vector<int> &fileStarts, vector<string> &fileNames, string fileName, string newFileName)
{
    int startBlock = -1;
    for (int i = 0; i < fileNames.size(); i++)
    {
        if (fileNames[i] == fileName)
        {
            startBlock = fileStarts[i];
            break;
        }
    }
    if (startBlock != -1)
    {
        fileNames[startBlock] = newFileName;
        cout << "File " << fileName << " renamed to " << newFileName << endl;
    }
    else
    {
        cout << "File " << fileName << " not found" << endl;
    }
}

void printBlocks(bool *allocated, int numBlocks)
{
    for (int i = 0; i < numBlocks; i++)
    {
        if (allocated[i])
        {
            cout << "X"
                 << ".";
        }
        else
        {
            cout << "_"
                 << ".";
        }
    }
    cout << endl;
}

int main()
{
    int numFiles;
    cout << "Enter the number of files to create: ";
    cin >> numFiles;

    bool *allocated = new bool[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++)
    {
        allocated[i] = false;
    }

    vector<int> fileStarts;
    vector<string> fileNames;

    for (int i = 1; i <= numFiles; i++)
    {
        string fileName;
        cout << "Enter the name of file " << i << ": ";
        cin >> fileName;
        int fileSize;
        cout << "Enter the size of file " << i << " in KB: ";
        cin >> fileSize;
        createFile(allocated, NUM_BLOCKS, fileStarts, fileNames, fileName, fileSize);
    }

    cout << "Allocated space: ";
    printBlocks(allocated, NUM_BLOCKS);

    cout << "Starting locations of created files:" << endl;
    for (int i = 0; i < fileStarts.size(); i++)
    {
        cout << "File " << i + 1 << ": block " << fileStarts[i] << endl;
    }

    // delete a file
    string fileName;
    cout << "Enter the name of the file to delete: ";
    cin >> fileName;
    deleteFile(allocated, NUM_BLOCKS, fileStarts, fileNames, fileName);

    cout << "Allocated space: ";
    printBlocks(allocated, NUM_BLOCKS);

    // rename a file
    cout<<"Do you want to rename a file? (y/n) ";
    char choice;
    cin>>choice;
    if(choice=='y')
    {
        cout<<"Files available: \n";
        for(int i=0;i<fileNames.size();i++)
        {
            cout<<fileNames[i]<<"\n";
        }
        cout << "Enter the name of the file to rename: ";
        cin >> fileName;
        string newFileName;
        cout << "Enter the new name of the file: ";
        cin >> newFileName;
        renameFile(allocated, NUM_BLOCKS, fileStarts, fileNames, fileName, newFileName);
        cout<<"Files after renaming: \n";
        for(int i=0;i<fileNames.size();i++)
        {
            cout<<fileNames[i]<<"\n";
        }
    }
    else
    {
        cout<<"Thank you!";
    }

    return 0;
}
