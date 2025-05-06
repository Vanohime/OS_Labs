#pragma once
#include "../Classes.h"

class CountingVisitor : public Visitor {
    std::atomic<int> count{ 0 };
public:
    void visit(Disk&) override { count++; }
    void visit(Folder&) override { count++; }
    void visit(TextFile&) override { count++; }
    void visit(ImageFile&) override { count++; }
    int getCount() const { return count.load(); }
};