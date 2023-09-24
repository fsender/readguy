#include <stdio.h> 
#include <string.h> 
#include <math.h>
int main(int argc,char ** argv)
{
	FILE *fp=NULL;
	FILE *fw=NULL;
	if(argc>1){
		fp=fopen(argv[1],"rb");
		fw=fopen("result.h","wb");
	}
	if(fp==NULL) return 1;
	
    printf("\n//File name : ");
    puts(argv[1]);
    printf("\nconst uint8_t val [] PROGMEM = {\n  ");
    fprintf(fw,"#ifndef _RESULT_H_\n");
    fprintf(fw,"#define _RESULT_H_\n");
    fprintf(fw,"\nconst uint8_t val [] = {\n  ");
	int hadread=0;
    while(1){
        //if(feof(fp)) break;
        int f_justread=fgetc(fp);
        if(f_justread==EOF) break;
        fprintf(fw,"0x%02x,",f_justread);
        hadread++;
        if((hadread & 1023) ==0){
        	printf(" // %d KB converted.\n",(hadread>>10));
        	fprintf(fw,"// %d KB converted.",(hadread>>10));
        }
        if((hadread & 31) ==0){
        	fprintf(fw,"\n  ");
        }
    }
    fprintf(fw,"\n};\n//Total File size is %d bytes.\n",hadread);
    fprintf(fw,"\n#endif\n");
	fclose(fp);
	fclose(fw);
    printf("\n};\nTotal File size is %d bytes.\n",hadread);
	return 0;
}
