#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric>
#include "BFS.h"
#include "DFS.h"
#include "AStar.h"
#include <chrono>

using namespace std;
using namespace sf;

const int ROWS = 302;
const int COLS = 32;
const float CELL_SIZE = 23.0f;

class UnionFind {
private:
    vector<int> parent;
public:
    UnionFind(int n) {
        parent.resize(n);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) parent[root_i] = root_j;
    }
};

struct Cell {
    int x, y;
    bool walls[4] = { true, true, true, true };
    bool visitedBySolver = false;
    int parentIndex = -1;
    bool isPath = false;
    bool isStart = false;
    bool isEnd = false;

    void draw(sf::RenderWindow& window) {
        float px = x * CELL_SIZE;
        float py = y * CELL_SIZE;
        RectangleShape bg(Vector2f(CELL_SIZE, CELL_SIZE));
        bg.setPosition(px, py);

        if (isStart) bg.setFillColor(Color::Blue);
        else if (isEnd) bg.setFillColor(Color::Red);
        else if (isPath) bg.setFillColor(Color::Green);
        else if (visitedBySolver) bg.setFillColor(Color(200, 160, 255, 100));
        else bg.setFillColor(Color::Black);

        window.draw(bg);

        Color wallColor = Color::White;
        float thickness = 2.0f;
        if (walls[0]) { RectangleShape line(Vector2f(CELL_SIZE, thickness)); line.setPosition(px, py); line.setFillColor(wallColor); window.draw(line); }
        if (walls[1]) { RectangleShape line(Vector2f(thickness, CELL_SIZE)); line.setPosition(px + CELL_SIZE - thickness, py); line.setFillColor(wallColor); window.draw(line); }
        if (walls[2]) { RectangleShape line(Vector2f(CELL_SIZE, thickness)); line.setPosition(px, py + CELL_SIZE - thickness); line.setFillColor(wallColor); window.draw(line); }
        if (walls[3]) { RectangleShape line(Vector2f(thickness, CELL_SIZE)); line.setPosition(px, py); line.setFillColor(wallColor); window.draw(line); }
    }

    void resetSolver() {
        visitedBySolver = false;
        isPath = false;
        parentIndex = -1;
    }
};

struct Edge { 
    int u, v, wallU, wallV; 
};

class Buton {
public:
    RectangleShape sekil;
    Text yazi;
    Buton(float x, float y, float w, float h, Font& font, string metin, Color renk) {
        sekil.setPosition(x, y);
        sekil.setSize(Vector2f(w, h));
        sekil.setFillColor(renk);
        yazi.setFont(font);
        yazi.setString(metin);
        yazi.setCharacterSize(18);
        yazi.setFillColor(Color::White);
        FloatRect yaziKutusu = yazi.getLocalBounds();
        yazi.setOrigin(yaziKutusu.left + yaziKutusu.width / 2.0f, yaziKutusu.top + yaziKutusu.height / 2.0f);
        yazi.setPosition(x + w / 2.0f, y + h / 2.0f);
    }
    void ciz(RenderWindow& pencere) { pencere.draw(sekil); pencere.draw(yazi); }
    bool icindeMi(Vector2f fareKonumu) { return sekil.getGlobalBounds().contains(fareKonumu); }
};

int main() {
    RenderWindow window(VideoMode(COLS * CELL_SIZE, (ROWS * CELL_SIZE + 100)), "Maze Engine");
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("C:/Users/samet/Desktop/MazeEngineAnimasyon/src/fonts/Quantico-Bold.ttf")) {
        cout << "Font bulunamadi!" << endl;
        return -1;
    }

    Buton dfsSolveButton(5, 740, 115, 50, font, "DFS Coz", Color::Red);
    Buton bfsSolveButton(130, 740, 115, 50, font, "BFS Coz", Color::Green);
    Buton aSolveButton(255, 740, 115, 50, font, "A* Coz", Color::Blue);
    Buton clearButton(380, 740, 115, 50, font, "Temizle", Color(100, 100, 100));

    Text sureText("Gecen Sure: 0 ms", font, 18);
    sureText.setFillColor(Color::White);
    sureText.setPosition(10, 800);

    vector<Cell> grid;
    for (int y = 0; y < ROWS; ++y) for (int x = 0; x < COLS; ++x) grid.push_back({ x, y });

    vector<Edge> edges;
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            int current = y * COLS + x;
            if (x < COLS - 1) edges.push_back({ current, current + 1, 1, 3 });
            if (y < ROWS - 1) edges.push_back({ current, current + COLS, 2, 0 });
        }
    }
    shuffle(edges.begin(), edges.end(), mt19937(random_device()()));
    UnionFind uf(ROWS * COLS);
    for (const auto& edge : edges) {
        if (uf.find(edge.u) != uf.find(edge.v)) {
            uf.unite(edge.u, edge.v);
            grid[edge.u].walls[edge.wallU] = false;
            grid[edge.v].walls[edge.wallV] = false;
        }
    }

    grid[0].isStart = true;
    grid.back().isEnd = true;

    BFS bfsSolver; DFS dfsSolver; AStar aStarSolver;

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mPos((float)Mouse::getPosition(window).x, (float)Mouse::getPosition(window).y);

                if (clearButton.icindeMi(mPos)) {
                    for (auto& cell : grid) cell.resetSolver();
                    sureText.setString("Sure: Sifirlandi");
                }
                else if (dfsSolveButton.icindeMi(mPos)) {
                    for (auto& cell : grid) cell.resetSolver();
                    auto start = chrono::high_resolution_clock::now();
                    dfsSolver.solve(grid, ROWS, COLS, 0, (int)grid.size() - 1, window,false);
                    auto end = chrono::high_resolution_clock::now();
                    sureText.setString("DFS Sure: (" + to_string(chrono::duration_cast<chrono::microseconds>(end-start).count()) + "mikro saniye)");
                    for (auto& cell : grid) cell.resetSolver();
                    dfsSolver.solve(grid, COLS, ROWS, 0, (int)grid.size() - 1, window, true);
                }
                else if (bfsSolveButton.icindeMi(mPos)) {
                    for (auto& cell : grid) cell.resetSolver();
                    auto start = chrono::high_resolution_clock::now();
                    bfsSolver.solve(grid, ROWS, COLS, 0, (int)grid.size() - 1, window, false);
                    auto end = chrono::high_resolution_clock::now();
                    sureText.setString("BFS Sure: (" + to_string(chrono::duration_cast<chrono::microseconds>(end-start).count()) + "mikro saniye)");
                    for (auto& cell : grid) cell.resetSolver();
                    bfsSolver.solve(grid, COLS, ROWS, 0, (int)grid.size() - 1, window, true);
                }
                else if (aSolveButton.icindeMi(mPos)) {
                    for (auto& cell : grid) cell.resetSolver();
                    auto start = chrono::high_resolution_clock::now();
                    aStarSolver.solve(grid, COLS, ROWS, 0, (int)grid.size() - 1, window, false);
                    auto end = chrono::high_resolution_clock::now();
                    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
                    sureText.setString("A* Sure: (" + to_string(chrono::duration_cast<chrono::microseconds>(end-start).count()) + "mikro saniye)");
                    for(auto& cell : grid) cell.resetSolver();
                    aStarSolver.solve(grid, COLS, ROWS, 0, (int)grid.size() - 1, window, true);
                }
            }
        }

        window.clear(Color::Black);
        for (Cell& cell : grid) cell.draw(window);
        dfsSolveButton.ciz(window);
        bfsSolveButton.ciz(window);
        aSolveButton.ciz(window);
        clearButton.ciz(window);
        window.draw(sureText);
        window.display();
    }
    return 0;
}