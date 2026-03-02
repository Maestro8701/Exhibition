#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "list.h"


//int32_t	от - 2 147 483 648 до 2 147 483 647	± 
//uint32_t	от 0 до 4 294 967 295	4 миллиарда
//uint64_t	от 0 до 18 446 744 073 709 551 615
    void LinkedList::Build(const std::vector<std::pair<std::string, int32_t>>& entries) {
        nodes_.clear();
        const std::size_t node_count = entries.size();
        if (node_count == 0) {
            return;
        }

        nodes_.reserve(node_count);        
        for (std::size_t i = 0; i < node_count; ++i) {
            nodes_.push_back(std::make_unique<ListNode>());
            nodes_[i]->data = entries[i].first;
            if (i > 0) {
                nodes_[i]->prev = nodes_[i - 1].get();
                nodes_[i - 1]->next = nodes_[i].get();
            }
        }
        for (std::size_t i = 0; i < node_count; ++i) {
            int32_t idx = entries[i].second;
            if (idx >= 0 && static_cast<std::size_t>(idx) < node_count) {
                nodes_[i]->rand = nodes_[static_cast<std::size_t>(idx)].get();
            }
        }        
    }

    bool LinkedList::Serialize(const std::string& filename) const {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            return false;
        }

        uint64_t node_count = nodes_.size();
        out.write(reinterpret_cast<const char*>(&node_count), sizeof(node_count));
        if (node_count == 0) {
            return true;
        }
   
        std::unordered_map<ListNode*, int32_t> ptr_to_idx;
        ptr_to_idx.reserve(nodes_.size());
        for (std::size_t i = 0; i < nodes_.size(); ++i) {
            ptr_to_idx[nodes_[i].get()] = i;
        }

       for (const auto& node : nodes_) {
            uint32_t length = static_cast<uint32_t>(node->data.size());
            out.write(reinterpret_cast<const char*>(&length), sizeof(length));
            if (length > 0) {
                out.write(node->data.data(), length);
            }

            int32_t r_idx = (node->rand) ? ptr_to_idx[node->rand] : -1;
            out.write(reinterpret_cast<const char*>(&r_idx), sizeof(r_idx));
        } 

        out.close();
        return true;
    }
    
    //Причина почему в коде используется тип uint64_t так как всегда занимает ровно 8 байт.
    // Это значит, что файл, записанный на одном компьютере, будет корректно 
    // прочитан на другом, даже если там другая разрядность системы.

    bool LinkedList::Deserialize(const std::string& filename) {
        nodes_.clear();
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            return false;
        }

        uint64_t node_count = 0;
        if (!in.read(reinterpret_cast<char*>(&node_count), sizeof(node_count))) {
            return false; 
        }

        if (node_count == 0) {
            return true;
        }

        nodes_.reserve(node_count);
        std::vector<int32_t> rand_indices(node_count);

        for (uint64_t i = 0; i < node_count; ++i) {
            uint32_t length;
            in.read(reinterpret_cast<char*>(&length), sizeof(length));

            auto node = std::make_unique<ListNode>();
            node->data.resize(length);
            if (length > 0) {
                in.read(&node->data[0], length);
            }
            in.read(reinterpret_cast<char*>(&rand_indices[i]), sizeof(int32_t));

            if (i > 0) {
                node->prev = nodes_[i - 1].get();
                nodes_[i - 1]->next = node.get();
            }
            nodes_.push_back(std::move(node));
        }

        for (uint64_t i = 0; i < node_count; ++i) {
            int32_t idx = rand_indices[i];
            if (idx >= 0 && static_cast<size_t>(idx) < node_count) {
                nodes_[i]->rand = nodes_[idx].get();
            }
        }

        return in.good();
    }

    //Следущие 2 метода для проверки списка один сравнивает другой записывает в файл чтобы на глаз можно было проверить оба списка
    bool LinkedList::Compare(const std::vector<std::pair<std::string, int32_t>>& original) const {
        if (nodes_.size() != original.size()) {
            return false;
        }

        std::unordered_map<ListNode*, int32_t> ptr_to_idx;
        for (std::size_t i = 0; i < nodes_.size(); ++i) {
            ptr_to_idx[nodes_[i].get()] = i;
        }

        for (std::size_t i = 0; i < nodes_.size(); ++i) {
            if (nodes_[i]->data != original[i].first) {
                return false;
            }

            int32_t current_rand_idx = -1;
            if (nodes_[i]->rand) {//.
                auto it = ptr_to_idx.find(nodes_[i]->rand);
                if (it != ptr_to_idx.end()) {
                    current_rand_idx = it->second;
                }
            }

            if (current_rand_idx != original[i].second) {
                return false;
            }
        }
        return true;
    }

    void LinkedList::DumpText(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Не удалось открыть файл '" << filename << "' для дампа.\n";
            return;
        }

        std::unordered_map<ListNode*, int32_t> ptr_to_idx;
        ptr_to_idx.reserve(nodes_.size());
        for (std::size_t i = 0; i < nodes_.size(); ++i) {
            ptr_to_idx[nodes_[i].get()] = i;
        }


        for (const auto& node_ptr : nodes_) {
            out << node_ptr->data << ';';
            if (node_ptr->rand) {
                out << ptr_to_idx[node_ptr->rand];
            }
            else {
                out << -1;
            }
            out << '\n';
        }
    }
