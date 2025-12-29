#ifndef MAZEENGINE_BFS_H
#define MAZEENGINE_BFS_H

#include <iostream>
#include <queue>
#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <SFML/System/Sleep.hpp>
using namespace std;
using namespace sf;

class BFS {
public:

    template<typename T> //main.cpp deki cell yapısını derleme anında alıyoruz
    void solve(vector<T>& grid,int rows,int cols,int startIndex,int endIndex,RenderWindow& window, bool visualize = true) {
        queue<int> q;
        q.push(startIndex);

        grid[startIndex].parentIndex = -1;
        grid[startIndex].visitedBySolver=true;

        bool found=false;

        while(!q.empty()) {
            if (visualize) {
                if (!window.isOpen())
                    break;
                window.clear(Color::Black);
                for(auto& cell : grid) cell.draw(window);
                window.display();
                sleep(milliseconds(1));
            }
            int index = q.front();
            q.pop();

            if (index==endIndex) {
                found=true;
                break;
            }

            //0 üst, 1sağ, 2 alt, 3 sol duvar

            //yukarı kontrolü (index-cols)
            if (index >= cols && !grid[index].walls[0]) {
                int neighbor = index - cols;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    q.push(neighbor);
                }
            }

            //sağ kontrolü (index+1)
            if ((index%cols)!=cols-1 && !grid[index].walls[1]) {
                int neighbor = index+1;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    q.push(neighbor);
                }
            }

            //alt kontrolü (index+cols)
            if (index < (rows-1)*cols && !grid[index].walls[2]) {
                int neighbor = index + cols;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    q.push(neighbor);
                }
            }

            //sol kontrolü (index-1)
            if ((index%cols)!=0 && !grid[index].walls[3]) {
                int neighbor = index - 1;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    q.push(neighbor);
                }
            }
        }
        //geri yol çizme
        if (found) {
            int current=endIndex;
            while (current!=startIndex && current!=-1) {
                grid[current].isPath=true;
                current=grid[current].parentIndex;
                if (visualize) {
                    window.clear(Color::Black);
                    for(auto& cell : grid) cell.draw(window);
                    window.display();
                    sleep(milliseconds(2));
                }
            }
            grid[startIndex].isPath=true;
            cout << "Yol bulundu ve çizildi!!" << endl;
        }else {
            cout << "Yol bulunamadi!" << endl;
        }
    }
 };


#endif //MAZEENGINE_BFS_H