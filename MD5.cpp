#include "MD5.h"
#include <bitset>
using namespace std;
uint32_t FunF(uint32_t x, uint32_t y, uint32_t z){
    return (x & y) | (~x & z);
}
uint32_t FunG(uint32_t x, uint32_t y, uint32_t z){
    return (x & z) | (~z & y);
}
uint32_t FunH(uint32_t x, uint32_t y, uint32_t z){
    return x ^ y ^ z;
}
uint32_t FunI(uint x, uint32_t y, uint32_t z){
    return y ^ (~z | x);
}
uint32_t rotateLeft(uint32_t bt, int c){
    return ((bt<<c)|((bt&0xffffffff)>>(32-c)));
}
vector<uint32_t> initT(){
    vector<uint32_t> T(64);
    for(int i=1;i<=64;i++){
        T[i-1] = (uint32_t)(pow(2.0,32)*fabs(sin(i)));
        //вектор T можно инициализировать полностью заранее, а можно создать его так
        //если вместо вещественного fabs использовать целочисленный abs,
        //можно потерять некоторые значения T
    }
    return T;
}
bool MD5(char*filename1,char*filename2){//MD5-хэширование файла произвольного размера
    vector<uint32_t> T=initT();
    ifstream file(filename1, ios::binary|ios::in);//файл-исходник
    ofstream md5file(filename2,ios::binary|ios::out);//файл с md5-хешем
    if((!file.is_open())||(!md5file.is_open()))
        return false;
    long long sizeFile = getSizeFile(filename1);//размер файла в битах
    long long newSize = 64*(1+(sizeFile+8)/64);
    vector<unsigned char> text(newSize,0);//выделяем размер так, чтобы он был кратен 512 битам заполняем весь вектор нулями
    char byte;//текущий считываемый байт
    int j=0;
    while(file.read(&byte,sizeof(char))){
        text[j++] = byte;//сначала переписываем в вектор все байты с текстом
    }
    text[j++]=0x80;//дописываем к тексту единицу и последовательность нулей
    //в следующий байт за текстом будет записано число 0х80, если мы переведем его в двоичную систему, получим 10000000 - единицу и 7 нулей
    text[newSize-8]=static_cast<unsigned char>((sizeFile*8) & 0xFF);
    text[newSize-7]=static_cast<unsigned char>(((sizeFile*8)>>8) & 0xFF);
    text[newSize-6]=static_cast<unsigned char>(((sizeFile*8)>>16) & 0xFF);
    text[newSize-5]=static_cast<unsigned char>(((sizeFile*8)>>24) & 0xFF);
    /*text[j-4]=static_cast<unsigned char>(((sizeFile*8)>>32) & 0xFF);
    text[j-3]=static_cast<unsigned char>(((sizeFile*8)>>40) & 0xFF);
    text[j-2]=static_cast<unsigned char>(((sizeFile*8)>>48) & 0xFF);
    text[j-1]=static_cast<unsigned char>(((sizeFile*8)>>56) & 0xFF);*/
    long long blocksCount = (newSize*8)/(MD5_MOD_CONSTANT);//число блоков для подсчета
    j=0;
    uint32_t a0=A;//создаем аbcd для хранения текущего состояния хэша
    uint32_t b0=B;
    uint32_t c0=C;
    uint32_t d0=D;
    uint32_t a=a0,b=b0,c=c0,d=d0;
    //в abcd будут храниться текущие состояния, в a0b0c0d0-состояния для предыдущего 512-битного блока или первоначальные инициализирующие состояния, с ними складываются текущие abcd
    for(int i=0;i<blocksCount;i++){//обходим каждый 512-битный блок
        vector<uint>X(16);//16 слов по 4 байта
        for(int k=0;k<16;k++){//копируем байты текста в Х
            X[k]=0;
            for(int l=0;l<4;l++){
                X[k]=X[k]|((uint32_t)(text[j++])<<(l*8));//побитовый сдвиг нужен, чтобы записать 4 байтные переменные в одну четырехбайтную
            }
        }
        //первый этап вычислений
        a=b+rotateLeft(a+FunF(b,c,d)+X[0]+T[0],7);
        d=a+rotateLeft(d+FunF(a,b,c)+X[1]+T[1],12);
        c=d+rotateLeft(c+FunF(d,a,b)+X[2]+T[2],17);
        b=c+rotateLeft(b+FunF(c,d,a)+X[3]+T[3],22);
        
        a=b+rotateLeft(a+FunF(b,c,d)+X[4]+T[4],7);
        d=a+rotateLeft(d+FunF(a,b,c)+X[5]+T[5],12);
        c=d+rotateLeft(c+FunF(d,a,b)+X[6]+T[6],17);
        b=c+rotateLeft(b+FunF(c,d,a)+X[7]+T[7],22);
        
        a=b+rotateLeft(a+FunF(b,c,d)+X[8]+T[8],7);
        d=a+rotateLeft(d+FunF(a,b,c)+X[9]+T[9],12);
        c=d+rotateLeft(c+FunF(d,a,b)+X[10]+T[10],17);
        b=c+rotateLeft(b+FunF(c,d,a)+X[11]+T[11],22);
        
        a=b+rotateLeft(a+FunF(b,c,d)+X[12]+T[12],7);
        d=a+rotateLeft(d+FunF(a,b,c)+X[13]+T[13],12);
        c=d+rotateLeft(c+FunF(d,a,b)+X[14]+T[14],17);
        b=c+rotateLeft(b+FunF(c,d,a)+X[15]+T[15],22);
        //второй этап
        a=b+rotateLeft(a+FunG(b,c,d)+X[1]+T[16],5);
        d=a+rotateLeft(d+FunG(a,b,c)+X[6]+T[17],9);
        c=d+rotateLeft(c+FunG(d,a,b)+X[11]+T[18],14);
        b=c+rotateLeft(b+FunG(c,d,a)+X[0]+T[19],20);

        a=b+rotateLeft(a+FunG(b,c,d)+X[5]+T[20],5);
        d=a+rotateLeft(d+FunG(a,b,c)+X[10]+T[21],9);
        c=d+rotateLeft(c+FunG(d,a,b)+X[15]+T[22],14);
        b=c+rotateLeft(b+FunG(c,d,a)+X[4]+T[23],20);
        
        a=b+rotateLeft(a+FunG(b,c,d)+X[9]+T[24],5);
        d=a+rotateLeft(d+FunG(a,b,c)+X[14]+T[25],9);
        c=d+rotateLeft(c+FunG(d,a,b)+X[3]+T[26],14);
        b=c+rotateLeft(b+FunG(c,d,a)+X[8]+T[27],20);
        
        a=b+rotateLeft(a+FunG(b,c,d)+X[13]+T[28],5);
        d=a+rotateLeft(d+FunG(a,b,c)+X[2]+T[29],9);
        c=d+rotateLeft(c+FunG(d,a,b)+X[7]+T[30],14);
        b=c+rotateLeft(b+FunG(c, d, a)+X[12]+T[31],20);
        //третий этап
        a=b+rotateLeft(a+FunH(b,c,d)+X[5]+T[32],4);
        d=a+rotateLeft(d+FunH(a,b,c)+X[8]+T[33],11);
        c=d+rotateLeft(c+FunH(d,a,b)+X[11]+T[34],16);
        b=c+rotateLeft(b+FunH(c,d,a)+X[14]+T[35],23);

        a=b+rotateLeft(a+FunH(b,c,d)+X[1]+T[36],4);
        d=a+rotateLeft(d+FunH(a,b,c)+X[4]+T[37],11);
        c=d+rotateLeft(c+FunH(d,a,b)+X[7]+T[38],16);
        b=c+rotateLeft(b+FunH(c,d,a)+X[10]+T[39],23);

        a=b+rotateLeft(a+FunH(b,c,d)+X[13]+T[40],4);
        d=a+rotateLeft(d+FunH(a,b,c)+X[0]+T[41],11);
        c=d+rotateLeft(c+FunH(d,a,b)+X[3]+T[42],16);
        b=c+rotateLeft(b+FunH(c,d,a)+X[6]+T[43],23);
  
        a=b+rotateLeft(a+FunH(b,c,d)+X[9]+T[44],4);
        d=a+rotateLeft(d+FunH(a,b,c)+X[12]+T[45],11);
        c=d+rotateLeft(c+FunH(d,a,b)+X[15]+T[46],16);
        b=c+rotateLeft(b+FunH(c,d,a)+X[2]+T[47],23);
        //четвертый этап
        a=b+rotateLeft(a+FunI(b,c,d)+X[0]+T[48],6);
        d=a+rotateLeft(d+FunI(a,b,c)+X[7]+T[49],10);
        c=d+rotateLeft(c+FunI(d,a,b)+X[14]+T[50],15);
        b=c+rotateLeft(b+FunI(c,d,a)+X[5]+T[51],21);

        a=b+rotateLeft(a+FunI(b,c,d)+X[12]+T[52],6);
        d=a+rotateLeft(d+FunI(a,b,c)+X[3]+T[53],10);
        c=d+rotateLeft(c+FunI(d,a,b)+X[10]+T[54],15);
        b=c+rotateLeft(b+FunI(c,d,a)+X[1]+T[55],21);
  
        a=b+rotateLeft(a+FunI(b,c,d)+X[8]+T[56],6);
        d=a+rotateLeft(d+FunI(a,b,c)+X[15]+T[57],10);
        c=d+rotateLeft(c+FunI(d,a,b)+X[6]+T[58],15);
        b=c+rotateLeft(b+FunI(c,d,a)+X[13]+T[59],21);

        a=b+rotateLeft(a+FunI(b,c,d)+X[4]+T[60],6);
        d=a+rotateLeft(d+FunI(a,b,c)+X[11]+T[61],10);
        c=d+rotateLeft(c+FunI(d,a,b)+X[2]+T[62],15);
        b=c+rotateLeft(b+FunI(c,d,a)+X[9]+T[63],21);
        
        //теперь складываем к текущим abcd результаты предыдущего блока (или инициализирующие значения, если это был первый блок)
        a=a+a0;b=b+b0;c=c+c0;d=d+d0;
        a0=a;b0=b;c0=c;d0=d;//запоминаем текущие значения для сложения с последующими
    }
    a0=b0=c0=d0=0;//изначально abcd при выводе будут "перевернуты" (будет обратный порядок байтов), их нужно развернуть, записав в обратном порядке
    
    a0=a0|(((a)&0x000000ff)<<24);
    b0=b0|(((b)&0x000000ff)<<24);
    c0=c0|(((c)&0x000000ff)<<24);
    d0=d0|(((d)&0x000000ff)<<24);
    a0=a0|(((a)&0x0000ff00)<<8);
    b0=b0|(((b)&0x0000ff00)<<8);
    c0=c0|(((c)&0x0000ff00)<<8);
    d0=d0|(((d)&0x0000ff00)<<8);
    a0=a0|(((a)&0x00ff0000)>>8);
    b0=b0|(((b)&0x00ff0000)>>8);
    c0=c0|(((c)&0x00ff0000)>>8);
    d0=d0|(((d)&0x00ff0000)>>8);
    a0=a0|(((a)&0xff000000)>>24);
    b0=b0|(((b)&0xff000000)>>24);
    c0=c0|(((c)&0xff000000)>>24);
    d0=d0|(((d)&0xff000000)>>24);
    cout<<"MD5: "<<hex<<setfill('0')<<setw(8)<<a0<<b0<<c0<<d0<<endl;
    md5file<<hex<<setfill('0')<<setw(8)<<a0<<b0<<c0<<d0<<endl;
    cout<<dec;//возвращаемся к десятичной системе
    file.close();
    md5file.close();
    return true;
}
long long getSizeFile(char*filename){//функция для выяснения размера в байтах
    ifstream file(filename, ios::binary);
    file.seekg(0,file.end);//перемещаем файловый указатель в конец файла
    long long res = file.tellg();//узнаем размер
    file.seekg(0,file.beg);//снова возвращаемся  в начало
    file.close();
    return res;
}
void printBinaryFile(char*filename){//печать бинарного файла в виде последовательности битов
    ifstream file(filename, ios::binary);
    short byte;
    while(file.read((char *)&byte,sizeof(short))){
        cout<<bitset<8>(byte)<<" ";
    }
    file.close();
}
