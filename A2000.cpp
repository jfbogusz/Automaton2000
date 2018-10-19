#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <string.h>
#include <assert.h>

using namespace std;

const uint32_t PRIME = 1000003;
const uint32_t SIZE = PRIME + 300;

const int LIMIT = 30;

const uint32_t _AUTOMATON_ = 0;
const uint32_t _START_ = 1;
const uint32_t _END_ = 2;

class Count 
{
public:
    uint32_t idx;
    uint32_t count;
    
    Count(uint32_t idx = 0, uint32_t count = 0): idx(idx), count(count) {};
    bool operator<(const Count& y) const;
};
 
class Node 
{
public:
    string word;
    vector <Count> count;
    bool sorted;
    
    Node(const string& world);
    void addEdge(const uint32_t end);
    friend ostream& operator<<(ostream& o, Node& n);
};

class A2000 {
public:
    static vector <Node> node;
    array<uint32_t, SIZE> hash_table;

    A2000(const string&);
    void process(string&); // process one sentence
    void say(); // write sentence to stdout
    
    static uint32_t hash(const string&);
    uint32_t process(const uint32_t start, const uint32_t end);
    uint32_t index(const char* world, const size_t len);
    void addEdge(const uint32_t start, const uint32_t end);
    
    friend ostream& operator<<(ostream& o, const A2000&);
};

// class Count

ostream& operator<<(ostream& o, const Count& c)
{
    o << A2000::node[c.idx].word /* << ' ' << c.idx */ << ' ' << c.count;
    return o;
}


bool Count::operator<(const Count& y) const 
{
    return (count < y.count) || ((count == y.count) && (A2000::node[idx].word > A2000::node[y.idx].word));
}

// class Node

ostream& operator<<(ostream& o, Node& n)
{
    o << n.word << endl;
    if (!n.sorted)
        sort(n.count.begin(), n.count.end(), [](const Count& x, const Count& y) {return y < x;});
    n.sorted = true;
    for(uint32_t i = 0; i < n.count.size(); ++i)
        o << '\t' << n.count[i] << endl;
    return o;
}

Node::Node(const string& word = "")
{
    this->word = word;
    sorted = true;
}

void Node::addEdge(const uint32_t end)
{
    for(uint32_t i = 0; i < count.size(); i++)
    {
        if (end == count[i].idx) 
        {
            count[i].count ++;
            if ((i > 0) && (count[i - 1] < count[i])) sorted = false;
            return;
        }
    }
    count.push_back(Count(end, 1));
    uint32_t i = count.size() - 1;
    if ((i > 0) && (count[i - 1] < count[i])) sorted = false;
}

// class A2000

vector <Node> A2000::node;

ostream& operator<<(ostream& o, const A2000& a)
{
    for(uint32_t i = 0; i < a.node.size(); ++i)
        o << a.node[i] << endl;
    return o;
}


A2000::A2000(const string& name = "Automaton2000") 
{
    hash_table.fill(0);
    assert (_AUTOMATON_ == index(&(name[0]), name.length()));
    assert (_START_ == index("_START_", sizeof("_START_") - 1));
    assert (_END_ == index("_END_", sizeof("_END_") - 1));
}

uint32_t A2000::hash(const string& s) 
{
    return std::hash<std::string>{}(s) % PRIME; 
}

void A2000::process(string& line) 
{
    size_t pos = line.find(": ");
    if (pos == std::string::npos) return;
    char *ch = &(line[pos + 2]);
    int len = line.length() - pos - 2;
    uint32_t start_idx = _START_;
    while(len > 0)
    {
        while((len > 0) && (*ch == ' ')) {len--; ch++;}
        char *word = ch;
        while ((len > 0) && (*ch != ' ')) {len--; ch++;}
        size_t wLen = ch - word;
        if (!wLen) continue; 
        start_idx = process(start_idx, index(word, wLen));
    }
    process(start_idx, _END_);
    if (line.find(node[_AUTOMATON_].word, pos) != std::string::npos) say();
}

uint32_t A2000::process(const uint32_t start, const uint32_t end) 
{
    // cerr << "process: " << start << ' ' << end << endl;
    switch (end) 
    {
        case _AUTOMATON_: return start;
        default: addEdge(start, end); break;
    }
    return end;
}

uint32_t A2000::index(const char *word, const size_t wLen)
{
    const string w(word, wLen);
    // cerr << "index: " << w;
    uint32_t idx = hash(w);
    while(hash_table[idx] != 0) 
    {
        if (w == node[hash_table[idx] - 1].word)
        {
            return hash_table[idx] - 1;
        }
        ++idx;
    }
    this->node.push_back(Node(w));
    hash_table[idx] = this->node.size();
    // cerr << ": " << this->node.size() - 1 << " new" << endl;
    return hash_table[idx] - 1;
}

void A2000::say()
{
    int limit = LIMIT;
    uint32_t idx = _START_;
    do {
        if (!node[idx].sorted) 
        {
            sort(node[idx].count.begin(), node[idx].count.end(), [](const Count& x, const Count& y) {return y < x;});
            node[idx].sorted = true;
        }
        idx = node[idx].count[0].idx;
        if (idx == _END_) break;
        cout << (limit == LIMIT ? string("") : string(" ")) + node[idx].word;
        --limit;
    } while (limit > 0);
    cout << endl;
    
//    static int ile = 0;
//    if (ile == 0) cerr << *this << endl;
//    ++ile;
}

void A2000::addEdge(const uint32_t start, const uint32_t end)
{
    if ((start == _START_) && (end == _END_)) return;
    node[start].addEdge(end);
}
    
int main()
{
    A2000 a;
    int n;
    cin >> n; cin.ignore();
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        a.process(line);
    }
//    a.say();
    cerr << a;
}
