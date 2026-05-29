#include "tgaimage.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

constexpr TGAColor white = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green = {0, 255, 0, 255};
constexpr TGAColor red = {0, 0, 255, 255};
constexpr TGAColor blue = {255, 128, 64, 255};
constexpr TGAColor yellow = {0, 200, 255, 255};

void cacheVerticesFromFile(std::string fileName,
                           std::vector<std::array<float, 3>> &vertices) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    std::cerr << "Error Occurred: Could not open file:  " << fileName << "  ."
              << std::endl;
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.size() >= 2 && line[0] == 'v' && line[1] == ' ') {
      std::array<float, 3> temp;
      int lastCharacterIndex = 0;

      for (int i = 0; i <= 2; i++) {
        std::string vertex;
        // Get the char right after each space
        int vertexCharacterIndex = line.find(' ', lastCharacterIndex) + 1;

        // Loop until we reach the ' '
        while (vertexCharacterIndex < line.size() &&
               line[vertexCharacterIndex] != ' ') {
          vertex += line[vertexCharacterIndex];
          vertexCharacterIndex++;
        }

        temp[i] = std::stof(vertex);
        lastCharacterIndex = vertexCharacterIndex;
      }

      vertices.push_back(temp);
    }
  }
}

void cacheFacesFromFile(std::string fileName,
                        std::vector<std::array<int, 3>> &faces) {
  std::ifstream file(fileName);

  if (!file.is_open()) {
    std::cerr << "Error Occurred: Could not open file:  " << fileName << "  ."
              << std::endl;
  }

  std::string line;
  while (std::getline(file, line)) {
    // Populate faces vector
    // See if the current line is face data
    if (line[0] == 'f') {
      std::array<int, 3> temp;
      int lastCharacterIndex = 0;

      for (int i = 0; i <= 2; i++) {
        std::string vertex;
        // Get the char right after each space
        int vertexCharacterIndex = line.find(' ', lastCharacterIndex) + 1;

        // Loop until we reach the '/'
        while (vertexCharacterIndex < line.size() &&
               line[vertexCharacterIndex] != '/') {
          vertex += line[vertexCharacterIndex];
          vertexCharacterIndex++;
        }

        temp[i] = std::stoi(vertex) -
                  1; // Decrement by 1 since the indices start at 1
        lastCharacterIndex = vertexCharacterIndex;
      }

      faces.push_back(temp);
    }
  }
  // for (const std::array<int, 3> a : faces) {
  //   std::cout << "–––––––––––––––––" << std::endl;
  //   for (const int b : a) {
  //     std::cout << b << std::endl;
  //   }
  //   std::cout << "–––––––––––––––––" << std::endl;
  // }
}

void drawLine(int ax, int ay, int bx, int by, TGAImage &framebuffer,
              TGAColor color) {
  bool steep = std::abs(ax - bx) < std::abs(ay - by);
  if (steep) { // if the line is steep, we transpose the image
    std::swap(ax, ay);
    std::swap(bx, by);
  }
  if (ax > bx) { // make it left−to−right
    std::swap(ax, bx);
    std::swap(ay, by);
  }
  int y = ay;
  int ierror = 0;
  for (int x = ax; x <= bx; x++) {
    if (steep) // if transposed, de−transpose
      framebuffer.set(y, x, color);
    else
      framebuffer.set(x, y, color);
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * (bx - ax) * (ierror > bx - ax);
  }
}

void drawTriangle(int vertexA, int vertexB, int vertexC) {}

int main(int argc, char **argv) {
  constexpr int width = 64;
  constexpr int height = 64;
  TGAImage framebuffer(width, height, TGAImage::RGB);

  std::vector<std::array<float, 3>> vertices;
  cacheVerticesFromFile("./obj/diablo3_pose/diablo3_pose.obj", vertices);

  for (const std::array<float, 3> a : vertices) {
    std::cout << "–––––––––––––––––" << std::endl;
    for (const float b : a) {
      std::cout << b << std::endl;
    }
    std::cout << "–––––––––––––––––" << std::endl;
  }

  std::vector<std::array<int, 3>> faces;
  cacheFacesFromFile("./obj/diablo3_pose/diablo3_pose.obj", faces);

  framebuffer.write_tga_file("framebuffer.tga");
  return 0;
}
