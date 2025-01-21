#include "Maze.h"

Maze::Maze(const std::vector<std::vector<char>> & map) {
    LoadMazeFromMap(map);
}

Maze & Maze::operator=(const std::vector<std::vector<char>> & map) {
    return LoadMazeFromMap(map);
}

Maze & Maze::LoadMazeFromMap(const std::vector<std::vector<char>> & map) {
    M = map;
    O = std::make_pair(0,0);
    Z = std::make_pair((long)M[0].size(), (long)M.size());
    for(auto p = 0; p < M.size(); p++) {
        for(auto q = 0; q < M[p].size(); q++) {
            if(IsStart(std::make_pair(q,p))) {
                S = std::make_pair(q,p);
            }
            if(IsExit(std::make_pair(q,p))) {
                E = std::make_pair(q,p);
            }
        }
    }
    return *this;
}

std::vector<std::vector<char>> Maze::RemoveDeadEnds() {
    size_t rpl = 1;
    while(rpl) {
        rpl  = Replace({{0,'#',0},{'#','.','#'}},{{0,0,0},{0,'#',0}});
        rpl += Replace({{'#','.','#'},{0,'.',0}},{{0,0,0},{0,'#',0}});
    }
    return M;
}
bool Maze::Matches(std::pair<int,int> position, const std::vector<std::vector<char>> &shape) const {
    if(position.first < 0 || position.second < 0 || position.second > M.size() - shape.size()) {
        return false;
    }
    for (auto i = 0; i < shape.size(); i++) {
        if(position.first > M[i].size() - shape[i].size()) {
            return false;
        }
        for (auto j = 0; j < shape[0].size(); j++) {
            if(shape[i][j] != 0) {
                if (M[position.second + i][position.first + j] != shape[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}
size_t Maze::Replace(const std::vector<std::pair<int,int>> & positions, std::vector<std::vector<char>> replace) {
    long cnt = 0;
    if(positions.empty()) {
        return cnt;
    }
    for(auto at : positions) {
        for(int ny = 0; ny < replace.size(); ny ++) {
            for(int nx = 0; nx < replace[0].size(); nx ++) {
                if(replace[ny][nx] != 0) {
                    cnt += M[at.second + ny][at.first + nx] == replace[ny][nx] ? 0 : 1;
                    M[at.second + ny][at.first + nx] = replace[ny][nx];
                }
            }
        }
    }
    return positions.size();
}
std::vector<std::pair<int,int>> Maze::Search(const std::vector<std::vector<char>> & needle) const {
    std::vector<std::pair<int,int>> matches;
    for(int y = 0; y <= M.size() - needle.size(); y++) {
        for(int x = 0; x <= M[0].size() - needle[0].size(); x++) {
            if(Matches(std::make_pair(x,y),needle)) {
                matches.emplace_back(std::make_pair(x,y));
            }
        }
    }
    return matches;
}

size_t Maze::Replace(const std::vector<std::vector<char>> & needle, std::vector<std::vector<char>> replace) {
    return Replace(Search(needle),replace);
}

Maze & Maze::NewMazeFromBlocks(const std::vector<std::pair<int,int>> & wall, int w, int h) {
    M = std::vector<std::vector<char>>();
    O = std::make_pair(0,0);
    Z = std::make_pair(w+1,h+1);
    std::vector<char> line;
    line.push_back(Maze::WALL_CHAR);
    line.insert(line.end(), w, Maze::EMPTY_CHAR);
    line.push_back(Maze::WALL_CHAR);
    std::vector<char> header;
    header.insert(header.end(), w + 2, Maze::WALL_CHAR);
    M.push_back(header);
    M.insert(M.end(), h, line);
    M.push_back(header);
    for(auto p : wall) {
        M[p.second + 1][p.first + 1] = Maze::WALL_CHAR;
    }
    
    S = std::make_pair(1,1);
    M[1][1] = Maze::START_CHAR;

    E = std::make_pair(w,h);
    M[h][w] = Maze::EXIT_CHAR;

    return *this;
}

long Maze::Width() const { 
    return (long)M[0].size(); 
}

long Maze::Height() const { 
    return (long)M.size(); 
}

std::pair<int,int> Maze::Origin() const { 
    return O; 
}

std::pair<int,int> Maze::Start() const { 
    return S; 
}

std::pair<int,int> Maze::Exit() const { 
    return E; 
}

std::pair<int,int> Maze::TopRight() const { 
    return Z; 
}

char Maze::GetTile(std::pair<int,int> p) const {
    if(p.first < 0 || p.first >= Width() || p.second < 0 || p.second >= Height()) {
        throw std::runtime_error("Out of bounds");
    }
    return M[p.second][p.first];
}

char Maze::SetTile(std::pair<int,int> p, char c) {
    char t = GetTile(p);
    M[p.second][p.first] = c;
    return t;
}

std::pair<int,int> Maze::Move(std::pair<int,int> c, Compass d) const {
    switch(d) {
        case Compass::NORTH: return std::make_pair(c.first, c.second - 1);
        case Compass::EAST: return std::make_pair(c.first + 1, c.second);
        case Compass::SOUTH: return std::make_pair(c.first, c.second + 1);
        case Compass::WEST: return std::make_pair(c.first - 1, c.second);
    }
    throw std::runtime_error("Invalid direction");
}

bool Maze::IsWall(std::pair<int,int> p) const {
    return GetTile(p) == WALL_CHAR;
}

bool Maze::IsFree(std::pair<int,int> p) const {
    return GetTile(p) == EMPTY_CHAR;
}

bool Maze::IsExit(std::pair<int,int> p) const {
    return GetTile(p) == EXIT_CHAR;
}

bool Maze::IsStart(std::pair<int,int> p) const {
    return GetTile(p) == START_CHAR;
}

bool Maze::IsDeadEnd(std::pair<int,int> p, Compass d) const {
    return Options(p,d).empty();
}
bool Maze::IsCrossPoint(std::pair<int,int> p, Compass d) const {
    return Options(p,d).size() > 1;
}
std::vector<Compass> Maze::Options(std::pair<int,int> p, Compass d) const {
    std::vector<Compass> options;

    if(!IsWall(Move(p,d))) {
        options.push_back(d);
    }
    if(!IsWall(Move(p,Compass::Clockwise(d)))) {   
        options.push_back(Compass::Clockwise(d));
    }
    if(!IsWall(Move(p,Compass::CounterClockwise(d)))) {   
        options.push_back(Compass::CounterClockwise(d));
    }
    return options;
}

char Maze::GetTileForward(std::pair<int,int> p, Compass d) const {
    return GetTile(Move(p,d));
}

char Maze::GetTileClockwise(std::pair<int,int> p, Compass d) const {
    return GetTile(Move(p,d.Clockwise()));
}

char Maze::GetTileCounterClockwise(std::pair<int,int> p, Compass d) const {
    return GetTile(Move(p,d.CounterClockwise()));
}

char Maze::GetTileReverse(std::pair<int,int> p, Compass d) const {
    return GetTile(Move(p,d.Reverse()));
}

char Maze::SetTileForward(std::pair<int,int> p, Compass d, char c) {
    return SetTile(Move(p,d),c);
}

char Maze::SetTileClockwise(std::pair<int,int> p, Compass d, char c) {
    return SetTile(Move(p,d.Clockwise()),c);
}

char Maze::SetTileCounterClockwise(std::pair<int,int> p, Compass d, char c) {
    return SetTile(Move(p,d.CounterClockwise()),c);
}

char Maze::SetTileReverse(std::pair<int,int> p, Compass d, char c) {
    return SetTile(Move(p,d.Reverse()),c);
}

const std::vector<std::vector<char>> & Maze::GetMap() const {
    return M;
}

bool Maze::operator==(const Maze & other) const {
    return M == other.M;
}

bool Maze::operator==(const std::vector<std::vector<char>> & map) const {
    return M == map;
}

bool Maze::operator!=(const Maze & other) const {
    return M != other.M;
}

bool Maze::operator!=(const std::vector<std::vector<char>> & map) const {
    return M != map;
}

Maze::operator std::vector<std::vector<char>>() const {
    return M;
}

std::ostream & operator<<(std::ostream & os, const Maze & maze) {
    for(auto p = 0; p < maze.M.size(); p++) {
        for(auto q = 0; q < maze.M[p].size(); q++) {
            os << maze.M[p][q];
        }
        os << std::endl;
    }
    return os;
}