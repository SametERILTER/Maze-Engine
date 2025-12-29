#include <iostream>
#include <vector>
#include <stack>
#include <thread>
#include <SFML/Graphics.hpp>

#ifndef MAZEENGINE_DFS_H
#define MAZEENGINE_DFS_H

using namespace std;
using namespace sf;

class DFS {
    public:

    template<typename T> //main.cpp deki cell yapısını derleme anında alıyoruz
 void solve(vector<T>& grid,int rows,int cols,int startIndex,int endIndex,RenderWindow& window,bool visualize = true) {
        stack<int> s;
        s.push(startIndex);

        grid[startIndex].parentIndex = -1;
        grid[startIndex].visitedBySolver=true;

        bool found=false;

        while(!s.empty()) {
            if (visualize) {
                if (!window.isOpen())
                    break;
                window.clear(Color::Black);
                for(auto& cell : grid) cell.draw(window);
                window.display();
                sleep(milliseconds(5));
            }
            int index=s.top();
            s.pop();

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
                    s.push(neighbor);
                }
            }

            //sağ kontrolü (index+1)
            if ((index%cols)!=cols-1 && !grid[index].walls[1]) {
                int neighbor = index+1;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    s.push(neighbor);
                }
            }

            //alt kontrolü (index+cols)
            if (index < (rows-1)*cols && !grid[index].walls[2]) {
                int neighbor = index + cols;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    s.push(neighbor);
                }
            }

            //sol kontrolü (index-1)
            if ((index%cols)!=0 && !grid[index].walls[3]) {
                int neighbor = index - 1;
                if (!grid[neighbor].visitedBySolver) {
                    grid[neighbor].visitedBySolver=true;
                    grid[neighbor].parentIndex = index;
                    s.push(neighbor);
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
                    sleep(milliseconds(10));
                }
            }
            grid[startIndex].isPath=true;
            cout << "Yol bulundu ve cizildi!!" << endl;
        }else {
            cout << "Yol bulunamadi!" << endl;
        }
    }
 };


#endif //MAZEENGINE_DFS_H