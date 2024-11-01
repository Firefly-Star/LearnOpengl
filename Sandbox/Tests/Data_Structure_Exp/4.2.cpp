#include "Entrypoint.h"
#if CHECK_ACTIVE(4, 2, 1)

#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <filesystem>

struct Node 
{
    char ch;       
    int freq;      
    Node* left;    
    Node* right;   

    Node(char character, int frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}
    ~Node()
    {
        delete left;
        delete right;
    }
};

std::unordered_map<std::string, Node*> tempTrees;
std::unordered_map<std::string, std::unordered_map<char, std::string>> tempCodes;

struct Compare 
{
    bool operator()(Node* left, Node* right) {
        return left->freq > right->freq; 
    }
};

struct WriteNode
{
    const Node* node;
    int id;

    WriteNode(const Node* node, int id) : node(node), id(id) {}
};

Node* buildHuffmanTree(const std::unordered_map<char, int>& frequency) 
{
    std::priority_queue<Node*, std::vector<Node*>, Compare> minHeap;

    for (const auto& pair : frequency) {
        minHeap.push(new Node(pair.first, pair.second));
    }

    while (minHeap.size() > 1) {
        Node* left = minHeap.top(); minHeap.pop();
        Node* right = minHeap.top(); minHeap.pop();

        Node* newNode = new Node('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        minHeap.push(newNode);
    }

    return minHeap.top();
}

void buildCode(Node* node, std::unordered_map<char, std::string>& record, std::string code = "")
{
    if (node == nullptr)
    {
        return;
    }

    if (node->left == nullptr && node->right == nullptr)
    {
        record[node->ch] = code;
    }

    buildCode(node->left, record, code + "0");
    buildCode(node->right, record, code + "1");
}

enum class Status
{
    AWaiting,
    Initialization,
    Encoding,
    Decoding,
    TreePrinting,
    Quit
};

std::unordered_map<char, int> GetFrequency()
{
    std::cout << "输入字符集大小: ";
    int size;
    std::cin >> size;
    std::unordered_map<char, int> frequency;
    while (size > 0)
    {
        char tempc;
        int tempi;
        std::cout << "输入字符和频率: \n";
        std::cin.ignore();
        std::cin.get(tempc);
        std::cin >> tempi;
        if (frequency.find(tempc) != frequency.end())
        {
            std::cout << "重复的字符！\n";
        }
        else
        {
            frequency[tempc] = tempi;
            --size;
        }
    }
    return frequency;
}

void SaveTree(const Node* root, const std::unordered_map<char, std::string> code)
{
    std::cout << "输入存储路径: ";
    std::string savePath;
    std::cin >> savePath;
    std::filesystem::path filePath(savePath);

    if (!std::filesystem::exists(filePath.parent_path())) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream ofs(savePath);
    if (!ofs.is_open()) {
        std::cerr << "无法打开文件：" << savePath << std::endl;
        return;
    }

    ofs << "字符编码: \n";

    for (const auto& [key, value] : code) {
        ofs << key << ", " << value << "\n"; 
    }

    ofs << "哈夫曼树: \n";
    ofs << "结点总数: " << 2 * code.size() - 1 << "\n";
    std::queue<WriteNode> temp;
    temp.push(WriteNode(root, 0));
    int id = 0;
    while (!temp.empty())
    {
        auto nowWrite = temp.front();
        temp.pop();
        int leftId = -1;
        int rightId = -1;
        if (nowWrite.node->left != nullptr)
        {
            ++id;
            leftId = id;
            temp.push(WriteNode(nowWrite.node->left, leftId));
        }
        if (nowWrite.node->right != nullptr)
        {
            ++id;
            rightId = id;
            temp.push(WriteNode(nowWrite.node->right, rightId));
        }

        ofs << nowWrite.node->ch << ", " << nowWrite.node->freq << ", " << nowWrite.id << ", " << leftId << ", " << rightId << "\n";
    }

    ofs.close();
    std::cout << "数据已成功保存到文件 " << savePath << std::endl;
}

std::pair<Node*, std::unordered_map<char, std::string>> ReadTree(const std::string& filePath) {
    std::unordered_map<char, std::string> encodingMap;
    std::unordered_map<int, Node*> nodeMap;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "无法打开文件!\n";
        std::unordered_map<char, std::string> temp;
        return { nullptr, temp };
    }

    std::string line;
    bool readingEncoding = true; 

    while (std::getline(file, line)) {
        if (line.find("哈夫曼树:") != std::string::npos) 
        {
            readingEncoding = false; 
            continue; 
        }

        if (line.find("字符编码") != std::string::npos || line.find("结点总数") != std::string::npos) 
        {
            continue; // Skip this line
        }

        if (readingEncoding) {
            // Read character encoding
            std::istringstream iss(line);
            char ch;
            int code;
            ch = line[0];
            // Read character and its encoding (code)
            if (iss.ignore(3) && iss >> code) { // Ignore comma and read the code
                encodingMap[ch] = std::to_string(code); // Add to encoding map
            }
        }
        else {
            // Read the tree structure
            std::istringstream iss(line);
            char ch = ' '; // Default to space for inner nodes
            int freq, id, leftId, rightId;

            // Parse using comma as a delimiter
            std::getline(iss, line, ','); // Read character (may be space)
            if (!line.empty()) {
                ch = line[0]; // Use the first character
            }

            // Read other fields
            iss >> freq;
            iss.ignore(2); // Ignore comma
            iss >> id;
            iss.ignore(2); // Ignore comma
            iss >> leftId;
            iss.ignore(2); // Ignore comma
            iss >> rightId;

            Node* newNode = new Node(ch, freq);
            nodeMap[id] = newNode; // Store the node by ID

            // Link children if they exist
            if (leftId != -1) {
                newNode->left = (Node*)(leftId);
            }
            if (rightId != -1) {
                newNode->right = (Node*)(rightId);
            }
        }
    }

    for (auto& pair : nodeMap)
    {
        if (pair.second->left != nullptr)
        {
            pair.second->left = nodeMap[(int)(pair.second->left)];
        }
        if (pair.second->right != nullptr)
        {
            pair.second->right = nodeMap[(int)(pair.second->right)];
        }
    }

    file.close();

    // The root node is assumed to be with ID 0.
    Node* root = nodeMap[0];

    return std::make_pair(root, encodingMap);
}

void TempSaveTree(Node* root, std::unordered_map<char, std::string> code)
{
    std::cout << "给这颗树一个名字：";
    std::string name;
    bool b = true;
    do
    {
        std::cin >> name;
        if (tempTrees.find(name) == tempTrees.end())
        {
            tempTrees[name] = root;
            tempCodes[name] = std::move(code);
            b = false;
        }
        else
        {
            std::cout << "该名字已存在，请重新输入：";
        }
    } while (b);
}

std::string ReadFile(const char* hint)
{
    std::cout << hint;
    std::string name;
    std::cin >> name;
    std::ifstream ifs(name);
    if (!ifs.is_open())
    {
        std::cout << "无效的文件路径.\n";
        return "";
    }
    
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

void WriteFile(std::string content)
{
    std::cout << "是否要进行存储:[Y/N] ";
    char c;
    std::cin >> c;
    if (c != 'Y')
    {
        return;
    }
    std::cout << "输入存储路径: ";
    std::string savePath;
    std::cin >> savePath;
    std::filesystem::path filePath(savePath);

    if (!std::filesystem::exists(filePath.parent_path())) {
        std::filesystem::create_directories(filePath.parent_path());
    }

    std::ofstream ofs(savePath);
    if (!ofs.is_open()) {
        std::cerr << "无法打开文件：" << savePath << std::endl;
        return;
    }
    ofs << content;

    ofs.close();
    std::cout << "数据已成功保存到文件 " << savePath << std::endl;
}

auto GetTempTree() -> std::pair<Node*, std::unordered_map<char, std::string>&>
{
    std::string name;
    std::cout << "输入名称: ";
    std::cin >> name;
    if (tempCodes.find(name) == tempCodes.end())
    {
        std::unordered_map<char, std::string> temp;
        std::cout << "不存在的命名!\n";
        return { nullptr, temp };
    }
    else
    {
        return { tempTrees[name], tempCodes[name] };
    }
}

std::string Encode(std::string content, std::unordered_map<char, std::string> code)
{
    std::string result;
    int l = content.length();
    for (int i = 0; i < l; ++i)
    {
        result = result + code[content[i]];
    }
    return result;
}

std::string Decode(std::string content, Node* root)
{
    int l = content.length();
    std::string result;
    int i = 0;
    Node* currentNode = root;
    while(i < l)
    {
        if (currentNode->left == nullptr && currentNode->right == nullptr)
        {
            result += currentNode->ch;
            currentNode = root;
        }
        else
        {
            if (content[i] == '0')
            {
                if (currentNode->left != nullptr)
                {
                    currentNode = currentNode->left;
                }
                else
                {
                    std::cout << "无效的编码!\n";
                    return "";
                }
            }
            else if (content[i] == '1')
            {
                if (currentNode->right != nullptr)
                {
                    currentNode = currentNode->right;
                }
                else
                {
                    std::cout << "无效的编码!\n";
                    return "";
                }
            }
            else
            {
                std::cout << "无效的编码!\n";
                return "";
            }
            ++i;
        }
    }
    if (currentNode->left == nullptr && currentNode->right == nullptr)
    {
        result += currentNode->ch;
    }
    return result;
}

auto VerifyTree() -> std::pair<Node*, std::unordered_map<char, std::string>&>
{
    std::cout << "目前已有编码: \n";
    for (auto const& pair : tempCodes)
    {
        std::cout << pair.first << "\n";
    }
    std::cout << "需要从文件中取表吗:[Y/N] \n";
    char c;
    std::cin >> c;
    if (c == 'Y')
    {
        std::cout << "输入文件路径: ";
        std::string name;
        std::cin >> name;
        auto [tree, code] = ReadTree(name);
        if (tree != nullptr)
        {
            TempSaveTree(tree, code);
            std::cout << "目前已有编码: \n";
            for (auto const& pair : tempCodes)
            {
                std::cout << pair.first << "\n";
            }
        }
    }
    return GetTempTree();
}

struct PrintNode
{
    Node* node;
    int id;
    int level;

    PrintNode(Node* node, int id, int level) : node(node), id(id), level(level) {}
};

void PrintTree(Node* root, int space = 0, int level = 0) {
    std::queue<PrintNode> temp;
    temp.push(PrintNode(root, 1, 0));
    int currentLevel = 0;
    int currentId = 1;
    while (!temp.empty())
    {
        PrintNode pn = temp.front(); temp.pop();
        int leftId = -1, rightId = -1;
        if (pn.node->left != nullptr)
        {
            temp.push(PrintNode(pn.node->left, 2 * pn.id, pn.level + 1));
            leftId = 2 * pn.id;
        }
        if (pn.node->right != nullptr)
        {
            temp.push(PrintNode(pn.node->right, 2 * pn.id + 1, pn.level + 1));
            rightId = 2 * pn.id + 1;
        }

        if (pn.level > currentLevel)
        {
            currentLevel = pn.level;
            std::cout << "\n";
        }
        std::cout << (pn.node->ch == '\0' ? 'r' : pn.node->ch) << ", id: " << pn.id << ", kids: (" << leftId << ", " << rightId << ")\n";
    }
    std::cout << "\n";
}

void Terminate()
{
    for (auto pair : tempTrees)
    {
        delete pair.second;
    }
}
int main() {
    Status status = Status::AWaiting;
    while (status != Status::Quit)
    {
        switch (status)
        {
        case Status::AWaiting:
        {
            std::cout << "输入功能: I(Initialization), E(Encoding), D(Decoding), T(Tree printing), Q(Quit): ";
            char x;
            std::cin >> x;
            switch (x)
            {
            case 'I':
                status = Status::Initialization; break;
            case 'E':
                status = Status::Encoding; break;
            case 'D':
                status = Status::Decoding; break;
            case 'T':
                status = Status::TreePrinting; break;
            case 'Q':
                status = Status::Quit; break;
            default:
                std::cout << "无效指令\n";
                break;
            }
            break;
        }
        case Status::Initialization:
        {
            auto frequency = GetFrequency();
            Node* root = buildHuffmanTree(frequency);
            std::unordered_map<char, std::string> code;
            buildCode(root, code);
            SaveTree(root, code);
            TempSaveTree(root, code);
            status = Status::AWaiting;
            break;
        }
        case Status::Encoding:
        {
            auto [tree, code] = VerifyTree();
            std::string content = ReadFile("输入待编码文件路径: ");
            std::string encodedContent = Encode(content, code);
            if (!encodedContent.empty())
            {
                std::cout << "编码结果: " << encodedContent << "\n";
                WriteFile(encodedContent);
            }
            status = Status::AWaiting;
            break;
        }
        case Status::Decoding:
        {
            auto [tree, code] = VerifyTree();
            std::string content = ReadFile("输入待解码文件路径: ");
            std::string decodedContent = Decode(content, tree);
            if (!decodedContent.empty())
            {
                std::cout << "解码结果: " << decodedContent << "\n";
                WriteFile(decodedContent);
            }
            status = Status::AWaiting;
            break;
        }
        case Status::TreePrinting:
        {
            auto [tree, code] = VerifyTree();
            if (tree != nullptr)
            {
                PrintTree(tree);
            }
            status = Status::AWaiting;
            break;
        }
        case Status::Quit:
        {
            break;
        }
        }
    }
    Terminate();
}


#endif