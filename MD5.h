#ifndef MD5_h
#define MD5_h
#include <iostream>
#include <fstream>
#include <chrono>//замер времени исполнения кода
#include <vector>
#include <cmath>
#define MD5_DIGEST_BYTES 16//размер выходного хэша
#define MD5_DIGEST_BITS 128
#define MD5_MOD_CONSTANT 512 //размер исходного текста в битах нужно сделать кратным этому числу путем расширения
long long getSizeFile(char*filename);//получение размера файла (в байтах)
bool MD5(char*filename1,char*filename2);//сама функция получения md5-хэша
void printBinaryFile(char*filename);//печать бинарного файла в виде нулей и единиц (поможет для отладки)
std::vector<uint32_t> initT();//инициализация T-вектора
const uint32_t A = 0x67452301;//четыре числа для хранения состояния md5-хэша
const uint32_t B = 0xEFCDAB89;
const uint32_t C = 0x98BADCFE;
const uint32_t D = 0x10325476;
uint32_t FunF(uint32_t x, uint32_t y, uint32_t z);//функции для 1, 2, и 4 этапа соответственно
uint32_t FunG(uint32_t x, uint32_t y, uint32_t z);
uint32_t FunH(uint32_t x, uint32_t y, uint32_t z);
uint32_t FunI(uint32_t x, uint32_t y, uint32_t z);
uint32_t rotateLeft(uint32_t bt, int c);//функция  для "разворота" последовательности бит
#endif
