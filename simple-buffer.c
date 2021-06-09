# include <stdio.h>
# include<stdlib.h>

int student_id;

void smash_my_buffer()
{
    int var = 0x12345678;
    int another_var = 0x0;

    char buf[20];
    gets(buf);

    if ((var != 0x12345678) || (another_var != 0))
	printf("You changed my local variables.\n");
    
    if (var == student_id)
	   printf("Nice works. You've changed my var to %d. That's what I need :)\n",var);
    else
 	   printf("Almost there. My var is %d now. Try harder.\n", var);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
	printf("Please provide your Student ID.\nUsage: %s <your id>\n", argv[0]);
	exit(0);
    }
    
    student_id = atoi(argv[1]);
    smash_my_buffer();
}
