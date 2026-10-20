#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

class SkipListNode {
public:
    int key;

    // forward[i] 表示第 i 层的下一个节点
    vector<SkipListNode*> forward;

    SkipListNode(int k, int level)
        : key(k), forward(level + 1, nullptr) {}
};


class SkipList {
private:
    int maxLevel;       // 允许的最大层数
    int currentLevel;   // 当前跳表实际最高层
    float p;            // 升层概率

    SkipListNode* head; // 哨兵头节点

public:
    SkipList(int maxL = 16, float prob = 0.5)
        : maxLevel(maxL),
          currentLevel(0),
          p(prob) {

        // head 必须覆盖所有层
        head = new SkipListNode(-1, maxLevel);

        srand((unsigned)time(nullptr));
    }


    ~SkipList() {
        SkipListNode* cur = head;

        // 第 0 层包含所有节点
        while (cur != nullptr) {
            SkipListNode* next = cur->forward[0];

            delete cur;

            cur = next;
        }
    }


    // 随机生成节点最高层数
    int randomLevel() {
        int level = 0;

        while ((float)rand() / RAND_MAX < p
               && level < maxLevel) {

            level++;
        }

        return level;
    }


    // 查找
    bool search(int key) {
        SkipListNode* cur = head;

        // 从最高层开始往下找
        for (int level = currentLevel;
             level >= 0;
             level--) {

            // 只要下一个节点还小于 key，就继续向右
            while (cur->forward[level] != nullptr &&
                   cur->forward[level]->key < key) {

                cur = cur->forward[level];
            }
        }

        // 此时 cur 是第 0 层中最后一个小于 key 的节点
        cur = cur->forward[0];

        return cur != nullptr &&
               cur->key == key;
    }


    // 插入
    void insert(int key) {
        // update[i]：
        // 第 i 层中，待插入位置前面的节点
        vector<SkipListNode*> update(maxLevel + 1);

        SkipListNode* cur = head;

        // 先像查找一样定位
        for (int level = currentLevel;
             level >= 0;
             level--) {

            while (cur->forward[level] != nullptr &&
                   cur->forward[level]->key < key) {

                cur = cur->forward[level];
            }

            update[level] = cur;
        }

        // 第 0 层中第一个 >= key 的节点
        cur = cur->forward[0];

        // 不允许重复元素
        if (cur != nullptr &&
            cur->key == key) {

            return;
        }

        // 随机决定新节点高度
        int newLevel = randomLevel();

        // 如果新节点比当前跳表还高
        if (newLevel > currentLevel) {

            for (int i = currentLevel + 1;
                 i <= newLevel;
                 i++) {

                update[i] = head;
            }

            currentLevel = newLevel;
        }

        // 创建新节点
        SkipListNode* newNode =
            new SkipListNode(key, newLevel);

        // 在每一层执行普通链表插入
        for (int i = 0;
             i <= newLevel;
             i++) {

            newNode->forward[i] =
                update[i]->forward[i];

            update[i]->forward[i] =
                newNode;
        }
    }


    // 删除
    void remove(int key) {
        vector<SkipListNode*> update(maxLevel + 1);

        SkipListNode* cur = head;

        // 找到每一层中的前驱节点
        for (int level = currentLevel;
             level >= 0;
             level--) {

            while (cur->forward[level] != nullptr &&
                   cur->forward[level]->key < key) {

                cur = cur->forward[level];
            }

            update[level] = cur;
        }

        // 第 0 层中第一个 >= key 的节点
        cur = cur->forward[0];

        // 没找到
        if (cur == nullptr ||
            cur->key != key) {

            return;
        }

        // 逐层删除
        for (int i = 0;
             i <= currentLevel;
             i++) {

            // cur 不存在于这一层
            // 更高层也不会存在
            if (update[i]->forward[i] != cur) {
                break;
            }

            // 普通链表删除：
            // update -> cur -> next
            // 变为
            // update ------> next
            update[i]->forward[i] =
                cur->forward[i];
        }

        delete cur;

        // 如果最高层已经空了，就降低 currentLevel
        while (currentLevel > 0 &&
               head->forward[currentLevel] == nullptr) {

            currentLevel--;
        }
    }


    // 打印整个跳表
    void print() {
        cout << "Skip List:" << endl;

        for (int level = currentLevel;
             level >= 0;
             level--) {

            cout << "Level "
                 << level
                 << ": ";

            SkipListNode* cur =
                head->forward[level];

            while (cur != nullptr) {
                cout << cur->key << " ";

                cur =
                    cur->forward[level];
            }

            cout << endl;
        }
    }
};


int main() {
    SkipList sl;
    sl.insert(2);
    sl.insert(3);
    sl.insert(5);
    sl.insert(7);
    sl.insert(8);
    sl.insert(9);
    sl.insert(10);
    sl.insert(11);
    sl.insert(13);
    sl.insert(15);
    sl.insert(20);
    sl.insert(25);

    sl.print();

    cout << endl;

    cout << "search 15: "
         << sl.search(15)
         << endl;

    cout << "search 17: "
         << sl.search(17)
         << endl;

    cout << endl;

    sl.remove(15);

    cout << "After remove 15:" << endl;

    sl.print();

    return 0;
}