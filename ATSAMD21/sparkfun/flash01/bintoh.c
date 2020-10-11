
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned int data[0x8000];

int main ( int argc, char *argv[] )
{
    unsigned int ra;
    unsigned int rb;

    FILE *fp;

    if(argc<2)
    {
        printf("bintoh binary output.h\n");
        return(1);
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        printf("Error opening file [%s]\n",argv[1]);
        return(1);
    }
    memset(data,0xFF,sizeof(data));
    rb=fread(data,1,sizeof(data),fp);
    fclose(fp);
    printf("%u bytes read\n",rb);
    if(rb>0x800)
    {
        printf("binary too big\n");
        return(1);
    }
    fp=fopen(argv[2],"wt");
    if(fp==NULL)
    {
        printf("Error creating file [%s]\n",argv[2]);
        return(1);
    }
    fprintf(fp,"\n");
    fprintf(fp,"static const unsigned int rom[]=\n");
    fprintf(fp,"{\n");
    for(ra=0;ra<(0x800>>2);ra++)
    {
        fprintf(fp,"0x%08X, //0x%04X\n",data[ra],ra<<2);
    }
    fprintf(fp,"};\n");
    fprintf(fp,"\n");

}

