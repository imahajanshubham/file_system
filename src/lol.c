#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NODE_SIZE 250
#define MAX_FILE_NAME 15
#define EXISTS 1
#define NOT_EXISTS 0
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_NODE_SIZE 250
#define MAX_FILE_NAME 15
#define EXISTS 1
#define NOT_EXISTS 0
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODE_SIZE 250
#define MAX_FILE_NAME 15
#define EXISTS 1
#define NOT_EXISTS 0

struct directory {

  char name[MAX_FILE_NAME], *dir_list[MAX_NODE_SIZE];

  unsigned int id, dir_id[MAX_NODE_SIZE];

  struct directory *parent, *inode;
};

int hash_code(char *directory_name) {

  int hash_code = 250, temp = 0;

  while (*directory_name != '\n' && *directory_name != '\0') {

    temp = *directory_name;
    hash_code = ((hash_code << 5) + hash_code) + temp;
    directory_name++;
  }

  return (uint8_t)hash_code;
}

void display_fs(struct directory *curr_dir) {

  int index = 0;
  static int flag = 0;

  /* struct directory *ptr = curr_dir; */

  while (curr_dir->dir_list[index] != NULL) {

    switch (flag) {

    case 0:
      printf("🖿 \e[1m%s\e[0m\t▶ ", curr_dir->dir_list[index]);

      flag = 1;
      break;

    default:
      printf("|\n|--> 🖿 \e[1m%s\e[0m\t▶ ", curr_dir->dir_list[index]);

      break;
    }

    printf("%d\n", curr_dir->dir_id[index++]);
  }
}

void update_fat(struct directory *curr_dir, char *dir_name, int dir_id) {

  int index;

  for (index = 0; curr_dir->dir_list[index] != NULL; index++)
    ;

  curr_dir->dir_list[index] = dir_name;

  curr_dir->dir_id[index] = dir_id;
}

void populate_directory(struct directory *curr_dir, char *directory_name) {

  int directory_id = hash_code(directory_name);

  strcpy(curr_dir->inode[directory_id].name, directory_name);

  curr_dir->inode[directory_id].id = directory_id;

  curr_dir->parent = curr_dir;
}

int directory_conflicts(struct directory *curr_dir, char *directory_name) {

  int index = 0;

  while (curr_dir->dir_list[index] != NULL) {

    if (!strcmp(curr_dir->dir_list[index++], directory_name))
      return EXISTS;
  }

  return NOT_EXISTS;
}

void create_directory(struct directory *curr_dir, char *directory_name) {

  int directory_id = hash_code(directory_name);

  printf("\nID - %d\n%s\n", directory_id, directory_name);

  curr_dir->inode = (struct directory *)malloc(250 * sizeof(struct directory));

  if (directory_conflicts(curr_dir, directory_name) == EXISTS)
    return;

  populate_directory(curr_dir, directory_name);

  update_fat(curr_dir, directory_name, directory_id);
}

void create_dependencies(struct directory *mf) {

  char df_tel[10];

  strcpy(df_tel, "DF_TEL");

  strcpy(df_tel, df_tel);

  create_directory(mf, df_tel);

  create_directory(mf, "DF_GSM");

  create_directory(mf, "APP_APP");
}

struct directory init_mf(char *name) {

  struct directory mf;

  strcpy(mf.name, name);

  mf.id = hash_code(mf.name);

  mf.inode = (struct directory *)malloc(250 * sizeof(struct directory));

  mf.parent = NULL;

  update_fat(&mf, name, mf.id);

  create_dependencies(&mf);

  return mf;
}

void process_choice(struct directory *curr_dir, int choice) {

  int index = 0;
  char directory_name[MAX_FILE_NAME];

  switch (choice) {

  case 1: {
    scanf("%s", directory_name);

    while (directory_name[index++] != '\n')
      ;

    directory_name[--index] = '\0';

    create_directory(curr_dir, directory_name);

    break;
  }

  case 2: {
    display_fs(curr_dir);
    break;
  }
  }
}

void display_options(void) {

  printf("\n%s%s%s%s%s%s", "[1] -> Create a directory\n",
         "[2] -> View directory contents\n",
         "[3] -> View directory contents with FIDs\n",
         "[4] -> Change directory\n", "[5] -> Switch current directory to MF\n",
         "[6] -> Exit the file system\n");
}

int main(void) {

  /* int choice; */

  /* char directory_name[ MAX_FILE_NAME ]; */

  struct directory mf, *curr_dir;

  mf = init_mf("SIM OS");

  curr_dir = &mf;

  /* int index; */
  display_options();
  /*
    do {
    display_options();
    scanf( "%d", &choice );

    if (choice == 0) break;

    index = 0;
    scanf( "%s", directory_name );

    create_directory(curr_dir, directory_name);

    } while ( choice );  do {
    display_options();
    scanf( "%d", &choice );

    if (choice == 0) break;

    index = 0;
    scanf( "%s", directory_name );

    create_directory(curr_dir, directory_name);

    } while ( choice );
  */
  display_fs(curr_dir);

  return EXIT_SUCCESS;
}
