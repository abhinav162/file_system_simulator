#include <iostream>
#include <cstring>
using namespace std;

#define BLOCK_SIZE 512 // this is the size of each block on the disk
#define NUM_BLOCKS 500 // this is used to simulate a disk with 500 blocks

struct file {
    char name[20];
    int size;
    int start_block;
};

struct disk {
    char blocks[NUM_BLOCKS][BLOCK_SIZE];
    bool allocated[NUM_BLOCKS];
};

void init_disk(disk *d) {
    memset(d->blocks, 0, sizeof(d->blocks));
    memset(d->allocated, 0, sizeof(d->allocated));
}

int allocate_contiguous(disk *d, file *f) {
    int num_blocks = f->size / BLOCK_SIZE + (f->size % BLOCK_SIZE > 0 ? 1 : 0);
    int start_block = -1;
    for (int i = 0; i < NUM_BLOCKS - num_blocks; i++) {
        if (!d->allocated[i]) {
            int j;
            for (j = i+1; j < i+num_blocks; j++) {
                if (d->allocated[j]) {
                    break;
                }
            }
            if (j == i+num_blocks) {
                start_block = i;
                break;
            }
        }
    }
    if (start_block == -1) {
        return -1;
    }
    f->start_block = start_block;
    for (int i = start_block; i < start_block + num_blocks; i++) {
        d->allocated[i] = true;
    }
    return start_block;
}

void free_blocks_contiguous(disk *d, int start_block, int num_blocks) {
    for (int i = start_block; i < start_block + num_blocks; i++) {
        d->allocated[i] = false;
    }
}

void display_blocks(disk *d) {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        if (d->allocated[i]) {
            cout << "X";
        } else {
            cout << ".";
        }
    }
    cout << endl;
}

int main() {
    disk d;
    init_disk(&d);

    file f1 = {"file1", 25, 0};
    int file1_start = allocate_contiguous(&d, &f1);
    if (file1_start == -1) {
        cout << "Error: Not enough space on disk" << endl;
        return 1;
    }

    cout << "File1 allocated at block " << f1.start_block << endl;
    display_blocks(&d);

    file f2 = {"file2", 45, 0};
    int file2_start = allocate_contiguous(&d, &f2);
    if (file2_start == -1) {
        cout << "Error: Not enough space on disk" << endl;
        return 1;
    }

    cout << "File2 allocated at block " << f2.start_block << endl;
    display_blocks(&d);

    free_blocks_contiguous(&d, f1.start_block, f1.size/BLOCK_SIZE + (f1.size % BLOCK_SIZE > 0 ? 1 : 0));
    cout << "File1 freed" << endl;
    display_blocks(&d);

    file f3 = {"file3", 60, 0};
    int file3_start = allocate_contiguous(&d, &f3);
    if (file3_start == -1) {
        cout << "Error: Not enough space on disk" << endl;
        return 1;
    }

    cout << "File3 allocated at block " << f3.start_block << endl;
    display_blocks(&d);

    free_blocks_contiguous(&d, f2.start_block, f2.size/BLOCK_SIZE + (f2.size % BLOCK_SIZE > 0 ? 1 : 0));
    cout << "File2 freed" << endl;
    display_blocks(&d);

    return 0;
}
