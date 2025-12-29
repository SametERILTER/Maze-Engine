#ifndef MAZEENGINE_ASTAR_H
#define MAZEENGINE_ASTAR_H

#include <vector>
#include <queue>
#include <cmath>
#include <limits>
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

struct Node {
    int index;
    float fCost;
    bool operator>(const Node& other) const { return fCost > other.fCost; }
};

class AStar {
public:
    float getHeuristic(int idx1, int idx2, int cols) {
        int x1 = idx1 % cols; int y1 = idx1 / cols;
        int x2 = idx2 % cols; int y2 = idx2 / cols;
        return (float)(abs(x1 - x2) + abs(y1 - y2));
    }

    template<typename T>
    void solve(vector<T>& grid, int cols, int rows, int startIndex, int endIndex, RenderWindow& window, bool visualize = true) {
        priority_queue<Node, vector<Node>, greater<Node>> pq;
        vector<float> gCost(grid.size(), numeric_limits<float>::max());

        gCost[startIndex] = 0;
        pq.push({ startIndex, getHeuristic(startIndex, endIndex, cols) });
        grid[startIndex].visitedBySolver = true;
        grid[startIndex].parentIndex = -1;

        bool found = false;

        while (!pq.empty()) {
            if (visualize) {
                if (!window.isOpen()) break;
                window.clear(Color::Black);
                for (auto& cell : grid) cell.draw(window);
                window.display();
                sleep(milliseconds(5));
            }
            int currentIdx = pq.top().index;
            pq.pop();

            if (currentIdx == endIndex) { found = true; break; }

            for (int i = 0; i < 4; i++) {
                if (grid[currentIdx].walls[i]) continue;

                int neighbor = -1;
                if (i == 0 && currentIdx >= cols) neighbor = currentIdx - cols;
                else if (i == 1 && (currentIdx % cols) != cols - 1) neighbor = currentIdx + 1;
                else if (i == 2 && currentIdx < (rows - 1) * cols) neighbor = currentIdx + cols;
                else if (i == 3 && (currentIdx % cols) != 0) neighbor = currentIdx - 1;

                if (neighbor != -1) {
                    float tentativeGCost = gCost[currentIdx] + 1;
                    if (tentativeGCost < gCost[neighbor]) {
                        gCost[neighbor] = tentativeGCost;
                        grid[neighbor].parentIndex = currentIdx;
                        grid[neighbor].visitedBySolver = true;
                        float fValue = tentativeGCost + getHeuristic(neighbor, endIndex, cols);
                        pq.push({ neighbor, fValue });
                    }
                }
            }
        }

        if (found) {
            int current = endIndex;
            while (current != -1 && current != startIndex) {
                grid[current].isPath = true;
                current = grid[current].parentIndex;

                if (visualize) {
                    window.clear(sf::Color::Black);
                    for (auto& cell : grid) cell.draw(window);
                    window.display();
                    sleep(milliseconds(10));
                }
            }
            grid[startIndex].isPath = true;
        }
    }
};
#endif