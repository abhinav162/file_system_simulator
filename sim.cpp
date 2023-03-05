#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;

const int NUM_BLOCKS = 1024;
const int BLOCK_SIZE = 32; // in KB

void createFile(bool *allocated, int numBlocks, vector<int> &fileStarts, string fileName, int fileSize) {
    int numRequiredBlocks = (fileSize + BLOCK_SIZE - 1) / BLOCK_SIZE;
    int startBlock = -1;
    for (int i = 0; i <= numBlocks - numRequiredBlocks; i++) {
        bool fits = true;
        for (int j = 0; j < numRequiredBlocks; j++) {
            if (allocated[i+j]) {
                fits = false;
                break;
            }
        }
        if (fits) {
            startBlock = i;
            for (int j = 0; j < numRequiredBlocks; j++) {
                allocated[i+j] = true;
            }
            break;
        }
    }
    if (startBlock != -1) {
        fileStarts.push_back(startBlock);
        cout << "Allocated " << numRequiredBlocks << " blocks for file " << fileName << " starting at block " << startBlock << endl;
    } else {
        cout << "Not enough contiguous free blocks to allocate for file " << fileName << endl;
    }
}

void printBlocks(bool *allocated, int numBlocks) {
    for (int i = 0; i < numBlocks; i++) {
        if (allocated[i]) {
            cout << "X" << ".";
        } else {
            cout << "_" << ".";
        }
    }
    cout << endl;
}

int main() {
    int numFiles;
    cout << "Enter the number of files to create: ";
    cin >> numFiles;

    bool *allocated = new bool[NUM_BLOCKS];
    for (int i = 0; i < NUM_BLOCKS; i++) {
        allocated[i] = false;
    }

    vector<int> fileStarts;

    for (int i = 1; i <= numFiles; i++) {
        string fileName;
        cout << "Enter the name of file " << i << ": ";
        cin >> fileName;
        int fileSize;
        cout << "Enter the size of file " << i << " in KB: ";
        cin >> fileSize;
        createFile(allocated, NUM_BLOCKS, fileStarts, fileName, fileSize);
    }

    cout << "Allocated space: ";
    printBlocks(allocated, NUM_BLOCKS);

    cout << "Starting locations of created files:" << endl;
    for (int i = 0; i < fileStarts.size(); i++) {
        cout << "File " << i+1 << ": block " << fileStarts[i] << endl;
    }

    delete[] allocated;

    return 0;
}