#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <fstream>

#define SECOND_TASK

using namespace std;

inline int str_to_int(std::string s){
    std::stringstream ss;
    ss << s;
    int i;
    ss >> i;
    return i;
}

struct Point{
    int x, y, manhattan, wire1_cost, wire2_cost;
    Point(){};
    Point(int a, int b, int c) : x(a), y(b), manhattan(abs(a)+abs(b)), wire1_cost(0), wire2_cost(c){}
};
#ifndef SECOND_TASK
    bool operator<(const Point& a, const Point& b){return a.manhattan < b.manhattan;}
#else
    bool operator<(const Point& a, const Point& b){return (a.wire1_cost+a.wire2_cost) < (b.wire1_cost + b.wire2_cost);}
#endif

bool operator==(const Point& a, const Point& b){return a.x == b.x && a.y == b.y;}

std::vector<std::string> parse_input(std::string input){
    std::vector<std::string> out(std::count(input.begin(), input.end(), ',')+1,  "");
    size_t idx = input.find(',');
    for(int i = 0; idx != input.npos; ++i){
        out[i] = input.substr(0, idx);
        input.erase(0, idx+1);
        idx = input.find(',');
    }
    out[out.size()-1] = input;
    return out;

}

void construct_wire(std::map<int,std::map<int, int>>& horizontals,
                    std::map<int,std::map<int,int>>& verticals,
                    std::string input){
    std::vector<std::string> inst = parse_input(input);
    int curr_x = 0, curr_y = 0;
    for(size_t i = 0; i < inst.size(); ++i){
        int val = str_to_int(inst[i].substr(1));
        switch(inst[i][0]){
        case 'R':
            horizontals[curr_y].emplace(curr_x, curr_x+val);
            curr_x += val;
            break;
        case 'L':
            horizontals[curr_y].emplace(curr_x-val, curr_x);
            curr_x -= val;
            break;
        case 'U':
            verticals[curr_x].emplace(curr_y, curr_y+val);
            curr_y += val;
            break;
        case 'D':
            verticals[curr_x].emplace(curr_y-val, curr_y);
            curr_y -= val;
            break;
        }
    }
}

std::set<Point> get_crossovers(std::map<int,std::map<int,int>>& horizontals,
                    std::map<int,std::map<int,int>>& verticals,
                    std::string input){
    std::set<Point> crossovers;
    std::vector<std::string> inst = parse_input(input);
    int curr_x = 0, curr_y = 0, step_counter = 0;
    for(size_t i = 0; i < inst.size(); ++i){
        int val = str_to_int(inst[i].substr(1));
        switch(inst[i][0]){
        case 'R':
            for(int j = curr_x; j < curr_x+val; ++j){
                if(verticals.find(j) != verticals.end()){
                    map<int, int>::iterator first_find = verticals.at(j).upper_bound(curr_y);
                    if (first_find == verticals.at(j).begin()){++step_counter; continue;}
                    int stop = std::prev(first_find)->second;
                    if(stop >= curr_y) crossovers.insert(Point(j, curr_y, step_counter));
                }
                ++step_counter;
            }
            curr_x += val;
            break;
        case 'L':
            for(int j = curr_x; j > curr_x-val; --j){
                if(verticals.find(j) != verticals.end()){
                    map<int, int>::iterator first_find = verticals.at(j).upper_bound(curr_y);
                    if (first_find == verticals.at(j).begin()){++step_counter; continue;}
                    int stop = std::prev(first_find)->second;
                    if(stop >= curr_y) crossovers.insert(Point(j, curr_y, step_counter));
                }
                ++step_counter;
            }
            curr_x -= val;
            break;
        case 'U':
            for(int j = curr_y; j < curr_y+val; ++j){
                if(horizontals.find(j) != horizontals.end()){
                    map<int, int>::iterator first_find = horizontals.at(j).upper_bound(curr_x);
                    if (first_find == horizontals.at(j).begin()){++step_counter; continue;}
                    int stop = std::prev(first_find)->second;
                    if(stop >= curr_x) crossovers.insert(Point(curr_x, j, step_counter));
                }
                ++step_counter;
            }
            curr_y += val;
            break;
        case 'D':
            for(int j = curr_y; j > curr_y-val; --j){
                if(horizontals.find(j) != horizontals.end()){
                    map<int, int>::iterator first_find = horizontals.at(j).upper_bound(curr_x);
                    if (first_find == horizontals.at(j).begin()){++step_counter; continue;}
                    int stop = std::prev(first_find)->second;
                    if(stop >= curr_x) crossovers.insert(Point(curr_x, j, step_counter));
                }
                ++step_counter;
            }
            curr_y -= val;
            break;
        }
    }
    return crossovers;
}


inline std::map<int, std::map<int, Point>> set_to_map(std::set<Point> crosses){
    std::map<int, std::map<int, Point>> ordered;
    for(Point p : crosses){
        ordered[p.x][p.y] = p;
    }
    for( auto m : ordered){
        std::cout << m.first << ": ";
        for(auto p : m.second){
            std::cout << p.first << ' ';
        }
        std::cout << std::endl;
    }
    return ordered;
}

std::set<Point> add_wire_cost(std::map<int,std::map<int,int>>& horizontals,
                    std::map<int,std::map<int,int>>& verticals,
                    std::set<Point> crossovers,
                    std::string input){
    std::set<Point> updated;
    std::map<int, std::map<int, Point>> ordered = set_to_map(crossovers);
    std::vector<std::string> inst = parse_input(input);
    int curr_x = 0, curr_y = 0, step_counter = 0;
    for(size_t i = 0; i < inst.size(); ++i){
        int val = str_to_int(inst[i].substr(1));
        switch(inst[i][0]){
        case 'R':
            for(int j = curr_x; j < curr_x+val; ++j){
                if(ordered.find(j) != ordered.end()){
                    if(ordered.at(j).find(curr_y) != ordered.at(j).end()){
                        Point p = ordered.at(j).at(curr_y);
                        if (p.wire1_cost != 0){++step_counter; continue;}
                        p.wire1_cost = step_counter;
                        updated.insert(p);
                    }
                }
                ++step_counter;
            }
            curr_x += val;
            break;
        case 'L':
            for(int j = curr_x; j > curr_x-val; --j){
                if(ordered.find(j) != ordered.end()){
                    if(ordered.at(j).find(curr_y) != ordered.at(j).end()){
                        Point p = ordered.at(j).at(curr_y);
                        if (p.wire1_cost != 0){++step_counter; continue;}
                        p.wire1_cost = step_counter;
                        updated.insert(p);
                    }
                }
                ++step_counter;
            }
            curr_x -= val;
            break;
        case 'U':
            for(int j = curr_y; j < curr_y+val; ++j){
                if(ordered.find(curr_x) == ordered.end()){++step_counter; continue;}
                if(ordered.at(curr_x).find(j) != ordered.at(curr_x).end()){
                    Point p = ordered.at(curr_x).at(j);
                    if (p.wire1_cost != 0){++step_counter; continue;}
                    p.wire1_cost = step_counter;
                    updated.insert(p);
                }
                ++step_counter;
            }
            curr_y += val;
            break;
        case 'D':
            for(int j = curr_y; j > curr_y-val; --j){
                if(ordered.find(curr_x) == ordered.end()){++step_counter; continue;}
                if(ordered.at(curr_x).find(j) != ordered.at(curr_x).end()){
                    Point p = ordered.at(curr_x).at(j);
                    if (p.wire1_cost != 0){++step_counter; continue;}
                    p.wire1_cost = step_counter;
                    updated.insert(p);
                }
                ++step_counter;
            }
            curr_y -= val;
            break;
        }
    }
    return updated;
}

int main()
{
    std::string in1, in2;
    {
        std::ifstream fin1("Wire1.txt");
        std::ifstream fin2("Wire2.txt");
        if(!fin1.good() || !fin2.good()) exit(-1);
        std::getline(fin1, in1);
        std::getline(fin2, in2);
    }
    std::map<int,std::map<int,int>> horizontals, verticals;
    construct_wire(horizontals, verticals, in1);
    std::set<Point> crosses = get_crossovers(horizontals, verticals, in2);
    #ifndef SECOND_TASK
    for(Point p : crosses){
        std::cout << "Crossing: " << p.x << ' ' << p.y << ' ' << p.manhattan << std::endl;
    }
    #else
    std::set<Point> updated = add_wire_cost(horizontals, verticals, crosses, in1);
    for(Point p : updated){
        std::cout << "Crossing: " << p.x << ' ' << p.y << ' ' << p.manhattan << ' ' << p.wire1_cost << '+' << p.wire2_cost << '=' << p.wire1_cost+p.wire2_cost << std::endl;
    }
    #endif
    return 0;
}
