#include "../inc/fc_GlobalVariables.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct fcp_F FCP_F;

int fc_HashCode(char *lv_NewFName) {
  int lv_NewFID = 250;

  while (*lv_NewFName != '\n' && *lv_NewFName != '\0') {
    lv_NewFID = (lv_NewFID << 5) + lv_NewFID;
    lv_NewFID += *lv_NewFName++;
  }

  return (uint8_t)lv_NewFID;
}

void fc_DisplayFileList(FCP_F *fcp_CurrF) {
  // Print current file name.
  printf("\n🅼 %s\n", fcp_CurrF->f_Name);

  // Print contents of current file.
  for (int counter = 1; counter <= fcp_CurrF->f_Count; counter++) {
    int index = fcp_CurrF->f_FAT[counter];

    printf("| -> 🅵 %s\t\t%x\t%p\n", fcp_CurrF->f_Child[index]->f_Name, index,
           fcp_CurrF->f_Child[index]);
  }

  // Print no. of contents.
  printf("\n( %d files )\n\n", fcp_CurrF->f_Count);
}

void fc_UpdateFAT(FCP_F *fcp_CurrF, int lv_NewFID) {
  int lv_fc_Index;

  // Get next free slot in FAT Table and update it.
  for (lv_fc_Index = 0; lv_fc_Index < fcp_CurrF->f_Count; lv_fc_Index++)
    ;

  fcp_CurrF->f_FAT[lv_fc_Index] = lv_NewFID;
}

void fc_PopulateFile(FCP_F *fcp_CurrF, char *lv_NewFName) {
  UI16 lv_NewFID = fc_HashCode(lv_NewFName);

  // Update directory name.
  strcpy(fcp_CurrF->f_Child[lv_NewFID]->f_Name, lv_NewFName);

  // Update directory FID and link it to the current directory.
  fcp_CurrF->f_Child[lv_NewFID]->f_ID = lv_NewFID;
  fcp_CurrF->f_Child[lv_NewFID]->f_Parent = fcp_CurrF;
}

int fc_FileErrors(FCP_F *fcp_CurrF, char *lv_NewFName) {
  UI16 lv_NewFID = fc_HashCode(lv_NewFName);

  // Check if same file is already present in
  // the current file.
  for (int Index = 0; Index <= fcp_CurrF->f_Count; Index++) {
    if (fcp_CurrF->f_FAT[Index] == lv_NewFID) {
      return FILE_PRESENT;
    }
  }

  return FILE_NOT_PRESENT;
}

int fc_CreateFile(FCP_F *fcp_CurrF, char *lv_NewFName) {
  UI16 lv_NewFID = fc_HashCode(lv_NewFName);

  // Check for file conflicts.
  if (fc_FileErrors(fcp_CurrF, lv_NewFName)) {
    return FILE_ERROR;
  }

  // Create new directory having size that can
  // accomodate 250 EFs/DFs.
  fcp_CurrF->f_Child[lv_NewFID] = (FCP_F *)malloc(250 * sizeof(FCP_F));

  // Update current directory's no of files.
  fcp_CurrF->f_Count++;

  // Populate the contents of newly created file.
  fc_PopulateFile(fcp_CurrF, lv_NewFName);
  fc_UpdateFAT(fcp_CurrF, lv_NewFID);

  return FILE_CREATED;
}

void fc_CreateMFInitFiles(FCP_F *fcp_MF) {
  fc_CreateFile(fcp_MF, "DF_TEL");
  fc_CreateFile(fcp_MF, "DF_GSM");
  fc_CreateFile(fcp_MF, "DF_PL");
}

void fc_InitMF(FCP_F *fcp_MF, char *lv_NewFName) {
  UI16 lv_NewFID = fc_HashCode(fcp_MF->f_Name);

  strcpy(fcp_MF->f_Name, lv_NewFName);

  fcp_MF->f_ID = lv_NewFID;
  fcp_MF->f_Parent = fcp_MF;

  // Create mandatory files,
  // i.e DF_TEL, DF_GSM and DF_PL.
  fc_CreateMFInitFiles(fcp_MF);
}

void fc_DisplayFCommands(void) {
  puts("❶ -> Create a directory");
  puts("❷ -> View directory contents");
  puts("❸ -> View directory contents with FIDs");
  puts("❹ -> Change directory");
  puts("❺ -> Switch current directory to MF");
  puts("❻ -> Exit the file system\n");
}

int main(void) {
  struct fcp_F fcp_MF;

  // *fcp_CurrF = NULL;
  fc_InitMF(&fcp_MF, "SIM OS");

  // fcp_CurrF = &fcp_MF;
  fc_DisplayFCommands();
  fc_CreateFile(&fcp_MF, "Downloads");
  fc_CreateFile(&fcp_MF, "Settings");
  fc_CreateFile(&fcp_MF, "Credentials");

  // Get command.

  /* char ch; */

  /* int index = 0, byte, fc_InputCommand[10]; */

  /* while (scanf("%d%c", &byte, &ch) != EOF) { */
  /*   if (ch == '\n') break; */

  /*   fc_InputCommand[index++] = byte; */
  /* } */

  /* fc_InputCommand[index] = byte; */

  /* for (index = 0; index < 10; index++) printf("%X ", fc_InputCommand[index]);
   */

  fc_DisplayFileList(&fcp_MF);

  return EXIT_SUCCESS;
}
