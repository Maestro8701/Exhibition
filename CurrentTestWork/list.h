#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class LinkedList {
public:
    LinkedList() = default;

    ~LinkedList() = default;

    void Build(const std::vector<std::pair<std::string, int32_t>>& entries);

    LinkedList(const LinkedList&) = delete;

    LinkedList& operator=(const LinkedList&) = delete;

    bool Serialize(const std::string& filename) const;

    bool Deserialize(const std::string& filename);

    bool Compare(const std::vector<std::pair<std::string, int32_t>>& original) const;

    void DumpText(const std::string& filename) const;

private:
    struct ListNode {
        ListNode* prev = nullptr;
        ListNode* next = nullptr;
        ListNode* rand = nullptr;
        std::string data;
    };

    std::vector< std::unique_ptr<ListNode>> nodes_;

};

