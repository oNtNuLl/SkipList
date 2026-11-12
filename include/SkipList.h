//
// Created by w on 2026/3/15.
//

#ifndef SKIPKV_SKIPLIST_H
#define SKIPKV_SKIPLIST_H
#include <iostream>
#include <cstring>

template<typename K, typename V>
struct Node
{
    K key;
    V value;
    int level;
    Node<K, V>** forward;

    Node(K key, V value,int l):key(key),value(value),level(l)
    {

        // forward = new Node<K,V>* [level + 1];
        // memset(forward,0,sizeof(Node<K,V>*) *(level + 1));
    }
};

template<typename K,typename V>
class SkipList
{
private:
    Node<K,V>* _header;
    int _max_level;
    int _skip_list_level;

public:
    SkipList(int max_level):_max_level(max_level),_skip_list_level(0)
    {
        // _header = new Node<K, V>(K(),V(),_max_level);
        _header = allocate_node(K(), V(), _max_level);
    }
    ~SkipList()
    {
        Node<K,V>* current = _header;

        while (current != nullptr)
        {
            Node<K,V>* next_node = current->forward[0];
            destroy_node(current);
            current = next_node;
        }
    }

    int insert(K key, V value)
    {

        Node<K,V> * update[_max_level + 1];
        memset(update, 0, sizeof(Node<K,V>*) * (_max_level + 1));
        Node<K,V> * current = _header;

        for (int i = _skip_list_level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }

            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key)
        {
            current->value = value;
            return 1;
        }

        int random_level = get_random_level();

        if (random_level > _skip_list_level)
        {
            for (int i = _skip_list_level + 1; i <= random_level;i++)
            {
                update[i] = _header;
            }
            _skip_list_level = random_level;
        }


        Node<K,V>* new_node = allocate_node(key, value, random_level);
        for (int i = 0; i <= random_level; i++)
        {
            new_node->forward[i] = update[i]->forward[i];

            update[i]->forward[i] = new_node;
        }
        return 0;
    }

    bool search(K key)
    {

        Node<K,V> * current = _header;

        for (int i = _skip_list_level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }


        }

        current = current->forward[0];

        if (current != nullptr && current->key == key)
        {
             std::cout << " 找到了！Key: " << key << " 对应的值是: " << current->value << std::endl;
            return true;
        }
        return false;
    }

    int remove(K key)
    {
        Node<K,V> * update[_max_level + 1];
        memset(update, 0, sizeof(Node<K,V>*) * (_max_level + 1));
        Node<K,V> * current = _header;

        for (int i = _skip_list_level; i >= 0; i--)
        {
            while (current->forward[i] != nullptr && current->forward[i]->key < key)
            {
                current = current->forward[i];
            }

            update[i] = current;
        }

        current = current->forward[0];

        if (current != nullptr && current->key == key)
        {

            for (int i = 0; i <= current->level; i++)
            {

                if (update[i]->forward[i] != current) {
                    break;
                }

                update[i]->forward[i] = current->forward[i];
            }

            while (_skip_list_level > 0 && _header->forward[_skip_list_level] == nullptr) {
                _skip_list_level--;
            }

            destroy_node(current);


            return 1;
        }

        return 0;

    }


    int get_random_level()
    {
        int level = 0;
        // rand() % 2 == 0 就是 50% 的概率（抛硬币正面）
        // 只要抛到正面，且还没顶到物理天花板，就继续加盖一层
        while ((rand() % 2 == 0) && (level < _max_level)) {
            level++;
        }
        return level;
    }


    void display_list()
    {
        std::cout << "\n=====  跳表物理结构 当前: " << _skip_list_level << " 层) =====" << std::endl;

        // 从最高层往下扫描
        for (int i = _skip_list_level; i >= 0; i--) {
            std::cout << "Level " << i << ":\t";

            Node<K, V>* current = _header->forward[i];
            Node<K, V>* base_node = _header->forward[0];

            while (base_node != nullptr) {

                std::string node_str = base_node->key+ ":" + base_node->value;
                if (current != nullptr && current->key == base_node->key) {
                    std::cout << node_str;
                    current = current->forward[i];
                } else {

                    std::string highway(node_str.length(), '-');
                    std::cout << highway;
                }

                std::cout << " -> ";
                base_node = base_node->forward[0];
            }
            std::cout << "NULL" << std::endl;
        }
        std::cout << "=================================================================\n" << std::endl;
    }
private:

    Node<K,V>* allocate_node(K key,V value,int level)
    {
        size_t total_size = sizeof(Node<K,V>) + sizeof(Node<K,V>*) * (level + 1);

        void* raw_memory = malloc(total_size);

        Node<K,V>* new_node =  new (raw_memory) Node<K,V>(key, value,level);

        new_node->forward = reinterpret_cast<Node<K, V>**>(static_cast<char*>(raw_memory) + sizeof(Node<K, V>));

        memset(new_node->forward, 0, sizeof(Node<K, V>*) * (level + 1));

        return new_node;
    }

    void destroy_node(Node<K,V>* node)
    {
        if (node != nullptr)
        {
            node->~Node();
            free(node);
        }
    }

};



#endif //SKIPKV_SKIPLIST_H