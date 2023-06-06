#include <iostream>
#include <stack>
#include <unordered_set>
#include <unordered_map>
#include <random>

using namespace std;

#include "img.h"

void showMatrix(int **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

class Maze {
private:
    int n;
    int **matrix;
    vector<int> edges;
    // nodeID -> nodeSet
    unordered_map<int, unordered_set<int>> nodeSetMap;
    // nodeID -> node所属集合ID
    unordered_map<int, int> nodeIDMap;
public:
    // 宽和高相同，取奇数
    /*
     *  假定
     *  (0, 0) 是迷宫的入口
     *  (n-1, n-1) 是迷宫的出口
     */
    Maze(int n) {
        this->n = n;
        this->matrix = new int *[n];
        for (int i = 0; i < n; i++) {
            matrix[i] = new int[n];
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // 0: 表示可以移动的区域
                // 1: 表示墙体
                if (i % 2 == 0) {
                    if (j % 2 == 0) {
                        matrix[i][j] = 0;
                        unordered_set<int> nodeSet;
                        nodeSet.insert(i * n + j);
                        nodeIDMap[i * n + j] = i * n + j;
                        nodeSetMap[i * n + j] = nodeSet;
                    } else {
                        matrix[i][j] = 1;
                        edges.push_back(i * n + j);
                    }
                } else {
                    matrix[i][j] = 1;
                    edges.push_back(i * n + j);
                }
            }
        }
    }

    bool merge(int node1, int node2) {
//        cout<<"merge"<<" "<<node1<<" "<<node2<<endl;
        // node1 和 node2 在一个集合中
        if (nodeIDMap[node1] == nodeIDMap[node2]) {
            return false;
        }
        // node1所属的集合ID有最小编号
        if (nodeIDMap[node1] > nodeIDMap[node2]) {
            int tmp = node1;
            node1 = node2;
            node2 = tmp;
        }

        unordered_set<int> node1Set = nodeSetMap[nodeIDMap[node1]];
        unordered_set<int> node2Set = nodeSetMap[nodeIDMap[node2]];
        node1Set.insert(node2Set.begin(), node2Set.end());
        for (int node:node2Set) {
            nodeIDMap[node] = nodeIDMap[node1];
        }
        nodeSetMap[nodeIDMap[node1]] = node1Set;
//        nodeSetMap.erase(nodeIDMap[node2]);
        return true;
    }

    void create() {
        int n = this->n;
        // 出口
        // 入口
        int entry = 0;
        int exit = (n - 1) * (n + 1);
        cout << "------------------2-------------" << endl;
        while (nodeIDMap[entry] != nodeIDMap[exit]) {
            cout << "############################" << endl;
            int idx = rand() % edges.size();
            int edge = edges[idx];
            edges.erase(edges.begin() + idx);
            // 尝试打破这个围墙
            int node = edge;
            nodeIDMap[node] = node;
            unordered_set<int> nodeSet;
            nodeSet.insert(node);
            nodeSetMap[node] = nodeSet;
            // 看看这个墙相邻的节点，是否属于不同的集合
            int x = edge / n;
            int y = edge - n * x;
            matrix[x][y] = 0;
            cout << "x=" << x << ",y=" << y << endl;
            this->show();
            int leftNode = (x - 1) * n + y;
            bool flag = false;
            if (x - 1 >= 0 && matrix[x - 1][y] == 0) {
                flag |= merge(node, leftNode);
            }
            int upNode = x * n + y + 1;
            if (y + 1 < n && matrix[x][y + 1] == 0) {
                flag |= merge(node, upNode);
            }
            int rightNode = (x + 1) * n + y;
            if (x + 1 < n && matrix[x + 1][y] == 0) {
                flag |= merge(node, rightNode);
            }
            int downNode = x * n + y - 1;
            if (y - 1 >= 0 && matrix[x][y - 1] == 0) {
                flag |= merge(node, downNode);
            }
            if (!flag) {
                matrix[x][y] = 1;
            }
        }
    }

    void drawPic(const char *filename) {
        int N = n + 2;
        int **image;
        image = new int *[N];
        for (int i = 0; i < N; i++) {
            image[i] = new int[N];
            for (int j = 0; j < N; j++) {
                image[i][j] = 2;
            }
        }

        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                image[i][j] = matrix[i - 1][j - 1];
            }
        }
        image[1][1] = 3;
        image[n][n] = 3;

        showMatrix(image, N, N);
        matrix2BMP(image, N, N, 40, filename);
    }

    void show() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0) {
                    cout << '0';
                } else {
                    cout << 'X';
                }
            }
            cout << endl;
        }
    }
};


int main() {
    srand(time(0));
    // n 必须是奇数
    Maze m(31);
    m.create();
//    m.show();
    m.drawPic("/tmp/maze.bmp");
}


