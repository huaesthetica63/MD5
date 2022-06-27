#include "MD5.h"
using namespace std;
int main() {
    srand(time(0));//инициализация рандомайзера
    long long sizeFile = getSizeFile((char*)"sample.pdf");//узнаем размер файла в байтах
    if(sizeFile==-1){
        cout<<"Error!"<<endl;
        return -1;
    }
    cout<<"Size of file: "<<sizeFile<<" bytes"<<endl;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    bool resOperation = MD5((char*)"sample.pdf", (char*)"MD5.txt");//вызов хэш-функции замеряем по времени выполнения
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    if(resOperation)
        cout << "Time of MD5-hash creating: " << chrono::duration_cast<chrono::milliseconds>(end - begin).count() << " milliseconds" << endl;
    else{
        cout<<"Error!"<<endl;
        return -1;
    }
    return 0;
}
