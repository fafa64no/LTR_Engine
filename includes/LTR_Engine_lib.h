#pragma once

#include <stdio.h>
#include <fstream>
#include <string>       //Used to get memset
#include <stdlib.h>     //Used to get malloc
#include <sys/stat.h>   //Used to get edit timestamp of files
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

// ############################################################################
//                            Defines
// ############################################################################
#ifdef _WIN32
#define DEBUG_BREAK() __debugbreak()
#define EXPORT_FN __declspec(dllexport)
#elif __linux__
#define DEBUG_BREAK() __builtin_debugtrap()
#elif __APPLE__
#define DEBUG_BREAK() __builtin_trap()
#endif

#define BIT(x)1<<(x)
#define KB(x)((unsigned long long)1024*x)
#define MB(x)((unsigned long long)1024*KB(x))
#define GB(x)((unsigned long long)1024*MB(x))
#define b8 char

// ############################################################################
//                            Logging
// ############################################################################
static int debug_instance_id;
const std::string debug_log_location="logs/";
static char* debug_log_path;
bool append_file(const char* filePath,std::string buffer);
void debug_log(std::string msg){
    append_file(debug_log_path,msg);
}
template <typename ...Args>
void _log(char* prefix,char* msg,Args... args){
    char formatBuffer[8192]={};
    sprintf(formatBuffer,"%s %s",prefix,msg);
    char textBuffer[8192]={};
    sprintf(textBuffer,formatBuffer,args...);
    puts(textBuffer);
}

#define SM_TRACE(msg,...){_log("TRACE: ",msg,##__VA_ARGS__);debug_log((std::string)msg);}
#define SM_WARN(msg,...){_log("WARN: ",msg,##__VA_ARGS__);debug_log((std::string)msg);}
#define SM_ERROR(msg,...){_log("ERROR: ",msg,##__VA_ARGS__);debug_log((std::string)msg);}

#define SM_ASSERT(x,msg,...){           \
    if (!(x)){                          \
        SM_ERROR(msg,##__VA_ARGS__);    \
        SM_ERROR("Assertion HIT");      \
        DEBUG_BREAK();                  \
    }                                   \
}

void disp_chars(char* start,int length){
    char temp[256]{0};int i{0};
    for(i;i<length;i++)temp[i]=start[i];
    temp[i+1]='\0';
    SM_TRACE(temp);
}
void disp_ascii_chars(char* start,int length){
    for(int i=0;i<length;i+=4){
        int value1=(*((int*)(&start[i])))&(0x000000ff);
        int value2=(*((int*)(&start[i+1])))&(0x000000ff);
        int value3=(*((int*)(&start[i+2])))&(0x000000ff);
        int value4=(*((int*)(&start[i+3])))&(0x000000ff);
        char hexString[9];
        sprintf(hexString,"%02x%02x%02x%02x",value1,value2,value3,value4);
        SM_TRACE(hexString);
    }
}

// ############################################################################
//                            Miscellaneous
// ############################################################################
float get_float_le(char* array){
    char temp[sizeof(float)];
    for(int i=0;i<sizeof(float);i++)temp[i]=array[sizeof(float)-i-1];
    return *((float*)&temp[0]);
}
float get_float_be(char* array){
    return *((float*)&array[0]);
}

unsigned short get_ushort_le(char* array){
    char temp[sizeof(unsigned short)];
    for(int i=0;i<sizeof(unsigned short);i++)temp[i]=array[sizeof(unsigned short)-i-1];
    return *((unsigned short*)&temp[0]);
}
unsigned short get_ushort_be(char* array){
    return *((unsigned short*)&array[0]);
}

unsigned int get_uint_fshort_le(char* array){
    char temp[sizeof(unsigned int)];
    for(int i=0;i<sizeof(unsigned int);i++)temp[i]=array[sizeof(unsigned int)-i-1];
    return *((unsigned int*)&temp[0])&0x0000ffff;
}
unsigned int get_uint_fshort_be(char* array){
    return (*((unsigned int*)&array[0]))&0x0000ffff;
}

void quatToMat(glm::mat4 &mat,glm::vec4 quat){
    ///TODO convert quaternion to matrix
}

// ############################################################################
//                            Bump Allocator
// ############################################################################
struct BumpAllocator{
    size_t capacity;
    size_t used;
    char* memory;
};
BumpAllocator make_bump_allocator(size_t size){
    BumpAllocator ba={};
    ba.memory=(char*)malloc(size);
    if (ba.memory){
        ba.capacity=size;
        memset(ba.memory,0,size);
    }else{
        SM_ASSERT(false,"Failed to allocate memory");
    }
    return ba;
}
char* bump_alloc(BumpAllocator* bumpAllocator,size_t size){
    char* result=nullptr;
    size_t alignedSize=(size+7) & ~ 7;
    if (bumpAllocator->used+alignedSize<=bumpAllocator->capacity){
        result=bumpAllocator->memory+bumpAllocator->used;
        bumpAllocator->used+=alignedSize;
    }else{
        SM_TRACE("Tried to allocate :");
        SM_TRACE((char*)std::to_string(size).c_str());
        SM_ASSERT(false,"BumpAllocator is full");
    }
    return result;
}

// ############################################################################
//                            File I/O 
// ############################################################################
enum SceneReadingState{
    WAITING_FOR_JSON,
    READING_JSON,

    WAITING_FOR_BIN,
    READING_BIN,

    COUNTING_BUFFERS,
    COUNTING_BUFFERVIEWS,
    COUNTING_ACCESSORS,
    COUNTING_MESHES,
    COUNTING_NODES,

    WAITING_FOR_END_OF_FILE,

    MESH_READING_STATE_COUNT
};
enum AccessorType{
    SCALAR,
    VEC2,
    VEC3,
    VEC4,
    UNKNOWN_ACCESSOR_TYPE,
    ACCESSOR_TYPE_COUNT
};

struct Buffer{
    unsigned int start;
    unsigned int length;
};
struct BufferView{
    Buffer* buffer;
    unsigned int byteLength;
    unsigned int byteOffset;
    unsigned int byteStride;
    unsigned int target;
};
struct Accessor{
    BufferView* bufferView;
    unsigned int componentType;
    unsigned int count;
    AccessorType type;
};
struct MeshConstructor{
    Accessor* positions;
    Accessor* normal;
    Accessor* texcoord;
    Accessor* indices;
};
struct MeshNodeConstructor{
    MeshConstructor* meshConstructor;
    glm::vec3 translation;
    glm::vec4 rotation;
    glm::vec3 scale;
};

long long get_timestamp(char* file){
    struct stat file_stat={};
    stat(file,&file_stat);
    return file_stat.st_mtime;
}
bool file_exists(char* filePath){
    SM_ASSERT(filePath,"No filePath supplied");
    auto file=fopen(filePath,"rb");
    if (!file){
        return false;
    }
    fclose(file);
    return true;
}
long get_file_size(char* filePath){
    SM_ASSERT(filePath,"No file path supplied");
    long fileSize=0;
    auto file=fopen(filePath,"rb");
    if (!file){
        SM_ERROR("Failed opening file: %s",filePath);
        return 0;
    }
    fseek(file,0,SEEK_END);
    fileSize=ftell(file);
    fseek(file,0,SEEK_SET);
    fclose(file);
    return fileSize;
}
char* read_file(char* filePath,int* fileSize,char* buffer){
    SM_ASSERT(filePath,"No filePath supplied");
    SM_ASSERT(fileSize,"No fileSize supplied");
    SM_ASSERT(buffer,"No buffer supplied");
    *fileSize=0;
    auto file=fopen(filePath,"rb");
    if (!file){
        SM_ERROR("Failed opening file: %s",filePath);
        return nullptr;
    }
    fseek(file,0,SEEK_END);
    *fileSize=ftell(file);
    fseek(file,0,SEEK_SET);
    memset(buffer,0,*fileSize+1);
    fread(buffer,sizeof(char),*fileSize,file);
    fclose(file);
    return buffer;
}
char* read_file(char* filePath,int* fileSize,BumpAllocator* bumpAllocator){
    char* file=nullptr;
    long fileSize2=get_file_size(filePath);
    if (fileSize2){
        char* buffer=bump_alloc(bumpAllocator,fileSize2+1);
        file=read_file(filePath,fileSize,buffer);
    }
    return file;
}
void write_file(char* filePath,char* buffer,int size){
    SM_ASSERT(filePath,"No filePath supplied");
    SM_ASSERT(buffer,"No buffer supplied");
    auto file=fopen(filePath,"wb");
    if (!file){
        SM_ERROR("Failed opening file: %s",filePath);
        return;
    }
    fwrite(buffer,sizeof(char),size,file);
    fclose(file);
}
void write_file(const char* filePath,char* buffer,int size){
    SM_ASSERT(filePath,"No filePath supplied");
    SM_ASSERT(buffer,"No buffer supplied");
    auto file=fopen(filePath,"wb");
    if (!file){
        SM_ERROR("Failed opening file: %s",filePath);
        return;
    }
    fwrite(buffer,sizeof(char),size,file);
    fclose(file);
}
bool copy_file(char* fileName,char* outputName,char* buffer){
    int fileSize=0;
    char* data=read_file(fileName,&fileSize,buffer);
    auto outputFile=fopen(outputName,"wb");
    if (!outputFile){
        SM_ERROR("Failed opening file: %s",outputName);
        return false;
    }
    int result=fwrite(data,sizeof(char),fileSize,outputFile);
    if (!result){
        SM_ERROR("Failed writing file: %s",outputName);
        return false;
    }
    fclose(outputFile);
    return true;
}
bool copy_file(char* fileName,char* outputName,BumpAllocator* bumpAllocator){
    char* file=nullptr;
    long fileSize2=get_file_size(fileName);
    if (fileSize2){
        char* buffer=bump_alloc(bumpAllocator,fileSize2+1);
        return copy_file(fileName,outputName,buffer);
    }
    return false;
}
bool append_file(const char* filePath,std::string buffer){
    std::fstream stream;
    stream.open(filePath,std::fstream::app);
    if (!stream.is_open()){
        SM_ERROR("Failed to open file stream: %s",filePath);
        return false;
    }
    stream<<buffer<<"\n";
    stream.close();
    return true;
}
void init_debug_log_system(){
    srand(get_timestamp("LTR_Engine.exe"));
    debug_instance_id=rand();
    {
        std::string strBuffer="";
        strBuffer.append(debug_log_location);
        strBuffer.append("LOG");
        strBuffer.append(std::to_string(debug_instance_id));
        strBuffer.append(".log");
        debug_log_path=(char*)malloc(sizeof(char)*strBuffer.length());
        strcpy(debug_log_path,strBuffer.c_str());
    }
    std::string debug_header="\n------------------------------------------------------";
    debug_header.append("\nDebug log file ");
    debug_header.append(std::to_string(debug_instance_id));
    debug_header.append("\n------------------------------------------------------");
    if (file_exists(debug_log_path)){
        write_file(debug_log_path,"",0);
        append_file(debug_log_path,debug_header);
    }else{
        std::ofstream outfile(debug_log_path);
        outfile<<debug_header<<std::endl;
        outfile.close();
    }
}
// ############################################################################
//                            GLB Stuff
// ############################################################################
unsigned int read_uint(std::vector<char> &buffer,int &i){
    unsigned int result{0};
    for(i;i<buffer.size();i++){
        if(buffer[i]<='9'&&buffer[i]>='0'){
            result*=10;
            result+=buffer[i]&0x0f;
        }else{
            if(buffer[i]!=','&&buffer[i]!='}')disp_chars(&buffer[i],16);
            if(buffer[i]=='}')i--;
            break;
        }
    }
    return result;
}
float read_float(std::vector<char> &buffer,int &i){
    float result{0};
    int afterDot=-0x7fff;
    int sign=1;
    for(i;i<buffer.size();i++){
        if(buffer[i]<='9'&&buffer[i]>='0'){
            result=(afterDot>0)?result+(buffer[i]&0x0f)*(float)pow(0.1,(double)afterDot):(float)10*result+(buffer[i]&0x0f);
            afterDot++;
        }else if(buffer[i]=='.'){
            afterDot=1;
        }else if(buffer[i]=='-'){
            sign=-1;
        }else{
            break;
        }
    }
    return sign*result;
}
glm::vec3 read_vec3(std::vector<char> &buffer,int &i){
    glm::vec3 result;
    for(int j=0;j<3;j++){
        result[j]=read_float(buffer,i);
        i++;
    }
    return result;
}
glm::vec4 read_vec4(std::vector<char> &buffer,int &i){
    glm::vec4 result;
    for(int j=0;j<4;j++){
        result[j]=read_float(buffer,i);
        i++;
    }
    return result;
}
AccessorType read_accessor_type(std::vector<char> &buffer,int &i){
    if(strncmp(&buffer[i],"SCALAR",6)==0)return SCALAR;
    else if(strncmp(&buffer[i],"VEC2",4)==0)return VEC2;
    else if(strncmp(&buffer[i],"VEC3",4)==0)return VEC3;
    else if(strncmp(&buffer[i],"VEC4",4)==0)return VEC4;
    return UNKNOWN_ACCESSOR_TYPE;
}
void read_glb_file(char* filePath,std::vector<char>* &buffer,BumpAllocator* persistentStorage){
    SM_TRACE("Loading glb scene...");
    std::ifstream input(filePath,std::ios::binary);
    buffer=new std::vector<char>(std::istreambuf_iterator<char>(input),{});
    int fileSize=buffer->size();
    SM_TRACE("\tFile size:");
    SM_TRACE((char*)std::to_string(fileSize).c_str());
}

void get_glb_structure(BumpAllocator* bumpAllocator,
            std::vector<char> &buffer,                  int &binStart,
            Buffer* &buffers,                           int &buffersCount,
            BufferView* &bufferViews,                   int &bufferViewsCount,
            Accessor* &accessors,                       int &accessorCount,
            MeshConstructor* &meshConstructors,         int &meshConstructorCount,
            MeshNodeConstructor* &meshNodeConstructors, int &meshNodeConstructorCount){
    int nodesPosInJSON{-1},meshesPosInJSON{-1},accessorsPosInJSON{-1},bufferViewsPosInJSON{-1},buffersPosInJSON{-1},incrementDepth{0};
    SceneReadingState currentReadingState=WAITING_FOR_JSON;
    buffersCount=0;
    bufferViewsCount=0;
    accessorCount=0;
    meshConstructorCount=0;
    meshNodeConstructorCount=0;
    //Get JSON positions
    for(int i=0;i<buffer.size();i++){
        switch(currentReadingState){
            case WAITING_FOR_JSON:{
                if(strncmp(&buffer[i],"JSON",4)==0){
                    currentReadingState=READING_JSON;
                    i+=5;
                    incrementDepth=1;
                }break;
            }
            case READING_JSON:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==0){
                            currentReadingState=WAITING_FOR_BIN;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        break;
                    }case '"':{
                        if(incrementDepth==1){
                            if(strncmp(&buffer[i+1],"nodes",5)==0){
                                i+=8;incrementDepth=2;
                                nodesPosInJSON=i+1;
                                currentReadingState=COUNTING_NODES;
                            }else if(strncmp(&buffer[i+1],"meshes",6)==0){
                                i+=9;incrementDepth=2;
                                meshesPosInJSON=i+1;
                                currentReadingState=COUNTING_MESHES;
                            }else if(strncmp(&buffer[i+1],"accessors",9)==0){
                                i+=12;incrementDepth=2;
                                accessorsPosInJSON=i+1;
                                currentReadingState=COUNTING_ACCESSORS;
                            }else if(strncmp(&buffer[i+1],"bufferViews",11)==0){
                                i+=14;incrementDepth=2;
                                bufferViewsPosInJSON=i+1;
                                currentReadingState=COUNTING_BUFFERVIEWS;
                            }else if(strncmp(&buffer[i+1],"buffers",7)==0){
                                i+=10;incrementDepth=2;
                                buffersPosInJSON=i+1;
                                currentReadingState=COUNTING_BUFFERS;
                            }
                        }
                    }
                }break;
            }
            case WAITING_FOR_BIN:{
                if(strncmp(&buffer[i],"BIN",4)==0){
                    binStart=i+4;
                    i=buffer.size();
                }break;
            }
            case COUNTING_NODES:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==1){
                            currentReadingState=READING_JSON;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        if(incrementDepth==3){
                            meshNodeConstructorCount++;
                        }
                        break;
                    }
                }break;
            }
            case COUNTING_MESHES:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==1){
                            currentReadingState=READING_JSON;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        if(incrementDepth==3){
                            meshConstructorCount++;
                        }
                        break;
                    }
                }break;
            }
            case COUNTING_ACCESSORS:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==1){
                            currentReadingState=READING_JSON;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        if(incrementDepth==3){
                            accessorCount++;
                        }
                        break;
                    }
                }break;
            }
            case COUNTING_BUFFERVIEWS:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==1){
                            currentReadingState=READING_JSON;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        if(incrementDepth==3){
                            bufferViewsCount++;
                        }
                        break;
                    }
                }break;
            }
            case COUNTING_BUFFERS:{
                switch(buffer[i]){
                    case ']': case '}':{
                        incrementDepth--;
                        if(incrementDepth==1){
                            currentReadingState=READING_JSON;
                        }
                        break;
                    }case '[': case '{':{
                        incrementDepth++;
                        if(incrementDepth==3){
                            buffersCount++;
                        }
                        break;
                    }
                }break;
            }
        }
    }
    buffers=(Buffer*)bump_alloc(bumpAllocator,sizeof(Buffer)*buffersCount);
    bufferViews=(BufferView*)bump_alloc(bumpAllocator,sizeof(BufferView)*bufferViewsCount);
    accessors=(Accessor*)bump_alloc(bumpAllocator,sizeof(Accessor)*accessorCount);
    meshConstructors=(MeshConstructor*)bump_alloc(bumpAllocator,sizeof(MeshConstructor)*meshConstructorCount);
    meshNodeConstructors=(MeshNodeConstructor*)bump_alloc(bumpAllocator,sizeof(MeshNodeConstructor)*meshNodeConstructorCount);
    int static posId;
    //Get buffer data structure
    incrementDepth=2;posId=0;
    for(int i=buffersPosInJSON;i<buffer.size();i++){
        switch(buffer[i]){
            case ']':case '}':{
                incrementDepth--;
                if(incrementDepth==1)i=buffer.size();
                break;
            }case '[':case '{':{
                incrementDepth++;
                if(incrementDepth==3)posId++;
                break;
            }case '"':{
                if(strncmp(&buffer[i+1],"byteLength",10)==0){
                    i+=13;
                    unsigned int byteLength{read_uint(buffer,i)};
                    buffers[posId-1].length=byteLength;
                }
            }
        }
    }
    //Get bufferView data structure
    incrementDepth=2;posId=0;
    for(int i=bufferViewsPosInJSON;i<buffer.size();i++){
        switch(buffer[i]){
            case ']':case '}':{
                incrementDepth--;
                if(incrementDepth==1)i=buffer.size();
                break;
            }case '[':case '{':{
                incrementDepth++;
                if(incrementDepth==3)posId++;
                break;
            }case '"':{
                if(strncmp(&buffer[i+1],"buffer",6)==0){
                    i+=9;
                    unsigned int bufferId{read_uint(buffer,i)};
                    bufferViews[posId-1].buffer=&buffers[bufferId];
                }else if(strncmp(&buffer[i+1],"byteLength",10)==0){
                    i+=13;
                    unsigned int byteLength{read_uint(buffer,i)};
                    bufferViews[posId-1].byteLength=byteLength;
                }else if(strncmp(&buffer[i+1],"byteOffset",10)==0){
                    i+=13;
                    unsigned int byteOffset{read_uint(buffer,i)};
                    bufferViews[posId-1].byteOffset=byteOffset;
                }else if(strncmp(&buffer[i+1],"byteStride",10)==0){
                    i+=13;
                    unsigned int byteStride{read_uint(buffer,i)};
                    bufferViews[posId-1].byteStride=byteStride;
                }else if(strncmp(&buffer[i+1],"target",6)==0){
                    i+=9;
                    unsigned int target{read_uint(buffer,i)};
                    bufferViews[posId-1].target=target;
                }
            }
        }
    }
    //Get Accessor data structure
    incrementDepth=2;posId=0;
    for(int i=accessorsPosInJSON;i<buffer.size();i++){
        switch(buffer[i]){
            case ']':case '}':{
                incrementDepth--;
                if(incrementDepth==1)i=buffer.size();
                break;
            }case '[':case '{':{
                incrementDepth++;
                if(incrementDepth==3)posId++;
                break;
            }case '"':{
                if(strncmp(&buffer[i+1],"bufferView",10)==0){
                    i+=13;
                    unsigned int bufferViewId{read_uint(buffer,i)};
                    accessors[posId-1].bufferView=&bufferViews[bufferViewId];
                }else if(strncmp(&buffer[i+1],"componentType",13)==0){
                    i+=16;
                    accessors[posId-1].componentType=read_uint(buffer,i);
                }else if(strncmp(&buffer[i+1],"count",5)==0){
                    i+=8;
                    accessors[posId-1].count=read_uint(buffer,i);
                }else if(strncmp(&buffer[i+1],"type",4)==0){
                    i+=7;
                    accessors[posId-1].type=read_accessor_type(buffer,i);
                }
            }
        }
    }
    //Get Mesh data structure
    incrementDepth=2;posId=0;
    for(int i=meshesPosInJSON;i<buffer.size();i++){
        switch(buffer[i]){
            case ']':case '}':{
                incrementDepth--;
                if(incrementDepth==1)i=buffer.size();
                break;
            }case '[':case '{':{
                incrementDepth++;
                if(incrementDepth==3)posId++;
                break;
            }case '"':{
                if(strncmp(&buffer[i+1],"POSITION",8)==0){
                    i+=11;
                    unsigned int accessorId{read_uint(buffer,i)};
                    meshConstructors[posId-1].positions=&accessors[accessorId];
                }else if(strncmp(&buffer[i+1],"NORMAL",6)==0){
                    i+=9;
                    unsigned int accessorId{read_uint(buffer,i)};
                    meshConstructors[posId-1].normal=&accessors[accessorId];
                }else if(strncmp(&buffer[i+1],"TEXCOORD_0",10)==0){
                    i+=13;
                    unsigned int accessorId{read_uint(buffer,i)};
                    meshConstructors[posId-1].texcoord=&accessors[accessorId];
                }else if(strncmp(&buffer[i+1],"indices",7)==0){
                    i+=10;
                    unsigned int accessorId{read_uint(buffer,i)};
                    meshConstructors[posId-1].indices=&accessors[accessorId];
                }
            }
        }
    }
    //Get Node data structure
    incrementDepth=2;posId=0;
    for(int i=nodesPosInJSON;i<buffer.size();i++){
        switch(buffer[i]){
            case ']':case '}':{
                incrementDepth--;
                if(incrementDepth==1)i=buffer.size();
                break;
            }case '[':case '{':{
                incrementDepth++;
                if(incrementDepth==3)posId++;
                //meshNodeConstructors[posId-1].scale=glm::vec3(1.0f,1.0f,1.0f);
                break;
            }case '"':{
                if(incrementDepth!=3)break;
                if(strncmp(&buffer[i+1],"mesh",4)==0){
                    i+=7;
                    unsigned int meshId{read_uint(buffer,i)};
                    meshNodeConstructors[posId-1].meshConstructor=&meshConstructors[meshId];
                }else if(strncmp(&buffer[i+1],"translation",11)==0){
                    i+=15;
                    meshNodeConstructors[posId-1].translation=read_vec3(buffer,i);
                    i--;
                }else if(strncmp(&buffer[i+1],"rotation",8)==0){
                    i+=12;
                    meshNodeConstructors[posId-1].rotation=read_vec4(buffer,i);
                    i--;
                }else if(strncmp(&buffer[i+1],"scale",5)==0){
                    i+=9;
                    meshNodeConstructors[posId-1].scale=read_vec3(buffer,i);
                    i--;
                }
            }
        }
    }
}



// ############################################################################
//                            Debug
// ############################################################################
void sort_glb_file(std::vector<char> &buffer){
    std::ofstream headerFile("logs/debug_test/header.log");
    std::ofstream contentFile("logs/debug_test/content.json");
    std::ofstream binFile("logs/debug_test/buffer.bin");

    SceneReadingState readingState=WAITING_FOR_JSON;
    for(int i=0;i<buffer.size();i++){
        switch(readingState){
            case WAITING_FOR_JSON:{
                headerFile<<(char)buffer[i];
                if(buffer[i+1]=='J'&&buffer[i+2]=='S'&&buffer[i+3]=='O'&&buffer[i+4]=='N'){
                    readingState=WAITING_FOR_BIN;
                    i+=4;
                }
                break;
            }
            case WAITING_FOR_BIN:{
                static int indents=0;
                switch(buffer[i]){
                    case '{':{
                        contentFile<<std::endl;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        contentFile<<(char)buffer[i]<<std::endl;
                        indents++;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        break;
                    }case '}':{
                        contentFile<<std::endl;
                        indents--;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        contentFile<<(char)buffer[i]<<std::endl;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        break;
                    }case '[':{
                        contentFile<<std::endl;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        contentFile<<(char)buffer[i]<<std::endl;
                        indents++;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        break;
                    }case ']':{
                        contentFile<<std::endl;
                        indents--;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        contentFile<<(char)buffer[i]<<std::endl;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        break;
                    }case ',':{
                        contentFile<<(char)buffer[i]<<std::endl;
                        for(int j=0;j<indents;j++)contentFile<<"\t";
                        break;
                    }default:{
                        contentFile<<(char)buffer[i];
                        break;
                    }
                }
                if(buffer[i+1]=='B'&&buffer[i+2]=='I'&&buffer[i+3]=='N'){
                    contentFile<<std::endl;
                    readingState=WAITING_FOR_END_OF_FILE;
                    i+=4;
                }
                break;
            }
            case WAITING_FOR_END_OF_FILE:{
                binFile<<(char)buffer[i];
                break;
            }
            default:{
                break;
            }
        }
    }
    headerFile<<std::endl;
    contentFile<<std::endl;
    binFile<<std::endl;
    headerFile.close();
    contentFile.close();
    binFile.close();
}

void disp_glb_structure(char* scenePath,BumpAllocator* transientStorage,BumpAllocator* persistentStorage){
    SM_TRACE("\n############## disp_glb_structure start ##############\n");
    SM_TRACE("Reading :");
    SM_TRACE(scenePath);
    std::vector<char>* buffer;
    read_glb_file(scenePath,buffer,persistentStorage);
    int binStart{-1};
    int bufferCount{0}, bufferViewCount{0},         accessorCount{0},       meshConstructorCount{0},            meshNodeConstructorCount{0};
    Buffer* buffers;    BufferView* bufferViews;    Accessor* accessors;    MeshConstructor* meshConstructors;  MeshNodeConstructor* meshNodeConstructors;
    SM_TRACE("...calling structure...");
    get_glb_structure(transientStorage,
                        *buffer,                binStart,
                        buffers,                bufferCount,
                        bufferViews,            bufferViewCount,
                        accessors,              accessorCount,
                        meshConstructors,       meshConstructorCount,
                        meshNodeConstructors,   meshNodeConstructorCount);
    SM_TRACE("\n----------------------\n");
    SM_TRACE("bufferCount :");
    SM_TRACE((char*)std::to_string(bufferCount).c_str());
    SM_TRACE("bufferViewCount :");
    SM_TRACE((char*)std::to_string(bufferViewCount).c_str());
    SM_TRACE("accessorCount :");
    SM_TRACE((char*)std::to_string(accessorCount).c_str());
    SM_TRACE("meshConstructorCount :");
    SM_TRACE((char*)std::to_string(meshConstructorCount).c_str());
    SM_TRACE("meshNodeConstructorCount :");
    SM_TRACE((char*)std::to_string(meshNodeConstructorCount).c_str());
    SM_TRACE("\n############## disp_glb_structure end ##############\n");
}








