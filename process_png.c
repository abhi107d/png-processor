#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#define header_size 8
const uint8_t png_header[8]={137,80,78,71,13,10,26,10};


//read a buffer from file
void read_buffer(FILE* png,void *buffer,size_t size){

    size_t ret=fread(buffer,size,1,png);    
    if(ret!=1){
        fprintf(stderr,"ERROR: problem while reading file\n%s",
                strerror(errno));
        exit(1);
    }
}


//print a give buffer
void print_buffer(void *buff,size_t size){
    uint8_t *buffer=buff; 
    for(size_t i=0;i<size;i++){
        printf("%u ",buffer[i]);
    }
    printf("\n");
}



//reverse a buffer bigendian to little
void reverse_buffer(void *buff,size_t size){
    uint8_t *buffer=buff;
    for(size_t i=0;i<size/2;i++){
        uint8_t temp=buffer[i];
        buffer[i]=buffer[size-1-i];
        buffer[size-1-i]=temp;
    }
}
//check if valid png
uint8_t *check_valid_png(FILE *png){

    //read header 
    uint8_t* header=malloc(header_size*sizeof(uint8_t));
    read_buffer(png,header,sizeof(header));
    if(memcmp(header,png_header,header_size)!=0){
        fprintf(stderr,"ERROR: The File is not png");
        exit(1);
    }//check if valid png
    return header;

}


void get_png_info(FILE* png){


    while(1){
        //read chunk length
        uint32_t length;
        read_buffer(png,&length,sizeof(length));
        printf("SIZE :");
        reverse_buffer(&length,sizeof(length));
        printf("%u \n",length);

        //reading the type
        uint8_t type[4];
        read_buffer(png,type,sizeof(type));
        printf("CHUNK TYPE :%.*s (0x/%08X)\n",(int)sizeof(type),type,*(uint32_t*)type);    

        //seek the file size to next chunk
        if(fseek(png,length,SEEK_CUR)<0){
            fprintf(stderr,"ERROR : chunk skip failed %s",strerror(errno));
            exit(1);
        }
        uint32_t crc;
        read_buffer(png,&crc,sizeof(crc));
        printf("CRC : 0x/%08X \n",crc);
        printf("________________________\n\n");

        if(*(uint32_t *) type==0x444E4549)break;

    }
}






int main(int argc,char *argv[]){
    if(argc==1){
        fprintf(stderr,"ERROR: Provide png path\n");
        exit(1);
    }
    char* path=argv[1];

    FILE *png=fopen(path,"rb");     
    if (png==NULL){
        fprintf(stderr,"ERROR: The file {%s}  couldnot be open\n",path);
        fprintf(stderr,"ERROR: Make sure the path is correct");
        exit(1);
    }
    //reading reading header and checking if valid png    
    printf("HEADER :");
    uint8_t *header= check_valid_png(png);
    print_buffer(header,sizeof(header));
    printf("\n\n");
    //parsing each chunk and getting info
    get_png_info(png);




    
    fclose(png);
    return 0;
}



