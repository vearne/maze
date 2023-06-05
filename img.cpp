#include <iostream>
#include <fstream>

using namespace std;

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t signature;     // 文件标识符 "BM"
    uint32_t fileSize;      // 文件大小
    uint16_t reserved1;     // 保留字段
    uint16_t reserved2;     // 保留字段
    uint32_t dataOffset;    // 数据偏移量
    uint32_t headerSize;    // 位图信息头大小
    int32_t width;         // 图像宽度
    int32_t height;        // 图像高度
    uint16_t planes;        // 颜色平面数
    uint16_t bitDepth;      // 位深度
    uint32_t compression;   // 压缩类型
    uint32_t dataSize;      // 图像数据大小
    int32_t hResolution;   // 水平分辨率
    int32_t vResolution;   // 垂直分辨率
    uint32_t colors;        // 调色板颜色数
    uint32_t importantColors;// 重要颜色数
};
#pragma pack(pop)


void createBMP(const char *filename, int width, int height, unsigned char *imageData) {
    std::ofstream file(filename, std::ios::out | std::ios::binary);

    if (!file) {
        std::cerr << "无法创建文件: " << filename << std::endl;
        return;
    }

    BMPHeader header;
    header.signature = 0x4D42;  // "BM"
    header.fileSize = sizeof(BMPHeader) + width * height * 3;
    header.reserved1 = 0;
    header.reserved2 = 0;
    header.dataOffset = sizeof(BMPHeader);
    header.headerSize = 40;
    header.width = width;
    header.height = height;
    header.planes = 1;
    header.bitDepth = 24;
    header.compression = 0;
    header.dataSize = width * height * 3;
    header.hResolution = 0;
    header.vResolution = 0;
    header.colors = 0;
    header.importantColors = 0;

    file.write(reinterpret_cast<char *>(&header), sizeof(BMPHeader));
    file.write(reinterpret_cast<char *>(imageData), width * height * 3);

    file.close();
}

void matrix2BMP(int **matrix, int rows, int cols, int BlockSize, const char *filename) {
    int width = cols * BlockSize;
    int height = rows * BlockSize;
    unsigned char *imageData = new unsigned char[width * height * 3];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            for (int k = 0; k < 3; ++k) {
                // 可以根据需要进行操作，如打印、修改等
                if (matrix[i / BlockSize][j / BlockSize] == 0) {
                    imageData[(i * width + j) * 3 + k] = 255;
                } else if (matrix[i / BlockSize][j / BlockSize] == 1){
                    imageData[(i * width + j) * 3 + k] = 105;
                }else if (matrix[i / BlockSize][j / BlockSize] == 2){
                    imageData[(i * width + j) * 3 + k] = 0;
                }
            }
        }
    }
    createBMP(filename, width, height, imageData);
}

